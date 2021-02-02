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

UsersGraphCreator::UsersGraphCreator(int transactionsNumberx, int addressesNumberx, int usersNumberx, int filesNumberx, string pathxx)
{
	transactionsNumber = transactionsNumberx;
	addressesNumber = addressesNumberx;
	usersNumber = usersNumberx;
	filesNumber = filesNumberx;
	pathx = pathxx;
}

vector<vector<int>> UsersGraphCreator::getTransactionsAddresses()
{
	return transactionsAddresses;
}

vector<set<int>> UsersGraphCreator::getNeighborsList()
{
	return neighborsList;
}

vector<pair<int, int>> UsersGraphCreator::getEdges()
{
	return edges;
}

vector<int> UsersGraphCreator::getAddresses()
{
	return addresses;
}

vector<set<int>> UsersGraphCreator::getContractedAddresses()
{
	return contractedAddresses;
}

vector<int> UsersGraphCreator::getComponentRepresentatives()
{
	return componentRepresentatives;
}


void UsersGraphCreator::contractAddresses(string relativePath)
{
	loadEdgesParallel(relativePath);
	createNeighborsListFromEdges();
	//loadNeighborsList();
	contractAddressesBFS();
}

void UsersGraphCreator::createNeighborsListParallel(string relativePath, int threadsNumber)
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
					fprintf(save[j / period], "%d %ld ",
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

void UsersGraphCreator::createNeighborsListFromEdges()
{
	neighborsList.clear();
	neighborsList.resize(addressesNumber);
	FOREACH(edge, edges)
	{
		neighborsList[edge->first].insert(edge->second);
		neighborsList[edge->second].insert(edge->first);
	}
}


void UsersGraphCreator::createEdgesParallel(string relativePath, int threadsNumber, bool performSave)
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

void UsersGraphCreator::loadEdgesParallel(string relativePath, int threadsNumber)
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

void UsersGraphCreator::createNeighborsList()
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
			fprintf(save, "%d %ld ", ix, neighborsList[ix].size());
			FOREACH(v, neighborsList[ix])
			{
				fprintf(save, "%d ", *v);
			}
			fprintf(save, "\n");
		}
	}
	fclose(save);
}

void UsersGraphCreator::contractAddressesBFS()
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

void UsersGraphCreator::BFS()
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