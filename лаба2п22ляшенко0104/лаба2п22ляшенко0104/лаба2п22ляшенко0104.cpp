#include <windows.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

// Function for Task 2 (variant 10)
void saveFileListToFile(LPCWSTR directoryPath, LPCWSTR outputFileName) {
    wcout << L"\nTASK 2: Saving file list to " << outputFileName << L"" << endl;

    wstring searchPath = directoryPath;
    if (searchPath.back() != L'\\') {
        searchPath += L"\\";
    }
    searchPath += L"*";

    HANDLE hOutputFile = CreateFile(outputFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hOutputFile == INVALID_HANDLE_VALUE) {
        DWORD error = GetLastError();
        wcerr << L"Error creating file " << outputFileName << L". Error code: " << error << endl;
        return;
    }

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        string header = "File list in directory: ";
        header += string(directoryPath, directoryPath + wcslen(directoryPath));
        header += "\r\n\r\n";
        DWORD bytesWritten;
        WriteFile(hOutputFile, header.c_str(), header.length(), &bytesWritten, NULL);

        int fileCount = 0;
        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                string fileInfo = string(findFileData.cFileName, findFileData.cFileName + wcslen(findFileData.cFileName));
                fileInfo += " (";
                fileInfo += to_string(findFileData.nFileSizeLow);
                fileInfo += " bytes)\r\n";

                WriteFile(hOutputFile, fileInfo.c_str(), fileInfo.length(), &bytesWritten, NULL);
                fileCount++;
            }
        } while (FindNextFile(hFind, &findFileData) != 0);

        string footer = "\r\nTotal files: ";
        footer += to_string(fileCount);
        footer += "\r\n";
        WriteFile(hOutputFile, footer.c_str(), footer.length(), &bytesWritten, NULL);

        FindClose(hFind);
        CloseHandle(hOutputFile);

        wcout << L"File list saved to " << outputFileName << endl;
        cout << "Files found: " << fileCount << endl;
    }
    else {
        DWORD error = GetLastError();
        wcerr << L"Error searching files in " << directoryPath << L". Error code: " << error << endl;
        CloseHandle(hOutputFile);
    }
}

int main() {
    SetConsoleOutputCP(1251);

    // All paths
    LPCWSTR directoryName = L"C:\\P22_Lyashenko";
    LPCWSTR fileName = L"C:\\P22_Lyashenko\\lyashenko.txt";
    LPCWSTR newFileName = L"C:\\P22_Lyashenko\\lyashenkoNA.txt";
    LPCWSTR diskRoot = L"C:\\";

    // TASK 1

    cout << "1. Creating directory" << endl;
    if (CreateDirectory(directoryName, NULL)) {
        wcout << L"Directory '" << directoryName << L"' created" << endl;
    }
    else {
        DWORD error = GetLastError();
        if (error == ERROR_ALREADY_EXISTS) {
            wcout << L"Directory '" << directoryName << L"' already exists" << endl;
        }
        else {
            wcerr << L"Directory creation error. Code: " << error << endl;
        }
    }
    cin.get();

    cout << "2. Creating file" << endl;
    HANDLE hFile = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        auto now = chrono::system_clock::now();
        time_t now_time = chrono::system_clock::to_time_t(now);
        tm timeinfo = {};
        localtime_s(&timeinfo, &now_time);

        stringstream ss;
        ss << put_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
        string timeStr = ss.str();
        const char* data = timeStr.c_str();
        DWORD bytesWritten;

        if (WriteFile(hFile, data, strlen(data), &bytesWritten, NULL)) {
            wcout << L"File '" << fileName << L"' created." << endl;
            cout << "Written " << bytesWritten << " bytes." << endl;
        }
        else {
            DWORD error = GetLastError();
            cerr << "Write error. Code: " << error << endl;
        }

        CloseHandle(hFile);
    }
    else {
        DWORD error = GetLastError();
        if (error == ERROR_FILE_EXISTS) {
            wcout << L"File '" << fileName << L"' already exists." << endl;
        }
        else {
            wcerr << L"File creation error '" << fileName << L"'. Code: " << error << endl;
        }
    }
    cin.get();

    cout << "3. Reading file" << endl;
    HANDLE hReadFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hReadFile != INVALID_HANDLE_VALUE) {
        char buffer[1024];
        DWORD bytesRead;

        if (ReadFile(hReadFile, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                cout << "Read " << bytesRead << " bytes: " << buffer << endl;
            }
            else {
                cout << "File is empty." << endl;
            }
        }
        else {
            DWORD error = GetLastError();
            cerr << "Read error. Code: " << error << endl;
        }

        CloseHandle(hReadFile);
    }
    else {
        DWORD error = GetLastError();
        wcerr << L"File open error. Code: " << error << endl;
    }
    cin.get();

    cout << "4. Renaming file" << endl;
    if (MoveFile(fileName, newFileName)) {
        wcout << L"File renamed to '" << newFileName << L"'." << endl;
    }
    else {
        DWORD error = GetLastError();
        wcerr << L"Rename error. Code: " << error << endl;
    }
    cin.get();

    cout << "5. Getting file info" << endl;
    HANDLE hInfoFile = CreateFile(newFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hInfoFile != INVALID_HANDLE_VALUE) {
        BY_HANDLE_FILE_INFORMATION fileInfo;
        if (GetFileInformationByHandle(hInfoFile, &fileInfo)) {
            wcout << L"File name: " << newFileName << endl;
            cout << "File size: " << fileInfo.nFileSizeLow << " bytes" << endl;
        }
        else {
            DWORD error = GetLastError();
            cerr << "File info error. Code: " << error << endl;
        }
        CloseHandle(hInfoFile);
    }
    else {
        DWORD error = GetLastError();
        wcerr << L"File open error. Code: " << error << endl;
    }
    cin.get();

    cout << "6. Deleting file" << endl;
    if (DeleteFile(newFileName)) {
        wcout << L"File '" << newFileName << L"' deleted." << endl;
    }
    else {
        DWORD error = GetLastError();
        wcerr << L"Delete error '" << newFileName << L"'. Code: " << error << endl;
    }
    cin.get();

    cout << "7. Deleting directory" << endl;
    if (RemoveDirectory(directoryName)) {
        wcout << L"Directory '" << directoryName << L"' deleted." << endl;
    }
    else {
        DWORD error = GetLastError();
        wcerr << L"Directory delete error. Code: " << error << endl;
    }
    cin.get();

    cout << "8. Listing files and directories in C:\\" << endl;
    wstring searchPath = wstring(diskRoot) + L"*";
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        int dirCount = 0, fileCount = 0;
        do {
            if (wcscmp(findFileData.cFileName, L".") != 0 && wcscmp(findFileData.cFileName, L"..") != 0) {
                if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    wcout << L"[DIR] " << findFileData.cFileName << endl;
                    dirCount++;
                }
                else {
                    wcout << L"[FILE] " << findFileData.cFileName;
                    cout << " (" << findFileData.nFileSizeLow << " bytes)" << endl;
                    fileCount++;
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0);

        FindClose(hFind);
        cout << "\nTotal: " << dirCount << " directories, " << fileCount << " files" << endl;
    }
    else {
        DWORD error = GetLastError();
        wcerr << L"File search error. Code: " << error << endl;
    }
    cin.get();

    // TASK 2 (variant 10)
    cout << "9. Starting Task 2 - Saving file list" << endl;
    LPCWSTR outputFile = L"C:\\file_list.txt";
    saveFileListToFile(diskRoot, outputFile);

    cout << "\nAll tasks completed. Press Enter to exit" << endl;
    cin.get();

    return 0;
}
