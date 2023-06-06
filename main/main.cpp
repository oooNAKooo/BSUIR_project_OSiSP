#include "main.h"

void delay(int milli)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milli));
}

void run_main()
{
    setlocale(0, "");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    disk_checker disk_checker;
    scan_file_system scan_system;
    menu my_menu;

    bool x = true, y;
    char choose;
    int second_choose;
    int param;
    char lastLetter = NULL;

    my_menu.greetings();

    do {
    label:
        my_menu.main();

        std::cout << "\t";
        std::cin >> choose;

        switch (choose) {
        case '1': {
            system("cls");
            char scanLetter;
            std::cout << "Введите букву диска для проверки (например: 'C'): ";
            std::cin >> scanLetter;
            std::cout << std::endl;
            scan_system.IsDriveAccessible(scanLetter);
            lastLetter = scanLetter;
            system("pause");
        }break;
        case '2': {
            system("cls");
            char driveLetter;
            std::cout << "Введите букву диска для сканирования (например: 'C'): ";
            std::cin >> driveLetter;
            std::cout << std::endl;
            std::string drivePath = std::string(1, driveLetter) + ":\\";
            if (disk_checker.IsNTFSFileSystem(drivePath)) {
                if (driveLetter != lastLetter)
                {
                    std::cout << "\tСразу проверьте диск на его доступность!" << std::endl << std::endl;
                    system("pause");
                    goto label;
                }
                else {
                    std::string drivePath = std::string(1, driveLetter) + ":\\";
                    std::cout << "Происходит сканирование диска '" << driveLetter << "'. Ожидайте." << std::endl;
                    delay(2000);
                    system("cls");
                    if (disk_checker.CheckDiskIntegrity(driveLetter))
                    {
                        std::cout << "\tCканирование диска '" << driveLetter << "' завершено. " << "Вот список всех файлов и директорий расположенных на этом диске:" << std::endl << std::endl;
                        scan_system.ScanFileSystem(drivePath); // вывод файлов
                    }
                    else
                    {
                        std::cout << "\tCканирование диска '" << driveLetter << "' завершено." << std::endl << std::endl;
                        y = true;
                        while (y) {
                            std::cout << "Ошибка при проверке целостности тома '" << driveLetter
                                << "'. Ошибка: " << disk_checker.GetLastErrorCode() << std::endl;
                            param = disk_checker.GetLastErrorCode();
                            my_menu.want_to_know();
                            std::cout << "\t";
                            std::cin >> second_choose;
                            switch (second_choose) {
                            case 1: {
                                system("cls");
                                my_menu.mistakes(param);
                                y = false;
                            }break;
                            case 0: {y = false; }break;
                            default: system("cls"); my_menu._default(); break;
                            }
                        }
                    }
                    std::cout << std::endl;
                    std::cout << std::endl;
                    system("pause");
                }
            }
            else {
                std::cout << "У диска '" << driveLetter << "' тип не NTFS. Сканирование диска невозможно!" << std::endl << std::endl;
                system("pause");
            }
        }break;
        case '3': {
            system("cls");
            char driveLetter;
            std::cout << "Введите букву диска для вычисления объема памяти (например: 'C'): ";
            std::cin >> driveLetter;
            std::cout << std::endl;
            if (driveLetter != lastLetter)
            {
                std::cout << "\tСразу проверьте диск на его доступность!" << std::endl << std::endl;
                system("pause");
                goto label;
            }
            else {
                ULONGLONG freeClusters = disk_checker.GetFreeClusters(driveLetter);
                ULONGLONG usedClusters = disk_checker.GetUsedClusters(driveLetter);
                ULONGLONG diskSpace = disk_checker.GetDiskSpace(driveLetter);
                std::cout << "Общий объем памяти на диске '" << driveLetter << "': " << diskSpace << " (bytes)" << std::endl;
                std::cout << "Свободная память на диске '" << driveLetter << "': " << freeClusters << " (bytes)" << std::endl;
                std::cout << "Занятая память на диске '" << driveLetter << "': " << usedClusters << " (bytes)" << std::endl << std::endl;
                std::cout << std::endl;
                system("pause");
            }
        }break;
        case 'q': my_menu.parting(); x = false; break;
        default: std::cout << std::endl; my_menu._default(); system("pause"); break;
        }
    } while (x);
}
