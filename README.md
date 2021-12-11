# Description
This repository accompanies the article https://doi.org/10.1016/j.frl.2020.101489 in which we analysed relationships between financial markets and the network of Bitcoin users (the so called *Bitcoin users' graph*). We were inspired by the idea of comparing the users' graph properties to Bitcoin price. This idea goes back to Kondor et al., https://iopscience.iop.org/article/10.1088/1367-2630/16/12/125003/pdf. 

The data used in the article https://doi.org/10.1016/j.frl.2020.101489 can be obtained using this repository. It is also possible to obtain the most recent data to keep the research up-to-date. The scripts contained in this repository can be used to obtain the time series derived from the Bitcoin users' graph. For the information about the time series:
- for the definition of the time series, see our article, Appendix, section 2. - the time series are denoted by *s<sub>i</sub>'*,
- how the time series defined above are incorporated into the main regression model, see our article, section 3.3 - the time series are defined there by the values *UsersGraphProperty<sub>i,j,t</sub>*, where *t=1,...,T* and *T* is the number of subsequent periods taken into account. 

The whole process of obtaining the aforementioned time series can be divided into the following steps:

1. **Users' graph extraction** - the creation of the graph is explained, e. g., in the subsection 4.1 of the Article of D. Maesa et. al., https://www.researchgate.net/publication/320026355_Data-driven_analysis_of_Bitcoin_properties_exploiting_the_users_graph. 
In the users' graph extraction process, we use the modified Bitcoin client of D. Kondor. The github repository of this client is available here: https://github.com/dkondor/bitcoin. Apart from the modified Bitcoin client, we use as well the following three repositories of D. Kondor: https://github.com/dkondfor/txedges, https://github.com/dkondor/join-utils, https://github.com/dkondor/sccs32s.

2. **Long-term subgraph extraction** - from the users' graph, we can extract as well a subgraph (called the "long-term-subgraph") which contains users who were active in a period long enough, took part in sufficient amount of transactions, and are represented by sufficient number of Bitcoin addresses. The idea of looking at the long-term-subgraph goes back to D. Kondor et. al, https://iopscience.iop.org/article/10.1088/1367-2630/16/12/125003. This concept has been also used in our article (see Appendix, 2. Obtaining values of variables from the users graph).

3. **Snapshots' creation** - we create snapshots for the periods taken into account as described in our article in Appendix in section 2. The snapshots are denoted there by *S<sub>i</sub>*, where *i=1,...,T*.

4. **Time series extraction** - using Principal Component Analysis, we exctract the time series *s<sub>i</sub>'* from the snapshots as described in our article, Appendix, section 2.

# Usage
All the scripts described below can be run on Linux. Assumming, you run the terminal in main project folder, you shall give the execution permission to all the project scripts (e.g. with `chmod +x path_to_script` command).

To generate the time series described in the [Description](#description) section, in the main project directory, run
<p align="center">
<code>sudo ./main.sh -bp blockchainDirPath -mran minimalRepresantativeAddressesNumber -mid minimalIntervalInDays -mtn minimalTransationsNumber -sdp snapshotPeriodInDays -ewp snapshotEdgeWeightParameter -cn componentsNumber</code>
</p>
or, if the number of blocks to be processed has been specified,

<p align="center">
<code>sudo ./main.sh -bp blockchainDirPath -bn blocksNumber -mran minimalRepresantativeAddressesNumber -mid minimalIntervalInDays -mtn minimalTransationsNumber -sdp snapshotPeriodInDays -ewp snapshotEdgeWeightParameter -cn componentsNumber</code>
</p>
Note that you may be prompted for interaction which should concern providing your home account password or accepting installation or build processes.

Each of the commands above generates the following data:

1. Bitcoin users' graph which is saved in the **contractions/usersGraph.dat** file. Each line of this file contains the following four parameters: *userInputID, userOutputID, bitcoinAmountInSatoshis, timeStamp*. The auxiliary ouput files generated in the **contractions** folder are: **txedges.dat**, **tx_times.dat**, **tx_edges_times.dat**, and **contracted_addresses.dat**. For their specification, see [txEdges.sh](./txEdges.sh), [txTimes.sh](./txTimes.sh), [txEdgesTimes.sh](./txEdgesTimes.sh), and [contractedAddresses.sh](./contractedAddresses.sh) respectively. If you wish to dump files only, see [dumpProcess.sh](./dumpProcess.sh), and if you wish to create the users graph from the dumped files, see [usersGraph.sh](./usersGraph.sh).

2. Long-term subgraph saved in the **contractions/long_term_subgraph.dat** file whose structure is the same as that of the users' graph. In order to generate only the long-term subgraph from the previously created users' graph, see the description in the [longTermSubgraph.sh](./longTermSubgraph.sh) script.

3. Snapshots - saved in separate files (one file for each snapshot, named with the date of the snapshot period) in the **snapshots** folder. The structure of each line in each snapshot file is as follows: *userInputID userOutputID edgeWeight,* where *edgeWeight* is the total number or amount of Bitcoins sent (whether the number or Bitcoin amount depends on the parameter *snapshotEdgeWeightParameter* from the main command) between the users with *userInptID* and *userOutputID* in the period for a given snapshot. To generate the snapshots separately (from the previously created long-term subgraph), see the description in the [snapshots.sh](./snapshots.sh) script.

4. Time series - saved in the *time_series* folder. As each time series is defined by the corresponding principal component (see our article, Appendix, section 2.), the files containing snapshots' data are named with components' idies. Each such file contains the column of *T* numbers defining the corresponding time series. To generate the time series separately (from the previously created snapshots), see the description in the [timeSeries.sh](./timeSeries.sh) script.
