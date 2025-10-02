#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int fd;
    char buffer[256];
    char filename[] = "example.txt";
    char text[] = "Привет, мир!\n";

    printf("Выполнение лабораторной работы №5\n\n");
    
    printf("Создание файла:\n");
    fd = open(filename, O_CREAT | O_WRONLY, 0644);
    if (fd == -1) {
        perror("Ошибка при создании файла");
        return 1;
    }
    printf("   Файл '%s' успешно создан\n", filename);
    close(fd);

    getchar();
    
    printf("\nЗапись в файл:\n");
    fd = open(filename, O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("Ошибка при открытии файла для записи");
        return 1;
    }
    
    if (write(fd, text, strlen(text)) == -1) {
        perror("Ошибка при записи");
        close(fd);
        return 1;
    }
    printf("   Данные успешно записаны в файл\n");
    printf("   Записано: %s", text);
    close(fd);

    getchar();
    
    printf("\nЧтение файла:\n");
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Ошибка при открытии файла для чтения");
        return 1;
    }
    
    int bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        perror("Ошибка при чтении");
        close(fd);
        return 1;
    }
    
    buffer[bytes_read] = '\0';
    printf("   Прочитано %d байт:\n", bytes_read);
    printf("   Содержимое: %s\n", buffer);
    close(fd);

    getchar();
    
    printf("\nИзменение размера файла:\n");
    fd = open(filename, O_RDWR);
    if (fd == -1) {
        perror("Ошибка при открытии файла для изменения размера");
        return 1;
    }
    
    off_t current_size = lseek(fd, 0, SEEK_END);
    printf("   Текущий размер файла: %ld байт\n", current_size);
    
    if (ftruncate(fd, 10) == -1) {
        perror("Ошибка при изменении размера");
        close(fd);
        return 1;
    }

    off_t new_size = lseek(fd, 0, SEEK_END);
    printf("   Новый размер файла: %ld байт\n", new_size);
    
    lseek(fd, 0, SEEK_SET);
    bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    buffer[bytes_read] = '\0';
    printf("   Содержимое после усечения: '%s'\n", buffer);
    close(fd);
    
    printf("\nУдаление файла:\n");
    printf("   Нажмите любую кнопку");
    getchar();
    
    if (unlink(filename) == -1) {
        perror("Ошибка при удалении файла");
        return 1;
    }
    
    printf("   Файл '%s' успешно удален\n", filename);
    
    printf("\nПроверка удаления:\n");
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        printf("   Файл действительно удален\n");
    } else {
        printf("   Ошибка: файл все еще существует!\n");
        close(fd);
    }
    
    return 0;
}
