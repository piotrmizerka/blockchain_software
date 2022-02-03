#!/bin/bash


# This script generates the long term subgraph from the users graph as specified in https://doi.org/10.1016/j.frl.2020.101489, Appendix, section 2
# The description of creating of the long-term subgraph with its subtleties may be also found
# at the beginning of ./necessary_programs/users-graph-analysis/main.cpp file.

# The script requires the following 6 parameters:
#
# (1) -mran|--minimalRepresantativeAddressesNumber - consider only the users represented by at least this number of bitcoin addresses,
# (2) -mid|--minimalIntervalInDays - consider users whose time distance between their first and last transaction is at least minimalIntervalInDays,
# (3) -mtn|--minimalTransationsNumber - consider users who participated in at least that number of transactions,
# (4) -ugp|--usersGraphPath - the path to the file contining the users graph,
# (5) -cap|--contractedAddressesPath - the path to the file containing all the representative bitcoin addresses for each user,
# (6) -ltsp|--longTermSubgraphPath - the path to the output file containing the long term subgraph.

# Running this srcipt:
#	./longTermSubgraph.sh -mran minimalRepresantativeAddressesNumber -mid minimalIntervalInDays -mtn minimalTransationsNumber -ugp usersGraphPath -cap contractedAddressesPath -ltsp longTermSubgraphPath

# Parse command line arguments as indicated here (in the top answer): https://stackoverflow.com/questions/192249/how-do-i-parse-command-line-arguments-in-bash

POSITIONAL=()
while [[ $# -gt 0 ]]
do
    key="$1"
    case $key in
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
        -ugp|--usersGraphPath)
        usersGraphPath="$2"
        shift # past argument
        shift # past value
        ;;
        -cap|--contractedAddressesPath)
        contractedAddressesPath="$2"
        shift # past argument
        shift # past value
        ;;
        -ltsp|--longTermSubgraphPath)
        longTermSubgraphPath="$2"
        shift # past argument
        shift # past value
        ;;
        *)    # unknown option
        POSITIONAL+=("$1") # save it in an array for later
        shift # past argument
        ;;
    esac
done

# derive long-term subgraph from the users graph
if [ ! -f "./necessary_programs/users-graph-analysis/longTermSubgraph" ] # check if longTermSubgraph is compiled
then
    g++ -o ./necessary_programs/users-graph-analysis/longTermSubgraph ./necessary_programs/users-graph-analysis/Graph.cpp ./necessary_programs/users-graph-analysis/stdafx.cpp ./necessary_programs/users-graph-analysis/main.cpp
fi

./necessary_programs/users-graph-analysis/longTermSubgraph $minimalRepresantativeAddressesNumber $minimalIntervalInDays $minimalTransationsNumber $usersGraphPath $contractedAddressesPath $longTermSubgraphPath 