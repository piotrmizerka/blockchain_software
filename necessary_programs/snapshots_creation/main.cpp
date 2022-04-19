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

time_t dateToTimestamp(string date)
{
    string yearString = date.substr(0,4);
    string monthString = date.substr(5, 2);
    string dayString = date.substr(8, 2);
    int year = 1000*(int(yearString[0])-48)+100*(int(yearString[1])-48)+
               10*(int(yearString[2])-48)+(int(yearString[3])-48);
    int month = 10*(int(monthString[0])-48)+(int(monthString[1])-48);
    int day = 10*(int(dayString[0])-48)+(int(dayString[1])-48);

    // The code below taken from Ubervan's answer from here: 
    // https://stackoverflow.com/questions/46033813/c-converting-datetime-to-timestamp
    time_t now = time(0);
    struct tm tmGMT = *gmtime ( &now ), tmLocaltime = *localtime(&now);
    time_t localGMTDifference = mktime(&tmLocaltime)-mktime(&tmGMT);
    struct tm tm;
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;

    return mktime(&tm)+localGMTDifference;
}

int main(int argc, char* argv[])
{
    string longTermSubgraphPath = argv[1];
    int snapshotPeriodInDays = atoi(argv[2]);
    string snapshotEdgeWeightParameter = argv[3];
    string snapshotPath = argv[4];
    string bhPath = argv[5];
    string beginningDate = argv[6]; // the date is assumed to be in the format YYYY_MM_DD, e.g. 2022_03_24

    // Determine the timestamp of the last considered block
    FILE* readBh = fopen(bhPath.c_str(), "r");
    long long blockTimestamp, maxBlockTimestamp = 0;
    int blockId, blockTransactionsNumber;
    char hashx[300];
    while(!feof(readBh))
    {
        fscanf(readBh, "%i %299s %lld %i", &blockId, hashx, &blockTimestamp, &blockTransactionsNumber);
        if(maxBlockTimestamp < blockTimestamp)maxBlockTimestamp = blockTimestamp;
    }
    fclose(readBh);

    // Convert the beginning date to consider to timestamp
    long long beginningDateTimestamp = dateToTimestamp(beginningDate);

    // Initialize the underlying edges occuring in each snapshot (they are the same for each snapshot).
    int snapshotsNumber = int(double(maxBlockTimestamp-beginningDateTimestamp)/double(snapshotPeriodInDays*86400))+1;

    map< pair<int,int>, long long > snapshotGraphEdges[snapshotsNumber];
    int u, v;
    long long bitcoinAmountSatoshis, timeStamp;
    FILE* readLongTermSubgraph = fopen(longTermSubgraphPath.c_str(), "r");
    while(
        fscanf(readLongTermSubgraph, "%i %i %lld %lld", 
               &u, 
               &v, 
               &bitcoinAmountSatoshis, 
               &timeStamp
        ) == 4
    )
    {
        for(int i=0;i<snapshotsNumber;i++)snapshotGraphEdges[i][make_pair(u,v)] = 0;
    }
    fclose(readLongTermSubgraph);

    // Create snapshots
    struct tm * timeInfo;
    string fileName;

    FILE* saveSnapshots[snapshotsNumber];
    time_t snapShotTimestamp = beginningDateTimestamp;

    for(int i=0;i<snapshotsNumber;i++)
    {
        timeInfo = localtime(&snapShotTimestamp);
        fileName = snapshotPath+"/"+dateToString(timeInfo)+".dat";
        saveSnapshots[i] = fopen(fileName.c_str(), "w");
        snapShotTimestamp += snapshotPeriodInDays*86400;
    }

    readLongTermSubgraph = fopen(longTermSubgraphPath.c_str(), "r");

    while(
        fscanf(readLongTermSubgraph, "%i %i %lld %lld", 
               &u, 
               &v, 
               &bitcoinAmountSatoshis, 
               &timeStamp
        ) == 4
    )
    {
        if(timeStamp >= beginningDateTimestamp)
        {
            // I don't know why I had to add one id - this seems to be irrelevant for the sake of experiments, though:
            int snapshotId = int(double(timeStamp-beginningDateTimestamp)/double(snapshotPeriodInDays*86400)); 
            if(snapshotEdgeWeightParameter == "w")
            {
                snapshotGraphEdges[snapshotId][make_pair(u,v)] += bitcoinAmountSatoshis;
            }
            else 
            {
                snapshotGraphEdges[snapshotId][make_pair(u,v)]++;
            }
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