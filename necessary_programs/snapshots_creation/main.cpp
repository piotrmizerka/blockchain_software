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
    time_t now = time(0); // TODO: change for the time of the last considered block!!!!!
    time_t beginningOfBitcoin = 1231722000; // TODO: add beggining time parameter!!!
    int snapshotsNumber = int(double(now-beginningOfBitcoin)/double(snapshotPeriodInDays*86400))+1;
    map< pair<int,int>, long long > snapshotGraphEdges[snapshotsNumber];
    int u, v;
    long long bitcoinAmountSatoshis, timeStamp;
    FILE* readLongTermSubgraph = fopen(longTermSubgraphPath.c_str(), "r");
    long long linesNumber = 0;
    while(!feof(readLongTermSubgraph))
    {
        fscanf(readLongTermSubgraph, "%i %i %lld %lld", &u, &v, &bitcoinAmountSatoshis, &timeStamp);
        for(int i=0;i<snapshotsNumber;i++)snapshotGraphEdges[i][make_pair(u,v)] = 0;
        linesNumber++;
    }
    fclose(readLongTermSubgraph);

    // Create snapshots
    struct tm * timeInfo;
    string fileName;

    FILE* saveSnapshots[snapshotsNumber+10];
    time_t snapShotTimestamp = beginningOfBitcoin;

    for(int i=0;i<snapshotsNumber;i++)
    {
        timeInfo = localtime(&snapShotTimestamp);
        fileName = snapshotPath+"/"+dateToString(timeInfo)+".dat";
        saveSnapshots[i] = fopen(fileName.c_str(), "w");
        snapShotTimestamp += snapshotPeriodInDays*86400;
    }

    readLongTermSubgraph = fopen(longTermSubgraphPath.c_str(), "r");

    for(long long i=0;i<linesNumber-1;i++)
    {
        fscanf(readLongTermSubgraph, "%i %i %lld %lld", &u, &v, &bitcoinAmountSatoshis, &timeStamp);

        // I don't know why I had to add one id - this seems to be irrelevant for the sake of experiments, though:
        int snapshotId = int(double(timeStamp-beginningOfBitcoin)/double(snapshotPeriodInDays*86400))+1; 
        if(snapshotEdgeWeightParameter == "w")
        {
            snapshotGraphEdges[snapshotId][make_pair(u,v)] += bitcoinAmountSatoshis;
        }
        else 
        {
            snapshotGraphEdges[snapshotId][make_pair(u,v)]++;
        }
    }
    fclose(readLongTermSubgraph);

    // Saving the snapshots
    for(int i=0;i<snapshotsNumber;i++)
    {
        FOREACH(edge, snapshotGraphEdges[i])fprintf(saveSnapshots[i], "%i %i %lld\n", (edge->first).first, (edge->first).second, edge->second);
        fclose(saveSnapshots[i]);
    }

    return 0;
}