// Create Bitcoin users graph from the contracted addresses with cpp to make it faster.

#include <fstream>
#include <cstdio>
#include <vector>

using namespace std;

int main(int argc, char **argv)
{
    // Determine maximal bitcoin address to know how big the vector to store user idies for bitcoin addresses need to be
    int maxAddress = 0;
    FILE *readContractedAddresses;
    int bitcoinAddress, userIdx;
    readContractedAddresses = fopen(argv[1], "r");
    while(!feof(readContractedAddresses))
    {
        fscanf(readContractedAddresses,"%i %i",&bitcoinAddress,&userIdx);
        if(maxAddress<bitcoinAddress)maxAddress = bitcoinAddress;
    }
    fclose(readContractedAddresses);

    // Save user idies for Bitcoin addresses in the memory - in the vector
    vector<int> userId(maxAddress+1);
    readContractedAddresses = fopen(argv[1], "r");
    while(!feof(readContractedAddresses))
    {
        fscanf(readContractedAddresses,"%i %i",&bitcoinAddress,&userIdx);
        userId[bitcoinAddress] = userIdx;
    }
    fclose(readContractedAddresses);

    // Save users graph in the dedicated file - usersGraph.dat
    FILE *readTxEdgesTimes, *saveUsersGraph;
    readTxEdgesTimes = fopen(argv[2],"r");
    saveUsersGraph = fopen(argv[3],"w");
    int inputBitcoinAddress, outputBitcoinAddress, timeStamp;
    float bitcoinAmount;
    while (!feof(readTxEdgesTimes))
    {
        fscanf(readTxEdgesTimes,"%i %i %f %i", &inputBitcoinAddress, &outputBitcoinAddress, &bitcoinAmount, &timeStamp);
        fprintf(saveUsersGraph, "%i %i %.0lf %i\n",userId[inputBitcoinAddress],userId[outputBitcoinAddress],bitcoinAmount,timeStamp);
    }
    fclose(readTxEdgesTimes);
    fclose(saveUsersGraph);

    return 0;
}
