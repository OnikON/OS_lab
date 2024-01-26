#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PATH_LENGTH 100
#define ERROR_BUFFER_LENGTH 50
#define FILE_NAME_LENGTH 500
#define FILE_DATA_BUFFER_LENGTH 512
#define A 1

typedef struct dir_paths {
  char old_dir_paths[PATH_LENGTH];
  char new_dir_paths[PATH_LENGTH];
} dir_paths;

void *FileCP(void *arg);
void *DirCP(void *arg);
void ThreadFileCP(dir_paths *dirs);
void ThreadDirCP(dir_paths *dirs);

int main(int argc, char *argv[]) {

  if (argc < 3) // old dir, new dir
  {
    printf("Not enough arguments\n");
    exit(A);
  }
  dir_paths *dirs = (dir_paths *)malloc(sizeof(dir_paths));
  strcpy(dirs->old_dir_paths, argv[1]);
  strcpy(dirs->new_dir_paths, argv[2]);

  // char* err = strstr(dirs->new_dir_paths, dirs->old_dir_paths);
  if (strstr(dirs->new_dir_paths, dirs->old_dir_paths)) {
    printf("Do not make new dir in old dir!\n");
    free(dirs);
    exit(2);
  }

  // making new dir with the same permision mode as old dir
  struct stat stats;
  lstat(dirs->old_dir_paths, &stats);
  mode_t mode = stats.st_mode;

  if (mkdir(dirs->new_dir_paths, mode) != 0) {
    perror("Couldn't make new directory: \n");
    free(dirs);
    exit(3);
  }

  DirCP(dirs);
  pthread_exit(NULL);
}

void *FileCP(void *arg) {
  dir_paths *dirs = (dir_paths *)arg;
  char err_buf[ERROR_BUFFER_LENGTH];

  printf("Copying %s file...\n", dirs->old_dir_paths);

  int new_file = 0, old_file = 0;

  while (old_file <= 0) {
    old_file = open(dirs->old_dir_paths, O_RDONLY);
    printf("Opened %s\n", dirs->old_dir_paths);
    if (old_file == -1) {
      if (errno == EMFILE) {
        usleep(1000);
      } else {
        strerror_r(errno, err_buf, ERROR_BUFFER_LENGTH);
        printf("Couldn't open file: %s\n", err_buf);

        free(dirs);
        pthread_exit(NULL);
      }
    }
  }

  printf("Creating file...%s\n", dirs->new_dir_paths);

  struct stat stats;
  lstat(dirs->old_dir_paths, &stats);

  mode_t mode = stats.st_mode;

  while (new_file == 0) {
    new_file = creat(dirs->new_dir_paths, mode);
    if (new_file == -1) {
      if (errno == EMFILE) {
        usleep(1000);
      } else {
        strerror_r(errno, err_buf, ERROR_BUFFER_LENGTH);
        printf("Couldn't create file: %s\n", err_buf);

        free(dirs);
        //f
        close(old_file);
        pthread_exit(NULL);
      }
    }
  }

  printf("New file created. Deskriptor = %d\n", new_file);

  char data[FILE_DATA_BUFFER_LENGTH];
  int read_byte = read(old_file, data, FILE_DATA_BUFFER_LENGTH);
  while (read_byte > 0) {
    write(new_file, data, read_byte);
    read_byte = read(old_file, data, FILE_DATA_BUFFER_LENGTH);
  }

  close(old_file);
  close(new_file);
  free(dirs);

  pthread_exit(NULL);
}

