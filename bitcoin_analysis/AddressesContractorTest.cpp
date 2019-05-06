#include "stdafx.h"

void AddressesContractorTest::createTransactionAddressesParallelGenerateBigTest(int transactionsNumber, int addressesNumber, int filesNumber, string relativePath)
{
	srand(time(NULL));
	int period = transactionsNumber / filesNumber + 1;
	FILE **saveTransactions, **saveInputs;
	saveTransactions = new FILE *[filesNumber];
	saveInputs = new FILE*[filesNumber];
	string pathxx;
	for (int i = 0;i < filesNumber;i++)
	{
		pathxx = pathx + relativePath+"transactions_separated\\transactions\\" + dec2String(i);
		saveTransactions[i] = fopen(pathxx.c_str(), "w");
		pathxx = pathx + relativePath+"transactions_separated\\inputs\\" + dec2String(i);
		saveInputs[i] = fopen(pathxx.c_str(), "w");
	}
	for (int i = 0;i < transactionsNumber;i++)fprintf(saveTransactions[i / period], "%d %d\n", i, 0);
	set<pair<int, int> > transactionAddresses;
	pair<int, int> temp;
	//for (int i = 0;i < 3 * transactionsNumber;i++)
	for (int i = 0;i < transactionsNumber;i++)
	{
		temp = make_pair(bigRandom(transactionsNumber), bigRandom(addressesNumber));
		while (transactionAddresses.find(temp) != transactionAddresses.end())
		{
			temp = make_pair(bigRandom(transactionsNumber), bigRandom(addressesNumber));
		}
		transactionAddresses.insert(temp);
	}
	int it = 0;
	//period = (3*transactionsNumber) / filesNumber + 1;
	period = transactionsNumber / filesNumber + 1;
	FOREACH(transAddr, transactionAddresses)
	{
		fprintf(saveInputs[it / period], "%d %d\n", transAddr->first, transAddr->second);
		it++;
	}
	for (int i = 0;i < filesNumber;i++)
	{
		fclose(saveTransactions[i]);
		fclose(saveInputs[i]);
	}
}

void AddressesContractorTest::loadTransactionsAddressesParallelTest(int transactionsNumber, int addressesNumber, int filesNumber)
{
	cout << "loadTransactionsAddressesParallel test...\n";
	string relativePath;
	testAddressesContractor = AddressesContractor(transactionsNumber, addressesNumber, 100, filesNumber, "C:\\Users\\Administrator\\Desktop\\bitcoin\\bitcoin_analysis\\tests\\AddressesContractor\\");
	time_t start = time(0);
	testAddressesContractor.loadTransactionsAddressesParallel("loadTransactionsAddressesParallel\\", 1);
	double seconds_since_start = difftime(time(0), start);
	cout << "Serlialized execution time: " << seconds_since_start << "s\n";
	vector<vector<int> > transactionsAddresses1 = testAddressesContractor.getTransactionsAddresses();
	start = time(0);
	testAddressesContractor.loadTransactionsAddressesParallel("loadTransactionsAddressesParallel\\", omp_get_max_threads());
	seconds_since_start = difftime(time(0), start);
	cout << "Parallel execution time: " << seconds_since_start << "s\n";
	vector<vector<int> > transactionsAddresses2 = testAddressesContractor.getTransactionsAddresses();
	set<int> temp1, temp2;
	if (transactionsAddresses1.size() == transactionsAddresses2.size())
	{
		bool check = true;
		for (int i = 0;i < transactionsAddresses1.size();i++)
		{
			temp1.clear();
			temp2.clear();
			FOREACH(address, transactionsAddresses1[i])temp1.insert(*address);
			FOREACH(address, transactionsAddresses2[i])temp2.insert(*address);
			if (temp1.size() == temp2.size())
			{
				FOREACH(address, temp1)
				{
					if (temp2.find(*address) == temp2.end())
					{
						check = false;
						break;
					}
				}
				if (check == false)break;
			}
			else
			{
				check = false;
				break;
			}
		}
		if (check == true)cout << "OK\n";
		else cout << "FAILED\n";
	}
	else cout << "FAILED\n";
}

