#ifndef SCANFILESYSTEM_H
#define SCANFILESYSTEM_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <Windows.h>

class scan_file_system {
public:
    scan_file_system();
    ~scan_file_system();
    void ScanFileSystem(const std::string& directory);
    bool IsDriveAccessible(char driveLetter);
};

#endif
