#!/bin/bash

# This is the main script creating from the original blockchain data the following: 
#   - the Bitcoin users' graph, 
#   - the long-term-subgraph (notation of D. Kondor et. al., 
#     https://iopscience.iop.org/article/10.1088/1367-2630/16/12/125003/pdf,
#     2.2. Extracting the core network),
#   - snapshots,
#   - time series (both for transactions number and value).
# The descriptions of the above are contained in our article, 
# https://doi.org/10.1016/j.frl.2020.101489, in Appendix.
# Moreover, this script provides a short statistics of the graph and Bitcoin transactions
# which includes:
# - the numbers of representating Bitcoin addresses for the most represented users in the users graph,
# - cumulative number of nodes, edges, and Bitcoin amount for each week, for both
#   active user subgraph and long-term subgraph. 
#
# The users' graph and the long-term subgraph contain rows of the following form:
#
#	inputUserId outputUserId    bitcoinAmount   timeStamp
#
# (bitcoinAmount is in Satoshis).
# 
# We create the snapshots from the long-term subgraph. Each snapshots is a subgraph
# of the long-term subgraph corresponding to a given period (we divide the time into equal periods).
# Each snapshot contains rows of the following form:
# 
#  inputUserId  outputUserId    amount_of_paramater_in_snapthos_period (paramater = transactions number or Bitcoin amount). 

# For each principal component derived from snapshots, we compute an associated time series in a specially defined way,
# as descirbed in our article, https://doi.org/10.1016/j.frl.2020.101489,  in Appendix, section 2.
# The structure of each time series is quite simple - it contains a single column of real numbers
# (the amount of these numbers is equal to the number of periods which is equal to snapshots' number).

# The main script removed by default the downloaded blockchain data after processing of them to free up disk space for further computations.
# In order to change this behavior, comment the line 175.

# The script requires the following parameter:
#
# (1) -bp|--blockchainDirPath - the path to the folder containing blockchain data files.

# It is also possible to specify the following additional parameters:
# (2) -bn|--blocksNumber - number of blocks to consider; default: consider the whole downloaded blockchain.
# The next three parameters apply to the long-term subgraph:
# (3) -mran|--minimalRepresantativeAddressesNumber - consider only the users represented by at least this number of bitcoin addresses, default value = 10,
# (4) -mid|--minimalIntervalInDays - consider users whose time distance between their first and last transaction is at least minimalIntervalInDays, default value = 1200,
# (5) -mtn|--minimalTransationsNumber - consider users who participated in at least that number of transactions, dafault value = 200.
# The following parameter concerns the snapshots:
# (6) -sdp|--snapshotPeriodInDays - the timespan of each snapshot in days, default value = 7.
# The following parameter concerns time series computed with Principal Component Analysis:
# (in turn, we get twice that number of time series at the end (for transactions number and value)):
# (7) -cn|--componentsNumber - number of principal components (base graphs) to consider (default value = 3).
# Additional paramaters:
# (8) -bd|--beginningDate - the date from which we create the snapshots; it is assumed to be
#                           in the format YYYY_MM_DD, e.g. 2022_03_24; default: 2009_01_12
#                           (the date of the first Bitcoin transaction),
# (9) -cs|--creationStrategy - 0 for creating complete bipartite graph for each transaction 
#                              (the way of D. Kondor), 1 - the way we created the graph for our article;
#                              see comments in "createGraph.cpp"; default 1.

# Sample usage if the additional parameters haven't been specified:
#   ./main.sh -bp blockchainDirPath

# Sample if the additional parameters have been specified:
#   ./main.sh -bp blockchainDirPath -bn blocksNumber -mran minimalRepresantativeAddressesNumber -mid minimalIntervalInDays -mtn minimalTransationsNumber -sdp snapshotPeriodInDays -cn componentsNumber -bd beginningDate -cs creationStrategy

# Parse command line arguments as indicated here (in the top answer): https://stackoverflow.com/questions/192249/how-do-i-parse-command-line-arguments-in-bash 
POSITIONAL=()
while [[ $# -gt 0 ]]
do
    key="$1"
    case $key in
        -bp|--blockchainDirPath)
        blockchainDirPath="$2"
        shift # past argument
        shift # past value
        ;;
        -bn|--blocksNumber)
        blocksNumber="$2"
        shift
        shift
        ;;
        -mran|--minimalRepresantativeAddressesNumber)
        minimalRepresantativeAddressesNumber="$2"
        shift
        shift
        ;;
        -mid|--minimalIntervalInDays)
        minimalIntervalInDays="$2"
        shift
        shift
        ;;
        -mtn|--minimalTransationsNumber)
        minimalTransationsNumber="$2"
        shift
        shift
        ;;
        -sdp|--snapshotPeriodInDays)
        snapshotPeriodInDays="$2"
        shift
        shift
        ;;
        -cn|--componentsNumber)
        componentsNumber="$2"
        shift
        shift
        ;;
        -bd|--beginningDate)
        beginningDate="$2"
        shift 
        shift
        ;;
        -cs|--creationStrategy)
        creationStrategy="$2"
        shift
        shift
        ;;
        *)    # unknown option
        POSITIONAL+=("$1") # save it in an array for later
        shift
        ;;
    esac
done

# Setting default parameters if not given
if [[ -z "$minimalRepresantativeAddressesNumber" ]]
then
    minimalRepresantativeAddressesNumber=10
