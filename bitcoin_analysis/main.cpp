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
	// Graph construction test
	/*vector <int> V = {1,2,3,4,5,6,7,99,100};
	vector<Edge> E = { { 1,2,0,0 },{ 2,1,0,0 },{ 1,3,0,0 },{ 3,1,0,0 }, { 1,4,0,0 },{ 4,1,0,0 },{ 1,5,0,0 },
					   { 5,1,0,0 },{ 2,4,0,0 },{ 4,2,0,0 },{ 2,5,0,0 },{ 5,2,0,0 },{ 3,5,0,0 },{ 5,3,0,0 },
					   { 6,7,0,0 },
					   { 99,100,0,0}, {99,1,0,0} };

	Graph testGraph(V = V, E = E);*/

	// Long-term subgraph creation
	int minimalRepresantativeAddressesNumber = atoi(argv[1]);
	int minimalIntervalInDays = atoi(argv[2]);
	int minimalTransationsNumber = atoi(argv[3]);
	string usersGraphPath = argv[4];
	string contractedAddressesPath = argv[5];
	Graph longTermSubgraphx = longTermSubgraph(minimalRepresantativeAddressesNumber, minimalIntervalInDays, 
											   minimalTransationsNumber, usersGraphPath, contractedAddressesPath);
	/*Graph longTermSubgraphx = longTermSubgraph(2, 10, 3,
		"C:\\Users\\Dyrektor\\Desktop\\blockchain_software_old\\tests\\Graph\\long_term_subgraph\\testUsersGraph.dat",
		"C:\\Users\\Dyrektor\\Desktop\\blockchain_software_old\\tests\\Graph\\long_term_subgraph\\testContractedAddresses.dat");*/

	return 0;
}

