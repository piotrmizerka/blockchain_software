#include "stdafx.h"

void AddressesContractor::createTransactionsAddresses()
{
	FILE *readTransactions, *readInputs;
	readTransactions = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\tests\\txh_test3.dat", "r+");
	readInputs = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\tests\\txin_test3.dat", "r+");
	transactionsAddresses.clear();
	transactionsAddresses.resize(transactionsNumber);
	allTransactions.clear();
	int it = 0, txId, blockId, addrId;
	while (!feof(readTransactions))
	{
		fscanf(readTransactions, "%i", &txId);
		fscanf(readTransactions, "%i", &blockId);
		allTransactions.push_back(txId);
	}
	neighborsList.resize(addressesNumber);
	while (!feof(readInputs))
	{
		fscanf(readInputs, "%i", &txId);
		fscanf(readInputs, "%i", &addrId);
		if (addrId >= 0)
		{
			transactionsAddresses[txId].push_back(addrId);
			neighborsList[addrId].clear();
		}
	}
	FILE *saveTransactionsAddresses;
	saveTransactionsAddresses = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\contractions\\transactions_input_addresses_cpp.dat","w+");
	FOREACH(trans, allTransactions)
	{
		fprintf(saveTransactionsAddresses, "%d ", *trans);
		FOREACH(address, transactionsAddresses[*trans])
		{
			fprintf(saveTransactionsAddresses, "%d ", *address);
		}
		fprintf(saveTransactionsAddresses, "\n");
	}
	fclose(readTransactions);
	fclose(readInputs);
	fclose(saveTransactionsAddresses);
}

void AddressesContractor::createUsersGraph()
{
	Graph result;
	vector <int> vertices;
	vector <Edge> edges;
	Edge edge;
	FILE *readTransactionsAddresses, *readContractedAddresses, *readOutputs, *saveUsersGraph;
	readTransactionsAddresses = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\contractions\\transactions_addresses_restricted.dat", "r+");
	readContractedAddresses = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\contractions\\contracted_addresses_restricted.dat", "r+");
	readOutputs = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\contractions\\out_restricted.dat", "r+");
	saveUsersGraph = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\users_graphs\\users_graph.dat", "w+");
	int txId, addrId, addrNumber;
	long long value;
	int it = 0;
	transactionsRepresentatives.clear();
	transactionsRepresentatives.resize(transactionsNumber);
	cout << "Clearing transactionsRepresentatives: " << endl;
	for (int i = 0;i < transactionsNumber;i++)
	{
		transactionsRepresentatives[i] = -1;
	}
	cout << "Reading transactions addresses:" << endl;
	while (!feof(readTransactionsAddresses))
	{
		fscanf(readTransactionsAddresses, "%i", &txId);
		fscanf(readTransactionsAddresses, "%i", &addrNumber);
		for (int i = 0;i < addrNumber;i++)
		{
			fscanf(readTransactionsAddresses, "%i", &addrId);
			if(i==0)transactionsRepresentatives[txId] = addrId;
		};
		if (it % 10000000 == 0)
		{
			cout << it << " " << addrId << endl;
		}
		it++;
	}
	vertices.clear();
	edges.clear();
	cout << "Clearing addressesVertices:" << endl;
	addressesVertices.clear();
	addressesVertices.resize(addressesNumber);
	for (int i = 0;i < addressesNumber;i++)
	{
		addressesVertices[i] = -1;
	}
	cout << "Reading contracted addresses:" << endl;
	it = 0;
	while (!feof(readContractedAddresses))
	{
		fscanf(readContractedAddresses, "%i", &addrNumber);
		for (int i = 0;i < addrNumber;i++)
		{
			fscanf(readContractedAddresses, "%i", &addrId);
			addressesVertices[addrId] = it;
		}
		if (it % 1000000 == 0)
		{
			cout << it << " " << addrId << endl;
		}
		it++;
	}
	cout << "Reading outputs:" << endl;
	it = 0;
	while (!feof(readOutputs))
	{
		fscanf(readOutputs, "%i", &txId);
		fscanf(readOutputs, "%i", &addrId);
		fscanf(readOutputs, "%lld", &value);
		if (addrId >= 0 && transactionsRepresentatives[txId] != -1)
		{
			if (addressesVertices[transactionsRepresentatives[txId]] != addressesVertices[addrId]) // removing loops
			{
				fprintf(saveUsersGraph, "%d %d %lld %d\n", addressesVertices[transactionsRepresentatives[txId]], addressesVertices[addrId], value, txId);
			}
		}
		if (it % 10000000 == 0)
		{
			cout << it << " " << addrId << endl;
		}
		it++;
	}
	fclose(readTransactionsAddresses);
	fclose(readContractedAddresses);
	fclose(readOutputs);
	fclose(saveUsersGraph);
}

