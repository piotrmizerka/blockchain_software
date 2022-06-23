#!/bin/bash

# Source used for this script: https://github.com/dkondor/txedges

# This scripts creates the elementary Bitcoin transactions' info (excluding timestamps which is done in the txEdgesTimes.sh script). 
# The output file contains rows of the following form
#
#	transactionId	bitcoinInputAddress	bitcoinOutputAddress	bitcoinAmount
#
# (bitcoinAmount is in Satoshis).
#
# The script requires the following 3 path parameters (appearing in that order):
#
# (1) txinPath - the path to the txin.dat file (not compressed) as specified at https://github.com/dkondor/bitcoin, Usage (dump mode)
#                altghough the specification of txin.dat contained there seems not to reflect the final format of txin.dat
#                I noticed this files contains rows consisting of 6 numbers instead of 7 - what is true
#                (and also important for us) is that the first number is "txId" and the last two are "addrID" and "sum",
# (2) txoutPath - the path to the txout.dat file (not compressed) as specified at https://github.com/dkondor/bitcoin, Usage (dump mode),
# (3) txedgesPath - the path to the output file (whose form has been specified at the beginning - lines 6 - 10).
#
# Sample usage (assuming you are in the same directory as the txEdgesTimes.sh script file):
#   ./txEdges.sh path_to_txin.dat path_to_txout.dat path_to_output

txinPath=$1
txoutPath=$2
txedgesPath=$3

# Some of the commands below taken from https://github.com/dkondor/txedges/README.md
if [[ ! -f "./necessary_programs/txedge" ]]
then
    rm -rf necessary_programs/txedges
    cd ./necessary_programs
    git clone https://github.com/dkondor/txedges.git
    cd txedges
    g++ -o ../txedge txedge.cpp -std=gnu++14 -O3 -march=native
    cd ..
    cd ..
    rm -rf necessary_programs/txedges
fi

./necessary_programs/txedge -i $txinPath -o $txoutPath > $txedgesPath
