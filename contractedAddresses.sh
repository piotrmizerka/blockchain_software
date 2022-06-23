#!/bin/bash

# Source used for this script: 
# (1) https://github.com/dkondor/sccs32s 

# This scripts creates the contraction file containing user idies corresponding to bitcoin addresses.
# The rows in the output file contain the following two columns:
#
#	bitcoin_addresss_id     user_id
#
# The script requires the following 4 path parameters (appearing in that order):
#
# (1) txinPath - the path to the txin.dat file,
# (2) txoutPath - the path to the txout.dat file,
# (3) contractionDataPath - the path to the auxiliary folder where the contraction files are to be stored,
# (4) connectedComponentsPath - the path to the output file (whose form has been specified at the beginning - lines 6 - 9).
#
# Sample usage (assuming you are in the same directory as the contractedAdresses.sh script file):
#   ./contractedAddresses.sh path_to_txin.dat path_to_contraction_data_dir path_to_output
#

txinPath=$1
txoutPath=$2
contractionDataPath=$3
connectedComponentsPath=$4

# Some of the commands in this script taken from: https://github.com/dkondor/sccs32s

# Prepare auxiliary graph containing the elementary edges between Bitcoin addresses
cat $txinPath | awk 'BEGIN{txl=0;addrl=0;addrl2=0;}{if($1 == txl) { if(addrl != $5) print addrl,$5; if(addrl2 != $5) print addrl2,$5; } else  { txl = $1; addrl = $5; print $5,$5;} addrl2 = $5;}' | uniq > $contractionDataPath/addr_edges.dat
cat $txoutPath | awk '{print $3,$3;}' | uniq >> $contractionDataPath/addr_edges.dat # some Bitcoin addresses from txout.dat may be potantially missing in txin.dat
sort -n -S 20G $contractionDataPath/addr_edges.dat | uniq > $contractionDataPath/addr_edges_sorted.dat
rm -f $contractionDataPath/addr_edges.dat

# Save edges number into a separate variable as it is required by sccs32s contraction script
edgesNumberInfo=$(wc -l $contractionDataPath/addr_edges_sorted.dat)
edgesNumberInfoArray=($edgesNumberInfo)
edgesNumber=${edgesNumberInfoArray[0]}

if [[ ! -f "./necessary_programs/sccs32s_exe" ]]
then
    rm -rf necessary_programs/sccs32s
    cd ./necessary_programs
    git clone https://github.com/dkondor/sccs32s.git
    cd sccs32s
    g++ -o ../sccs32s_exe sccs32s.cpp -std=gnu++14 -O3 -march=native
    cd ..
    cd ..
    rm -rf necessary_programs/sccs32s
fi

./necessary_programs/sccs32s_exe -N $edgesNumber -t sccstmp -r < $contractionDataPath/addr_edges_sorted.dat > $connectedComponentsPath

rm -f $contractionDataPath/addr_edges_sorted.dat