void AddressesContractor::createUsersGraphParallel(string relativePath, int threadsNumber)
{
	createTransactionsAddressesParallel(relativePath, threadsNumber, false);
	/*ofstream save;
	save.open("C:\\Users\\Administrator\\Desktop\\tescik1.txt");
	vector<int> tescik1;
	tescik1.resize(transactionsNumber);
	for (int i = 0;i < transactionsNumber;i++)tescik1[i] = transactionsAddresses[i].size();
	sort(tescik1.rbegin(), tescik1.rend());
	for (int i = 0;i < tescik1.size();i++)save << tescik1[i] << endl;
	save.close();*/
	transactionsRepresentatives.clear();
	transactionsRepresentatives.resize(transactionsNumber);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < transactionsNumber;i++)
		{
			transactionsRepresentatives[i] = -1;
		}
	}
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < transactionsNumber;i++)
		{
			if (transactionsAddresses[i].size() > 0)
			{
				transactionsRepresentatives[i] = transactionsAddresses[i][0];
			}
		}
	}
	createEdgesParallel(relativePath, threadsNumber, false);
	//createEdgesParallel(relativePath, threadsNumber, true);
	//loadEdgesParallel(relativePath);
	/*set<pair<int, int> > test;
	FOREACH(edge, edges)test.insert(*edge);
	bool check = true;
	FOREACH(edge, test)
	{
		pair<int, int> temp = make_pair(edge->second+1, edge->first);
		if (test.find(temp) == test.end())
		{
			check = false;
		}
	}*/
	labelConnectedComponentsParallel(threadsNumber);
	/*ofstream save;
	save.open("C:\\Users\\Administrator\\Desktop\\tescik1.txt");
	vector<int> tescik1;
	tescik1.resize(addressesNumber);
	for (int i = 0;i < addressesNumber;i++)tescik1[componentRepresentatives[i]]++;
	sort(tescik1.rbegin(), tescik1.rend());
	for (int i = 0;i < tescik1.size();i++)save << tescik1[i] << endl;
	save.close();*/
	normalizeLabelsParallel(threadsNumber);
	addressesVertices.clear();
	addressesVertices.resize(addressesNumber);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < addressesNumber;i++)
		{
			addressesVertices[i] = normalizedLabels[componentRepresentatives[i]];
		}
	}
	FILE **readOutputs, **saveUsersGraph;
	readOutputs = new FILE*[filesNumber];
	saveUsersGraph = new FILE*[filesNumber];
	string pathxx;
	for (int i = 0;i < filesNumber;i++)
	{
		pathxx = pathx + relativePath + "transactions_separated\\outputs\\" + dec2String(i);
		readOutputs[i] = fopen(pathxx.c_str(), "r");
		pathxx = pathx + relativePath + "users_graphs\\users_graph\\" + dec2String(i) + ".txt";
		saveUsersGraph[i] = fopen(pathxx.c_str(), "w");
	}
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < filesNumber;i++)
		{
			int txId, addrId;
			long long value;
			while (!feof(readOutputs[i]))
			{
				fscanf(readOutputs[i], "%d %d %lld", &txId, &addrId, &value);
				if (addrId >= 0 && transactionsRepresentatives[txId] != -1)
				{
					if (addressesVertices[transactionsRepresentatives[txId]] != addressesVertices[addrId]) // removing loops
					{
						fprintf(saveUsersGraph[i], "%d %d %lld %d\n", 
							addressesVertices[transactionsRepresentatives[txId]], addressesVertices[addrId], value, txId);
					}
				}
			}
		}
	}
	for (int i = 0;i < filesNumber;i++)
	{
		fclose(readOutputs[i]);
		fclose(saveUsersGraph[i]);
	}
}

void AddressesContractor::saveBiggestClustersSizes()
{
	FILE *read, *save;
	read = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\contractions\\clusters_sizes.dat", "r");
	save = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\contractions\\biggest_clusters_sizes.dat", "w");
	int sizex;
	clustersSizes.clear();
	int it = 0;
	while (!feof(read))
	{
		fscanf(read, "%i", &sizex);
		if (sizex > 100)
		{
			clustersSizes.push_back(sizex);
		}
		if (it % 10000000 == 0)cout << it << endl;
		it++;
	}
	sort(clustersSizes.rbegin(), clustersSizes.rend());
	for (int i = 0;i < clustersSizes.size();i++)fprintf(save, "%d\n", clustersSizes[i]);
	fclose(read);
	fclose(save);
}

void AddressesContractor::restrictOut()
{
	FILE *readOutputs, *readTransactionsAddresses, *save;
	readOutputs = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\small_files\\out.dat", "r+");
	readTransactionsAddresses = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\contractions\\transactions_addresses_restricted.dat", "r+");
	save = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\contractions\\out_restricted.dat", "w+");
	cout << "Clearing considered addresses:" << endl;
	consideredAddresses.clear();
	consideredAddresses.resize(addressesNumber);
	for (int i = 0;i < addressesNumber;i++)
	{
		if (i % 10000000 == 0)cout << i << endl;
		consideredAddresses[i] = -1;
	}
	cout << "Clearing considered transactions:" << endl;
	consideredTransactions.clear();
	consideredTransactions.resize(transactionsNumber);
	for (int i = 0;i < transactionsNumber;i++)
	{
		if (i % 10000000 == 0)cout << i << endl;
		consideredTransactions[i] = false;
	}
	int txId, addrNumber, addrId;
	int it = 0;
	cout << "Reading transactions addresses restricted:" << endl;
	while (!feof(readTransactionsAddresses))
	{
		fscanf(readTransactionsAddresses, "%i", &txId);
		fscanf(readTransactionsAddresses, "%i", &addrNumber);
		consideredTransactions[txId] = true;
		for (int i = 0;i < addrNumber;i++)
		{
			fscanf(readTransactionsAddresses, "%i", &addrId);
			consideredAddresses[addrId] = 1;
		}
		if (it % 10000000 == 0)
		{
			cout << it << " " << addrNumber << endl;
		}
		it++;
	}
	long long value;
	it = 0;
	cout << "Reading outs and saving restricted:" << endl;
	while (!feof(readOutputs))
	{
		fscanf(readOutputs, "%i", &txId);
		fscanf(readOutputs, "%i", &addrId);
		fscanf(readOutputs, "%lld", &value);
		if (txId != -1 && addrId != -1)
		{
			if (consideredTransactions[txId] == true && consideredAddresses[addrId] == 1)
			{
				fprintf(save, "%d %d %lld\n", txId, addrId, value);
			}
		}
		if (it % 10000000 == 0)
		{
			cout << it << " " << txId << " " << addrId << " " << value << endl;
		}
		it++;
	}
	fclose(readOutputs);
	fclose(readTransactionsAddresses);
	fclose(save);
}

AddressesContractor::AddressesContractor(int transactionsNumberx, int addressesNumberx, int usersNumberx, int filesNumberx, string pathxx)
{
	transactionsNumber = transactionsNumberx;
	addressesNumber = addressesNumberx;
	usersNumber = usersNumberx;
	filesNumber = filesNumberx;
	pathx = pathxx;
}

vector<vector<int>> AddressesContractor::getTransactionsAddresses()
{
	return transactionsAddresses;
}

vector<set<int>> AddressesContractor::getNeighborsList()
{
	return neighborsList;
}

vector<pair<int, int>> AddressesContractor::getEdges()
{
	return edges;
}

vector<int> AddressesContractor::getAddresses()
{
	return addresses;
}

vector<set<int>> AddressesContractor::getContractedAddresses()
{
	return contractedAddresses;
}

