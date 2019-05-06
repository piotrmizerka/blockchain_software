#pragma once

string dec2String(long long n);
string double2String(double x,int decimalPlaces);/*lower-decimal estimate*/
double vectorAngle(double x1, double y1, double x2, double y2);
double normalizeAngle(double angle);/*finds a represanetive of a given angle lying in [0,2pi]*/
double smallerAngleDifference(double normalizedAngle1, double normalizedAngle2);/*computes the smaller arc between two normalized angles*/
pair <double, double> arcMiddlePoint(double x1, double y1, double x2, double y2, double rotationCenterX, double rotationCenterY);
int bigRandom(int bigNumber);/*generates random numbers for bigger limits*/
// parallel sorting - source: https://stackoverflow.com/questions/16007640/openmp-parallel-quicksort
void quicksort(int * a, int p, int r);
int partition(int * a, int p, int r);