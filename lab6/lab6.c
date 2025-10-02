#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>

#define SEM_KEY 1234

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main() {
    int pipefd[2];
    char buffer[100];
    pid_t pid;
    
    printf("Выполнение лабораторной работы №6\n\n");
    
    printf("Создание процесса:\n");
    pid = fork();
    
    if (pid < 0) {
        fprintf(stderr, "Ошибка при вызове fork()\n");
        return 1;
    } else if (pid == 0) {
        printf("   Дочерний процесс: PID = %d, PPID = %d\n", getpid(), getppid());
        exit(0);
    } else {
        printf("   Родительский процесс: PID = %d, создал дочерний с PID = %d\n", getpid(), pid);
        wait(NULL);
    }

    getchar();
    
    printf("\nВыполнение программы в процессе:\n");
    pid = fork();
    
    if (pid < 0) {
        fprintf(stderr, "Ошибка при вызове fork()\n");
        return 1;
    } else if (pid == 0) {
        printf("   Дочерний процесс выполняет команду 'ls -l':\n");
        execl("/bin/ls", "ls", "-l", NULL);
        perror("Ошибка при вызове execl");
        exit(1);
    } else {
        wait(NULL);
        printf("   Команда выполнена успешно\n");
    }

    getchar();
    
    printf("\nОбмен данными через канал:\n");
    
    if (pipe(pipefd) == -1) {
        perror("Ошибка при создании канала");
        return 1;
    }
    
    pid = fork();
    
    if (pid < 0) {
        fprintf(stderr, "Ошибка при вызове fork()\n");
        return 1;
    } else if (pid == 0) {
        close(pipefd[0]);
        char message[] = "Привет от дочернего процесса!";
        printf("   Дочерний процесс отправляет: %s\n", message);
        write(pipefd[1], message, strlen(message) + 1);
        close(pipefd[1]);
        exit(0);
    } else {
        close(pipefd[1]);
        wait(NULL);
        read(pipefd[0], buffer, sizeof(buffer));
        printf("   Родительский процесс получил: %s\n", buffer);
        close(pipefd[0]);
    }

    getchar();
    
    printf("\nСинхронизация семафорами:\n");
    
    int sem_id;
    struct sembuf sops;
    
    sem_id = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("Ошибка при создании семафора");
        return 1;
    }
    
    union semun arg;
    arg.val = 1;
    if (semctl(sem_id, 0, SETVAL, arg) == -1) {
        perror("Ошибка при установке значения семафора");
        return 1;
    }
    
    printf("   Семафор создан и установлен в 1\n");
    
    pid = fork();
    
    if (pid < 0) {
        fprintf(stderr, "Ошибка при вызове fork()\n");
        return 1;
    } else if (pid == 0) {
        sleep(1);
        printf("   Дочерний процесс пытается захватить семафор...\n");
        
        sops.sem_num = 0;
        sops.sem_op = -1;
        sops.sem_flg = 0;
        semop(sem_id, &sops, 1);
        
        printf("   Дочерний процесс захватил семафор и работает...\n");
        sleep(2);
        
        sops.sem_op = 1;
        semop(sem_id, &sops, 1);
        printf("   Дочерний процесс освободил семафор\n");
        
        exit(0);
    } else {
        printf("   Родительский процесс пытается захватить семафор...\n");
        
        sops.sem_num = 0;
        sops.sem_op = -1;
        sops.sem_flg = 0;
        semop(sem_id, &sops, 1);
        
        printf("   Родительский процесс захватил семафор и работает...\n");
        sleep(3);
        
        sops.sem_op = 1;
        semop(sem_id, &sops, 1);
        printf("   Родительский процесс освободил семафор\n");
        
        wait(NULL);
        
        if (semctl(sem_id, 0, IPC_RMID) == -1) {
            perror("Ошибка при удалении семафора");
        } else {
            printf("   Семафор удален\n");
        }
    }
    
    return 0;
}