void AddressesContractorTest::loadNeighborsListParallelTest(int transactionsNumber, int addressesNumber, int filesNumber)
{
	cout << "loadNeighborsListParallel test...\n";
	string relativePath;
	testAddressesContractor = AddressesContractor(transactionsNumber, addressesNumber, 100, filesNumber, "C:\\Users\\Administrator\\Desktop\\bitcoin\\bitcoin_analysis\\tests\\AddressesContractor\\");
	time_t start = time(0);
	testAddressesContractor.loadNeighborsListParallel("loadNeighborsListParallel\\", 1);
	double seconds_since_start = difftime(time(0), start);
	cout << "Serlialized execution time: " << seconds_since_start << "s\n";
	vector<set<int> > neighborsList1 = testAddressesContractor.getNeighborsList();
	start = time(0);
	testAddressesContractor.loadNeighborsListParallel("loadNeighborsListParallel\\", omp_get_max_threads());
	seconds_since_start = difftime(time(0), start);
	cout << "Parallel execution time: " << seconds_since_start << "s\n";
	vector<set<int> > neighborsList2 = testAddressesContractor.getNeighborsList();
	set<int> temp1, temp2;
	if (neighborsList1.size() == neighborsList2.size())
	{
		bool check = true;
		for (int i = 0;i < neighborsList1.size();i++)
		{
			temp1.clear();
			temp2.clear();
			FOREACH(address, neighborsList1[i])temp1.insert(*address);
			FOREACH(address, neighborsList2[i])temp2.insert(*address);
			if (temp1.size() == temp2.size())
			{
				FOREACH(address, temp1)
				{
					if (temp2.find(*address) == temp2.end())
					{
						check = false;
						break;
					}
				}
				if (check == false)break;
			}
			else
			{
				check = false;
				break;
			}
		}
		if (check == true)cout << "OK\n";
		else cout << "FAILED\n";
	}
	else cout << "FAILED\n";
}

void AddressesContractorTest::loadEdgesParallelTest(int transactionsNumber, int addressesNumber, int filesNumber)
{
	cout << "loadEdgesParallel test...\n";
	//string relativePath;
	testAddressesContractor = AddressesContractor(transactionsNumber, addressesNumber, 100, filesNumber, "C:\\Users\\Administrator\\Desktop\\bitcoin\\bitcoin_analysis\\tests\\AddressesContractor\\");
	time_t start = time(0);
	testAddressesContractor.loadEdgesParallel("loadEdgesParallel\\", 1);
	double seconds_since_start = difftime(time(0), start);
	cout << "Serlialized execution time: " << seconds_since_start << "s\n";
	vector<pair<int,int> > edges1 = testAddressesContractor.getEdges();
	vector<int> addresses1 = testAddressesContractor.getAddresses();
	start = time(0);
	testAddressesContractor.loadEdgesParallel("loadEdgesParallel\\", omp_get_max_threads());
	seconds_since_start = difftime(time(0), start);
	cout << "Parallel execution time: " << seconds_since_start << "s\n";
	vector<pair<int,int> > edges2 = testAddressesContractor.getEdges();
	vector<int> addresses2 = testAddressesContractor.getAddresses();
	bool checkEdges = true;
	if (edges1.size() == edges2.size())
	{
		for (int i = 0;i < edges1.size();i++)
		{
			if (edges1[i].first != edges2[i].first || edges1[i].second != edges2[i].second)
			{
				checkEdges = false;
				break;
			}
		}
	}
	else checkEdges = false;
	bool checkAddresses = true;
	if (addresses1.size() == addresses2.size())
	{
		set<int> adds1, adds2;
		FOREACH(address, addresses1)adds1.insert(*address);
		FOREACH(address, addresses2)adds2.insert(*address);
		FOREACH(address, adds1)if (adds2.find(*address) == adds2.end())checkAddresses = false;
	}
	else checkAddresses = false;
	if (checkEdges == true && checkAddresses == true)cout << "OK\n";
	else cout << "FAILED\n";
}

