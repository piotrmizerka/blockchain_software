#!/bin/bash

# Source used for this script: 
# (1) https://github.com/dkondor/join-utils
#
# This scripts creates the Bitcoin transactions' timestamps. The output file contains rows of the following form
#
#	transactionId timeStamp
#
# (bitcoinAmount is in Satoshis).
#
#
# The script requires the following 4 parameter paths (appearing as parameters in that order):
#
# (1) txPath - the path to the tx.dat file (not compressed) as specified at https://github.com/dkondor/bitcoin, Usage (dump mode),
# (2) bhPath - the path to the bh.dat file (not compressed) as specifed at https://github.com/dkondor/bitcoin, Usage (dump mode),
# (3) tx_timesPath - the path to the output file (whose form has been specified at the beginning - lines 8 - 12).
#
# Example usage (assuming you are in the same directory as the txTimes.sh script file):
#
# The sample run is then as follows:
#   ./txTimes.sh path_to_tx.dat path_to_bh.dat path_to_output

txPath=$1
bhPath=$2
tx_timesPath=$3

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

./necessary_programs/numjoin -1 2 -o1 1 -2 1 -o2 3 $txPath $bhPath > $tx_timesPath
