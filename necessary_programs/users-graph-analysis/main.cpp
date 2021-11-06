// This program is responsible for creation of the subgraph of the users graph induced by the most active users,
// as well as for the creation of the long-term subgraph. The subgraph induced by the most active users
// is the subgraph containing only those users who are represented by at least minimalRepresentativeAddressesNumber,
// where minimalRepresentativeAddressesNumber is the prescribed minimal number of representing Bitcoin addresses
// that each user in the subgraph shall be represented by. The long-term subgraph, on the other hand is the subgraph
// of the users graph represented by users who were active by at least minimalIntervalInDays number of days
// and took part in at least minimalTransactionsNumber of transactions. In our article, https://doi.org/10.1016/j.frl.2020.101489 
// [p. 7], we extracted first the subgraph induced by the most active users represented by at least 10 Bitcoin addresses,
// and only then we extracted from this subgraph the long-term subgraph with minimalIntervalInDays = 1200
// and minimalTransactionsNumber = 200.

// Note that it is possible also to extract the long-term subgraph with the restriction conccerning minimal number
// of Bitcoin addresses at once. In this case, however, the obtained subgraph may be potentially bigger than 
// the long-term subgraph generated from the already restricted users graph to the most active users
// (all the parameters all left intact). This is due to the fact that in the latter process we may include
// users who were "false" active by sufficient amount of days or "false" took part in a sufficient amount
// of transactions - in the sense that these numbers were enlarged by transactions involving users NOT 
// represented by sufficient amount of Bitcoin addresses - these users would have been erased when proceeding
// according to the first way of creating of the long-term subgraph. 


#include "stdafx.h"

// long-term subgraph creation
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
    if(minimalRepresentativeAddressesNumber > 0)
    {
        readContractedAddresses = fopen(contractedAddressesPath.c_str(), "r");
        while (!feof(readContractedAddresses))
        {
            fscanf(readContractedAddresses, "%i %i", &addressId, &userId);
            representativeAddressesNumber[userId]++;
        }
        fclose(readContractedAddresses);
    }

    // Determine activity period and transactions number for each user
    int initStartTimeStamp = 1931195950;
    if(minimalIntervalInDays == 0 && minimalTransactionsNumber == 0)initStartTimeStamp = 0;
    vector< pair< pair<int, int>, int > > usersActivityData(maxUserId + 1, make_pair(make_pair(initStartTimeStamp, 0), 0));
    FILE* readUsersGraph;
    int userInputID, userOutputID, timeStamp;
    long long bitcoinAmountInSatoshis;
    if(minimalIntervalInDays > 0 || minimalTransactionsNumber > 0)
    {
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
    }

    // Extract the long-term subgraph
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