void AddressesContractorTest::labelConnectedComponentsParallelGenerateBigTest(int addressesNumber, int edgesNumber, int filesNumber)
{
	srand(time(NULL));
	int period = edgesNumber / filesNumber + 1;
	FILE **saveEdges;
	saveEdges = new FILE *[filesNumber];
	string pathxx;
	for (int i = 0;i < filesNumber;i++)
	{
		pathxx = pathx + "labelConnectedComponentsParallel\\contractions\\edges\\" + dec2String(i)+".txt";
		saveEdges[i] = fopen(pathxx.c_str(), "w");
	}
	set<pair<int, int> > edges;
	srand(time(NULL));
	pair<int, int> temp, temp2;
	for (int i = 0;i < edgesNumber;i++)
	{
		temp = make_pair(bigRandom(addressesNumber), bigRandom(addressesNumber));
		//while (/*edges.find(temp) != edges.end() ||*/ temp.first == temp.second)
		//{
		//	temp = make_pair(bigRandom(addressesNumber), bigRandom(addressesNumber));
		//}
		//edges.insert(temp);
		//temp2.first = temp.second;
		//temp2.second = temp.first;
		//edges.insert(temp2);
		fprintf(saveEdges[i / period], "%d %d\n", temp.first, temp.second);
		//fprintf(saveEdges[i / period], "%d %d\n", temp.first, temp.second);
		//fprintf(saveEdges[i / period], "%d %d\n", temp.first, temp.first);
		fprintf(saveEdges[i / period], "%d %d\n", temp.second, temp.first);
		//fprintf(saveEdges[i / period], "%d %d\n", temp.second, temp.first);
	}
	for (int i = 0;i < filesNumber;i++)fclose(saveEdges[i]);
}

void AddressesContractorTest::createUsersGraphParallelGenerateBigTest(int transactionsNumber, int addressesNumber, int filesNumber)
{
	createTransactionAddressesParallelGenerateBigTest(transactionsNumber, addressesNumber, filesNumber, "createUsersGraphParallel\\");
	srand(time(NULL));
	int period = (3*transactionsNumber) / filesNumber + 1;
	FILE **saveOutputs;
	saveOutputs = new FILE*[filesNumber];
	string pathxx;
	for (int i = 0;i < filesNumber;i++)
	{
		pathxx = pathx +"createUsersGraphParallel\\transactions_separated\\outputs\\" + dec2String(i);
		saveOutputs[i] = fopen(pathxx.c_str(), "w");
	}
	for (int i = 0;i < 3 * transactionsNumber;i++)
	{
		fprintf(saveOutputs[i / period], "%d %d %d\n", 
			bigRandom(transactionsNumber), bigRandom(addressesNumber), rand()%100*1000);
	}
	for (int i = 0;i < filesNumber;i++)fclose(saveOutputs[i]);
}

