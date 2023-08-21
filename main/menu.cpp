#include <iostream>
#include "menu.h"

menu::menu() {}

menu::~menu() {}

void menu::greetings()
{
	std::cout << "\t\t\t\t\t\tДобро пожаловать!" << std::endl << std::endl;
	std::cout << "\tДанная программа является утилитой проверки целостности файловой системы NTFS." << std::endl;
	std::cout << "\tДля корректной работы программы действуйте согласно инструкций, представленных в программе." << std::endl;
	std::cout << "\tХорошего дня!" << std::endl << std::endl;
	system("pause");
}

void menu::main()
{
	system("cls");
	std::cout << "\t\tМеню программы:" << std::endl << std::endl;
	std::cout << "\t[1] - Проверка доступности диска;" << std::endl;
	std::cout << "\t[2] - Произвести сканирование системы;" << std::endl;
	std::cout << "\t[3] - Вычисление размера диска;" << std::endl;
	std::cout << "\t[q] - Выход." << std::endl << std::endl;
}

void menu::parting()
{
	system("cls");
	std::cout << "\tПрограмма завершена. До свидания!" << std::endl;
}

void menu::_default()
{
	std::cout << "Введена неверная команда! Повторите попытку." << std::endl << std::endl;
}

void menu::mistakes(int param)
{
	system("cls");
	std::cout << "\t\tВОЗМОЖНЫЕ ОШИБКИ:" << std::endl << std::endl;
	std::cout << "\t[1] - ВЫЗВАННАЯ ФУНКЦИЯ НЕ ПОДДЕРЖИВАЕТСЯ;" << std::endl;
	std::cout << "\t[2] - ДИСК НЕ БЫЛ НАЙДЕН;" << std::endl;
	std::cout << "\t[5] - НЕДОСТАТОЧНО ПРАВ;" << std::endl;
	std::cout << "\t[21] - ДИСК НЕ ГОТОВ;" << std::endl;
	std::cout << "\t[87] - УКАЗЫВАЕТ НА НЕВЕРНЫЕ ПАРАМЕТРЫ переданные в функцию DeviceIoControl;" << std::endl;
	std::cout << "\t[1784] - НЕПРАВИЛЬНЫЙ БУФЕР ВВОДА/ВЫВОДА." << std::endl << std::endl;
	std::cout << "\tВаш код ошибки: " << param << "." << std::endl << std::endl;
	std::cout << "\tНе обнаружили код вашей ошибки в этом меню? Напишите разработчикам по данному адресу: 'ghost150816@gmail.com'" << std::endl << std::endl;
}

void menu::want_to_know()
{
	std::cout << "Желаете узнать, что это за ошибка?" << std::endl;
	std::cout << "\t[1] - Да;" << std::endl;
	std::cout << "\t[0] - Нет." << std::endl;
}
