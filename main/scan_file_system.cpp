#include "scan_file_system.h"
#include <iostream>
#include <Windows.h>
#include <filesystem>

scan_file_system::scan_file_system() {}

scan_file_system::~scan_file_system() {}

void scan_file_system::ScanFileSystem(const std::string& directory) {
    std::string searchPath = directory + "\\*"; // формируем путь для поиска файлов и директорий
    WIN32_FIND_DATAA fileInfo; // используем для взятия информации о файле
    HANDLE searchHandle = FindFirstFileA(searchPath.c_str(), &fileInfo); // указатель на дескриптор 
    if (searchHandle != INVALID_HANDLE_VALUE) // проверка успешного открытия поиска
        // до недействительного значения дескриптора
        {
            do
            {
                std::string fileName = fileInfo.cFileName; // получение имени файла из структуры WIN32_FIND_DATAA
                if (!(fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) // проверка того, что это не папка
                {
                    std::string filePath = directory + "\\" + fileName; // путь
                    ULONGLONG fileSize = static_cast<ULONGLONG>(fileInfo.nFileSizeLow) | (static_cast<ULONGLONG>(fileInfo.nFileSizeHigh) << 32); // для получения размера
                    // ULONGLONG позволяет работать с размерами файлов >4 Гб
                    // проверка расширений файлов
                    if (fileName.substr(fileName.length() - 5) == ".docx" || //  начинаем с 5 символа
                        fileName.substr(fileName.length() - 5) == ".xlsx" ||
                        fileName.substr(fileName.length() - 4) == ".pdf" ||
                        fileName.substr(fileName.length() - 4) == ".jpg" ||
                        fileName.substr(fileName.length() - 4) == ".txt" ||
                        fileName.substr(fileName.length() - 4) == ".rar" ||
                        fileName.substr(fileName.length() - 4) == ".zip")
                    {
                        std::ifstream file(filePath, std::ios::binary); // путь в бинарном формате
                        if (file)
                        {
                            char signature[4]; // массив для хранения сигнатуры
                            file.read(signature, 4); // чтение первых 4 байт файла
                            file.close();
                            // сохраняем 4 символов, потом сравниваем их с начальной сигнатурой
                            // РК - первые 2 символа сигнатуры
                            // х03\х04 - вторые 2 символа форматов
                            if (std::string(signature, 4) != "PK\x03\x04") // Проверка сигнатуры файла
                            {
                                std::cout << "[Поврежденный файл] " << filePath << " (" << fileSize << " bytes)" << std::endl;
                            }
                            else
                            {
                                std::cout << "[Файл] " << filePath << " (" << fileSize << " bytes)" << std::endl;
                            }
                        }
                    }
                }
                else // если это директория
                {
                    if (fileName != "." && fileName != "..") // . - текущий каталог, .. - родительский каталог
                    {
                        std::string subdirectory = directory + "\\" + fileName; // путь к папке
                        std::cout << "[Папка] " << subdirectory << std::endl;
                        ScanFileSystem(subdirectory); // рекурсивный вызов функции для сканирования поддиректорий
                    }
                }
            } while (FindNextFileA(searchHandle, &fileInfo)); // поиск следующего файла/директории
            FindClose(searchHandle); // закрытие дескриптора для освобод ресов
        }
}

bool scan_file_system::IsDriveAccessible(char driveLetter)
{
    std::string rootPath = std::string(1, driveLetter) + ":\\";
    UINT driveType = GetDriveTypeA(rootPath.c_str()); // получение типа диска
    // для получения типа диска
    // UINT - безнаковое положительно число

    if (driveType == DRIVE_UNKNOWN || driveType == DRIVE_NO_ROOT_DIR) // тип диска неизвестен || не корневой каталог
    {
        std::cout << "При анализе диска '" << driveLetter << "' возникла ошибка." << std::endl << std::endl;
        return false;
    }
    else
    {
        std::cout << "Диск '" << driveLetter << "': доступен." << std::endl << std::endl;
        return true;
    }
}