vector<int> AddressesContractor::getComponentRepresentatives()
{
	return componentRepresentatives;
}

void AddressesContractor::restrictTransactionsAddresses()
{
	FILE *readContractedAddresses, *readTransactionsAddresses, *save;
	readContractedAddresses = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\contractions\\contracted_addresses_restricted.dat", "r+");
	readTransactionsAddresses = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\contractions\\transactions_addresses_no_repetitions.dat", "r+");
	save = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\contractions\\transactions_addresses_restricted.dat", "w+");
	cout << "Clearing considered addresses:" << endl;
	consideredAddresses.clear();
	consideredAddresses.resize(addressesNumber);
	for (int i = 0;i < addressesNumber;i++)
	{
		if (i % 10000000 == 0)cout << i << endl;
		consideredAddresses[i] = -1;
	}
	int addrNumber, addrId, addrIdx;
	int it = 0;
	cout << "Reading contracted addresses restricted:" << endl;
	while (!feof(readContractedAddresses))
	{
		fscanf(readContractedAddresses, "%i", &addrNumber);
		for (int i = 0;i < addrNumber;i++)
		{
			fscanf(readContractedAddresses, "%i", &addrId);
			consideredAddresses[addrId] = 1;
		}
		if (it % 100000 == 0)
		{
			cout << it << " " << addrNumber << endl;
		}
		it++;
	}
	it = 0;
	cout << "Reading transactions addresses and saving restricted:" << endl;
	int txId;
	while (!feof(readTransactionsAddresses))
	{
		fscanf(readTransactionsAddresses, "%i", &txId);
		fscanf(readTransactionsAddresses, "%i", &addrNumber);
		if (addrNumber > 0)
		{
			fscanf(readTransactionsAddresses, "%i", &addrIdx);
			if (consideredAddresses[addrIdx] == 1)
			{
				fprintf(save, "%d %d %d ", txId, addrNumber, addrIdx);
				for (int i = 1;i < addrNumber;i++)
				{
					fscanf(readTransactionsAddresses, "%i", &addrId);
					fprintf(save, "%d ", addrId);
				}
				fprintf(save, "\n");
			}
			else
			{
				for (int i = 1;i < addrNumber;i++)
				{
					fscanf(readTransactionsAddresses, "%i", &addrId);
				}
			}
		}
		if (it % 10000000 == 0)
		{
			cout << it << " " << addrNumber << endl;
		}
		it++;
	}
	fclose(readContractedAddresses);
	fclose(readTransactionsAddresses);
	fclose(save);
}

void AddressesContractor::restrictTransactionsAddressesParallel()
{

}

void AddressesContractor::restrictContractedAddresses(int minimalClusterSize)
{
	FILE *readContractedAddresses, *save, *readClustersSizes;
	readContractedAddresses = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\contractions\\contracted_addresses_cpp.dat", "r+");
	readClustersSizes = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\contractions\\clusters_sizes.dat", "r+");
	save = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\contractions\\contracted_addresses_restricted.dat", "w+");
	int addrNumber, addrId, clusterSize;
	int it = 0;
	clustersSizes.resize(usersNumber);
	cout << "Initializing clustersSizes:" << endl;
	for (int i = 0;i < 160000000;i++)
	{
		clustersSizes[i] = 0;
		if (i % 10000000 == 0)cout << i << endl;
	}
	it = 0;
	cout << "Reading clusters sizes:" << endl;
	while (!feof(readClustersSizes))
	{
		fscanf(readClustersSizes, "%i", &clusterSize);
		clustersSizes[it] = clusterSize;
		if (it % 10000000 == 0)
		{
			cout << it << " " << clusterSize << endl;
		}
		it++;
	}
	it = 0;
	cout << "Reading contracted addresses:" << endl;
	while (!feof(readContractedAddresses))
	{
		fscanf(readContractedAddresses, "%i", &addrNumber);
		if (addrNumber >= minimalClusterSize)
		{
			fprintf(save, "%d ", addrNumber);
			for (int i = 0;i < addrNumber;i++)
			{
				fscanf(readContractedAddresses, "%i", &addrId);
				fprintf(save, "%d ", addrId);
			}
			fprintf(save, "\n");
		}
		else
		{
			for (int i = 0;i < addrNumber;i++)
			{
				fscanf(readContractedAddresses, "%i", &addrId);
			}
		}
		if (it % 10000000 == 0)
		{
			cout << it << " " << addrNumber << endl;
		}
		it++;
	}
	fclose(readContractedAddresses);
	fclose(readClustersSizes);
	fclose(save);
}

void AddressesContractor::contractAddresses(string relativePath)
{
	loadEdgesParallel(relativePath);
	createNeighborsListFromEdges();
	//loadNeighborsList();
	contractAddressesBFS();
}

void AddressesContractor::contractAdddressesFromRepresentatives()
{
	contractedAddresses.clear();
	set<int> representatives;
	for (int i = 0;i < addressesNumber;i++)if (consideredAddresses[i]==0)representatives.insert(componentRepresentatives[i]);
	vector<int> representativesIds;
	representativesIds.resize(addressesNumber);
	int currentId = 0;
	FOREACH(representative, representatives)
	{
		representativesIds[*representative] = currentId;
		currentId++;
	}
	contractedAddresses.resize(currentId);
	vector<int> toAdd;
	for (int i = 0;i < addressesNumber;i++)
	{
		if (consideredAddresses[i]==0)contractedAddresses[representativesIds[componentRepresentatives[i]]].insert(i);
		else toAdd.push_back(i);
	}
	/*set<int> temp;
	for (int i = 0;i < toAdd.size();i++)
	{
		temp.clear();
		temp.insert(toAdd[i]);
		contractedAddresses.push_back(temp);
	}*/
}