fi
if [[ -z "$minimalIntervalInDays" ]]
then
    minimalIntervalInDays=1200
fi
if [[ -z "$minimalTransationsNumber" ]]
then
    minimalTransationsNumber=200
fi
if [[ -z "$snapshotPeriodInDays" ]]
then
    snapshotPeriodInDays=7
fi
if [[ -z "$componentsNumber" ]]
then
    componentsNumber=3
fi
if [[ -z "$beginningDate" ]]
then
    beginningDate=2009_01_12
fi
if [[ -z "$creationStrategy" ]]
then
    creationStrategy=1
fi

rm -rf ./dumped_files ./contractions
mkdir ./dumped_files ./contractions

if [[ ! -z "$blocksNumber" ]]
then
    if [ -d ./necessary_programs/bitcoin ] # check if bitcoin dump version is already installed in the repos subdirectory
    then 
        ./dumpProcess.sh -bp $blockchainDirPath -dp $(pwd)/dumped_files -bn $blocksNumber -bdvp $(pwd)/necessary_programs/bitcoin
    else
        ./dumpProcess.sh -bp $blockchainDirPath -dp $(pwd)/dumped_files -bn $blocksNumber
    fi
else # dump the whole downloaded blockchain if the number of blocks hasn't been specified
    if [ -d ./necessary_programs/bitcoin ] # check if bitcoin dump version is already installed in the repos subdirectory
    then 
        ./dumpProcess.sh -bp $blockchainDirPath -dp $(pwd)/dumped_files -bdvp $(pwd)/necessary_programs/bitcoin
    else
        ./dumpProcess.sh -bp $blockchainDirPath -dp $(pwd)/dumped_files
    fi
fi

rm -rf $blockchainDirPath # comment if there is sufficient amount of disk space to store blockchain_data together with the generated files

# remove unnecassary dumpmed files to save disk space
rm -rf ./dumped_files/addresses.dat ./dumped_files/missing.dat ./dumped_files/multiple.dat ./dumped_files/nonstandard.dat ./dumped_files/txh.dat ./dumped_files/txout_unspent.dat

# create users graph from the dumped files
./usersGraph.sh -dp ./dumped_files -cp ./contractions -cs $creationStrategy

rm -rf ./dumped_files/txin.dat ./dumped_files/txout.dat ./dumped_files/tx.dat
rm -rf ./contractions/tx_edges_times.dat

echo "STEP (6.A) Saving statistics of contraction process..."
rm -rf ./control_statistics/mostRepresentedUsers ./control_statistics/most_represented_users_statistics.txt
g++ ./control_statistics/mostRepresentedUsers.cpp -o ./control_statistics/mostRepresentedUsers
chmod +x ./control_statistics/mostRepresentedUsers
./control_statistics/mostRepresentedUsers ./contractions/contracted_addresses.dat ./control_statistics/most_represented_users_statistics.txt

echo "STEP (7). Creating the long-term subgraph..."
./longTermSubgraph.sh -mran $minimalRepresantativeAddressesNumber -mid 0 -mtn 0 -ugp ./contractions/users_graph.dat -cap ./contractions/contracted_addresses.dat -ltsp ./contractions/active_users_subgraph.dat
./longTermSubgraph.sh -mran 0 -mid $minimalIntervalInDays -mtn $minimalTransationsNumber -ugp ./contractions/active_users_subgraph.dat -cap ./contractions/contracted_addresses.dat -ltsp ./contractions/long_term_subgraph.dat
# ./longTermSubgraph.sh -mran $minimalRepresantativeAddressesNumber -mid $minimalIntervalInDays -mtn $minimalTransationsNumber -ugp ./contractions/usersGraph.dat -cap ./contractions/contracted_addresses.dat -ltsp ./contractions/long_term_subgraph.dat # in case we would like to create the long-term subgraph at once - note that it may be potentially larger!
echo "STEP (7.A) Saving active subgraph's and long-term subgraph's statistics..."
rm -rf ./control_statistics/graphStatistics ./control_statistics/active_subgraph_statistics.txt ./control_statistics/long_term_subgraph_statistics.txt
g++ ./control_statistics/graphStatistics.cpp -o ./control_statistics/graphStatistics
chmod +x ./control_statistics/graphStatistics
./control_statistics/graphStatistics ./contractions/active_users_subgraph.dat ./control_statistics/active_subgraph_statistics.txt 2011_04_29
./control_statistics/graphStatistics ./contractions/long_term_subgraph.dat ./control_statistics/long_term_subgraph_statistics.txt 2009_01_12
rm -rf ./contractions/active_users_subgraph.dat

echo "STEP (8). Creating snapshots..."
chmod +x ./snapshots.sh
./snapshots.sh -ltsp ./contractions/long_term_subgraph.dat -sdp $snapshotPeriodInDays -ewp n -sp ./snapshots_number -bhp ./dumped_files/bh.dat -bd $beginningDate
./snapshots.sh -ltsp ./contractions/long_term_subgraph.dat -sdp $snapshotPeriodInDays -ewp w -sp ./snapshots_value -bhp ./dumped_files/bh.dat -bd $beginningDate

rm -rf ./dumped_files

echo "STEP (9). Computing time series of principal components from snapshots..."
chmod +x ./timeSeries.sh
./timeSeries.sh -sp ./snapshots_number -tsp ./time_series_number -cn $componentsNumber
./timeSeries.sh -sp ./snapshots_value -tsp ./time_series_value -cn $componentsNumber