#pragma once

#include <string>
#include <Windows.h>
#include <iostream>

class disk_checker
{
public:
    disk_checker();
    ~disk_checker();

    bool CheckDiskIntegrity(char driveLetter);
    DWORD GetLastErrorCode();
    ULONGLONG GetFreeClusters(char driveLetter);
    ULONGLONG GetUsedClusters(char driveLetter);
    ULONGLONG GetDiskSpace(char driveLetter);

    bool IsNTFSFileSystem(const std::string& directory);

private:
    HANDLE volume;
    DWORD lastErrorCode;
};