void AddressesContractor::contractAddressesParallel(string relativePath, int threadsNumber, bool loadEdges)
{
	if (loadEdges == true)loadEdgesParallel(relativePath, threadsNumber);
	else
	{
		createTransactionsAddressesParallel(relativePath, threadsNumber);
		createEdgesParallel(relativePath, threadsNumber);
	}
	labelConnectedComponentsParallel(threadsNumber);
	map<pair<int, int>, vector<int> > componentThreadAddresses;
	vector<bool> numbersComponentRepresentatives;
	numbersComponentRepresentatives.resize(addressesNumber);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < addressesNumber;i++)numbersComponentRepresentatives[i] = false;
	}
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < addresses.size();i++)
		{
			componentThreadAddresses[make_pair(componentRepresentatives[addresses[i]], omp_get_thread_num())].push_back(addresses[i]);
			numbersComponentRepresentatives[componentRepresentatives[addresses[i]]] = true;
		}
	}
	vector < vector<int> > contractedAddressesx;
	contractedAddressesx.resize(addressesNumber);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < addressesNumber;i++)
		{
			if (numbersComponentRepresentatives[i] == true)
			{
				//contractedAddressesx[i].push_back(0);
				for (int j = 0;j < threadsNumber;j++)
				{
					if (componentThreadAddresses.find(make_pair(i, j)) != componentThreadAddresses.end())
					{
						FOREACH(address, componentThreadAddresses[make_pair(i, j)])
						{
							contractedAddressesx[i].push_back(*address);
						}
					}
				}
			}
		}
	}
	vector<vector<int> > threadsComponentRepresentatives;
	threadsComponentRepresentatives.resize(threadsNumber);
	contractedAddresses.clear();
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < addressesNumber;i++)
		{
			if (contractedAddressesx[i].size() > 0)
			{
				threadsComponentRepresentatives[omp_get_thread_num()].push_back(i);
			}
		}
	}
	vector<int> numberRepresentativesUpToThread;
	numberRepresentativesUpToThread.resize(threadsNumber);
	numberRepresentativesUpToThread[0] = 0;
	for (int i = 1;i < threadsNumber;i++)
	{
		numberRepresentativesUpToThread[i] = numberRepresentativesUpToThread[i - 1] 
			+ threadsComponentRepresentatives[i - 1].size();
	}
	int connectedComponentsNumber = numberRepresentativesUpToThread[threadsNumber - 1] 
		+ threadsComponentRepresentatives[threadsNumber - 1].size();
	contractedAddresses.clear();
	contractedAddresses.resize(connectedComponentsNumber);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < threadsNumber;i++)
		{
			for (int j = 0;j < threadsComponentRepresentatives[i].size();j++)
			{
				set<int> temp;
				FOREACH(address, contractedAddressesx[threadsComponentRepresentatives[i][j]])
				{
					temp.insert(*address);
				}
				contractedAddresses[numberRepresentativesUpToThread[i] + j] = temp;
			}
		}
	}
	FILE **saveContractedAddresses;
	saveContractedAddresses = new FILE*[filesNumber];
	string pathxx;
	for (int i = 0;i < filesNumber;i++)
	{
		pathxx = pathx + relativePath + "contractions\\contracted_addresses\\" + dec2String(i);
		saveContractedAddresses[i] = fopen(pathxx.c_str(), "w");
	}
	int period = contractedAddresses.size() / filesNumber + 1;
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < contractedAddresses.size();i += period)
		{
			for (int j = i;j < min(i + period, contractedAddresses.size());j++)
			{
				fprintf(saveContractedAddresses[j / period], "%d ", contractedAddresses[j].size());
				FOREACH(address, contractedAddresses[j])
				{
					fprintf(saveContractedAddresses[j / period], "%d ", *address);
				}
				fprintf(saveContractedAddresses[j / period], "\n");
			}
		}
	}
	for (int i = 0;i < filesNumber;i++)fclose(saveContractedAddresses[i]);
}

void AddressesContractor::labelConnectedComponentsParallel(int threadsNumber)
{
	single.clear();
	single.resize(addressesNumber);
	starx.clear();
	starx.resize(addressesNumber);
	componentRepresentatives.clear();
	componentRepresentatives.resize(addressesNumber);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < addresses.size();i++)
		{
			componentRepresentatives[addresses[i]] = addresses[i];
			single[addresses[i]] = true;
		}
	}
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < edges.size();i++)
		{
			if (edges[i].first > edges[i].second)
			{
				hook(edges[i].first, edges[i].second);
				single[edges[i].first] = single[edges[i].second] = false;
			}
		}
	}
	bool noChange;
	int it = 0;
	do
	{
		#pragma omp parallel num_threads(threadsNumber)
		{
			#pragma omp for
			for (int i = 0;i < addresses.size();i++)star(addresses[i]);
		}
		#pragma omp parallel num_threads(threadsNumber)
		{
			#pragma omp for
			for (int i = 0;i < edges.size();i++)
			{
				int Pi = componentRepresentatives[edges[i].first];
				int Pj = componentRepresentatives[edges[i].second];
				if (starx[edges[i].first] == true &&
					Pi > Pj)
				{
					hook(edges[i].first, edges[i].second);
				}
			}
		}
		#pragma omp parallel num_threads(threadsNumber)
		{
			#pragma omp for
			for (int i = 0;i < addresses.size();i++)star(addresses[i]);
		}
		#pragma omp parallel num_threads(threadsNumber)
		{
			#pragma omp for
			for (int i = 0;i < edges.size();i++)
			{
				int Pi = componentRepresentatives[edges[i].first];
				int Pj = componentRepresentatives[edges[i].second];
				if (starx[edges[i].first] == true &&
					Pi != Pj)
				{
					hook(edges[i].first, edges[i].second);
				}
			}
		}
		noChange = true;
		#pragma omp parallel num_threads(threadsNumber)
		{
			#pragma omp for
			for (int i = 0;i < addresses.size();i++)
			{
				int Pv = componentRepresentatives[addresses[i]];
				int PPv = componentRepresentatives[Pv];
				if (Pv != PPv)
				{
					componentRepresentatives[addresses[i]] = componentRepresentatives[componentRepresentatives[addresses[i]]];
					noChange = false;
				}
			}
		}
		it++;
	} while (noChange == false);
	//cout << "Number of iterations: " << it << endl;
}

void AddressesContractor::hook(int u, int v)
{
	if (componentRepresentatives[componentRepresentatives[v]] != componentRepresentatives[u])
	{
		componentRepresentatives[componentRepresentatives[u]] = componentRepresentatives[v];
	}
}

