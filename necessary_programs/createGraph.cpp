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
    FILE *saveUsersGraph;
    saveUsersGraph = fopen(argv[3],"w");

    FILE *readTxEdgesTimes;
    readTxEdgesTimes = fopen(argv[2],"r");
    int motherTxId, inputBitcoinAddress, outputBitcoinAddress, timeStamp;
    float bitcoinAmount;

    int maxTxId = -1;
    while(
            fscanf(
                readTxEdgesTimes,"%i %i %i %f %i",
                &motherTxId, 
                &inputBitcoinAddress, 
                &outputBitcoinAddress, 
                &bitcoinAmount, 
                &timeStamp
            ) == 5
    )
    {
        if(maxTxId<motherTxId)maxTxId = motherTxId;
    }
    fclose(readTxEdgesTimes);
    
    // Use the method of D. Kondor - each transaction contributes a complete bibartite graph of
    // elementary transactions (potantially more edges); see the description here: https://github.com/dkondor/txedges
    if(atoi(argv[4]) == 0)
    {
        readTxEdgesTimes = fopen(argv[2],"r");
        while(
            fscanf(
                    readTxEdgesTimes,"%i %i %i %f %i",
                    &motherTxId, 
                    &inputBitcoinAddress, 
                    &outputBitcoinAddress, 
                    &bitcoinAmount, 
                    &timeStamp
            ) == 5
        )
        {
            fprintf(
                    saveUsersGraph, "%i %i %.0lf %i\n",
                    userId[inputBitcoinAddress],
                    userId[outputBitcoinAddress],
                    bitcoinAmount,
                    timeStamp
            );
        }
        fclose(readTxEdgesTimes);
    }

    // Use a method we used for generating the data in our article - each transaction output conrtibutes
    // only one edge whose source is a representative of transaction's input Bitcoin addresses
    // Moreover, loops are removed.
    // Note that in this approach the number of edges in the users graph is equal to the number
    // of lines in the txout.dat file (minus potential loops).
    else
    {
        // read timestamps for each transaction from readTxEdgesTimes
        vector <int> transactionTimestamps(maxTxId+1);
        readTxEdgesTimes = fopen(argv[2],"r");
        while(!feof(readTxEdgesTimes))
        {
            fscanf(
                    readTxEdgesTimes,"%i %i %i %f %i",
                    &motherTxId, 
                    &inputBitcoinAddress, 
                    &outputBitcoinAddress, 
                    &bitcoinAmount, 
                    &timeStamp
            );
            transactionTimestamps[motherTxId] = timeStamp;
        }
        fclose(readTxEdgesTimes);

        // Determine representatives of input addresses for each transaction
        FILE *readTxIn;
        readTxIn = fopen(argv[5],"r");
        int txID, notImportant1, notImportant2, notImportant3, addrID;
        long long sum;
        vector <int> transactionRepresentatives(maxTxId+1,-1);
        while(!feof(readTxIn))
        {
            fscanf(
                readTxIn,"%i %i %i %i %i %lld",
                &txID,
                &notImportant1, 
                &notImportant2,
                &notImportant3,
                &addrID,
                &sum
            );
            if(transactionRepresentatives[txID] == -1)transactionRepresentatives[txID] = addrID;
        }
        fclose(readTxIn);

        // determine the edges in the users graph
        FILE *readTxOut;
        readTxOut = fopen(argv[6], "r");
        while (
            fscanf(
                readTxOut,"%i %i %i %lld",
                &txID,
                &notImportant1, 
                &addrID,
                &sum
            ) == 4 
        )
        {
            if(userId[transactionRepresentatives[txID]] != userId[addrID]) // removing loops
            {
                fprintf(
                    saveUsersGraph, "%i %i %lld %i\n",
                    userId[transactionRepresentatives[txID]],
                    userId[addrID],
                    sum,
                    transactionTimestamps[txID]
                );
            }
        }
        fclose(readTxOut);
    }

    fclose(saveUsersGraph);

    return 0;
}
