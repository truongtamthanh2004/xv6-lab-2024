#include "kernel/types.h"
#include "user/user.h"

void sieve(int p_left[2]) __attribute__((noreturn));

void
sieve(int p_left[2]) {
    close(p_left[1]);
    int prime;

    if (read(p_left[0], &prime, sizeof(int)) == 0) {
        close(p_left[0]);
        exit(0);
    }

    printf("prime %d\n", prime);

    int p_right[2];
    pipe(p_right);

    if (fork() == 0) {
        close(p_left[0]);
        sieve(p_right);
    } else {
        close(p_right[0]);

        int num;
        while (read(p_left[0], &num, sizeof(int)) != 0) {
            if (num % prime != 0) {
                write(p_right[1], &num, sizeof(int));
            }
        }

        close(p_left[0]);
        close(p_right[1]);
        wait(0);
        exit(0);
    }
}

int
main(int argc, char *argv[]) {
    int p[2]; 
    pipe(p);

    if (fork() == 0) {
        sieve(p);
    } else {
        close(p[0]);
        for (int i = 2; i <= 280; i++) {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        wait(0);
    }

    exit(0);
}
