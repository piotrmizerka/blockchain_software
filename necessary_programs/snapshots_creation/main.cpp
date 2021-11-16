#include "stdafx.h"

string dateToString(tm * timeInfo)
{
    string result = to_string(timeInfo->tm_year+1900)+"_";
    if(timeInfo->tm_mon < 9)result += "0";
    result += (to_string(timeInfo->tm_mon+1)+"_");
    if(timeInfo->tm_mday < 10)result += "0";
    result += to_string(timeInfo->tm_mday);

    return result;
}

int main(int argc, char* argv[])
{
    string longTermSubgraphPath = argv[1];
    int snapshotPeriodInDays = atoi(argv[2]);
    string snapshotEdgeWeightParameter = argv[3];
    string snapshotPath = argv[4];

    // Initialize the underlying edges occuring in each snapshot (they are the same for each snapshot).
    time_t now = time(0);
    time_t beginningOfBitcoin = 1231722000;
    int snapshotsNumber = (now-beginningOfBitcoin)/(snapshotPeriodInDays*86400)+1;
    map< pair<int,int>, long long > snapshotGraphEdges[snapshotsNumber];
    int u, v;
    long long bitcoinAmountSatoshis, timeStamp;
    FILE* readLongTermSubgraph = fopen(longTermSubgraphPath.c_str(), "r");
    while(!feof(readLongTermSubgraph))
    {
        fscanf(readLongTermSubgraph, "%i %i %lld %lld", &u, &v, &bitcoinAmountSatoshis, &timeStamp);
        for(int i=0;i<snapshotsNumber;i++)snapshotGraphEdges[i][make_pair(u,v)] = 0;
    }
    fclose(readLongTermSubgraph);

    // Create snapshots
    struct tm * timeInfo;
    string fileName;

    FILE* saveSnapshots[snapshotsNumber];
    time_t snapShotTimestamp = beginningOfBitcoin;

    for(int i=0;i<snapshotsNumber;i++)
    {
        timeInfo = localtime(&snapShotTimestamp);
        // fileName = "../../snapshots/"+dateToString(timeInfo)+".dat";
        // fileName = "/home/piotr/Desktop/bitcoin/blockchain_software/snapshots/text.dat";
        // fileName = "/home/piotr/Desktop/bitcoin/blockchain_software/snapshots/"+dateToString(timeInfo)+".dat";
        fileName = snapshotPath+"/"+dateToString(timeInfo)+".dat";
        saveSnapshots[i] = fopen(fileName.c_str(), "w");

        // cout << saveSnapshots[i]->_flags << endl;

        snapShotTimestamp += snapshotPeriodInDays*86400;
    }

    // readLongTermSubgraph = fopen(longTermSubgraphPath.c_str(), "r");
    // while(!feof(readLongTermSubgraph))
    // {
    //     fscanf(readLongTermSubgraph, "%i %i %lld %lld", &u, &v, &bitcoinAmountSatoshis, &timeStamp);
    //     if(snapshotEdgeWeightParameter == "w")
    //     {
    //         snapshotGraphEdges[(timeStamp-beginningOfBitcoin)/(snapshotPeriodInDays*86400)][make_pair(u,v)] += bitcoinAmountSatoshis;
    //     }
    //     else 
    //     {
    //         snapshotGraphEdges[(timeStamp-beginningOfBitcoin)/(snapshotPeriodInDays*86400)][make_pair(u,v)]++;
    //     }
    // }
    // fclose(readLongTermSubgraph);

    // // Saving the snapshots
    // for(int i=0;i<snapshotsNumber;i++)
    // {
    //     FOREACH(edge, snapshotGraphEdges[i])fprintf(saveSnapshots[i], "%i %i %lld\n", (edge->first).first, (edge->first).second, edge->second);
    //     fclose(saveSnapshots[i]);
    // }

    return 0;
}