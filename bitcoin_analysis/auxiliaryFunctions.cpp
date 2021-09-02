#include "stdafx.h"

string dec2String(long long n)
{
	string result = "";
	if (n == 0)
	{
		return "0";
	}
	while (n > 0)
	{
		result = char(n % 10 + 48) + result;
		n /= 10;
	}
	return result;
}

string double2String(double x, int decimalPlaces) // lower - decimal estimate
{
	if (decimalPlaces == 0)return dec2String(long long(x));
	string result, tempx;
	double temp=x;
	for (int i = 0; i < decimalPlaces; i++)temp *= 10;
	tempx = dec2String(long long(temp));
	result = tempx.substr(0, tempx.size() - decimalPlaces);
	if (result.size() == 0)result = "0";
	result += ".";
	result += tempx.substr(tempx.size() - decimalPlaces, decimalPlaces);
	return result;
}

int bigRandom(int bigNumber)
{
	if (bigNumber > 30000)
	{
		int maxLevel = bigNumber / 30000;
		return (rand() % maxLevel) * 30000 + (rand() % 30000);
	}
	else return rand() % bigNumber;
}

void quicksort(int * a, int p, int r)
{
	int div;

	if (p < r) {
		div = partition(a, p, r);
			#pragma omp parallel sections
		{
			#pragma omp section
			quicksort(a, p, div - 1);
			#pragma omp section
			quicksort(a, div + 1, r);

		}
	}
}

int partition(int * a, int p, int r)
{
	int lt[1000];
	int gt[1000];
	int i;
	int j;
	int key = a[r];
	int lt_n = 0;
	int gt_n = 0;

#pragma omp parallel for
	for (i = p; i < r; i++) {
		if (a[i] < a[r]) {
			lt[lt_n++] = a[i];
		}
		else {
			gt[gt_n++] = a[i];
		}
	}

	for (i = 0; i < lt_n; i++) {
		a[p + i] = lt[i];
	}

	a[p + lt_n] = key;

	for (j = 0; j < gt_n; j++) {
		a[p + lt_n + j + 1] = gt[j];
	}

	return p + lt_n;
}