void AddressesContractorTest::createUsersGraphParallelTest(int transactionsNumber, int addressesNumber, int filesNumber)
{
	cout << "createUsersGraphParallel test...\n";
	testAddressesContractor = AddressesContractor(transactionsNumber, addressesNumber, 100000, filesNumber, pathx);
	time_t start = time(0);
	testAddressesContractor.createUsersGraphParallel("createUsersGraphParallel\\");
	double seconds_since_start = difftime(time(0), start);
	cout << "Parallel execution time: " << seconds_since_start << "s\n";
	//map<pair<int, int>, pair<long long,int> > parallelGraph, serializedGraph;
	map<pair<int,int>,int>  parallelGraphx, serializedGraphx;
	vector<int> parallelGraph, serializedGraph;
	FILE *read;
	string pathxx;
	pair<int, int> edge;
	pair<long long, int> edgeData;
	for (int i = 0;i < filesNumber;i++)
	{
		pathxx = pathx + "createUsersGraphParallel\\users_graphs\\users_graph\\" + dec2String(i) + ".txt";
		read = fopen(pathxx.c_str(), "r");
		while (!feof(read))
		{
			fscanf(read, "%d %d %lld %d", &edge.first, &edge.second, &edgeData.first, &edgeData.second);
			parallelGraphx[edge]++;
		}
		fclose(read);
	}
	//AddressesContractor testAddressesContractor2 = AddressesContractor(transactionsNumber, addressesNumber, 100000, filesNumber, pathx);
	start = time(0);
	testAddressesContractor.createUsersGraphParallel("createUsersGraphParallel\\",1);
	//testAddressesContractor2.createUsersGraphParallel("createUsersGraphParallel\\", 1);
	seconds_since_start = difftime(time(0), start);
	cout << "Serialized execution time: " << seconds_since_start << "s\n";
	for (int i = 0;i < filesNumber;i++)
	{
		pathxx = pathx + "createUsersGraphParallel\\users_graphs\\users_graph\\" + dec2String(i) + ".txt";
		read = fopen(pathxx.c_str(), "r");
		while (!feof(read))
		{
			fscanf(read, "%d %d %lld %d", &edge.first, &edge.second, &edgeData.first, &edgeData.second);
			serializedGraphx[edge]++;
		}
		fclose(read);
	}
	if (parallelGraphx.size() == serializedGraphx.size())
	{
		FOREACH(elt, parallelGraphx)parallelGraph.push_back(elt->second);
		FOREACH(elt, serializedGraphx)serializedGraph.push_back(elt->second);
		sort(parallelGraph.begin(), parallelGraph.end());
		sort(serializedGraph.begin(), serializedGraph.end());
		bool check = true;
		for(int i=0;i<parallelGraph.size();i++)
		{
			if (serializedGraph[i] != parallelGraph[i])
			{
				cout << "FAILED\n";
				check = false;
				break;
			}
		}
		if (check)cout << "OK\n";
	}
	else
	{
		cout << "FAILED\n";
	}
}

bool AddressesContractorTest::getTestValue()
{
	return testValue;
}

void AddressesContractorTest::labelConnectedComponentsParallelTest(int transactionsNumber, int addressesNumber, int filesNumber)
{
	cout << "labelConnectedComponentsParallel test...\n";
	string relativePath;
	testAddressesContractor = AddressesContractor(transactionsNumber, addressesNumber, 100, filesNumber, "C:\\Users\\Administrator\\Desktop\\bitcoin\\bitcoin_analysis\\tests\\AddressesContractor\\");
	testAddressesContractor.loadEdgesParallel("labelConnectedComponentsParallel\\");
	time_t start = time(0);
	testAddressesContractor.labelConnectedComponentsParallel();
	double seconds_since_start = difftime(time(0), start);
	cout << "Parallel execution time: " << seconds_since_start << "s\n";
	testAddressesContractor.contractAdddressesFromRepresentatives();
	vector<set<int> > contractedAddresses1x = testAddressesContractor.getContractedAddresses();
	start = time(0);
	testAddressesContractor.labelConnectedComponentsParallel(1);
	seconds_since_start = difftime(time(0), start);
	cout << "Serialized execution time: " << seconds_since_start << "s\n";
	start = time(0);;
	AddressesContractor testAddressesContractor2 = AddressesContractor(transactionsNumber, addressesNumber, 100, filesNumber, "C:\\Users\\Administrator\\Desktop\\bitcoin\\bitcoin_analysis\\tests\\AddressesContractor\\");
	testAddressesContractor2.contractAddresses("labelConnectedComponentsParallel\\");
	seconds_since_start = difftime(time(0), start);
	cout << "Serialized BFS execution time: " << seconds_since_start << "s\n";
	vector<set<int> > contractedAddresses2x = testAddressesContractor2.getContractedAddresses();
	set<int> temp1, temp2;
	if (contractedAddresses1x.size() == contractedAddresses2x.size())
	{
		bool check = true;
		vector<int> componentRepresentativesx = testAddressesContractor.getComponentRepresentatives();
		vector<set<int> > contractedAddresses1xx, contractedAddresses2xx;
		contractedAddresses1xx.resize(addressesNumber);
		contractedAddresses2xx.resize(addressesNumber);
		for (int i = 0;i < contractedAddresses1x.size();i++)
		{
			contractedAddresses1xx[*contractedAddresses1x[i].begin()] = contractedAddresses1x[i];
		}
		for (int i = 0;i < contractedAddresses2x.size();i++)
		{
			contractedAddresses2xx[*contractedAddresses2x[i].begin()] = contractedAddresses2x[i];
		}
		vector<set<int> > contractedAddresses1, contractedAddresses2;
		for (int i = 0;i < addressesNumber;i++)
		{
			if (contractedAddresses1xx[i].size() > 0)
			{
				contractedAddresses1.push_back(contractedAddresses1xx[i]);
			}
			if (contractedAddresses2xx[i].size() > 0)
			{
				contractedAddresses2.push_back(contractedAddresses2xx[i]);
			}
		}
		for (int i = 0;i < contractedAddresses1.size();i++)
		{
			temp1.clear();
			temp2.clear();
			FOREACH(address, contractedAddresses1[i])temp1.insert(*address);
			FOREACH(address, contractedAddresses2[i])temp2.insert(*address);
			if (temp1.size() == temp2.size())
			{
				FOREACH(address, temp1)
				{
					if (temp2.find(*address) == temp2.end())
					{
						check = false;
						break;
					}
				}
				if (check == false)break;
			}
			else
			{
				check = false;
				break;
			}
		}
		if (check == true)
		{
			cout << "OK\n";
			testValue = true;
		}
		else
		{
			cout << "FAILED\n";
			testValue = false;
		}
	}
	else
	{
		cout << "FAILED\n";
		testValue = false;
	}
	//testAddressesContractor.normalizeLabelsParallel();
}

