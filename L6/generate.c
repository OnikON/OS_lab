#include <stdio.h>

int main () {
    for (int i = 100; i > 0; i--) {
        for (int j = i; j > 0; j--) {
            printf("%d_",j);
        }
        printf("\n");
    }
}