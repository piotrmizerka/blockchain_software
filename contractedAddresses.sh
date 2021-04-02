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
# The script requires the following 3 path parameters (appearing as parameters in that order):
#
# (1) txinPath - the path to the txin.dat file (not compressed) containing transaction idies and their timestamps,
# (2) contractionDataPath - the path to the auxiliary folder where the auxiliary contraction files are to be stored,
# (3) connectedComponentsPath - the path to the output file
#
# and the additional 4th paramater scc32sPath which is the path to the cloned repository https://github.com/dkondor/sccs32s 
 
# (the scc32sPath package is assummed to be compiled in advance).

# (4) scc32sPath - the path to the output file (whose form has been specified at the beginning - lines 8 - 12),
#
#
# Example usage (assuming you are in the same directory as the contractedAdresses.sh script file):
#
#	./contractedAddresses.sh path_to_txin.dat path_to_contraction_data_dir path_to_output path_to_scc32sPath"


txinPath=$1
contractionDataPath=$2
connectedComponentsPath=$3
scc32sPath=$4

createAuxiliaryGraph=`cat $txinPath | awk 'BEGIN{txl=0;addrl=0;addrl2=0;}{if($1 == txl) { if(addrl != $5) print addrl,$5; if(addrl2 != $5) print addrl2,$5; } else  { txl = $1; addrl = $5;} addrl2 = $5;}' | uniq > $contractionDataPath/addr_edges.dat`


eval $createAuxiliaryGraph

sortEdgesInAuxiliaryGraph=`sort -n -S 20G $contractionDataPath/addr_edges.dat | uniq > $contractionDataPath/addr_edges_sorted.dat`

eval $sortEdgesInAuxiliaryGraph

findConnectedComponents=`$scc32sPath/sccs32s -N 496529253 -t sccstmp -r < $contractionDataPath/addr_edges_sorted.dat > $connectedComponentsPath`

eval $findConnectedComponents
