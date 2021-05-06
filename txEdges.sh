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
# The script requires the following 3 path parameters (appearing as parameters in that order):
#
# (1) txinPath - the path to the txin.dat file (not compressed) as specified at https://senseable2015-6.mit.edu/bitcoin/,
# (2) txoutPath - the path to the txout.dat file (not compressed) as specified at as specified at https://senseable2015-6.mit.edu/bitcoin/,
# (3) txedgesPath - the path to the output file (whose form has been specified at the beginning - lines 6 - 11),
#
# It is also possible to specify the additional 4th paramater txedgesRepoPath which is the path to the cloned repository 
# https://github.com/dkondor/txedges (the txedges package is assummed to be compiled in advance).
#
#
# Example usage (assuming you are in the same directory as the txEdgesTimes.sh script file):
#
# If the path to the repositoty wasn't specified, the txedge executable
# is assumed to be in the ./necessary_programs folder. The sample run is then as follows:
#   ./txEdges.sh path_to_txin.dat path_to_txout.dat path_to_output
#
# If the path to the txedges repository was specifed:
#	./txEdges.sh path_to_txin.dat path_to_txout.dat path_to_output path_to_tx_edges_repo"


txinPath=$1
txoutPath=$2
txedgesPath=$3
txedgesRepoPath=$4

# The commands below analogous as at https://github.com/dkondor/txedges/README.md
if [[ ! -z "$txedgesRepoPath" ]] # check if the txedges repo path was given
then
    $txedgesRepoPath/txedge -i $txinPath -o $txoutPath > $txedgesPath
else # if the txedges repo path not given, assume the txedge executable is in ./necessary_programs folder
    ./necessary_programs/txedge -i $txinPath -o $txoutPath > $txedgesPath
fi


