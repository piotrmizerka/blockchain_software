# Description
This repository accompanies the article https://doi.org/10.1016/j.frl.2020.101489. The data used in this article can be obtained using this repository. It is also possible to obtain the most recent data to keep the research up-to-date. The two main features of the repository are the creation of Bitcoin users' graph (which is a model of the Bitcoin users' network) and the operations on the graph itself.

The creation of the graph is explained, e. g., in the subsection 4.1 of the Article of D. Maesa et. al., https://www.researchgate.net/publication/320026355_Data-driven_analysis_of_Bitcoin_properties_exploiting_the_users_graph. 

In the users graph extraction process, we use the modified Bitcoin client of D. Kondor. The github repository of this client is available here: https://github.com/dkondor/bitcoin.

Apart from the modified Bitcoin client, we use as well the following three repositories of D. Kondor: https://github.com/dkondor/txedges, https://github.com/dkondor/join-utils, https://github.com/dkondor/sccs32s.

From the users' graph, we can extract as well a subgraph (called the "long-term-subgraph") which contains users who were active in a period long enough, took parti in sufficient amount of transactions, and are represented by sufficient number of Bitcoin addresses. The idea of looking at the long-term-subgraph goes back to D. Kondor et. al, https://iopscience.iop.org/article/10.1088/1367-2630/16/12/125003. This concept has been also used in our article (see Appendix, 2. Obtaining values of variables from the users graph).

# Usage
The first ferature of the main script (that is [main.sh](./main.sh)) is the creation of the Bitcoin users graph saved in the **usesrsGraph.dat** file containing lines with information about the edges as follows:
<p align="center">
  userInputID &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;userOutputID &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;bitcoinAmountInSatoshis &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;timeStamp
</p>

Next, the long-term-subgraph is generated from the users' graph. The structure of the long-term-subgraph is exactly the same as of the users' graph. The long-term-subgraph is stored in the **longTermSubgraph.dat** file. The long-term subgraph can be also generated sperately, outside the main script. For details, see the corresponding script: [longTermSubgraph.sh](./longTermSubgraph.sh).

The scripts can be run on Linux. Assumming, you run the terminal in main project folder, and have given the execution permission to all the project scripts (e.g. with `chmod +x path_to_script` command), the outputs can be generated with the following command:

<p align="center">
<code>sudo ./main.sh -bp blockchainDirPath -mran minimalRepresantativeAddressesNumber -mid minimalIntervalInDays -mtn minimalTransationsNumber</code>
</p>
or, if the number of blocks to be processed has been specified,

<p align="center">
<code>sudo ./main.sh -bp blockchainDirPath -bn blocksNumber -mran minimalRepresantativeAddressesNumber -mid minimalIntervalInDays -mtn minimalTransationsNumber</code>
</p>

The last three parameters of the commands above characterize the long-term-subgraph.

Note that you may be prompted for interaction which should concern providing your home account password or accepting installation or build processes.

The auxiliary ouput files generated are: **txedges.dat**, **tx_times.dat**, **tx_edges_times.dat**, and **contracted_addresses.dat**. For their specification, see [txEdges.sh](./txEdges.sh), [txTimes.sh](./txTimes.sh), [txEdgesTimes.sh](./txEdgesTimes.sh), and [contractedAddresses.sh](./contractedAddresses.sh) respectively.

All the output files are generated in the `contractions` folder in the main project directory.

If you wish to dump files only, see [dumpProcess.sh](./dumpProcess.sh), and if you wish to create the users graph from the dumped files, see [usersGraph.sh](./usersGraph.sh).