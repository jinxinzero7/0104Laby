#include <windows.h>
#include <iostream>
#include <string>
#include <tlhelp32.h>

using namespace std;

// Simple function for our thread
DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    cout << "Thread is running with parameter: " << *(int*)lpParam << endl;
    for (int i = 0; i < 5; i++) {
        cout << "Thread working... " << i << endl;
        Sleep(1000);
    }
    cout << "Thread finished" << endl;
    return 0;
}

int main() {
    SetConsoleOutputCP(1251);

    // get current process info
    cout << "1. Getting current process info" << endl;
    HANDLE currentProcess = GetCurrentProcess();
    DWORD processId = GetCurrentProcessId();
    DWORD priority = GetPriorityClass(currentProcess);

    cout << "Current Process ID: " << processId << endl;
    cout << "Current Priority: " << priority << endl;
    cin.get();

    // change process priority
    cout << "2. Changing process priority" << endl;
    if (SetPriorityClass(currentProcess, IDLE_PRIORITY_CLASS)) {
        cout << "Priority changed to IDLE" << endl;
    }
    else {
        cout << "Failed to change priority" << endl;
    }
    cin.get();

    // create a new process
    cout << "3.Creating new process (notepad)" << endl;

    STARTUPINFO startupInfo;
    PROCESS_INFORMATION processInfo;

    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    ZeroMemory(&processInfo, sizeof(processInfo));

    wchar_t notepadCommand[] = L"notepad.exe";

    // start notepad
    BOOL result = CreateProcessW(
        NULL,
        notepadCommand,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &startupInfo,
        &processInfo
    );

    if (result) {
        cout << "Notepad started with PID: " << processInfo.dwProcessId << endl;
        cout << "Notepad will close automatically in 3 seconds..." << endl;
        Sleep(3000);

        // terminate notepad
        TerminateProcess(processInfo.hProcess, 0);
        cout << "Notepad terminated" << endl;

        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
    }
    else {
        cout << "Failed to start notepad" << endl;
    }
    cin.get();

    // create threads
    cout << "4. Creating threads..." << endl;

    int threadData = 42;
    HANDLE hThread = CreateThread(
        NULL,
        0,
        ThreadFunction,
        &threadData,
        0,
        NULL
    );

    if (hThread != NULL) {
        cout << "Thread created, waiting for completion..." << endl;
        WaitForSingleObject(hThread, INFINITE);

        DWORD exitCode;
        GetExitCodeThread(hThread, &exitCode);
        cout << "Thread exit code: " << exitCode << endl;

        CloseHandle(hThread);
    }
    else {
        cout << "Failed to create thread" << endl;
    }
    cin.get();

    // process listing
    cout << "5. Listing running processes" << endl;

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W processEntry;
        processEntry.dwSize = sizeof(PROCESSENTRY32W);

        if (Process32FirstW(hSnapshot, &processEntry)) {
            int count = 0;
            do {
                if (count < 10) { // Show only first 10 processes
                    wcout << L"PID: " << processEntry.th32ProcessID
                        << L" | Name: " << processEntry.szExeFile << endl;
                }
                count++;
            } while (Process32NextW(hSnapshot, &processEntry) && count < 10);

            cout << "... and " << (count - 10) << " more processes" << endl;
        }
        CloseHandle(hSnapshot);
    }
    cin.get();

    // synchronization with mutex
    cout << "6. Demonstrating mutex synchronization" << endl;

    HANDLE hMutex = CreateMutexW(NULL, FALSE, NULL);

    if (hMutex != NULL) {
        cout << "Acquiring mutex..." << endl;
        WaitForSingleObject(hMutex, INFINITE);
        cout << "Mutex acquired - critical section" << endl;
        Sleep(2000);
        cout << "Releasing mutex" << endl;
        ReleaseMutex(hMutex);
        CloseHandle(hMutex);
    }

    cin.get();

    cout << "7. Creating process with command line arguments..." << endl;
    wchar_t commandLine[] = L"cmd.exe /c echo Hello from child process && timeout 2";

    result = CreateProcessW(
        NULL,
        commandLine,
        NULL,
        NULL,
        FALSE,
        CREATE_NO_WINDOW,
        NULL,
        NULL,
        &startupInfo,
        &processInfo
    );

    if (result) {
        cout << "Child process started, waiting..." << endl;
        WaitForSingleObject(processInfo.hProcess, INFINITE);

        DWORD exitCode;
        GetExitCodeProcess(processInfo.hProcess, &exitCode);
        cout << "Child process exit code: " << exitCode << endl;

        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
    }
    else {
        cout << "Failed to start child process" << endl;
    }

    cout << endl << "all tasks completed <3" << endl;

    cin.get();

    return 0;
}