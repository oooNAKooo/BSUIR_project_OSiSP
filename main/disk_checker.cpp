#include "disk_checker.h"

disk_checker::disk_checker(){}

disk_checker::~disk_checker()
{
    if (volume != INVALID_HANDLE_VALUE) // если открытие "ОК"
        CloseHandle(volume); // закрытие открытого дескриптора
}

bool disk_checker::CheckDiskIntegrity(char driveLetter) // функция проверки доступности 
{
    std::string drivePath = "\\\\.\\" + std::string(1, driveLetter) + ":"; // времення строка из 1 символа
    // формируем путь к диску прямого доступа "\.\С:"

    volume = CreateFileA( // создание/открытие файла
        drivePath.c_str(), // путь, который нужно открыть
        GENERIC_READ | GENERIC_WRITE, // можно читать и записывать
        FILE_SHARE_READ | FILE_SHARE_WRITE, // для доступа работы другим процессам
        NULL, // атрибут безопасности
        OPEN_EXISTING, // указываем на открытие существующего файла
        FILE_FLAG_WRITE_THROUGH | FILE_FLAG_NO_BUFFERING, // храниние и буферизация при операциях
        NULL // атрибут шаблон файла
    );

    if (volume == INVALID_HANDLE_VALUE) // если у дескриптора ошибка при открытии 
    {
        lastErrorCode = GetLastError(); // вернуть последнюю ошибку в рамках текущего потока
        return false; // ошибка в процессе работы
    }

    DWORD bytesReturned;
    BOOL result = DeviceIoControl(
        volume,  // дескриптор, куда отправлен код
        IOCTL_DISK_IS_WRITABLE, // нужно выполнить проверку
        NULL, // указат доп вход данные
        0, // размер вход данных
        NULL, // указат доп выход данные
        0, // размер выход данных
        &bytesReturned, // указат на переменную, в кот сохранено кол-во байт
        NULL // не асинхронная операция
    );

    if (result)
    {
        return true; // "ОК"
    }
    else
    {
        lastErrorCode = GetLastError();
        return false; // "НЕ ОК"
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
        drivePath.c_str(), // путь к диску
        reinterpret_cast<PULARGE_INTEGER>(&freeClusters),
        reinterpret_cast<PULARGE_INTEGER>(&totalClusters),
        reinterpret_cast<PULARGE_INTEGER>(&sectorsPerCluster)
    );
    if (result)
    {
        return freeClusters; // вернуть кол-во памяти свобод
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
        usedClusters = totalClusters - freeClusters; // используемая память
    }

    return usedClusters;
}

ULONGLONG disk_checker::GetDiskSpace(char driveLetter)
{
    std::string rootPath = std::string(1, driveLetter) + ":\\";
    ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes; // большой формат - кол-во сохранено кол-во байт

    if (GetDiskFreeSpaceExA(rootPath.c_str(), &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes)) // 
    {
        ULONGLONG totalSpace = totalNumberOfBytes.QuadPart; // кол-во всего памяти
        return totalSpace; // 
    }
    else
    {
        std::cout << "Не удалось получить информацию о свободном пространстве на диске '" << driveLetter << "':" << std::endl << std::endl;
        return 0;
    }
}

bool disk_checker::IsNTFSFileSystem(const std::string& directory)
{
    std::string rootPath = directory.substr(0, 3);  // Получаем корневой путь диска (например, "C:\\")
    // извлекаем первые 3 символа из строки directory

    DWORD fileSystemFlags = 0; // для флагов ФС
    char volumeNameBuffer[MAX_PATH]; // название тома
    DWORD volumeSerialNumber = 0; // серийник тома
    DWORD maximumComponentLength = 0; // макс длина имени

    if (GetVolumeInformationA( // инфа о томе
        rootPath.c_str(), // путь 
        volumeNameBuffer, // имя тома
        sizeof(volumeNameBuffer), // размер буфера
        &volumeSerialNumber, // сохранение серийника
        &maximumComponentLength, // макс длина ФС
        &fileSystemFlags, // флаги ФС
        nullptr, // метка тома
        0)) // размер доп данных
    {
        return (fileSystemFlags & FILE_NAMED_STREAMS) != 0;  // Проверяем наличие флага FILE_NAMED_STREAMS, характерного для NTFS
        // true == ntfs
    }

    return false;
}
