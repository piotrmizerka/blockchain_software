#!/bin/bash


# This script generates the users' graph from the preocmputed files containing outputs of the scripts
# "contractedAddresses.sh" and "txEdgesTimes" 

# The script requires the following 4 parameters:
#
# (1) -cap|--contractedAddressesPath - the path to the output file of the script "contractedAddresses.sh",
# (2) -tetp|--txEdgesTimesPath - the path to the output file of the script "txEdgesTimes.sh",
# (3) -ugp|--usersGraphPath - the path to the output file containing the users' graph.
# (4) -cs|--creationStrategy - 0 for creating complete bipartite graph for each transaction 
#                              (way of D. Kondor), 1 - the way we created the graph for our article;
#                              see description in "createGraph.cpp",
# and the following 2 auxiliary parameters in case -cs = 1 (in practice, for tests only):

# (5) -tip|--txinPath - the path to the txin.dat file, default: ./dumped_files/txin.dat,
# (6) -top|--txoutPath - the path to the txout.dat file, default: ./dumped_files/txout.dat.

# Running this srcipt:
#	./createGraph.sh -cap contractedAddressesPath -tetp txEdgesTimesPath -ugp usersGraphPath -cs creationStrategy

# Parse command line arguments as indicated here (in the top answer): https://stackoverflow.com/questions/192249/how-do-i-parse-command-line-arguments-in-bash
POSITIONAL=()
while [[ $# -gt 0 ]]
do
    key="$1"
    case $key in
       -cap|--contractedAddressesPath)
        contractedAddressesPath="$2"
        shift # past argument
        shift # past value
        ;;
        -tetp|--txEdgesTimesPath)
        txEdgesTimesPath="$2"
        shift # past argument
        shift # past value
        ;;
        -ugp|--usersGraphPath)
        usersGraphPath="$2"
        shift # past argument
        shift # past value
        ;;
        -cs|--creationStrategy)
        creationStrategy="$2"
        shift # past argument
        shift # past value
        ;;
        -tip|--txinPath)
        txinPath="$2"
        shift # past argument
        shift # past value
        ;;
        -top|--txoutPath)
        txoutPath="$2"
        shift # past argument
        shift # past value
        ;;
        *)    # unknown option
        POSITIONAL+=("$1") # save it in an array for later
        shift # past argument
        ;;
    esac
done

if [[ -z "$txinPath" ]]
then
    txinPath=./dumped_files/txin.dat
fi
if [[ -z "$txoutPath" ]]
then
    txoutPath=./dumped_files/txout.dat
fi

rm -rf ./necessary_programs/createGraph
g++ ./necessary_programs/createGraph.cpp -o ./necessary_programs/createGraph
chmod +x ./necessary_programs/createGraph


if [[ $creationStrategy == 1 ]]
then
    ./necessary_programs/createGraph $contractedAddressesPath $txEdgesTimesPath $usersGraphPath 1 $txinPath $txoutPath
else
    ./necessary_programs/createGraph $contractedAddressesPath $txEdgesTimesPath $usersGraphPath 0
fi