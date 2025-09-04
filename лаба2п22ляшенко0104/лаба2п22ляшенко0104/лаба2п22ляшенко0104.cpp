#include <windows.h>
#include <iostream>
#include <ctime>
using namespace std;
int main() {
	SetConsoleOutputCP(1251);
	LPCWSTR directoryName = L"C:\\P22\\popa"; // Замените путь и имя каталога по вашему усмотрению
	HANDLE hFile = CreateFile(L"C:\\P22\\popa\\lyashenko.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (CreateDirectory(directoryName, NULL)) {
			cout << "Каталог '" << directoryName << "' успешно создан." << endl;
	}
	else {
		DWORD error = GetLastError();
		if (error == ERROR_ALREADY_EXISTS) {
			cout << "Каталог '" << directoryName << "' уже существует." << endl;
		}
		else {
			cerr << "Не удалось создать каталог. Код ошибки: " << error << endl;
		}
	}
	std::cout << "Не удалось открыть файл" << std::endl;
	if (hFile != INVALID_HANDLE_VALUE) {
		char buffer[1024];
		DWORD bytesRead;
		if (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, NULL)) {
			time_t currentTime;
			time(&currentTime);
	
			const char* data = ctime(&currentTime);
			DWORD bytesWritten;
			if (WriteFile(hFile, data, strlen(data), &bytesWritten, NULL)) {
				// Запись прошла успешно
				std::cout << "Записано " << bytesWritten << " байт." << std::endl;
			}
			else {
				// Обработать ошибку записи
				DWORD error = GetLastError();
				std::cerr << "Ошибка записи в файл. Код ошибки: " << error <<
					std::endl;
			}
			std::cout << "Не удалось открыть файл" << std::endl;
		}
		std::cout << "Не удалось открыть файл" << std::endl;
		CloseHandle(hFile);
	}
	else {
		std::cout << "Не удалось открыть файл" << std::endl;
	}
	std::cout << "Не удалось открыть файл" << std::endl;
}


