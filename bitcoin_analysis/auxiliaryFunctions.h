#pragma once

string dec2String(long long n);
string double2String(double x,int decimalPlaces);/*lower-decimal estimate*/
int bigRandom(int bigNumber);/*generates random numbers for bigger limits*/
// parallel sorting - source: https://stackoverflow.com/questions/16007640/openmp-parallel-quicksort
void quicksort(int * a, int p, int r);
int partition(int * a, int p, int r);