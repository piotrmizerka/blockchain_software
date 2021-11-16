#!/bin/bash


# This script generates the snapshots from the long term subgraph as specified in https://doi.org/10.1016/j.frl.2020.101489, Appendix, section 2, the second paragraph
# The description of creating of the long-term subgraph with its subtleties may be also found
# at the beginning of ./necessary_programs/users-graph-analysis/main.cpp file.

# The script requires the following 4 parameters:
#
# (1) -ltsp|--longTermSubgraphPath - the path to the file containing the long-term subgraph,
# (2) -spd|--snapshotPeriodInDays - the timespan of a snapshot in days,
# (3) -ewp|--snapshotEdgeWeightParameter - "w" for Bitcoin amount, "n" for the number of elementary transactions,
# (4) -sp|--snapshotsPath - the path to the snapshots' folder.

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
        shift # past argument
        shift # past value
        ;;
        -sdp|--snapshotPeriodInDays)
        snapshotPeriodInDays="$2"
        shift # past argument
        shift # past value
        ;;
        -ewp|--snapshotEdgeWeightParameter)
        snapshotEdgeWeightParameter="$2"
        shift # past argument
        shift # past value
        ;;
        -sp|--snapshotsPath)
        snapshotsPath="$2"
        shift # past argument
        shift # past value
        ;;
        *)    # unknown option
        POSITIONAL+=("$1") # save it in an array for later
        shift # past argument
        ;;
    esac
done

if [ ! -f "./necessary_programs/snapshots/snapshots" ] # check if snaphots script is compiled
then
    g++ -o ./necessary_programs/snapshots_creation/snapshots ./necessary_programs/snapshots_creation/main.cpp ./necessary_programs/snapshots_creation/stdafx.cpp
fi

./necessary_programs/snapshots_creation/snapshots $longTermSubgraphPath $snapshotPeriodInDays $snapshotEdgeWeightParameter ./snapshots