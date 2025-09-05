#include <windows.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

int main() {
    // создание путей файлов и директорий
    LPCWSTR directoryName = L"C:\\Users\\student\\Desktop\\directoryLyash\\P22";
    LPCWSTR fileName = L"C:\\Users\\student\\Desktop\\directoryLyash\\P22\\lyashenko.txt";

    // создание директории
    if (CreateDirectory(directoryName, NULL)) {
        cout << "Directory '" << directoryName << "' successfully created" << endl;
    }
    else {
        DWORD error = GetLastError();
        if (error == ERROR_ALREADY_EXISTS) {
            cerr << "Directory '" << directoryName << "' is already exists" << endl;
        }
        else {
            cerr << "Directory creating error. Error code: " << error << endl;
        }
    }

    // стоп
    cin.get();

    // создание файла внутри созданной папки
    HANDLE hFile = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {

        // получаем текущее время
        auto now = chrono::system_clock::now();
        time_t now_time = chrono::system_clock::to_time_t(now);
        tm timeinfo = {};
        localtime_s(&timeinfo, &now_time);

        // форматирование времени в const char*
        stringstream ss;
        ss << put_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
        string timeStr = ss.str();
        const char* data = timeStr.c_str();
        DWORD bytesWritten;

        // стоп
        cin.get();

        // запись времени в файл
        if (WriteFile(hFile, data, strlen(data), &bytesWritten, NULL)) {
            cout << "File '" << fileName << "' successfully created." << endl;
            cout << "Written " << bytesWritten << " bytes." << endl;
        }
        else {
            DWORD error = GetLastError();
            cerr << "Write error. Error code: " << error << endl;
        }

        CloseHandle(hFile);
    }
    else {
        DWORD error = GetLastError();
        if (error == ERROR_FILE_EXISTS) {
            cout << "File '" << fileName << "' is already exists." << endl;
        }
        else {
            cerr << "Failed to create file '" << fileName << " '. Error" << error << endl;
        }
    }

    // стоп
    cin.get();

    // чтение файла
    cout << "\nReading file content:" << endl;

    HANDLE hReadFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hReadFile != INVALID_HANDLE_VALUE) {
        char buffer[1024];
        DWORD bytesRead;

        if (ReadFile(hReadFile, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            
            if (bytesRead > 0) {
                // нулевой терминатор
                buffer[bytesRead] = '\0';

                cout << "Read " << bytesRead << " bytes: " << buffer << endl;
            }
            else {
                cout << "File is empty." << endl;
            }
        }
        else {
            // обработка ошибки чтения
            DWORD error = GetLastError();
            cerr << "Read error. Error code: " << error << endl;
        }

        CloseHandle(hReadFile);
    }
    else {
        DWORD error = GetLastError();
        cerr << "Cannot open file for reading. Error code: " << error << endl;
    }

    // стоп 
    cin.get();

    // переименование файла
    LPCWSTR newFileName = L"C:\\Users\\student\\Desktop\\directoryLyash\\newDirectory\\lyashenkoNA.txt";

    if (MoveFile(fileName, newFileName)) {
        cout << "File successfully renamed to '" << newFileName << " '." << endl;
    }
    else {
        DWORD error = GetLastError();
        cerr << "Cannot rename the file. Error code: " << error << endl;
    }

    // стоп
    cin.get();

    // информация о файле

    HANDLE hReadInfoFile = CreateFile(newFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        BY_HANDLE_FILE_INFORMATION fileInfo;
        if (GetFileInformationByHandle(hReadInfoFile, &fileInfo)) {
            cout << "File name: " << newFileName << endl;
            cout << "File size: " << fileInfo.nFileSizeLow << " byte" << endl;
        }
        else {
            DWORD error = GetLastError();
            cerr << "Cannot get info about file. Error code: " << error << endl;
        }
        CloseHandle(hReadInfoFile);
    }
    else {
        DWORD error = GetLastError();
        cerr << "Cannot open file. Error code: " << error << endl;
    }

    //стоп 
    cin.get();

    // удаление
    if (DeleteFile(newFileName)) {
        cout << "File '" << newFileName << "' successfully deleted." << endl;
    }
    else {
        DWORD error = GetLastError();
        cerr << "Cannot delete file '" << newFileName << " '. Error code:" << error << endl;
    }

    // стоп
    cin.get();

    //удаление каталога
    if (RemoveDirectory(directoryName)) {
        cout << "Directory '" << directoryName << "' succesfully deleted." << endl;
    }
    else {
        DWORD error = GetLastError();
        cerr << "Cannot delete directory. Error code: " << error << endl;
    }

    // показ всех каталогов в папке targerDirectory
    LPCWSTR targetDirectory = L"C:\\testp22\\*";

    cout << "\nDirectories in C:\\testp22" << endl;
    //стоп
    cin.get();

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(targetDirectory, &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        int dirCount = 0;
        do {
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (wcscmp(findFileData.cFileName, L".") != 0 &&
                    wcscmp(findFileData.cFileName, L"..") != 0) {
                    wcout << L"[" << ++dirCount << L"] " << findFileData.cFileName << endl;
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0);

        FindClose(hFind);
        cout << "Total directories found: " << dirCount << endl;
    }
    else {
        DWORD error = GetLastError();
        wcerr << L"Cannot enumerate directories in: " << targetDirectory << L". Error code: " << error << endl;
    }

    return 0;
}
