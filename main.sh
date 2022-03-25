#!/bin/bash

# This is the main script creatingfrom the original blockchain data the following: 
#   - the Bitcoin users' graph, 
#   - the long-term-subgraph,
#   - snapshots,
#   - time series.
# The descriptions of the above are contained in our article, 
# https://doi.org/10.1016/j.frl.2020.101489, in Appendix.
#
# The users' graph, the long-term subgraph, and each snapshot contain rows of the following form
#
#	inputUserId outputUserId    bitcoinAmount   timeStamp
#
# (bitcoinAmount is in Satoshis).
# 
# We create the snapshots from the long-term subgraph. Each snapshots is a subgraph
# of the long-term subgraph corresponding to a given period (we divide the time into equal periods).

# For each principal component, we compute an associated time series in a specially defined way,
# as descirbed in our article, https://doi.org/10.1016/j.frl.2020.101489,  in Appendix, section 2.
# The structure of each time series is quite simple - it contains a single column of real numbers
# (the amount of these numbers is equal to the number of periods which is equal to snapshots' number).

# The script requires the following parameters:
#
# (1) -bp|--blockchainDirPath - the path to the folder containing blockchain data files

# It is also possible to specify the following additional parameters:
# (2) -bn|--blocksNumber - number of blocks to consider; default: consider the whole downloaded blockchain
# The next three parameters apply to the long-term subgraph:
# (3) -mran|--minimalRepresantativeAddressesNumber - consider only the users represented by at least this number of bitcoin addresses, default value = 10
# (4) -mid|--minimalIntervalInDays - consider users whose time distance between their first and last transaction is at least minimalIntervalInDays, default value = 1200
# (5) -mtn|--minimalTransationsNumber - consider users who participated in at least that number of transactions, dafault value = 200.
# The following parameters concern the snapshot subgraphs:
# (6) -sdp|--snapshotPeriodInDays - the timespan of snapshots in days, default value = 7
# (7) -ewp|--snapshotEdgeWeightParameter - "w" for considering of Bitcoin amount as edges, "n" for the number of elementary transactions, default value = w
# The last parameter is the number of principal components (and the number of time series in turn):
# (8) -cn|--componentsNumber - number of principal components (base graphs) to consider (default value = 3)
# (9) -bd|--beginningDate - the date from which we create the snapshots; it is assumed to be
#                           in the format YYYY_MM_DD, e.g. 2022_03_24; default: 2009_01_12
#                           (the date of the first Bitcoin transaction)

# If the additional parameters haven't been specified:
#   ./main.sh -bp blockchainDirPath

# If the additional parameters have been specified:
#   ./main.sh -bp blockchainDirPath -bn blocksNumber -mran minimalRepresantativeAddressesNumber -mid minimalIntervalInDays -mtn minimalTransationsNumber -sdp snapshotPeriodInDays -ewp snapshotEdgeWeightParameter -cn componentsNumber

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
        shift # past argument
        shift # past value
        ;;
        -mran|--minimalRepresantativeAddressesNumber)
        minimalRepresantativeAddressesNumber="$2"
        shift # past argument
        shift # past value
        ;;
        -mid|--minimalIntervalInDays)
        minimalIntervalInDays="$2"
        shift # past argument
        shift # past value
        ;;
        -mtn|--minimalTransationsNumber)
        minimalTransationsNumber="$2"
        shift # past argument
        shift # past value
        ;;
        -sdp|--snapshotPeriodInDays)
        snapshotPeriodInDays="$2"
        shift # past argument
        shift # past value
        ;;
        -ewp|--snapshotEdgeWeightParameter)
        snapshotEdgeWeightParameter="$2"
        shift # past argument
        shift # past value
        ;;
        -cn|--componentsNumber)
        componentsNumber="$2"
        shift # past argument
        shift # past value
        ;;
        -bd|--beginningDate)
        beginningDate="$2"
        shift # past argument
        shift # past value
        ;;
        *)    # unknown option
        POSITIONAL+=("$1") # save it in an array for later
        shift # past argument
        ;;
    esac
done

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

# create users graph from the dumped files
./usersGraph.sh ./dumped_files ./contractions

rm -rf ./dumped_files/txin.dat ./dumped_files/txout.dat ./dumped_files/tx.dat
rm -rf ./contractions/tx_edges_times.dat ./contractions/tx_times.dat ./contractions/txedges.dat

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
if [[ -z "$snapshotEdgeWeightParameter" ]]
then
    snapshotEdgeWeightParameter="w"
fi
if [[ -z "$componentsNumber" ]]
then
    componentsNumber=3
fi
if [[ -z "$beginningDate" ]]
then
    beginningDate=2009_01_12
fi

echo "STEP (7). Creating the long-term subgraph..."
./longTermSubgraph.sh -mran $minimalRepresantativeAddressesNumber -mid 0 -mtn 0 -ugp ./contractions/users_graph.dat -cap ./contractions/contracted_addresses.dat -ltsp ./contractions/active_users_subgraph.dat
./longTermSubgraph.sh -mran 0 -mid $minimalIntervalInDays -mtn $minimalTransationsNumber -ugp ./contractions/active_users_subgraph.dat -cap ./contractions/contracted_addresses.dat -ltsp ./contractions/long_term_subgraph.dat
# ./longTermSubgraph.sh -mran $minimalRepresantativeAddressesNumber -mid $minimalIntervalInDays -mtn $minimalTransationsNumber -ugp ./contractions/usersGraph.dat -cap ./contractions/contracted_addresses.dat -ltsp ./contractions/long_term_subgraph.dat # in case we would like to create the long-term subgraph at once - note that it may be potentially larger!
rm -rf ./contractions/active_users_subgraph.dat

echo "STEP (8). Creating snapshots..."
chmod +x ./snapshots.sh
./snapshots.sh -ltsp ./contractions/long_term_subgraph.dat -sdp $snapshotPeriodInDays -ewp $snapshotEdgeWeightParameter -sp ./snapshots -bhp ./dumped_files/bh.dat -bd $beginningDate

rm -rf ./dumped_files/bh.dat

echo "STEP (9). Computing time series of principal components from snapshots..."
chmod +x ./timeSeries.sh
./timeSeries.sh -sp ./snapshots -tsp ./time_series -cn $componentsNumber