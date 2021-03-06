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
	Graph graph;
	//graph.saveLongtermUsersSubgraphContractedEdges();
	/*graph.saveSubgraphTimeInterval(
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph3\\long_term_subgraph.dat",
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph3\\long_term_subgraph2.dat",
		1367107200, 1518134400);*/
	//graph.saveLongTermUsersSubgraph(600, 1000);
	graph.saveTransactionTimeSeriesMoreComponentsParallel(
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph3\\",
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca_trans_number_weekly_from_2013\\",
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca_trans_value_weekly_from_2013\\",
		5,7);
	/*graph.saveTransactionTimeSeriesComponent(
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph2\\",
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca_trans_number_daily_from_2013\\",
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca_trans_value_daily_from_2013\\",
		0);*/
	/*graph.saveVarianceByIntervalsParallel("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca_trans_number_daily_from_2013\\",
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph2\\snapshots_trans_number_normalized\\",
		8,0);
	graph.saveVarianceByIntervalsParallel("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca_trans_number_daily_from_2013\\",
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph2\\snapshots_trans_number_normalized\\",
		8,1);
	graph.saveVarianceByIntervalsParallel("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca_trans_number_daily_from_2013\\",
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph2\\snapshots_trans_number_normalized\\",
		8, 2);
	graph.saveVarianceByIntervalsParallel("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca_trans_number_daily_from_2013\\",
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph2\\snapshots_trans_number_normalized\\",
		8, 3);
	graph.saveVarianceByIntervalsParallel("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca_trans_number_daily_from_2013\\",
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph2\\snapshots_trans_number_normalized\\",
		8, 4);
	graph.saveVarianceByIntervalsParallel("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca_trans_number_daily_from_2013\\",
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph2\\snapshots_trans_number_normalized\\",
		8, 5);
	graph.saveVarianceByIntervalsParallel("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca_trans_value_daily_from_2013\\",
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph2\\snapshots_trans_value_normalized\\",
		8, 0);
	graph.saveVarianceByIntervalsParallel("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca_trans_value_daily_from_2013\\",
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph2\\snapshots_trans_value_normalized\\",
		8, 1);
	graph.saveVarianceByIntervalsParallel("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca_trans_value_daily_from_2013\\",
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph2\\snapshots_trans_value_normalized\\",
		8,2);
	graph.saveVarianceByIntervalsParallel("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca_trans_value_daily_from_2013\\",
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph2\\snapshots_trans_value_normalized\\",
		8, 3);
	graph.saveVarianceByIntervalsParallel("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca_trans_value_daily_from_2013\\",
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph2\\snapshots_trans_value_normalized\\",
		8, 4);
	graph.saveVarianceByIntervalsParallel("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\pca_trans_value_daily_from_2013\\",
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph2\\snapshots_trans_value_normalized\\",
		8, 5);*/
	//graph.saveTransactionTimeSeries("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph2\\");
	//graph.setFilesNumber(29);
	/*graph.createGraphSnapshots(
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph2\\long_term_subgraph\\",
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph3\\snapshots_trans_value\\",
		7, "transactions_value");*/
	/*graph.createGraphSnapshotsParallel(
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph2\\long_term_subgraph\\",
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph3\\snapshots_trans_number\\",
		7, "transactions_number", omp_get_max_threads(), true);*/
	/*graph.normalizeSnapshotsParallel(
		"C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph3\\",
		7,8);*/
	//GraphTest graphTest(10);
	//graphTest.loadUsersGraphParallelGenerateBigTest(10000,5000);
	//graphTest.loadUsersGraphParallelTest();
	//graphTest.createTransactionsTimesParallelGenerateBigTest(1000000,500000);
	//graphTest.createTransactionsTimesParallelTest();
	//graphTest.createGraphSnapshotsParallelGenerateBigTest(10000000,10);
	//graphTest.createGraphSnapshotsParallelTest("transactions_value", true);

	//AddressesContractorTest addressesContractorTest;
	//addressesContractorTest.createTransactionAddressesParallelGenerateBigTest(40,60,10);
	//addressesContractorTest.createTransactionAddressesParallelGenerateBigTest(300000,360000);
	//AddressesContractor addressesContractor = AddressesContractor(300000, 360000, 100, 100, "C:\\Users\\Administrator\\Desktop\\bitcoin\\bitcoin_analysis\\tests\\AddressesContractor\\");
	//addressesContractor.createTransactionsAddressesParallel("createTransactionAddressesParallel\\");
	//addressesContractor.loadTransactionsAddressesParallel("loadTransactionsAddressesParallel\\");
	//addressesContractor.createNeighborsListParallel("createNeighborsListParallel\\");
	//addressesContractorTest.loadTransactionsAddressesParallelTest();
	//addressesContractorTest.loadNeighborsListParallelTest();
	//addressesContractor.createEdgesParallel("createEdgesParallel\\");
	//addressesContractorTest.loadEdgesParallelTest();
	//addressesContractorTest.contractAddressesParallelTest(11,11,1);
	//addressesContractor.contractAddressesParallel("contractAddressesParallel\\");
	/*bool check = true;
	//addressesContractorTest.labelConnectedComponentsParallelGenerateBigTest(6, 4, 1);
	while (check)
	{
		addressesContractorTest.labelConnectedComponentsParallelGenerateBigTest(10000, 6000);
		addressesContractorTest.labelConnectedComponentsParallelTest(30000, 10000);
		//check = addressesContractorTest.getTestValue();
		check = addressesContractorTest.forestCheckx;
	}*/
	//addressesContractorTest.labelConnectedComponentsParallelTest(30000, 10000,1);
	//addressesContractorTest.labelConnectedComponentsParallelGenerateBigTest();
	//addressesContractorTest.labelConnectedComponentsParallelTest();
	//addressesContractorTest.labelConnectedComponentsParallelGenerateBigTest(20, 10);
	//addressesContractorTest.labelConnectedComponentsParallelTest(300000, 20);
	//setmaxstdio(4000);
	//addressesContractorTest.createUsersGraphParallelGenerateBigTest(10000,5000,1);
	//while(1==1)
	//addressesContractorTest.createUsersGraphParallelTest(10000,5000,1);
	//int a = 0;
	//TransactionHypergraphTest transactionHypergraphTest;
	//transactionHypergraphTest.clusterTest();
	//transactionHypergraphTest.clusterGenerateBigTest();
	//transactionHypergraphTest.createUsersGraphTest(true,true);
	//transactionHypergraphTest.createUsersGraphGenerateBigTest();
	//transactionHypergraphTest.testAll(false, true);
	//transactionHypergraphTest.generateBigTestAll();
	//GraphTest graphTest;
	//graphTest.computeClusteringCoefficientsTest(false,true);
	//graphTest.computeClusteringCoefficientsGenerateBigTest();
	//graphTest.determineConnectedComponentsTest(false,true);
	//graphTest.determineConnectedComponentsGenerateBigTest();
	//graphTest. computeStronglyConnectedComponentsTest(false,true);
	//graphTest.computeStronglyConnectedComponentsGenerateBigTest();
	//graphTest.computeVerticesDegreesTest(false,true);
	//graphTest.computeVerticesDegreesGenerateBigTest();
	//graphTest.averageDirectedDegreeTest(false,true);
	//graphTest.averageDirectedDegreeGenerateBigTest();
	//graphTest.directedDegreeSumTest(false,true);
	//graphTest.directedDegreeSumGenerateBigTest();
	//graphTest.edgesWeightSumTest(false,true);
	//graphTest.edgesWeightSumGenerateBigTest();
	//graphTest.testAll(false,true);
	//graphTest.generateBigTestAll();
	//Graph graph;
	//graph.normalizeSnapshots("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph\\", 1);
	//graph.normalizeSnapshots("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\active_users_subgraph\\", 7);
	//string longTermUsersInputPath = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph\\long_term_subgraph.dat";
	//string longTermUsersOutputPath = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\long_term_subgraph\\snapshots\\";
	//string activeUsersInputPath = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\active_users_subgraph\\active_users_subgraph_contracted_edges.dat";
	//string activeUsersOutputPath = "C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\active_users_subgraph\\snapshots\\";
	//graph.createGraphSnapshots(longTermUsersInputPath, longTermUsersOutputPath, 1,"transaction_number");
	//graph.createGraphSnapshots(activeUsersInputPath, activeUsersOutputPath, 7);

	system("pause");
	return 0;
}

