#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <math.h>
#include <fstream>
#include <string.h>
#include <algorithm>

using namespace std;
struct student
{
	char* Lastname;
	char* Name;
	double avg_mark;
};

void Sort(student Array[], size_t size)
{
	int size2 = size - 1;
	int lastSwap = size2;
	for (size_t z = 0; z < size - 1; z++)
	{

		bool SortCheck = false;
		

		for (size_t i = 0; i < size2; i++)
		{
			if (Array[i].avg_mark < Array[i + 1].avg_mark)
			{
				swap(Array[i], Array[i + 1]);
				SortCheck = true;
				lastSwap = i;

			}
		}
		size2 = lastSwap;
		if (!SortCheck)
		{
			break;
		}
	}
}

int main()
{
	fstream iFile("input.txt");

	size_t size = 0;
	iFile >> size;

	student *students = new student[size];
	char buf[250];
	for (size_t i = 0; i < size; i++)
	{
		iFile >> buf;
		students[i].Lastname = new char[strlen(buf) + 1];
		strcpy(students[i].Lastname, buf);

		iFile >> buf;
		students[i].Name = new char[strlen(buf) + 1];
		strcpy(students[i].Name, buf);

		int k;
		double avg = 0;
		for (size_t i = 0; i < 3; i++)
		{
			iFile >> k;
			avg += k;
		}
		students[i].avg_mark = avg / 3.0;
	}
	iFile.close();
	Sort(students, size);

	ofstream oFile("output.txt");
	string result = "";
	for (size_t i = 0; i < size; i++)
	{
		// strcat(strcat(students[i].Lastname, " "), strcat(students[i].Name, "\n"));
		result += students[i].Lastname;
		result += " ";
		result += students[i].Name;
		result += '\n';
	}
	oFile << result.c_str();
	oFile.close();

	/*	for (size_t i = 0; i < size; i++)
		{
			delete[] students[i].Lastname;
			delete[] students[i].Name;

		}
		delete[] students;*/
	return 0;
}