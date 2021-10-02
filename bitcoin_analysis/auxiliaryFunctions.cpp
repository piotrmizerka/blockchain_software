#include "stdafx.h"

// long-term subgraph (as describet in REFERENCE) creation
Graph longTermSubgraph(int minimalRepresentativeAddressesNumber, int minimalIntervalInDays, int minimalTransactionsNumber, string usersGraphPath, string contractedAddressesPath)
{
	// Determine users represented by at least minimalRepresentativeAddressesNumber
	FILE* readContractedAddresses;
	readContractedAddresses = fopen(contractedAddressesPath.c_str(), "r");
	int maxUserId = 0, addressId, userId;
	while (!feof(readContractedAddresses))
	{
		fscanf(readContractedAddresses, "%i %i", &addressId, &userId);
		if (maxUserId < userId)maxUserId = userId;
	}
	fclose(readContractedAddresses);
	vector<int> representativeAddressesNumber(maxUserId + 1, 0);
	readContractedAddresses = fopen(contractedAddressesPath.c_str(), "r");
	while (!feof(readContractedAddresses))
	{
		fscanf(readContractedAddresses, "%i %i", &addressId, &userId);
		representativeAddressesNumber[userId]++;
	}
	fclose(readContractedAddresses);

	// Determine activity period and transactions number for each user
	vector< pair< pair<int, int>, int > > usersActivityData(maxUserId + 1, make_pair(make_pair(1931195950, 0), 0));
	FILE* readUsersGraph;
	int userInputID, userOutputID, timeStamp;
	long long bitcoinAmountInSatoshis;
	readUsersGraph = fopen(usersGraphPath.c_str(), "r");
	while (!feof(readUsersGraph))
	{
		fscanf(readUsersGraph, "%i %i %lld %i", &userInputID, &userOutputID, &bitcoinAmountInSatoshis, &timeStamp);
		if (timeStamp < usersActivityData[userInputID].first.first)usersActivityData[userInputID].first.first = timeStamp;
		if (timeStamp > usersActivityData[userInputID].first.second)usersActivityData[userInputID].first.second = timeStamp;
		if (timeStamp < usersActivityData[userOutputID].first.first)usersActivityData[userOutputID].first.first = timeStamp;
		if (timeStamp > usersActivityData[userOutputID].first.second)usersActivityData[userOutputID].first.second = timeStamp;
		usersActivityData[userInputID].second++;
		usersActivityData[userOutputID].second++;
	}
	fclose(readUsersGraph);

	// Extract the long-term subgraph as described in REFERENCE
	vector <int> V;
	vector < Edge > E;
	int transactionsNumber = 0;
	vector<bool> consideredVertices(maxUserId, false);
	readUsersGraph = fopen(usersGraphPath.c_str(), "r");
	while (!feof(readUsersGraph))
	{
		fscanf(readUsersGraph, "%i %i %lld %i", &userInputID, &userOutputID, &bitcoinAmountInSatoshis, &timeStamp);
		if (representativeAddressesNumber[userInputID] >= minimalRepresentativeAddressesNumber && representativeAddressesNumber[userOutputID] >= minimalRepresentativeAddressesNumber &&
			(double(usersActivityData[userInputID].first.second - usersActivityData[userInputID].first.first) / 86400) >= minimalIntervalInDays &&
			(double(usersActivityData[userOutputID].first.second - usersActivityData[userOutputID].first.first) / 86400) >= minimalIntervalInDays &&
			usersActivityData[userInputID].second >= minimalTransactionsNumber && usersActivityData[userOutputID].second >= minimalTransactionsNumber)
		{
			if (consideredVertices[userInputID] == false)
			{
				V.push_back(userInputID);
				consideredVertices[userInputID] = true;
			}
			if (consideredVertices[userOutputID] == false)
			{
				V.push_back(userOutputID);
				consideredVertices[userOutputID] = true;
			}
			E.push_back({ userInputID,userOutputID,timeStamp,double(bitcoinAmountInSatoshis) });
			transactionsNumber++;
		}
	}
	fclose(readUsersGraph);

	return Graph(V, E, transactionsNumber, maxUserId);
}

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

