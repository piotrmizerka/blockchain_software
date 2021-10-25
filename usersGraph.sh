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
#
# The script requires the following 2 parameters (in that order):
#
# (1) dumpedDirPath - the path to the folder containing dumped files as specified at https://github.com/dkondor/bitcoin 
# (2) contractionsDirPath - the path to the folder where the users' graph will be stored,
#
# It is also possible to specify the paths to the following 3 repositiories:
# (3) txedgesRepoPath - the path to the cloned repository: https://github.com/dkondor/txedges 
# (4) joinUtilsRepoPath - the path to the cloned repository: https://github.com/dkondor/join-utils 
# (5) sccs32sPath - the path to the cloned repository: https://github.com/dkondor/sccs32s 
#
# All other script files (i. e. contractedAddresses.sh, sortTx.sh, txEdges.sh, txEdgesTimes.sh,
# and txTimes.sh) are assumed to be in the same folder as usersGraph.sh and the script should be run
# from that folder only.
#
# Example usage (assuming you are in the same directory as the usersGraph.sh script file):
#
# If the paths to repositoties weren't specified, the txedge, numjoin, and sccs32s executables
# are assumed to be in the ./necessary_programs folder. The sample run is then as follows:
#   ./usersGraph.sh dumped_dir_path contractions_dir_path
#
# If the paths to the repositoties were specifed:
#	./usersGraph.sh dumped_dir_path contractions_dir_path txedges_repo_path join_utils_repo_path sccs32s_path

dumpedDirPath=$1
contractionsDirPath=$2
txedgesRepoPath=$3
joinUtilsRepoPath=$3
sccs32sPath=$4

# Sort txin.dat and txout.dat if necessary
echo "STEP (1). Sorting txin.dat and txout.dat if necessary..."
./sortTx.sh $dumpedDirPath/txin.dat $dumpedDirPath/txout.dat

# Create elementary edges from transactions
echo "STEP (2). Creatiing edges from Bitcoin transactions - the result being saved to the txEdges.sh file..."
./txEdges.sh $dumpedDirPath/txin.dat $dumpedDirPath/txout.dat $contractionsDirPath/txedges.dat $txedgesRepoPath

# Create transaction timestamps
echo "STEP (3). Computing transaction timestamps - the result being saved to the txTimes.sh file..."
./txTimes.sh $dumpedDirPath/tx.dat $dumpedDirPath/bh.dat $contractionsDirPath/tx_times.dat $joinUtilsRepoPath > /dev/null 2>&1

# Create timestamps of elementary edges
echo "STEP (4). Adding timestamps to edges - the result being saved to the txEdgesTimes.sh file..."
./txEdgesTimes.sh $contractionsDirPath/tx_times.dat $contractionsDirPath/txedges.dat $contractionsDirPath/tx_edges_times.dat $joinUtilsRepoPath > /dev/null 2>&1

# Contract bitcoin addresses to users' idies
echo "STEP (5). Computing user idies of Bitcoin addresses in the contraction process - the result being saved to the contractedAddresses.sh file..."
./contractedAddresses.sh $dumpedDirPath/txin.dat $dumpedDirPath/txout.dat $contractionsDirPath $contractionsDirPath/contracted_addresses.dat $sccs32sPath

# Create Bitcoin users' graph
echo "STEP (6). Creating users graph..."
if [ ! -f ./createGraph ]
then
    echo " Compiling the cpp script to create the users graph and setting the permissions to the compiled executable..."
    g++ ./createGraph.cpp -o createGraph
    chmod +x ./createGraph
    echo " Creating the graph with the compiled script..."
fi
./createGraph $contractionsDirPath/contracted_addresses.dat $contractionsDirPath/tx_edges_times.dat $contractionsDirPath/usersGraph.dat
