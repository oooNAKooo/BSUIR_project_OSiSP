#include <iostream>
#include "menu.h"

menu::menu() {}

menu::~menu() {}

void menu::greetings()
{
	std::cout << "\t\t\t\t\t\t����� ����������!" << std::endl << std::endl;
	std::cout << "\t������ ��������� �������� �������� �������� ����������� �������� ������� NTFS." << std::endl;
	std::cout << "\t��� ���������� ������ ��������� ���������� �������� ����������, �������������� � ���������." << std::endl;
	std::cout << "\t�������� ���!" << std::endl << std::endl;
	system("pause");
}

void menu::main()
{
	system("cls");
	std::cout << "\t\t���� ���������:" << std::endl << std::endl;
	std::cout << "\t[1] - �������� ����������� �����;" << std::endl;
	std::cout << "\t[2] - ���������� ������������ �������;" << std::endl;
	std::cout << "\t[3] - ���������� ������� �����;" << std::endl;
	std::cout << "\t[q] - �����." << std::endl << std::endl;
}

void menu::parting()
{
	system("cls");
	std::cout << "\t��������� ���������. �� ��������!" << std::endl;
}

void menu::_default()
{
	std::cout << "������� �������� �������! ��������� �������." << std::endl << std::endl;
}

void menu::mistakes(int param)
{
	system("cls");
	std::cout << "\t\t��������� ������:" << std::endl << std::endl;
	std::cout << "\t[1] - ��������� ������� �� ��������������;" << std::endl;
	std::cout << "\t[2] - ���� �� ��� ������;" << std::endl;
	std::cout << "\t[5] - ������������ ����;" << std::endl;
	std::cout << "\t[21] - ���� �� �����;" << std::endl;
	std::cout << "\t[87] - ��������� �� �������� ��������� ���������� � ������� DeviceIoControl;" << std::endl;
	std::cout << "\t[1784] - ������������ ����� �����/������." << std::endl << std::endl;
	std::cout << "\t��� ��� ������: " << param << "." << std::endl << std::endl;
	std::cout << "\t�� ���������� ��� ����� ������ � ���� ����? �������� ������������� �� ������� ������: 'ghost150816@gmail.com'" << std::endl << std::endl;
}

void menu::want_to_know()
{
	std::cout << "������� ������, ��� ��� �� ������?" << std::endl;
	std::cout << "\t[1] - ��;" << std::endl;
	std::cout << "\t[0] - ���." << std::endl;
}
