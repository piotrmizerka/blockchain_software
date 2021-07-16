/// konfiguracja opengla: https://www.youtube.com/watch?v=0CQP8huwLCg

#include "stdafx.h"

Graph usersGraph;
vector <Edge> edgs;

void printMemoryUsageInfo()
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
	DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
	cout << "Total virtual memory: " << totalVirtualMem << endl;
	cout << "Virtual memory currently used: " << virtualMemUsed << endl;
	DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
	cout << "Total physical memory: " << totalPhysMem << endl;
	DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
	cout << "Physical memory currently used: " << physMemUsed << endl;
}

using namespace Concurrency;

int main(int argc, char* argv[])
{
	vector <int> V = { 1,2,3,4,5 };
	//vector<Edge> E = { { 1,2,0,0 } ,{ 1,3,0,0 },{ 1,4,0,0 },{ 1,5,0,0 },{ 2,4,0,0 },{ 2,5,0,0 },{ 3,5,0,0 },{ 5,3,0,0 } };
	vector<Edge> E = { { 1,2,0,0 },{ 2,1,0,0 },{ 1,3,0,0 },{ 3,1,0,0 }, { 1,4,0,0 },{ 4,1,0,0 },{ 1,5,0,0 },{ 5,1,0,0 },{ 2,4,0,0 },{ 4,2,0,0 },{ 2,5,0,0 },{ 5,2,0,0 },{ 3,5,0,0 },{ 5,3,0,0 } };

	Graph clusteringCoefficientTestGraph(V = V, E = E);
	map<int, double> clusteringCoefficients = clusteringCoefficientTestGraph.computeClusteringCoefficients();

	FOREACH(coeff, clusteringCoefficients)printf("%d %.2lf\n", coeff->first, coeff->second);

	return 0;
}

