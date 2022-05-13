#!/bin/bash

# Source used for this script: 
# (1) https://github.com/dkondor/join-utils

# This scripts creates the elementary Bitcoin transactions' info. The output file contains rows of the following form
#
#	(mother)transactionId bitcoinInputAddress	bitcoinOutputAddress	bitcoinAmount timeStamp
#
# (bitcoinAmount is in Satoshis).
#
#
# The script requires the following 3 path parameters (appearing as parameters in that order):
#
# (1) tx_timesPath - the path to the tx_times.dat file (not compressed) containing transaction idies and their timestamps,
# (2) txedgesPath - the path to the txedges.dat file as shown in the 'Example usage' at https://github.com/dkondor/txedges ,
# (3) tx_edges_timesPath - the path to the output file (whose form has been specified at the beginning - lines 8 - 12),
#
# Example usage (assuming you are in the same directory as the txEdgesTimes.sh script file):
#
# The sample run is then as follows:
#   ./txEdgesTimes.sh path_to_tx_times.dat path_to_txedges.dat path_to_output
#

tx_timesPath=$1
txedgesPath=$2
tx_edges_timesPath=$3

# Some of the commands below taken from https://github.com/dkondor/join-utils
if [[ ! -f "./necessary_programs/numjoin" ]]
then
    rm -rf necessary_programs/join-utils
    cd ./necessary_programs
    git clone https://github.com/dkondor/join-utils.git
    cd join-utils
    g++ -o ../numjoin numeric_join.cpp -std=gnu++11 -O3
    cd ..
    cd ..
    rm -rf necessary_programs/join-utils
fi

./necessary_programs/numjoin -j 1 -o1 1,2,3,4 -o2 2 $txedgesPath $tx_timesPath > $tx_edges_timesPath