void *DirCP(void *arg) {
  printf("do[%s]:%d\n", __func__, __LINE__);
  dir_paths *dirs = (dir_paths *)arg;

  printf("\toldDir - %s\n\tnewDir = %s\n", dirs->old_dir_paths,
         dirs->new_dir_paths);

  DIR *ODir_fd = NULL;
  char err_buf[ERROR_BUFFER_LENGTH];

  while (ODir_fd == NULL) {
    ODir_fd = opendir(dirs->old_dir_paths); // opened directory
    printf("opened dir\n");
    if (ODir_fd == NULL) {
      if (errno == EMFILE) {
        usleep(1000);
      } else {
        strerror_r(errno, err_buf, ERROR_BUFFER_LENGTH);
        printf("Couldn't open directory: %s\n", err_buf);

        //close(ODir_fd);
        free(dirs);
        pthread_exit(NULL);
      }
    }
  }

  struct dirent *cur_file;
  char entry_buffer[sizeof(struct dirent) +
                    pathconf(dirs->old_dir_paths, PATH_LENGTH) + 1];
  struct dirent *entry = (struct dirent *)entry_buffer;

  printf("Created buffers\n");
  printf("do[%s]:%d - ", __func__, __LINE__);
  printf("[pid:%d, tid:%d]\n", getpid(), gettid());

  // while(cur_file != NULL) // reading files from directory except "." and ".."
  do {
    int dir = readdir_r(ODir_fd, entry, &cur_file);
    if (dir != 0) {
      strerror_r(errno, err_buf, ERROR_BUFFER_LENGTH);
      printf("Couldn't open directory: %s\n", err_buf);
      //f
      closedir(ODir_fd);
      free(dirs);
      pthread_exit(NULL);
    }
    if (cur_file == NULL)
      break;

    if (strcmp(cur_file->d_name, ".") == 0 ||
        strcmp(cur_file->d_name, "..") == 0) {
      continue;
    }

    printf("Curent file is %s\n", cur_file->d_name);

    char tmp[] = "%s/%s";
    char old_filename[FILE_NAME_LENGTH];
    char new_filename[FILE_NAME_LENGTH];
    sprintf(old_filename, tmp, dirs->old_dir_paths, cur_file->d_name);
    sprintf(new_filename, tmp, dirs->new_dir_paths, cur_file->d_name);

    struct stat file_stats;

    lstat(old_filename, &file_stats);
    if (S_ISDIR(file_stats.st_mode)) {
      printf("[cur_file != NULL]do[%s]:%d\n", __func__, __LINE__);
      printf("Curent file is dir!\n");

      dir_paths *new_dir_pathss = (dir_paths *)malloc(sizeof(dir_paths));
      strcpy(new_dir_pathss->old_dir_paths, old_filename);
      strcpy(new_dir_pathss->new_dir_paths, new_filename);
      printf("do:%d\n", __LINE__);
      ThreadDirCP(new_dir_pathss);
    } else if (S_ISREG(file_stats.st_mode)) {
      printf("Curent file is reg!\n");

      dir_paths *new_dir_pathss = (dir_paths *)malloc(sizeof(dir_paths));
      strcpy(new_dir_pathss->old_dir_paths, old_filename);
      strcpy(new_dir_pathss->new_dir_paths, new_filename);

      ThreadFileCP(new_dir_pathss);
    } else {
      continue;
    }
  } while (cur_file != NULL);

  closedir(ODir_fd);
  free(dirs);
  pthread_exit(NULL);
}

void ThreadFileCP(dir_paths *dirs) {
  pthread_t tid;
  pthread_attr_t attrs;
  char err_buf[ERROR_BUFFER_LENGTH];

  pthread_attr_init(&attrs);
  pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);

  int err = pthread_create(&tid, &attrs, FileCP, dirs);
  printf("Started file copy!\n");
  if (err != 0) {
    strerror_r(err, err_buf, ERROR_BUFFER_LENGTH);
    printf("Couldn't create new thread: %s\n", err_buf);

    free(dirs);
    pthread_exit(NULL);
  }

  pthread_attr_destroy(&attrs);
}

void ThreadDirCP(dir_paths *dirs) {
  printf("do[%s]:%d\n", __func__, __LINE__);
  struct stat stats;

  lstat(dirs->old_dir_paths, &stats);
  mode_t mode = stats.st_mode;

  if (mkdir(dirs->new_dir_paths, mode) != 0) {
    printf("Couldn't create new dir! paths - %s\n", dirs->new_dir_paths);
    pthread_exit(NULL);
  }
  printf("New dir created\n");

  pthread_t tid;
  pthread_attr_t attrs;
  char err_buf[ERROR_BUFFER_LENGTH];

  pthread_attr_init(&attrs);
  pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
  printf("do[%s]:%d\n", __func__, __LINE__);
  int err = pthread_create(&tid, &attrs, DirCP, dirs);
  if (err != 0) {
    strerror_r(err, err_buf, ERROR_BUFFER_LENGTH);
    printf("Couldn't create new thread: %s\n", err_buf);

    free(dirs);
    pthread_exit(NULL);
  }

  //free(dirs);
  pthread_attr_destroy(&attrs);
  printf("do[%s]:%d\n", __func__, __LINE__);
}