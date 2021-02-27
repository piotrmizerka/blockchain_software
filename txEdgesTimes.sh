#!/bin/bash

# Sources used for this script: 
# (1) https://senseable2015-6.mit.edu/bitcoin/
# (2) https://github.com/dkondor/txedges
# (3) https://github.com/dkondor/join-utils

# This scripts creates the elementary Bitcoin transactions' info. The output file contains rows of the following form
#
#	transactionId	bitcoinInputAddress	bitcoinOutputAddress	bitcoinAmount timeStamp
#
# (bitcoinAmount is in Satoshis).
#
#
# The script requires the following 4 files (appearing as parameters in that order):
#
# (1) tx_timesPath - the path to the tx_times.dat file (not compressed) containing transaction idies and their timestamps,
# (2) txedgesPath - the path to the txedges.dat file as shown in the 'Example usage' at https://github.com/dkondor/txedges ,
# (3) tx_edges_timesPath - the path to the output file (whose form has been specified at the beginning - lines 8 - 12),
#
# and the additional 4th paramater joinutilsPath which is the path to the cloned repository https://github.com/dkondor/join-utils 
# (the join-utils package is assummed to be compiled in advance).
#
#
# Example usage (assuming you are in the same directory as the txEdgesTimes.sh script file):
#
#	./txEdgesTimes.sh path_to_tx_times.dat path_to_txedges.dat path_to_output path_to_join_utils"


tx_timesPath=$1
txedgesPath=$2
tx_edges_timesPath=$3
joinutilsPath=$4

# The command below analogous as at https://github.com/dkondor/txedges/blob/master/txedges_whole.sh
commandToRun="$joinutilsPath/numjoin -j 1 -o1 2,3,4 -o2 2 $txedgesPath $tx_timesPath > $tx_edges_timesPath"


eval $commandToRun
