#!/bin/bash

# Source used for this script: 
# (1) https://github.com/dkondor/sccs32s 

# This scripts creates the contraction file containing user idies corresponding to bitcoin addresses
# as described here: https://senseable2015-6.mit.edu/bitcoin/. The rows in the output file contain
# the following two columns 
#
#	[bitcoin addresss id]	[user id]
#
#
# The script requires the following 4 path parameters (appearing as parameters in that order):
#
# (1) txinPath - the path to the txin.dat file,
# (1) txoutPath - the path to the txout.dat file,
# (2) contractionDataPath - the path to the auxiliary folder where the auxiliary contraction files are to be stored,
# (3) connectedComponentsPath - the path to the output file
#
# It is also possible to specify the additional 5th paramater scc32sPath which is the path to the cloned 
# repository https://github.com/dkondor/sccs32s (the scc32sPath package is assummed to be compiled in advance).
#
# (5) scc32sPath - the path to the output file (whose form has been specified at the beginning - lines 8 - 12),
#
#
# Example usage (assuming you are in the same directory as the contractedAdresses.sh script file):
#
# If the path to the repositoty wasn't specified, the sccs32s executable
# is assumed to be in the ./necessary_programs folder. The sample run is then as follows:
#   ./contractedAddresses.sh path_to_txin.dat path_to_contraction_data_dir path_to_output
#
# If the path to the scce32s repository was specifed:
#	./contractedAddresses.sh path_to_txin.dat path_to_contraction_data_dir path_to_output path_to_scc32sPath"


txinPath=$1
txoutPath=$2
contractionDataPath=$3
connectedComponentsPath=$4
scc32sPath=$5

# Prepare auxiliary graph containing the elementary edges between Bitcoin addresses
cat $txinPath | awk 'BEGIN{txl=0;addrl=0;addrl2=0;}{if($1 == txl) { if(addrl != $5) print addrl,$5; if(addrl2 != $5) print addrl2,$5; } else  { txl = $1; addrl = $5; print $5,$5;} addrl2 = $5;}' | uniq > $contractionDataPath/addr_edges.dat
cat $txoutPath | awk '{print $3,$3;}' | uniq >> $contractionDataPath/addr_edges.dat # some Bitcoin addresses from txout.dat may be potantially missing in txin.dat
sort -n -S 20G $contractionDataPath/addr_edges.dat | uniq > $contractionDataPath/addr_edges_sorted.dat
rm -f $contractionDataPath/addr_edges.dat

# Save edges edges number into a separate variable as it is required by sccs32s contraction script
edgesNumberInfo=$(wc -l $contractionDataPath/addr_edges_sorted.dat)
edgesNumberInfoArray=($edgesNumberInfo)
edgesNumber=${edgesNumberInfoArray[0]}

# Contraction process
if [[ ! -z "$scc32sPath" ]] # check if the scc32s repo path was given
then
    $scc32sPath/sccs32s -N $edgesNumber -t sccstmp -r < $contractionDataPath/addr_edges_sorted.dat > $connectedComponentsPath
else # if the sccs32s repo path not given, assume the txedge executable is in ./necessary_programs folder
    ./necessary_programs/sccs32s -N $edgesNumber -t sccstmp -r < $contractionDataPath/addr_edges_sorted.dat > $connectedComponentsPath
fi
rm -f $contractionDataPath/addr_edges_sorted.dat
