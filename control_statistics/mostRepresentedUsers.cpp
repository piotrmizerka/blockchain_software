#include <fstream>
#include <cstdio>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <set>

using namespace std;

int main(int argc, char **argv)
{
    FILE *readContractedAddresses;
    readContractedAddresses = fopen(argv[1], "r");
    int userID, bitcoinAddress;
    vector <int> userRepresentativesNumber(400000000, 0);
    set <int> users, addresses;
    while(
        fscanf(
            readContractedAddresses, "%i %i", 
            &bitcoinAddress, 
            &userID
        ) == 2
    )
    {
        userRepresentativesNumber[userID]++;
        users.insert(userID);
        addresses.insert(bitcoinAddress);
    }
    fclose(readContractedAddresses);

    sort(userRepresentativesNumber.rbegin(), userRepresentativesNumber.rend());

    FILE *saveMostRepresentedUsers;
    saveMostRepresentedUsers = fopen(argv[2],"w");
    fprintf(saveMostRepresentedUsers, "Number of addresses: %lu\n", addresses.size());
    fprintf(saveMostRepresentedUsers, "Number of users: %lu\n", users.size());
    fprintf(saveMostRepresentedUsers, "Number of addresses representing the ten most represented users:\n");
    for(int i=0;i<10;i++)fprintf(saveMostRepresentedUsers, "%d\n", userRepresentativesNumber[i]);
    fclose(saveMostRepresentedUsers);

    return 0;
}
