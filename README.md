# Description
This project provides an extraction of the Bitcoin users graph - the model of the Bitcoin users network. The creation of the graph is explained, e. g., in the subsection 4.1 of the Article of D. Maesa et. al., https://www.researchgate.net/publication/320026355_Data-driven_analysis_of_Bitcoin_properties_exploiting_the_users_graph. 

In the users graph extraction process, we use the modified Bitcoin client of D. Kondor. The github repository of this client is available here: https://github.com/dkondor/bitcoin.

Apart from the modified Bitcoin client, we use as well the following three repositories of D. Kondor: https://github.com/dkondor/txedges, https://github.com/dkondor/join-utils, https://github.com/dkondor/sccs32s.

# Usage
The main script (that is [main.sh](./main.sh)) creates the Bitcoin users graph saved in the **usesrsGraph.dat** file containing lines with information about the edges as follows:
<p align="center">
  userInputID &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;userOutputID &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;bitcoinAmountInSatoshis &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;timeStamp
</p>

The scripts can be run on Linux. Assumming, you run the terminal in main project folder, and have given the execution permission to all the project scripts (e.g. with `chmod +x path_to_script` command), the outputs can be generated with the following command:

<p align="center">
<code>./main.sh -bp blockchainDirPath</code>
</p>
or, if the number of blocks to be processed has been specified,

<p align="center">
<code>./main.sh -bp blockchainDirPath -bn blocksNumber</code>
</p>

Note that if some dependencies have not been installed you may be prompted for interaction which should concern providing your home account password or accepting installation or build processes.

The auxiliary ouput files generated are: **txedges.dat**, **tx_times.dat**, **tx_edges_times.dat**, and **contracted_addresses.dat**. For their specification, see [txEdges.sh](./txEdges.sh), [txTimes.sh](./txTimes.sh), [txEdgesTimes.sh](./txEdgesTimes.sh), and [contractedAddresses.sh](./contractedAddresses.sh) respectively.

All the output files are generated in the `contractions` folder in the main project directory.

If you wish to dump files only, see [dumpProcess.sh](./dumpProcess.sh), and if you wish to create the users graph from the dumped files, see [usersGraph.sh](./usersGraph.sh).