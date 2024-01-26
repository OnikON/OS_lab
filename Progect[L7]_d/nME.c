#include<stdio.h>
#include<errno.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<dirent.h>
#include<pthread.h>
#include<sys/stat.h>
#include<sys/types.h>

#define PATH_LENGTH 100
#define ERROR_BUFFER_LENGTH 50
#define FILE_NAME_LENGTH 500
#define FILE_DATA_BUFFER_LENGTH 512

typedef struct dirInfo
{
    char old_dir[PATH_LENGTH];
    char new_dir[PATH_LENGTH];
} dirInfo;

void file_copy_thread(dirInfo* dirs);
void dir_copy_thread(dirInfo* dirs);

void* copy_file(void* arg)
{
    dirInfo* dirs = (dirInfo*)arg;
    char err_buf[ERROR_BUFFER_LENGTH];

    printf("Copying %s file...\n", dirs->old_dir);

    int new_file = 0,
        old_file = 0;

    while (old_file == 0)
    {
        old_file = open(dirs->old_dir, O_RDONLY);
        printf("Opened %s\n", dirs->old_dir);
        if(old_file == -1)
        {
            if(errno == EMFILE)
            {
                usleep(1000);
            }
            else
            {
                strerror_r(errno, err_buf, ERROR_BUFFER_LENGTH);
                printf("Couldn't open file: %s\n", err_buf);

                free(dirs);
                pthread_exit(NULL);
            }
        }
    }
    
    printf("Creating file...%s\n", dirs->new_dir);
  
    struct stat stats;
    lstat(dirs->old_dir, &stats);
    mode_t mode = stats.st_mode;  
    
    while (new_file == 0)
    {
        new_file = creat(dirs->new_dir, mode);
        if(new_file == -1)
        {
            if(errno == EMFILE)
            {
                usleep(1000);
            }
            else
            {
                strerror_r(errno, err_buf, ERROR_BUFFER_LENGTH);
                printf("Couldn't create file: %s\n", err_buf);

                free(dirs);
                pthread_exit(NULL);
            }
        }
    }

    printf("New file created. Deskriptor = %d\n", new_file);

    char data[FILE_DATA_BUFFER_LENGTH];
    int read_byte = read(old_file, data, FILE_DATA_BUFFER_LENGTH);
    while (read_byte > 0)
    {
        write(new_file, data, read_byte);
        read_byte = read(old_file, data, FILE_DATA_BUFFER_LENGTH);
    }

    close(old_file);
    close(new_file);
    free(dirs);

    pthread_exit(NULL);
}

void* copy_dir(void* arg)
{
    dirInfo* dirs = (dirInfo*)arg;

    DIR* old_dir_fd = NULL;
    char err_buf[ERROR_BUFFER_LENGTH];

    while(old_dir_fd == NULL)
    {
        old_dir_fd = opendir(dirs->old_dir); // opened directory
        printf("opened dir\n");
        if(old_dir_fd == NULL)
        {
            if(errno == EMFILE)
            {
                usleep(1000);
            }
            else
            {
                strerror_r(errno, err_buf, ERROR_BUFFER_LENGTH);
                printf("Couldn't open directory: %s\n", err_buf);

                free(dirs);
                pthread_exit(NULL);
            }
        }
    }

    struct dirent* cur_file;
    char entry_buffer[sizeof(struct dirent) + pathconf(dirs->old_dir, PATH_LENGTH) + 1];
    struct dirent* entry = (struct dirent*)entry_buffer;

    printf("Created buffers\n");

    while(cur_file != NULL) // reading files from directory except "." and ".."
    {
        int dir = readdir_r(old_dir_fd, entry, &cur_file);
        if(dir != 0)
        {
            strerror_r(errno, err_buf, ERROR_BUFFER_LENGTH);
            printf("Couldn't open directory: %s\n", err_buf);
            
            free(dirs);
            pthread_exit(NULL);
        }
        else
        {
            if(cur_file != NULL)
            {
                if(strcmp(cur_file->d_name, ".") == 0 || strcmp(cur_file->d_name, "..") == 0)
                {
                    continue;
                }

                printf("Curent file is %s\n", cur_file->d_name);

                char tmp[] = "%s/%s";
                char old_filename[FILE_NAME_LENGTH];
                char new_filename[FILE_NAME_LENGTH];
                sprintf(old_filename, tmp, dirs->old_dir, cur_file->d_name);
                sprintf(new_filename, tmp, dirs->new_dir, cur_file->d_name);

                struct stat file_stats;
                lstat(old_filename, &file_stats);
                if(S_ISDIR(file_stats.st_mode))
                {
                    printf("Curent file is dir!\n");

                    dirInfo* new_dirs = (dirInfo*)malloc(sizeof(dirInfo));
                    strcpy(new_dirs->old_dir, old_filename);
                    strcpy(new_dirs->new_dir, new_filename);

                    dir_copy_thread(new_dirs);
                }
                else if(S_ISREG(file_stats.st_mode))
                {
                    printf("Curent file is reg!\n");
                    
                    dirInfo* new_dirs = (dirInfo*)malloc(sizeof(dirInfo));
                    strcpy(new_dirs->old_dir, old_filename);
                    strcpy(new_dirs->new_dir, new_filename);

                    file_copy_thread(new_dirs);
                }
                else
                {
                    continue;
                }
            }
        }
    }

    closedir(old_dir_fd);
    free(dirs);
    pthread_exit(NULL);
}

