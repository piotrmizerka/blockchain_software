#!/bin/bash


# This script computes the time series from the snapshots which were computed as described in "snapshots.sh" file.

# The script requires the following 3 parameters:
#
# (1) -sp|--snapshotsPath - the path to the file containing the snapshots,
# (2) -tsp|--timeSeriesPath - the path to the folder where time series are to be stored,
# (3) -cn|--componentsNumber - number of principal components (base graphs) to consider.

# Running this srcipt:
#	./timeSeries.sh -sp snapshotsPath -tsp timeSeriesPath -cn componentsNumber

# Parse command line arguments as indicated here (in the top answer): https://stackoverflow.com/questions/192249/how-do-i-parse-command-line-arguments-in-bash

POSITIONAL=()
while [[ $# -gt 0 ]]
do
    key="$1"
    case $key in
        -sp|--snapshotsPath)
        snapshotsPath="$2"
        shift # past argument
        shift # past value
        ;;
        -tsp|--timeSeriesPath)
        timeSeriesPath="$2"
        shift # past argument
        shift # past value
        ;;
        -cn|--componentsNumber)
        componentsNumber="$2"
        shift # past argument
        shift # past value
        ;;
        *)    # unknown option
        POSITIONAL+=("$1") # save it in an array for later
        shift # past argument
        ;;
    esac
done

rm -rf $timeSeriesPath
mkdir $timeSeriesPath

if [[ ! -d "./necessary_programs/pca/pca_venv" ]] # if pca_venv is present, assume everything is compiled properly
then
    if ! command -v python3 &> /dev/null
    then
        sudo apt-get install python3-venv
    fi
    python3 -m venv ./necessary_programs/pca/pca_venv
    chmod +x ./necessary_programs/pca/pca_venv/bin/activate
    source ./necessary_programs/pca/pca_venv/bin/activate
    python3 -m pip install -U scikit-learn
fi

./necessary_programs/pca/pca_venv/bin/python ./necessary_programs/pca/pca.py $snapshotsPath $timeSeriesPath $componentsNumber
