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

double vectorAngle(double x1, double y1, double x2, double y2)
{
	double x = x2 - x1, y = y2 - y1;
	double angle = asin(y / (sqrt(x*x + y * y)));
	if (x >= 0)return angle;
	else return PI - angle;
}

double normalizeAngle(double angle)
{
	double result = angle;
	if (angle < 0)
	{
		while (result < 0)result += (2 * PI);
	}
	else if (angle >= 2 * PI)
	{
		while (result >= 2 * PI)result -= (2 * PI);
	}
	return result;
}

double smallerAngleDifference(double normalizedAngle1, double normalizedAngle2)
{
	double result;
	if (abs(normalizedAngle1 - normalizedAngle2) <= PI)result = abs(normalizedAngle1 - normalizedAngle2);
	else result = 2 * PI - abs(normalizedAngle1 - normalizedAngle2);
	return result;
}

pair<double, double> arcMiddlePoint(double x1, double y1, double x2, double y2, double rotationCenterX, double rotationCenterY)
{
	pair<double, double> result;
	double a = 0.5*(x1 + x2) - rotationCenterX, b = 0.5*(y1 + y2) - rotationCenterY;
	double rSquare = (x1 - rotationCenterX)*(x1 - rotationCenterX) + (y1 - rotationCenterY)*(y1 - rotationCenterY);
	double t = sqrt(rSquare / (a*a + b * b));
	result.first = a * t + rotationCenterX;
	result.second = b * t + rotationCenterY;

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

