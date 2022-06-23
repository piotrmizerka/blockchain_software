#!/bin/bash

# This script generates the snapshots from the long term subgraph as specified in 
# https://doi.org/10.1016/j.frl.2020.101489, Appendix, section 2, the second paragraph.
# 
# Each snapshot is a file containing rows of the following form:
# 
# userInputID userOutputID edgeWeight,
# 
# where edgeWeight is the total number or amount of Bitcoins sent.
# 
# The description of creating of the long-term subgraph with its subtleties may be also found
# at the beginning of ./necessary_programs/users-graph-analysis/main.cpp file.

# The snapshots are created as follows. First, we form the underlying directed graph consisting of edges u->v
# such that in the long-term subgraph there was at least one elementary Bitcoin transaction from the user u to the user v.
# Then we restrict our attention to the prescribed period of time and divide this period into equal intervals given by
# "snapshotPeriodInDays" parameter. Finally, for each such an interval, we compute the amount of the corresponding
# parameter for the underlying graph. For example, suppose our long-term subgraph consistis of thre edges:
# (a->b, Satoshi amount: 1000, timestamp: 2022.05.18),
# (c->d, Satoshi amount: 3000, timestamp: 2022.05.17),
# (e->f, Satoshi amount: 3000, timestamp: 2022.05.16),
# and we set "snapshotPeriodInDays" to be 1, and "beginningDate" to 2022.05.17. Then, our underlying graph consists
# of edges a->b, c->d, and, e->f. The first snapshot (dating 2022.05.17) defined by transaction number is then:
# a b 0
# c d 1
# e f 0,
# and the second snapshot (dating 2022.05.18) defined by transaction number is
# a b 1
# c d 0
# e f 0,
# while for transaction value the situation looks as follows:
# 2022.05.17:
# a b 0
# c d 3000
# e f 0,
# 2022.05.18:
# a b 1000
# c d 0
# e f 0.
# Note that we could as well neglect the edge e->f in the underlying graph as it is not important for our period 
# to be considered. This is not done, however at this point (the state for 2022.05.18) since this is precisely the way
# we created the graph for our article. 

# The script requires the following 6 parameters:
#
# (1) -ltsp|--longTermSubgraphPath - the path to the file containing the long-term subgraph,
# (2) -spd|--snapshotPeriodInDays - the timespan of a snapshot in days,
# (3) -ewp|--snapshotEdgeWeightParameter - "w" for Bitcoin amount, "n" for the number of elementary transactions,
# (4) -sp|--snapshotsPath - the path to the snapshots' folder,
# (5) -bhp|--blockhashPath - the path to the bh.dat file as described at https://github.com/dkondor/bitcoin,
# (6) -bd|--beginningDate - the date from which we create the snapshots; it is assumed to be
#                           in the format YYYY_MM_DD, e.g. 2022_03_24

# Running this srcipt:
#	./snapshots.sh -ltsp longTermSubgraphPath -sdp snapshotPeriodInDays -ewp snapshotEdgeWeightParameter -sp snapshotsPath

# Parse command line arguments as indicated here (in the top answer): https://stackoverflow.com/questions/192249/how-do-i-parse-command-line-arguments-in-bash
POSITIONAL=()
while [[ $# -gt 0 ]]
do
    key="$1"
    case $key in
        -ltsp|--longTermSubgraphPath)
        longTermSubgraphPath="$2"
        shift
        shift
        ;;
        -sdp|--snapshotPeriodInDays)
        snapshotPeriodInDays="$2"
        shift
        shift
        ;;
        -ewp|--snapshotEdgeWeightParameter)
        snapshotEdgeWeightParameter="$2"
        shift
        shift
        ;;
        -sp|--snapshotsPath)
        snapshotsPath="$2"
        shift
        shift
        ;;
        -bhp|--blockhashPath)
        blockhashPath="$2"
        shift
        shift
        ;;
        -bd|--beginningDate)
        beginningDate="$2"
        shift
        shift
        ;;
        *)
        POSITIONAL+=("$1")
        shift
        ;;
    esac
done

rm -rf ./necessary_programs/snapshots_creation/snapshots
g++ -o ./necessary_programs/snapshots_creation/snapshots ./necessary_programs/snapshots_creation/main.cpp ./necessary_programs/snapshots_creation/stdafx.cpp

rm -rf $snapshotsPath
mkdir $snapshotsPath

./necessary_programs/snapshots_creation/snapshots $longTermSubgraphPath $snapshotPeriodInDays $snapshotEdgeWeightParameter $snapshotsPath $blockhashPath $beginningDate
