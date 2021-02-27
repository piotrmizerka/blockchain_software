#!/bin/bash

# Sources used for this script: 
# (1) https://senseable2015-6.mit.edu/bitcoin/
# (2) https://github.com/dkondor/txedges
# (3) https://github.com/dkondor/join-utils
#
# This scripts creates the Bitcoin transactions' timestamps. The output file contains rows of the following form
#
#	transactionId timeStamp
#
# (bitcoinAmount is in Satoshis).
#
#
# The script requires the following 4 files (appearing as parameters in that order):
#
# (1) txPath - the path to the tx.dat file (not compressed) as specified at https://senseable2015-6.mit.edu/bitcoin/,
# (2) bhPath - the path to the bh.dat file (not compressed) as specifed at https://senseable2015-6.mit.edu/bitcoin/,
# (3) tx_timesPath - the path to the output file (whose form has been specified at the beginning - lines 8 - 12),
#
# and the additional 4th paramater joinutilsPath which is the path to the cloned repository https://github.com/dkondor/join-utils 
# (the join-utils package is assummed to be compiled in advance).
#
#
# Example usage (assuming you are in the same directory as the txTimes.sh script file):
#
#	./txTimes.sh path_to_tx.dat path_to_bh.dat path_to_output path_to_join_utils"


txPath=$1
bhPath=$2
tx_timesPath=$3
joinutilsPath=$4

echo "Creating tx_times.dat - the file containing timestamps of transactions"

# The command below analogous as at https://github.com/dkondor/txedges/blob/master/txedges_whole.sh
commandToRun="$joinutilsPath/numjoin -1 2 -o1 1 -2 1 -o2 3 $txPath $bhPath > $tx_timesPath"

eval $commandToRun