void AddressesContractor::star(int v)
{
	if (single[v] == true)starx[v] = false;
	else starx[v] = true;
	int Pv = componentRepresentatives[v];
	int PPv = componentRepresentatives[Pv];
	if (Pv != PPv)
	{
		starx[v] = false;
		starx[PPv] = false;
	}
	if (starx[Pv] == false)starx[v] = false;
}

void AddressesContractor::normalizeLabelsParallel(int threadsNumber)
{
	vector <bool> considered;
	considered.resize(addressesNumber);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < addressesNumber;i++)considered[i] = false;
	}
	int period = addressesNumber / threadsNumber + 1;
	vector< vector<int> > threadsRepresentatives;
	threadsRepresentatives.resize(threadsNumber);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < addressesNumber;i++)
		{
			if (consideredAddresses[i] != -1)
			{
				considered[componentRepresentatives[i]] = true;
			}
		}
	}
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < addressesNumber;i++)
		{
			if (considered[i])
			{
				threadsRepresentatives[omp_get_thread_num()].push_back(i);
			}
		}
	}
	vector<int> representativesUpToThread;
	representativesUpToThread.resize(threadsNumber);
	representativesUpToThread[0] = 0;
	for (int i = 1;i < threadsNumber;i++)
	{
		representativesUpToThread[i] = representativesUpToThread[i - 1] + threadsRepresentatives[i - 1].size();
	}
	normalizedLabels.clear();
	normalizedLabels.resize(addressesNumber);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < threadsNumber;i++)
		{
			for (int j = 0;j < threadsRepresentatives[i].size();j++)
			{
				normalizedLabels[threadsRepresentatives[i][j]] = representativesUpToThread[i] + j;
			}
		}
	}
}

void AddressesContractor::createTransactionsAddressesParallel(string relativePath, int threadsNumber, bool performSave)
{
	FILE **readTransactions, **readInputs;
	readTransactions = new FILE*[filesNumber];
	readInputs = new FILE*[filesNumber];
	string pathxx;
	for (int i = 0;i < filesNumber;i++)
	{
		pathxx = pathx + relativePath + "transactions_separated\\transactions\\"+dec2String(i);
		readTransactions[i] = fopen(pathxx.c_str(), "r");
		pathxx = pathx + relativePath + "transactions_separated\\inputs\\" + dec2String(i);
		readInputs[i] = fopen(pathxx.c_str(), "r");
	}
	transactionsAddresses.clear();
	transactionsAddresses.resize(transactionsNumber);
	//vector<int> allTransactionsPrivate;
	vector<set<int> > allTransactionsSet;
	allTransactionsSet.resize(filesNumber);
	vector<vector<int> > allTransactionsVector;
	allTransactionsVector.resize(filesNumber);
	neighborsList.resize(addressesNumber);
	map<pair<int, int>, set<int> > threadsTransactionsAddresses;
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < filesNumber;i++)
		{
			while (!feof(readTransactions[i]))
			{
				int txId, blockId;
				fscanf(readTransactions[i], "%d %d", &txId, &blockId);
				if (txId >= 0 && txId <= 3 * transactionsNumber / 2)allTransactionsSet[i].insert(txId);
			}
			while (!feof(readInputs[i]))
			{
				int txId, addrId;
				fscanf(readInputs[i], "%i", &txId);
				fscanf(readInputs[i], "%i", &addrId);
				if (addrId >= 0)
				{
					threadsTransactionsAddresses[make_pair(txId, omp_get_thread_num())].insert(addrId);
					neighborsList[addrId].clear();
				}
			}
			FOREACH(transaction, allTransactionsSet[i])allTransactionsVector[i].push_back(*transaction);
		}
	}
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < transactionsNumber;i++)
		{
			set<int> currentTransactionAddresses;
			for (int j = 0;j < threadsNumber;j++)
			{
				if (threadsTransactionsAddresses.find(make_pair(i, j)) != threadsTransactionsAddresses.end())
				{
					FOREACH(address, threadsTransactionsAddresses[make_pair(i, j)])
					{
						currentTransactionAddresses.insert(*address);
					}
				}
			}
			FOREACH(address, currentTransactionAddresses)
			{
				transactionsAddresses[i].push_back(*address);
			}
		}
	}
	vector<int> transactionsNumberUpToFile;
	transactionsNumberUpToFile.resize(filesNumber);
	for (int i = 0;i < filesNumber;i++)transactionsNumberUpToFile[i] = 0;
	for (int i = 1;i < filesNumber;i++)
	{
		transactionsNumberUpToFile[i] = transactionsNumberUpToFile[i - 1] + allTransactionsVector[i - 1].size();
	}
	int transactionsNumberxx = transactionsNumberUpToFile[filesNumber-1] + allTransactionsVector[filesNumber - 1].size();
	allTransactions.clear();
	allTransactions.resize(transactionsNumberxx);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < filesNumber;i++)
		{
			for (int j = 0;j < allTransactionsVector[i].size();j++)
			{
				allTransactions[transactionsNumberUpToFile[i] + j] = allTransactionsVector[i][j];
			}
		}
	}
	FILE **saveTransactionsAddresses;
	saveTransactionsAddresses = new FILE*[filesNumber];
	if (performSave)
	{
		for (int i = 0;i < filesNumber;i++)
		{
			pathxx = pathx + relativePath + "contractions\\transactions_input_addresses\\" + dec2String(i);
			saveTransactionsAddresses[i] = fopen(pathxx.c_str(), "w");
		}
		int period = allTransactions.size() / filesNumber + 1;
		#pragma omp parallel num_threads(threadsNumber)
		{
			#pragma omp for
			for (int i = 0;i < allTransactions.size();i += period)
			{
				for (int j = i;j < min(i + period, allTransactions.size());j++)
				{
					fprintf(saveTransactionsAddresses[j / period], "%d %d ",
						allTransactions[j], transactionsAddresses[allTransactions[j]].size());
					FOREACH(address, transactionsAddresses[allTransactions[j]])
					{
						fprintf(saveTransactionsAddresses[j / period], "%d ", *address);
					}
					fprintf(saveTransactionsAddresses[j / period], "\n");
				}
			}
		}
		for (int i = 0;i < filesNumber;i++)
		{
			fclose(readTransactions[i]);
			fclose(readInputs[i]);
			fclose(saveTransactionsAddresses[i]);
		}
	}
}

