#!/bin/bash


# Source used for this script: https://github.com/dkondor/txedges

# This scripts creates the elementary Bitcoin transactions' info (excluding timestamps which is done in the txEdgesTimes.sh script). 
# The output file contains rows of the following form
#
#	transactionId	bitcoinInputAddress	bitcoinOutputAddress	bitcoinAmount
#
# (bitcoinAmount is in Satoshis).
#
#
# The script requires the following 4 files (appearing as parameters in that order):
#
# (1) txinPath - the path to the txin.dat file (not compressed) as specified at https://senseable2015-6.mit.edu/bitcoin/,
# (2) txoutPath - the path to the txout.dat file (not compressed) as specified at as specified at https://senseable2015-6.mit.edu/bitcoin/,
# (3) txedgesPath - the path to the output file (whose form has been specified at the beginning - lines 6 - 11),
#
# and the additional 4th paramater txedgesRepoPath which is the path to the cloned repository https://github.com/dkondor/txedges 
# (the txedges package is assummed to be compiled in advance).
#
#
# Example usage (assuming you are in the same directory as the txEdgesTimes.sh script file):
#
#	./txEdges.sh path_to_txin.dat path_to_txout.dat path_to_output path_to_tx_edges_repo"


txinPath=$1
txoutPath=$2
txedgesPath=$3
txedgesRepoPath=$4

# The command below analogous as at https://github.com/dkondor/txedges/README.md
commandToRun="$txedgesRepoPath/txedge -i $txinPath -o $txoutPath > $txedgesPath"


eval $commandToRun
