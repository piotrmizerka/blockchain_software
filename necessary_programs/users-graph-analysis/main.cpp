#include "stdafx.h"

// long-term subgraph (as described in REFERENCE) creation
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

int main(int argc, char* argv[])
{
    // Long-term subgraph creation
    int minimalRepresantativeAddressesNumber = atoi(argv[1]);
    int minimalIntervalInDays = atoi(argv[2]);
    int minimalTransationsNumber = atoi(argv[3]);
    string usersGraphPath = argv[4];
    string contractedAddressesPath = argv[5];
    Graph longTermSubgraphx = longTermSubgraph(minimalRepresantativeAddressesNumber, minimalIntervalInDays,
                                               minimalTransationsNumber, usersGraphPath, contractedAddressesPath);
    
    string savePath = argv[6];
    longTermSubgraphx.saveGraph(savePath);

    return 0;
}