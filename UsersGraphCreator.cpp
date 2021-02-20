#include "stdafx.h"




void UsersGraphCreator::contractAddresses(string relativePath)
{
	createNeighborsList();
	contractAddressesBFS();
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

vector < vector<int> > UsersGraphCreator::transactionsAddresses()
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
	return transactionsAddresses;
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