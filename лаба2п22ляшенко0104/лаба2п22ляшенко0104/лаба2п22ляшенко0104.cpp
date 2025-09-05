#include <windows.h>
#include <iostream>
#include <string>
#include <ctime>
#include <vector>

// Функция для преобразования std::string в std::wstring
std::wstring stringToWideString(const std::string& str) {
    if (str.empty()) return L"";
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr[0], size_needed);
    return wstr;
}

int main() {
    SetConsoleOutputCP(1251);

    // ЗАДАНИЕ 1
    std::cout << "=== ВЫПОЛНЕНИЕ ЗАДАНИЯ 1 ===" << std::endl;

    // Создаем каталог с названием группы
    std::wstring groupDir = L"P22";
    CreateDirectoryW(groupDir.c_str(), NULL);
    std::wcout << L"Создан каталог: " << groupDir << std::endl;

    // Создаем файл с фамилией
    std::wstring surnameFile = L"Lyshenko.txt";
    std::wstring fullPath = groupDir + L"\\" + surnameFile;

    // Записываем текущую дату в файл
    HANDLE hFile = CreateFileW(fullPath.c_str(), GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    std::cout << "1";

    if (hFile != INVALID_HANDLE_VALUE) {
        // Получаем текущую дату
        time_t now = time(0);
        char dt[26];
        ctime_s(dt, sizeof(dt), &now);

        // Преобразуем дату в широкий формат
        std::wstring wdt = stringToWideString(dt);

        std::cout << "2";

        DWORD bytesWritten;
        // Записываем данные в файл
        BOOL writeResult = WriteFile(hFile, wdt.c_str(),
            static_cast<DWORD>(wdt.size() * sizeof(wchar_t)),
            &bytesWritten, NULL);
        std::cout << "3";
        if (!writeResult) {
            std::wcout << L"Ошибка записи в файл: " << GetLastError() << std::endl;
        }
        CloseHandle(hFile);
        std::wcout << L"Создан файл с датой: " << fullPath << std::endl;
        std::cout << "4";
    }

    // Читаем и выводим содержимое файла
    hFile = CreateFileW(fullPath.c_str(), GENERIC_READ, 0, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile != INVALID_HANDLE_VALUE) {
        wchar_t buffer[100];
        DWORD bytesRead;
        // Читаем данные из файла
        BOOL readResult = ReadFile(hFile, buffer, sizeof(buffer) - sizeof(wchar_t),
            &bytesRead, NULL);
        if (readResult && bytesRead > 0) {
            // Добавляем нулевой терминатор
            buffer[bytesRead / sizeof(wchar_t)] = L'\0';
            std::wcout << L"Содержимое файла: " << buffer << std::endl;
        }
        else {
            std::wcout << L"Ошибка чтения файла или файл пуст" << std::endl;
        }
        CloseHandle(hFile);
    }

    // Переименовываем файл
    std::wstring newName = groupDir + L"\\Lyashenko_NA.txt";
    if (MoveFileW(fullPath.c_str(), newName.c_str())) {
        std::wcout << L"Файл переименован: " << newName << std::endl;
    }
    else {
        std::wcout << L"Ошибка переименования файла" << std::endl;
    }

    // Определяем размер файла
    WIN32_FILE_ATTRIBUTE_DATA fileInfo;
    if (GetFileAttributesExW(newName.c_str(), GetFileExInfoStandard, &fileInfo)) {
        std::wcout << L"Размер файла: " << fileInfo.nFileSizeLow << L" байт" << std::endl;
    }
    else {
        std::wcout << L"Ошибка получения размера файла" << std::endl;
    }

    // Удаляем файл и каталог
    if (DeleteFileW(newName.c_str()) && RemoveDirectoryW(groupDir.c_str())) {
        std::wcout << L"Файл и каталог удалены" << std::endl;
    }
    else {
        std::wcout << L"Ошибка удаления файла или каталога" << std::endl;
    }

    // Выводим список файлов и каталогов на диске C:
    std::wcout << L"\nСодержимое диска C:\\:" << std::endl;
    WIN32_FIND_DATAW findData;
    HANDLE hFind = FindFirstFileW(L"C:\\*", &findData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::wcout << findData.cFileName << std::endl;
        } while (FindNextFileW(hFind, &findData) != 0);
        FindClose(hFind);
    }

    // ЗАДАНИЕ 2 (10 вариант)
    std::wcout << L"\n=== ВЫПОЛНЕНИЕ ЗАДАНИЯ 2 ===" << std::endl;

    // Создаем список файлов в текущей директории
    std::wstring outputFile = L"file_list.txt";
    HANDLE hOutput = CreateFileW(outputFile.c_str(), GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hOutput != INVALID_HANDLE_VALUE) {
        hFind = FindFirstFileW(L"*", &findData);
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                    DWORD bytesWritten;
                    std::wstring fileName = std::wstring(findData.cFileName) + L"\r\n";
                    // Записываем данные в файл
                    BOOL writeResult = WriteFile(hOutput, fileName.c_str(),
                        static_cast<DWORD>(fileName.size() * sizeof(wchar_t)),
                        &bytesWritten, NULL);
                    if (!writeResult) {
                        std::wcout << L"Ошибка записи в файл списка" << std::endl;
                    }
                }
            } while (FindNextFileW(hFind, &findData) != 0);
            FindClose(hFind);
        }
        CloseHandle(hOutput);
        std::wcout << L"Список файлов сохранен в: " << outputFile << std::endl;
    }

    return 0;
}
