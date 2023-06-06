#include "scan_file_system.h"
#include <iostream>
#include <Windows.h>
#include <filesystem>

scan_file_system::scan_file_system() {}

scan_file_system::~scan_file_system() {}

void scan_file_system::ScanFileSystem(const std::string& directory) {
    std::string searchPath = directory + "\\*"; // ��������� ���� ��� ������ ������ � ����������
    WIN32_FIND_DATAA fileInfo; // ���������� ��� ������ ���������� � �����
    HANDLE searchHandle = FindFirstFileA(searchPath.c_str(), &fileInfo); // ��������� �� ���������� 
    if (searchHandle != INVALID_HANDLE_VALUE) // �������� ��������� �������� ������
        // �� ����������������� �������� �����������
        {
            do
            {
                std::string fileName = fileInfo.cFileName; // ��������� ����� ����� �� ��������� WIN32_FIND_DATAA
                if (!(fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) // �������� ����, ��� ��� �� �����
                {
                    std::string filePath = directory + "\\" + fileName; // ����
                    ULONGLONG fileSize = static_cast<ULONGLONG>(fileInfo.nFileSizeLow) | (static_cast<ULONGLONG>(fileInfo.nFileSizeHigh) << 32); // ��� ��������� �������
                    // ULONGLONG ��������� �������� � ��������� ������ >4 ��
                    // �������� ���������� ������
                    if (fileName.substr(fileName.length() - 5) == ".docx" || //  �������� � 5 �������
                        fileName.substr(fileName.length() - 5) == ".xlsx" ||
                        fileName.substr(fileName.length() - 4) == ".pdf" ||
                        fileName.substr(fileName.length() - 4) == ".jpg" ||
                        fileName.substr(fileName.length() - 4) == ".txt" ||
                        fileName.substr(fileName.length() - 4) == ".rar" ||
                        fileName.substr(fileName.length() - 4) == ".zip")
                    {
                        std::ifstream file(filePath, std::ios::binary); // ���� � �������� �������
                        if (file)
                        {
                            char signature[4]; // ������ ��� �������� ���������
                            file.read(signature, 4); // ������ ������ 4 ���� �����
                            file.close();
                            // ��������� 4 ��������, ����� ���������� �� � ��������� ����������
                            // �� - ������ 2 ������� ���������
                            // �03\�04 - ������ 2 ������� ��������
                            if (std::string(signature, 4) != "PK\x03\x04") // �������� ��������� �����
                            {
                                std::cout << "[������������ ����] " << filePath << " (" << fileSize << " bytes)" << std::endl;
                            }
                            else
                            {
                                std::cout << "[����] " << filePath << " (" << fileSize << " bytes)" << std::endl;
                            }
                        }
                    }
                }
                else // ���� ��� ����������
                {
                    if (fileName != "." && fileName != "..") // . - ������� �������, .. - ������������ �������
                    {
                        std::string subdirectory = directory + "\\" + fileName; // ���� � �����
                        std::cout << "[�����] " << subdirectory << std::endl;
                        ScanFileSystem(subdirectory); // ����������� ����� ������� ��� ������������ �������������
                    }
                }
            } while (FindNextFileA(searchHandle, &fileInfo)); // ����� ���������� �����/����������
            FindClose(searchHandle); // �������� ����������� ��� ������� �����
        }
}

bool scan_file_system::IsDriveAccessible(char driveLetter)
{
    std::string rootPath = std::string(1, driveLetter) + ":\\";
    UINT driveType = GetDriveTypeA(rootPath.c_str()); // ��������� ���� �����
    // ��� ��������� ���� �����
    // UINT - ���������� ������������ �����

    if (driveType == DRIVE_UNKNOWN || driveType == DRIVE_NO_ROOT_DIR) // ��� ����� ���������� || �� �������� �������
    {
        std::cout << "��� ������� ����� '" << driveLetter << "' �������� ������." << std::endl << std::endl;
        return false;
    }
    else
    {
        std::cout << "���� '" << driveLetter << "': ��������." << std::endl << std::endl;
        return true;
    }
}