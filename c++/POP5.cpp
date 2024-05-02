#include<iostream>
#include"omp.h"
#include<thread>

using namespace std;

const int rows = 5000;
const int cols = 5000;

const int Treadds = 32;

int MyArr[rows][cols];

void init_arr();

int FindMin();

int FindSum();
int FindSum(int);

int main() {

	init_arr();

	omp_set_nested(1);
	double t1 = omp_get_wtime();
#pragma omp parallel sections
	{
#pragma omp section
		{
			int d = FindSum();
			double t2 = omp_get_wtime();

#pragma omp critical
			{
				std::cout << d << endl;
				cout << t2 - t1 << endl;
			}
		}

#pragma omp section
		{
			int d = FindMin();
			int dd = FindSum(d);
			double t2 = omp_get_wtime();
#pragma omp critical
			{
				cout << dd << endl;
				cout << d << endl;
				cout << t2 - t1 << endl;
			}
		}
	}
	return 2;
}

int FindSum()
{
	int sum = 0;
#pragma omp parallel for reduction(+:sum) num_threads(Treadds)
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			sum = sum + MyArr[i][j];
		}
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return sum;
}

int FindSum(int row)
{
	int sum = 0;
#pragma omp parallel for reduction(+:sum) num_threads(2)
	for (int j = 0; j < cols; j++)
	{
		sum = sum + MyArr[row][j];
	}


return sum;
}

int FindMin()
{
	int IndexOfMin = rows - 1;
#pragma omp parallel for num_threads(Treadds/2)
	for (int i = 0; i < rows; i++)
	{
		int t1 = FindSum(i);
		{
			if (t1 < FindSum(IndexOfMin))
			{
#pragma omp critical
				{
					if (t1 < FindSum(IndexOfMin))
					{

						IndexOfMin = i;
					}
				}
			}
		}
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return IndexOfMin;
}


void init_arr() {
	for (int j = 0; j < rows; j++)
	{
		for (int i = 0; i < cols; i++) {
			//MyArr[j][i] = (i+1)*(j+1);
			MyArr[j][i] = rand()%100;
		}
	}
}