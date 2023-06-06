#include "disk_checker.h"

disk_checker::disk_checker(){}

disk_checker::~disk_checker()
{
    if (volume != INVALID_HANDLE_VALUE) // ���� �������� "��"
        CloseHandle(volume); // �������� ��������� �����������
}

bool disk_checker::CheckDiskIntegrity(char driveLetter) // ������� �������� ����������� 
{
    std::string drivePath = "\\\\.\\" + std::string(1, driveLetter) + ":"; // �������� ������ �� 1 �������
    // ��������� ���� � ����� ������� ������� "\.\�:"

    volume = CreateFileA( // ��������/�������� �����
        drivePath.c_str(), // ����, ������� ����� �������
        GENERIC_READ | GENERIC_WRITE, // ����� ������ � ����������
        FILE_SHARE_READ | FILE_SHARE_WRITE, // ��� ������� ������ ������ ���������
        NULL, // ������� ������������
        OPEN_EXISTING, // ��������� �� �������� ������������� �����
        FILE_FLAG_WRITE_THROUGH | FILE_FLAG_NO_BUFFERING, // �������� � ����������� ��� ���������
        NULL // ������� ������ �����
    );

    if (volume == INVALID_HANDLE_VALUE) // ���� � ����������� ������ ��� �������� 
    {
        lastErrorCode = GetLastError(); // ������� ��������� ������ � ������ �������� ������
        return false; // ������ � �������� ������
    }

    DWORD bytesReturned;
    BOOL result = DeviceIoControl(
        volume,  // ����������, ���� ��������� ���
        IOCTL_DISK_IS_WRITABLE, // ����� ��������� ��������
        NULL, // ������ ��� ���� ������
        0, // ������ ���� ������
        NULL, // ������ ��� ����� ������
        0, // ������ ����� ������
        &bytesReturned, // ������ �� ����������, � ��� ��������� ���-�� ����
        NULL // �� ����������� ��������
    );

    if (result)
    {
        return true; // "��"
    }
    else
    {
        lastErrorCode = GetLastError();
        return false; // "�� ��"
    }
}

DWORD disk_checker::GetLastErrorCode()
{
    return lastErrorCode;
}

ULONGLONG disk_checker::GetFreeClusters(char driveLetter)
{
    std::string drivePath = std::string(1, driveLetter) + ":\\";
    ULONGLONG freeClusters = 0;
    ULONGLONG totalClusters = 0;
    ULONGLONG sectorsPerCluster = 0;
    ULONGLONG bytesPerSector = 0;

    BOOL result = GetDiskFreeSpaceExA(
        drivePath.c_str(), // ���� � �����
        reinterpret_cast<PULARGE_INTEGER>(&freeClusters),
        reinterpret_cast<PULARGE_INTEGER>(&totalClusters),
        reinterpret_cast<PULARGE_INTEGER>(&sectorsPerCluster)
    );
    if (result)
    {
        return freeClusters; // ������� ���-�� ������ ������
    }
    else
    {
        return 0;
    }
}

ULONGLONG disk_checker::GetUsedClusters(char driveLetter)
{
    ULONGLONG totalClusters = 0;
    ULONGLONG freeClusters = 0;
    ULONGLONG usedClusters = 0;

    std::string drivePath = std::string(1, driveLetter) + ":\\";
    BOOL result = GetDiskFreeSpaceExA(
        drivePath.c_str(),
        reinterpret_cast<PULARGE_INTEGER>(&freeClusters),
        reinterpret_cast<PULARGE_INTEGER>(&totalClusters),
        NULL
    );

    if (result)
    {
        usedClusters = totalClusters - freeClusters; // ������������ ������
    }

    return usedClusters;
}

ULONGLONG disk_checker::GetDiskSpace(char driveLetter)
{
    std::string rootPath = std::string(1, driveLetter) + ":\\";
    ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes; // ������� ������ - ���-�� ��������� ���-�� ����

    if (GetDiskFreeSpaceExA(rootPath.c_str(), &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes)) // 
    {
        ULONGLONG totalSpace = totalNumberOfBytes.QuadPart; // ���-�� ����� ������
        return totalSpace; // 
    }
    else
    {
        std::cout << "�� ������� �������� ���������� � ��������� ������������ �� ����� '" << driveLetter << "':" << std::endl << std::endl;
        return 0;
    }
}

bool disk_checker::IsNTFSFileSystem(const std::string& directory)
{
    std::string rootPath = directory.substr(0, 3);  // �������� �������� ���� ����� (��������, "C:\\")
    // ��������� ������ 3 ������� �� ������ directory

    DWORD fileSystemFlags = 0; // ��� ������ ��
    char volumeNameBuffer[MAX_PATH]; // �������� ����
    DWORD volumeSerialNumber = 0; // �������� ����
    DWORD maximumComponentLength = 0; // ���� ����� �����

    if (GetVolumeInformationA( // ���� � ����
        rootPath.c_str(), // ���� 
        volumeNameBuffer, // ��� ����
        sizeof(volumeNameBuffer), // ������ ������
        &volumeSerialNumber, // ���������� ���������
        &maximumComponentLength, // ���� ����� ��
        &fileSystemFlags, // ����� ��
        nullptr, // ����� ����
        0)) // ������ ��� ������
    {
        return (fileSystemFlags & FILE_NAMED_STREAMS) != 0;  // ��������� ������� ����� FILE_NAMED_STREAMS, ������������ ��� NTFS
        // true == ntfs
    }

    return false;
}