void AddressesContractor::loadTransactionsAddresses()
{
	FILE *readTransactionsAddresses;
	readTransactionsAddresses = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\contractions\\transactions_addresses_no_repetitions.dat", "r+");
	int txId, addrNumber, addrId;
	int it = 0;
	transactionsAddresses.clear();
	transactionsAddresses.resize(transactionsNumber);
	consideredAddresses.clear();
	consideredAddresses.resize(addressesNumber);
	cout << "Clearing considered addresses before loading:" << endl;
	for (int i = 0;i < addressesNumber;i++)
	{
		consideredAddresses[i] = -1;
		if (i % 10000000 == 0)cout << i << endl;
	}
	allTransactions.clear();
	neighborsList.clear();
	neighborsList.resize(addressesNumber);
	cout << "Loading transactions addresses:" << endl;
	while (!feof(readTransactionsAddresses))
	{
		fscanf(readTransactionsAddresses, "%i", &txId);
		fscanf(readTransactionsAddresses, "%i", &addrNumber);
		for (int i = 0;i < addrNumber;i++)
		{
			fscanf(readTransactionsAddresses, "%i", &addrId);
			transactionsAddresses[txId].push_back(addrId);
			if (consideredAddresses[addrId] == -1)
			{
				consideredAddresses[addrId] = 0;
				neighborsList[addrId].clear();
			}
		}
		allTransactions.push_back(txId);
		if (it % 10000000 == 0)
		{
			cout << it << " " << txId << endl;
		}
		it++;
	}
	fclose(readTransactionsAddresses);
}

void AddressesContractor::loadTransactionsAddressesParallel(string relativePath, int threadsNumber)
{
	FILE **readTransactionsAddresses;
	readTransactionsAddresses = new FILE*[filesNumber];
	string pathxx;
	for (int i = 0;i < filesNumber;i++)
	{
		pathxx = pathx + relativePath + "contractions\\transactions_input_addresses\\" + dec2String(i);
		readTransactionsAddresses[i] = fopen(pathxx.c_str(), "r");
	}
	transactionsAddresses.clear();
	transactionsAddresses.resize(transactionsNumber);
	allTransactions.clear();
	neighborsList.clear();
	neighborsList.resize(addressesNumber);
	map<pair<int, int>, set<int> > threadsTransactionsAddresses;
	vector<int> allTransactionsPrivate;
	vector<set<int> > allTransactionsSet;
	allTransactionsSet.resize(filesNumber);
	vector<vector<int> > allTransactionsVector;
	allTransactionsVector.resize(filesNumber);
	#pragma omp parallel private(allTransactionsPrivate) num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < filesNumber;i++)
		{
			int txId, addrNumber, addrId;
			while (!feof(readTransactionsAddresses[i]))
			{
				fscanf(readTransactionsAddresses[i], "%d %d", &txId, &addrNumber);
				allTransactionsSet[i].insert(txId);
				for (int j = 0;j < addrNumber;j++)
				{
					fscanf(readTransactionsAddresses[i], "%d", &addrId);
					threadsTransactionsAddresses[make_pair(txId,omp_get_thread_num())].insert(addrId);
					neighborsList[addrId].clear();
				}
			}
			FOREACH(transaction, allTransactionsSet[i])allTransactionsVector[i].push_back(*transaction);
		}
	}
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < transactionsNumber;i++)
		{
			set<int> currentTransactionAddresses;
			for (int j = 0;j < threadsNumber;j++)
			{
				if (threadsTransactionsAddresses.find(make_pair(i, j)) != threadsTransactionsAddresses.end())
				{
					FOREACH(address, threadsTransactionsAddresses[make_pair(i, j)])
					{
						currentTransactionAddresses.insert(*address);
					}
				}
			}
			FOREACH(address, currentTransactionAddresses)
			{
				transactionsAddresses[i].push_back(*address);
			}
		}
	}
	vector<int> transactionsNumberUpToFile;
	transactionsNumberUpToFile.resize(filesNumber);
	for (int i = 0;i < filesNumber;i++)transactionsNumberUpToFile[i] = 0;
	for (int i = 1;i < filesNumber;i++)
	{
		transactionsNumberUpToFile[i] = transactionsNumberUpToFile[i - 1] + allTransactionsVector[i - 1].size();
	}
	int transactionsNumberxx = transactionsNumberUpToFile[filesNumber - 1] + allTransactionsVector[filesNumber - 1].size();
	allTransactions.clear();
	allTransactions.resize(transactionsNumberxx);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < filesNumber;i++)
		{
			for (int j = 0;j < allTransactionsVector[i].size();j++)
			{
				allTransactions[transactionsNumberUpToFile[i] + j] = allTransactionsVector[i][j];
			}
		}
	}
	for (int i = 0;i < filesNumber;i++)fclose(readTransactionsAddresses[i]);
}

void AddressesContractor::createNeighborsListParallel(string relativePath, int threadsNumber)
{
	FILE **save;
	save = new FILE*[filesNumber];
	string pathxx;
	for (int i = 0;i < filesNumber;i++)
	{
		pathxx = pathx + relativePath + "contractions\\neighbors_list\\" + dec2String(i);
		save[i] = fopen(pathxx.c_str(), "w");
	}
	map<pair<int, int>, vector<int> > threadsNeighborsList;
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < allTransactions.size();i++)
		{
			vector <int> addresses;
			FOREACH(address, transactionsAddresses[allTransactions[i]])addresses.push_back(*address);
			int sizex = addresses.size(); sizex--;
			for (int i = 0;i < sizex;i++)
			{
				threadsNeighborsList[make_pair(addresses[i], omp_get_thread_num())].push_back(addresses[i + 1]);
				threadsNeighborsList[make_pair(addresses[i+1], omp_get_thread_num())].push_back(addresses[i]);
			}
		}
	}
	consideredAddresses.resize(addressesNumber);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < addressesNumber;i++)consideredAddresses[i] = -1;
	}
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < addressesNumber;i++)
		{
			vector< vector<int> > vectors2Merge;
			for (int j = 0;j < threadsNumber;j++)
			{
				if (threadsNeighborsList.find(make_pair(i, j)) != threadsNeighborsList.end())
				{
					vectors2Merge.push_back(threadsNeighborsList[make_pair(i, j)]);
					consideredAddresses[i] = 0;
				}
			}
			FOREACH(vector, vectors2Merge)FOREACH(address, *vector)neighborsList[i].insert(*address);
		}
	}
	int period = addressesNumber / filesNumber + 1;
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < addressesNumber;i += period)
		{
			for (int j = i;j < min(i + period, addressesNumber);j++)
			{
				if (consideredAddresses[j] == 0)
				{
					fprintf(save[j / period], "%d %d ",
						j, neighborsList[j].size());
					FOREACH(address, neighborsList[j])
					{
						fprintf(save[j / period], "%d ", *address);
					}
					fprintf(save[j / period], "\n");
				}
			}
		}
	}
	for(int i=0;i<filesNumber;i++)fclose(save[i]);
}

