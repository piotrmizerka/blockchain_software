# Description
This project provides an extraction of the Bitcoin users graph - the model of the Bitcoin users network. The creation of the graph is explained, e. g., in the subsection $4.1$ of the Article of D. Maesa et. al., https://www.researchgate.net/publication/320026355_Data-driven_analysis_of_Bitcoin_properties_exploiting_the_users_graph. 

In the users graph extraction process, we use the data which can be downloaded with the modified Bitcoin client of D. Kondor. The github repository of this client is available here: https://github.com/dkondor/bitcoin. The files we need for the users graph extraction process are: **tx.dat**, **txin.dat**, **txout.dat**, and **bh.dat**, which are described here: https://github.com/dkondor/bitcoin#usage-dump-mode. <span style="color:red">**IMPORTANT NOTICE**: the **txin.dat** file is required to have the **prev_i** column erased</span>.

Apart from the files extracted from the original Bitcoin blockchain described in the paragraph above, we use also the programs of D. Kondor available in the following three repositories: https://github.com/dkondor/txedges, https://github.com/dkondor/join-utils, https://github.com/dkondor/sccs32s.

# Usage
The main script (that is [usersGraph.sh](./usersGraph.sh)) creates the Bitcoin users graph saved in the **usesrsGraph.dat** file containing lines with information about the edges as follows:
<p align="center">
  userInputID &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;userOutputID &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;bitcoinAmountInSatoshis &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;timeStamp
</p>

The scripts can be run on Linux. In order to run the main script properly, put **tx.dat**, **txin.dat**, **txout.dat**, and **bh.dat** in a separate folder (call it **dumped_dir** but you can use any name you want) and create another folder where the main otuput, as well as the auxiliary outputs, will be stored (call it **contractions_dir**, for example). Assumming, you run the terminal in main project folder, and have given the execution permission to all the project scripts (e.g. with `chmod +x path_to_script` command), the outputs can be generated with the following command:

<p align="center">
<code>./usersGraph.sh &nbsp;&nbsp;path_to_dumped_dir &nbsp;&nbsp;path_to_contractions_dir</code>
</p>

The auxiliary ouput files generated are: **txedges.dat**, **tx_times.dat**, **tx_edges_times.dat**, and **contracted_addresses.dat**. For their specification, see [txEdges.sh](./txEdges.sh), [txTimes.sh](./txTimes.sh), [txEdgesTimes.sh](./txEdgesTimes.sh), and [contractedAddresses.sh](./contractedAddresses.sh) respectively.