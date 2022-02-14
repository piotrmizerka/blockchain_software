#!/bin/bash


# This script generates the users' graph from the preocmputed files containing outputs of the scripts
# "contractedAddresses.sh" and "txEdgesTimes" 

# The script requires the following 3 parameters:
#
# (1) -cap|--contractedAddressesPath - the path to the output file of the script "contractedAddresses.sh",
# (2) -tetp|--txEdgesTimesPath - the path to the output file of the script "txEdgesTimes.sh",
# (3) -ugp|--usersGraphPath - the path to the output file containing the users' graph.

# Running this srcipt:
#	./createGraph.sh -cap contractedAddressesPath -tetp txEdgesTimesPath -ugp usersGraphPath

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
        *)    # unknown option
        POSITIONAL+=("$1") # save it in an array for later
        shift # past argument
        ;;
    esac
done

if [ ! -f ./createGraph ]
then
    echo " Compiling the cpp script to create the users graph and setting the permissions to the compiled executable..."
    g++ ./createGraph.cpp -o createGraph
    chmod +x ./createGraph
    echo " Creating the graph with the compiled script..."
fi

./createGraph $contractedAddressesPath $txEdgesTimesPath $usersGraphPath