#include <fstream>
#include <cstdio>
#include <vector>
#include <iostream>
#include <string>
#include <set>

using namespace std;

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

const int WEEKS_NUMBER = 2000; // (2000 weeks is appr. 39 years)

int main(int argc, char **argv)
{
    FILE *readActiveUsersSubgraph;
    readActiveUsersSubgraph = fopen(argv[1], "r");
    int userInputID, userOutputID, timeStamp;
    long long bitcoinAmountInSatoshis;
    vector <long long> bitcoinAmountWeekly(WEEKS_NUMBER,0), edgesNumberWeekly(WEEKS_NUMBER,0);
    vector <int> nodesNumberWeekly(WEEKS_NUMBER,0);
    set<int> vertsCons;
    string beginningDate = argv[3];
    long long begTimeStamp = dateToTimestamp(beginningDate);

    int maxUserId = 0;

    while(
        fscanf(
            readActiveUsersSubgraph, "%i %i %lld %i", 
            &userInputID, 
            &userOutputID, 
            &bitcoinAmountInSatoshis, 
            &timeStamp
        ) == 4
    )
    {
        if(timeStamp >= begTimeStamp)
        {
            if(vertsCons.find(userInputID) == vertsCons.end())
            {
                nodesNumberWeekly[(timeStamp-begTimeStamp)/(7*86400)]++;
                vertsCons.insert(userInputID);
            }
            if(vertsCons.find(userOutputID) == vertsCons.end())
            {
                nodesNumberWeekly[(timeStamp-begTimeStamp)/(7*86400)]++;
                vertsCons.insert(userOutputID);
            }
            bitcoinAmountWeekly[(timeStamp-begTimeStamp)/(7*86400)] += bitcoinAmountInSatoshis;
            edgesNumberWeekly[(timeStamp-begTimeStamp)/(7*86400)] ++;
        }

        if(maxUserId<userInputID)maxUserId = userInputID;
        if(maxUserId<userOutputID)maxUserId = userOutputID;
    }
    fclose(readActiveUsersSubgraph);

    FILE *saveActiveSubgraphStatistics;
    saveActiveSubgraphStatistics = fopen(argv[2],"w");
    int nodesNumberCumulative = 0;
    long long bitcoinAmountCumulative = 0;
    long long edgesNumberCumulative = 0;
    for(int i=0;i<WEEKS_NUMBER;i++)
    {
        fprintf(
            saveActiveSubgraphStatistics,
            "%d %lld %lld\n",
            nodesNumberCumulative+nodesNumberWeekly[i],
            bitcoinAmountCumulative+bitcoinAmountWeekly[i],
            edgesNumberCumulative+edgesNumberWeekly[i]
        );
        nodesNumberCumulative += nodesNumberWeekly[i];
        bitcoinAmountCumulative += bitcoinAmountWeekly[i];
        edgesNumberCumulative += edgesNumberWeekly[i];
    }
    fclose(saveActiveSubgraphStatistics);

    return 0;
}
