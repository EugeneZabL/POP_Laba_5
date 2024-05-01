#include<iostream>
#include"omp.h"
#include<thread>

using namespace std;

const int rows = 100;
const int cols = 50;

int MyArr[rows][cols];

void init_arr();

int FindMin();

int FindSum();
int FindSum(int);

int main() {

	init_arr();

	omp_set_nested(1);
#pragma omp parallel sections
	{
#pragma omp section
		{
			std::cout << FindSum() << endl;
			double t1 = omp_get_wtime();
			cout << t1 << endl;
		}

#pragma omp section
		{
			int t = FindMin();
			int tt = FindSum(t);
			double t1 = omp_get_wtime(); 
#pragma omp critical
			{
				cout << tt << endl;
				cout << t << endl;
				cout << t1 << endl;
			}
		}
	}
	return 0;
}

int FindSum()
{
	int sum = 0;
#pragma omp parallel for reduction(+:sum) num_threads(rows)
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			sum = sum + MyArr[i][j];
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	return sum;
}

int FindSum(int row)
{
	int sum = 0;
#pragma omp parallel for reduction(+:sum) num_threads(cols)
	for (int j = 0; j < cols; j++)
	{
		sum = sum + MyArr[row][j];
		//std::this_thread::sleep_for(std::chrono::second(1));
	}


return sum;
}

int FindMin()
{
	int IndexOfMin = rows - 1;
#pragma omp parallel for num_threads(rows/2)
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