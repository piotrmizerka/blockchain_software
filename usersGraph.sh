#!/bin/bash

# Sources used for this script: 
# (1) https://github.com/dkondor/bitcoin 
# (2) https://github.com/dkondor/txedges
# (3) https://github.com/dkondor/join-utils
# (4) https://github.com/dkondor/sccs32s 

# This scripts creates the Bitcoin users' graph from the dumped files. 
# The output file contains rows of the following form
#
#	inputUserId	outputUserId	bitcoinAmount timeStamp
#
# (bitcoinAmount is in Satoshis).
#
# The script requires the following 3 parameters (in that order):
#
# (1) -dp|--dumpedDirPath - the path to the folder containing dumped files as specified in "dumpProcess.sh" script; 
#                           default: dumped_files,
# (2) -cp|--contractionsDirPath - the path to the folder where the users' graph will be stored; 
#                                 default: ./contractions,
# (3) -cs|--creationStrategy - 0 for creating complete bipartite graph for each transaction 
#                              (way of D. Kondor), 1 - the way we created the graph for our article;
#                              see description in "createGraph.cpp".

# All other script files (i.e. contractedAddresses.sh, sortTx.sh, txEdges.sh, txEdgesTimes.sh,
# and txTimes.sh) are assumed to be in the same folder as usersGraph.sh and the script should be run
# from that folder only.
#
# Sample usage (assuming you are in the same directory as the usersGraph.sh script file):
#   ./usersGraph.sh dumped_dir_path contractions_dir_path creation_strategy
# 

dumpedDirPath=$1
contractionsDirPath=$2
creationStrategy=$3

# Parse command line arguments as indicated here (in the top answer): https://stackoverflow.com/questions/192249/how-do-i-parse-command-line-arguments-in-bash 
POSITIONAL=()
while [[ $# -gt 0 ]]
do
    key="$1"
    case $key in
        -dp|--dumpedDirPath)
        dumpedDirPath="$2"
        shift
        shift
        ;;
        -cp|--contractionsDirPath)
        contractionsDirPath="$2"
        shift
        shift
        ;;
        -cs|--creationStrategy)
        creationStrategy="$2"
        shift
        shift
        ;;
        *)
        POSITIONAL+=("$1")
        shift
        ;;
    esac
done

# Setting default parameters if not given
if [[ -z "$dumpedDirPath" ]]
then
    dumpedDirPath=./dumped_files
fi
if [[ -z "$contractionsDirPath" ]]
then
    contractionsDirPath=./contractions
fi

# Sort txin.dat and txout.dat if necessary
echo "STEP (1). Sorting txin.dat and txout.dat if necessary..."
sort -n -k 1 -k 2 -k 3 -k 4 -k 5 -k 6 -o $dumpedDirPath/txin.dat $dumpedDirPath/txin.dat
sort -n -k 1 -k 2 -k 3 -k 4 -k 5 -k 6 -o $dumpedDirPath/txout.dat $dumpedDirPath/txout.dat

# Create elementary edges from transactions
echo "STEP (2). Creatiing edges from Bitcoin transactions - the result being saved to the txedges.dat file..."
./txEdges.sh $dumpedDirPath/txin.dat $dumpedDirPath/txout.dat $contractionsDirPath/txedges.dat

# Create transaction timestamps
echo "STEP (3). Computing transaction timestamps - the result being saved to the tx_times.dat file..."
./txTimes.sh $dumpedDirPath/tx.dat $dumpedDirPath/bh.dat $contractionsDirPath/tx_times.dat

# Create timestamps of elementary edges
echo "STEP (4). Adding timestamps to edges - the result being saved to the tx_edges_times.dat file..."
./txEdgesTimes.sh $contractionsDirPath/tx_times.dat $contractionsDirPath/txedges.dat $contractionsDirPath/tx_edges_times.dat

rm -rf $contractionsDirPath/tx_times.dat $contractionsDirPath/txedges.dat

# Contract bitcoin addresses to users' idies
echo "STEP (5). Computing user idies of Bitcoin addresses in the contraction process - the result being saved to the contracted_addresses.dat file..."
./contractedAddresses.sh $dumpedDirPath/txin.dat $dumpedDirPath/txout.dat $contractionsDirPath $contractionsDirPath/contracted_addresses.dat $sccs32sPath

# Create Bitcoin users' graph
echo "STEP (6). Creating users graph..."
./createGraph.sh -cap $contractionsDirPath/contracted_addresses.dat -tetp $contractionsDirPath/tx_edges_times.dat -ugp $contractionsDirPath/users_graph.dat -cs $creationStrategy -tip $dumpedDirPath/txin.dat -top $dumpedDirPath/txout.dat