void AddressesContractor::createNeighborsListFromEdges()
{
	neighborsList.clear();
	neighborsList.resize(addressesNumber);
	FOREACH(edge, edges)
	{
		neighborsList[edge->first].insert(edge->second);
		neighborsList[edge->second].insert(edge->first);
	}
}

void AddressesContractor::loadNeighborsList()
{
	FILE *read;
	read = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\contractions\\neighbors_list_customized.dat", "r+");
	int addr, addrNumber, addrId;
	consideredAddresses.clear();
	consideredAddresses.resize(addressesNumber);
	neighborsList.clear();
	neighborsList.resize(addressesNumber);
	for (int i = 0;i < addressesNumber;i++)
	{
		consideredAddresses[i] = -1;
		neighborsList[i].clear();
	}
	while (!feof(read))
	{
		fscanf(read, "%i", &addrId);
		fscanf(read, "%i", &addrNumber);
		if (consideredAddresses[addrId] == -1)
		{
			consideredAddresses[addrId] = 0;
		}
		for (int i = 0;i < addrNumber;i++)
		{
			int addr;
			fscanf(read, "%i", &addr);
			neighborsList[addrId].insert(addr);
		}
	}
	fclose(read);
}

void AddressesContractor::loadNeighborsListParallel(string relativePath, int threadsNumber)
{
	FILE **read;
	read = new FILE*[filesNumber];
	string pathxx;
	for (int i = 0;i < filesNumber;i++)
	{
		pathxx = pathx + relativePath + "contractions\\neighbors_list\\" + dec2String(i);
		read[i] = fopen(pathxx.c_str(), "r");
	}
	consideredAddresses.clear();
	consideredAddresses.resize(addressesNumber);
	neighborsList.clear();
	neighborsList.resize(addressesNumber);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < addressesNumber;i++)
		{
			consideredAddresses[i] = -1;
			neighborsList[i].clear();
		}
	}
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < filesNumber;i++)
		{
			int addr, addrNumber, addrId;
			while (!feof(read[i]))
			{
				fscanf(read[i], "%d %d", &addrId, &addrNumber);
				consideredAddresses[addrId] = 0;
				for (int j = 0;j < addrNumber;j++)
				{
					fscanf(read[i], "%d", &addr);
					neighborsList[addrId].insert(addr);
				}
			}
			fclose(read[i]);
		}
	}
}

void AddressesContractor::createEdgesParallel(string relativePath, int threadsNumber, bool performSave)
{
	FILE **save;
	save = new FILE*[filesNumber];
	if (performSave)
	{
		string pathxx;
		for (int i = 0;i < filesNumber;i++)
		{
			pathxx = pathx + relativePath + "contractions\\edges\\" + dec2String(i)+".txt";
			save[i] = fopen(pathxx.c_str(), "w");
		}
	}
	vector<vector<pair<int,int> > > threadsEdges;
	threadsEdges.resize(threadsNumber);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < allTransactions.size();i++)
		{
			vector <int> addresses;
			FOREACH(address, transactionsAddresses[allTransactions[i]])addresses.push_back(*address);
			int sizex = addresses.size(); sizex--;
			for (int i = 0;i < sizex;i++)
			{
				threadsEdges[omp_get_thread_num()].push_back(make_pair(addresses[i], addresses[i + 1]));
				threadsEdges[omp_get_thread_num()].push_back(make_pair(addresses[i+1], addresses[i]));
			}
		}
	}
	int edgesNumber = 0;
	FOREACH(threadVector, threadsEdges)edgesNumber += threadVector->size();
	consideredAddresses.resize(addressesNumber);
	edges.resize(edgesNumber);
	vector<int> edgesUpToThread;
	edgesUpToThread.resize(threadsNumber);
	edgesUpToThread[0] = 0;
	for (int i = 1;i < threadsNumber;i++)edgesUpToThread[i] = edgesUpToThread[i - 1] + threadsEdges[i-1].size();
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < addressesNumber;i++)consideredAddresses[i] = -1;
	}
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < threadsNumber;i++)
		{
			for (int j = 0;j < threadsEdges[i].size();j++)
			{
				edges[edgesUpToThread[i] + j] = threadsEdges[i][j];
				consideredAddresses[threadsEdges[i][j].first] = 0;
			}
		}
	}
	if (performSave)
	{
		int period = edgesNumber / filesNumber + 1;
		#pragma omp parallel num_threads(threadsNumber)
		{
			#pragma omp for
			for (int i = 0;i < edgesNumber;i += period)
			{
				for (int j = i;j < min(i + period, edgesNumber);j++)
				{
					fprintf(save[j / period], "%d %d\n", edges[j].first, edges[j].second);
					//if (edges[j].first==0&&edges[j].second==0)cout << j  << endl;
				}
			}
		}
		for (int i = 0;i < filesNumber;i++)fclose(save[i]);
	}
	vector<vector<int> > threadsAddresses;
	threadsAddresses.resize(threadsNumber);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < addressesNumber;i++)
		{
			if (consideredAddresses[i] == 0)threadsAddresses[omp_get_thread_num()].push_back(i);
		}
	}
	vector<int> addressesNumberUpToThread;
	addressesNumberUpToThread.resize(threadsNumber);
	addressesNumberUpToThread[0] = 0;
	for (int i = 1;i < threadsNumber;i++)
	{
		addressesNumberUpToThread[i] = addressesNumberUpToThread[i - 1] + threadsAddresses[i - 1].size();
	}
	int addressesNumberx = addressesNumberUpToThread[threadsNumber - 1] + threadsAddresses[threadsNumber - 1].size();
	addresses.resize(addressesNumberx);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < threadsNumber;i ++)
		{
			for (int j = 0;j < threadsAddresses[i].size();j++)
			{
				addresses[addressesNumberUpToThread[i] + j] = threadsAddresses[i][j];
			}
		}
	}
}

