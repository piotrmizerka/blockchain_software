#!/bin/bash

# This scripts creates the Bitcoin users' graph and the long-term-subgraph 
# from the original blockchain data. 
#
# The output files contain rows of the following form
#
#	inputUserId	outputUserId	bitcoinAmount timeStamp
#
# (bitcoinAmount is in Satoshis).
# TODO: description of further output: snapshots and time series!

# The script requires the following parameters:
#
# (1) -bp|--blockchainDirPath - the path to the folder containing blockchain data files

# It is also possible to specify the following additional parameters:
# (2) -bn|--blocksNumber - number of blocks to consider,
# The next three parameters apply to the long-term subgraph:
# (3) -mran|--minimalRepresantativeAddressesNumber - consider only the users represented by at least this number of bitcoin addresses, default value = 10, TODO: DAFAULT!!
# (4) -mid|--minimalIntervalInDays - consider users whose time distance between their first and last transaction is at least minimalIntervalInDays, default value = 1200 TODO: DAFAULT!!
# (5) -mtn|--minimalTransationsNumber - consider users who participated in at least that number of transactions, dafault value = 200. TODO: DAFAULT!!
# The following parameters concern the snapshot subgraphs:
# (6) -sdp|--snapshotPeriodInDays - the timespan of snapshots in days, default value = 7, TODO: DAFAULT!!
# (7) -ewp|--snapshotEdgeWeightParameter - "w" for considering of Bitcoin amount as edges, "n" for the number of elementary transactions, default value = w, TODO: DAFAULT!!
# The last parameter is the number of principal components (and the number of time series in turn):
# (8) -cn|--componentsNumber - number of principal components (base graphs) to consider (default = 3). TODO: DEFAULT

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

rm -rf ./dumped_files ./contractions/tx_edges_times.dat ./contractions/tx_times.dat ./contractions/txedges.dat

echo "STEP (7). Creating the long-term subgraph..."
./longTermSubgraph.sh -mran $minimalRepresantativeAddressesNumber -mid 0 -mtn 0 -ugp ./contractions/users_graph.dat -cap ./contractions/contracted_addresses.dat -ltsp ./contractions/active_users_subgraph.dat
./longTermSubgraph.sh -mran 0 -mid $minimalIntervalInDays -mtn $minimalTransationsNumber -ugp ./contractions/active_users_subgraph.dat -cap ./contractions/contracted_addresses.dat -ltsp ./contractions/long_term_subgraph.dat
# ./longTermSubgraph.sh -mran $minimalRepresantativeAddressesNumber -mid $minimalIntervalInDays -mtn $minimalTransationsNumber -ugp ./contractions/usersGraph.dat -cap ./contractions/contracted_addresses.dat -ltsp ./contractions/long_term_subgraph.dat # in case we would like to create the long-term subgraph at once - note that it may be potentially larger!
rm -rf ./contractions/active_users_subgraph.dat

echo "STEP (8). Creating snapshots..."
# if [[ -z "$snapshotPeriodInDays" ]] PARSING ARGS WHEN NOT GIVEN: TODO: NOT WORKING!!
# then
#     snapshotPeriodInDays = 7
# fi
# if [[ -z "$snapshotEdgeWeightParameter" ]]
# then
#     snapshotEdgeWeightParameter = "w"
# fi
chmod +x ./snapshots.sh
./snapshots.sh -ltsp ./contractions/long_term_subgraph.dat -sdp $snapshotPeriodInDays -ewp $snapshotEdgeWeightParameter -sp ./snapshots

echo "STEP (9). Computing time series of principal components from snapshots..."
chmod +x ./timeSeries.sh
./timeSeries.sh -sp ./snapshots -tsp ./time_series -cn $componentsNumber