void file_copy_thread(dirInfo* dirs)
{
    pthread_t tid;
    pthread_attr_t attrs;
    char err_buf[ERROR_BUFFER_LENGTH];

    pthread_attr_init(&attrs);
    pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);

    int err = pthread_create(&tid, &attrs, copy_file, dirs);
    printf("Started file copy!\n");
    if(err != 0)
    {
        strerror_r(err, err_buf, ERROR_BUFFER_LENGTH);
        printf("Couldn't create new thread: %s\n", err_buf);

        free(dirs);
        pthread_exit(NULL);
    }

    pthread_attr_destroy(&attrs);
}

void dir_copy_thread(dirInfo* dirs)
{
    struct stat stats;
    lstat(dirs->old_dir, &stats);
    mode_t mode = stats.st_mode;  
  
    if(mkdir(dirs->new_dir, mode) != 0)
    {
        printf("(194)Couldn't create new dir!\n");
        pthread_exit(NULL);
    }
    printf("New dir created\n");

    pthread_t tid;
    pthread_attr_t attrs;
    char err_buf[ERROR_BUFFER_LENGTH];

    pthread_attr_init(&attrs);
    pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);

    int err = pthread_create(&tid, &attrs, copy_dir, dirs);
    if(err != 0)
    {
        strerror_r(err, err_buf, ERROR_BUFFER_LENGTH);
        printf("Couldn't create new thread: %s\n", err_buf);

        free(dirs);
        pthread_exit(NULL);
    }

    pthread_attr_destroy(&attrs);
}

int main(int argc, char* argv[])
{
    if(argc < 3) // old dir, new dir
    {
        printf("Not enough arguments\n");

        return 0;
    }

    dirInfo* dirs = (dirInfo*)malloc(sizeof(dirInfo));
    strcpy(dirs->old_dir, argv[1]);
    strcpy(dirs->new_dir, argv[2]);

    char* err = strstr(dirs->new_dir, dirs->old_dir);
    if(err)
    {
        printf("Do not make new dir in old dir!\n");
        free(dirs);
        return 0;
    }
    
    // making new dir with the same permision mode as old dir
    struct stat stats;
    lstat(dirs->old_dir, &stats);
    mode_t mode = stats.st_mode;  
  
    if(mkdir(dirs->new_dir, mode) != 0) // 0777 means binary: 0b 111 111 111, or permission: rwx-rwx-rwx == decimal 511 
    {
        perror("Couldn't make new directory: \n");

        return 0;
    }

    copy_dir(dirs);
    pthread_exit(NULL);
}

/*
struct dirent :
    char d_reserved[16]  -  Reserved

    unsigned int d_fileno_gen_id  -  The generation ID associated with the file ID.

    unsigned int d_reclen  -  The length of the directory entry in bytes.

    ...

    unsigned int d_namelen  -  The length of the name in bytes, excluding the null terminator.

    har	d_name[640]  -  A string that gives the name of a file in the directory. 
*/
   