void AddressesContractor::loadEdgesParallel(string relativePath, int threadsNumber)
{
	FILE **readEdges;
	readEdges = new FILE*[filesNumber];
	string pathxx;
	for (int i = 0;i < filesNumber;i++)
	{
		pathxx = pathx + relativePath + "contractions\\edges\\" + dec2String(i) + ".txt";
		readEdges[i] = fopen(pathxx.c_str(), "r");
	}
	edges.clear();
	vector< vector<pair<int, int> >  > filesEdges;
	filesEdges.resize(filesNumber);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < filesNumber;i++)
		{
			pair<int,int> edge;
			while (!feof(readEdges[i]))
			{
				fscanf(readEdges[i], "%d %d", &edge.first, &edge.second);
				filesEdges[i].push_back(edge);
			}
		}
	}
	vector<int> edgesNumberUpToFile;
	edgesNumberUpToFile.resize(filesNumber);
	for (int i = 0;i < filesNumber;i++)edgesNumberUpToFile[i] = 0;
	for (int i = 1;i < filesNumber;i++)
	{
		edgesNumberUpToFile[i] = edgesNumberUpToFile[i - 1] + filesEdges[i - 1].size();
	}
	int edgesNumberxx = edgesNumberUpToFile[filesNumber - 1] + filesEdges[filesNumber - 1].size();
	edges.resize(edgesNumberxx);
	consideredAddresses.resize(addressesNumber);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < addressesNumber;i++)consideredAddresses[i] = -1;
	}
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < filesNumber;i++)
		{
			for (int j = 0;j < filesEdges[i].size();j++)
			{
				edges[edgesNumberUpToFile[i] + j] = filesEdges[i][j];
				consideredAddresses[filesEdges[i][j].first] = 0;
				consideredAddresses[filesEdges[i][j].second] = 0;
			}
		}
	}
	for (int i = 0;i < filesNumber;i++)fclose(readEdges[i]);
	vector<vector<int> > threadsAddresses;
	threadsAddresses.resize(threadsNumber);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < addressesNumber;i++)
		{
			if (consideredAddresses[i] == 0)threadsAddresses[omp_get_thread_num()].push_back(i);
		}
	}
	vector<int> addressesNumberUpToThread;
	addressesNumberUpToThread.resize(threadsNumber);
	addressesNumberUpToThread[0] = 0;
	for (int i = 1;i < threadsNumber;i++)
	{
		addressesNumberUpToThread[i] = addressesNumberUpToThread[i - 1] + threadsAddresses[i - 1].size();
	}
	int addressesNumberx = addressesNumberUpToThread[threadsNumber - 1] + threadsAddresses[threadsNumber - 1].size();
	addresses.resize(addressesNumberx);
	#pragma omp parallel num_threads(threadsNumber)
	{
		#pragma omp for
		for (int i = 0;i < threadsNumber;i++)
		{
			for (int j = 0;j < threadsAddresses[i].size();j++)
			{
				addresses[addressesNumberUpToThread[i] + j] = threadsAddresses[i][j];
			}
		}
	}
}

void AddressesContractor::createNeighborsList()
{
	FILE *save;
	save = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\contractions\\neighbors_list_test1.dat","w+");
	vector <int> addresses;
	int sizex;
	FOREACH(trans, allTransactions)
	{
		addresses.clear();
		FOREACH(address, transactionsAddresses[*trans])
		{
			addresses.push_back(*address);
		}
		sizex = addresses.size(); sizex--;
		for (int ix = 0;ix < sizex;ix++)
		{
			neighborsList[addresses[ix]].insert(addresses[ix + 1]);
			neighborsList[addresses[ix + 1]].insert(addresses[ix]); 
		}
	}
	for (int ix = 0;ix < addressesNumber;ix++)
	{
		if (consideredAddresses[ix] == 0)
		{
			fprintf(save, "%d %d ", ix, neighborsList[ix].size());
			FOREACH(v, neighborsList[ix])
			{
				fprintf(save, "%d ", *v);
			}
			fprintf(save, "\n");
		}
	}
	fclose(save);
}

void AddressesContractor::contractAddressesBFS()
{
	contractedAddresses.clear();
	currentComponent.clear();
	//FILE *save;
	//save = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\contractions\\contracted_addresses_cpp.dat", "w+");
	for (int i = 0;i < addressesNumber;i++)
	{
		if (consideredAddresses[i] == 0)
		{
			layer.clear();
			layer.insert(i);
			currentComponent.clear();
			BFS();
			contractedAddresses.push_back(currentComponent);
			/*fprintf(save, "%d ", currentComponent.size());
			FOREACH(addr, currentComponent)
			{
				fprintf(save, "%d ", *addr);
			}
			fprintf(save,"\n");*/
		}
	}
	//fclose(save);
}

void AddressesContractor::BFS()
{
	currentLayer = layer;
	nextLayer.clear();
	while (!currentLayer.empty())
	{
		nextLayer.clear();
		FOREACH(addr, currentLayer)
		{
			consideredAddresses[*addr] = 1;
			currentComponent.insert(*addr);
		}
		FOREACH(address, currentLayer)
		{
			FOREACH(neighbor, neighborsList[*address])
			{
				if (consideredAddresses[*neighbor] == 0)
				{
					nextLayer.insert(*neighbor);
					consideredAddresses[*neighbor] = 1;
				}
			}
		}
		currentLayer = nextLayer;
	}
}