void AddressesContractorTest::contractAddressesParallelTest(int transactionsNumber, int addressesNumber, int filesNumber)
{
	cout << "contractAddressesParallel test...\n";
	string relativePath;
	testAddressesContractor = AddressesContractor(transactionsNumber, addressesNumber, 100, filesNumber, "C:\\Users\\Administrator\\Desktop\\bitcoin\\bitcoin_analysis\\tests\\AddressesContractor\\");
	time_t start = time(0);
	//testAddressesContractor.contractAddressesParallel("contractAddressesParallel\\", 1);
	testAddressesContractor.contractAddressesParallel("contractAddressesParallel\\small_test_2\\", 1,true);
	double seconds_since_start = difftime(time(0), start);
	cout << "Serlialized execution time: " << seconds_since_start << "s\n";
	vector<set<int> > contractedAddresses1 = testAddressesContractor.getContractedAddresses();
	start = time(0);
	//testAddressesContractor.contractAddressesParallel("contractAddressesParallel\\", omp_get_max_threads());
	testAddressesContractor.contractAddressesParallel("contractAddressesParallel\\small_test_2\\", omp_get_max_threads(),true);
	seconds_since_start = difftime(time(0), start);
	cout << "Parallel execution time: " << seconds_since_start << "s\n";
	vector<set<int> > contractedAddresses2 = testAddressesContractor.getContractedAddresses();
	set<int> temp1, temp2;
	if (contractedAddresses1.size() == contractedAddresses2.size())
	{
		bool check = true;
		for (int i = 0;i < contractedAddresses1.size();i++)
		{
			temp1.clear();
			temp2.clear();
			FOREACH(address, contractedAddresses1[i])temp1.insert(*address);
			FOREACH(address, contractedAddresses2[i])temp2.insert(*address);
			if (temp1.size() == temp2.size())
			{
				FOREACH(address, temp1)
				{
					if (temp2.find(*address) == temp2.end())
					{
						check = false;
						break;
					}
				}
				if (check == false)break;
			}
			else
			{
				check = false;
				break;
			}
		}
		if (check == true)cout << "OK\n";
		else cout << "FAILED\n";
	}
	else cout << "FAILED\n";
}

