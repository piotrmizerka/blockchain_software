#!/bin/bash

# This scripts creates the Bitcoin users' graph from the original blockchain data. 
# The output file contains rows of the following form
#
#	inputUserId	outputUserId	bitcoinAmount timeStamp
#
# (bitcoinAmount is in Satoshis).

# The script requires the following parameters:
#
# (1) -bp|--blockchainDirPath - the path to the folder containing blockchain data files

# It is also possible to specify the following two additional parameter:
# (2) -bn|--blocksNumber - number of blocks to consider

# If the additional parameter hasn't been specified:
#   ./main.sh -bp blockchainDirPath

# If the additional parameter has been specified:
#   ./main.sh -bp blockchainDirPath -bn blocksNumber

# Parse command line arguments as indicated here (in the top answer): https://stackoverflow.com/questions/192249/how-do-i-parse-command-line-arguments-in-bash 

POSITIONAL=()
while [[ $# -gt 0 ]]
do
    key="$1"
    case $key in
        -bp|--blockchainDirPath)
        blockchainDirPath="$2"
        shift # past argument
        shift # past value
        ;;
        -bn|--blocksNumber)
        blocksNumber="$2"
        shift # past argument
        shift # past value
        ;;
        *)    # unknown option
        POSITIONAL+=("$1") # save it in an array for later
        shift # past argument
        ;;
    esac
done

rm -rf ./dumped_files ./contractions
mkdir ./dumped_files ./contractions

if [[ ! -z "$blocksNumber" ]]
then
    if [ -d ./necessary_programs/bitcoin ] # check if bitcoin dump version is already installed in the repos subdirectory
    then 
        ./dumpProcess.sh -bp $blockchainDirPath -dp $(pwd)/dumped_files -bn $blocksNumber -bdvp $(pwd)/necessary_programs/bitcoin
    else
        ./dumpProcess.sh -bp $blockchainDirPath -dp $(pwd)/dumped_files -bn $blocksNumber
    fi
else # dump the whole downloaded blockchain if the number of blocks hasn't been specified
    if [ -d ./necessary_programs/bitcoin ] # check if bitcoin dump version is already installed in the repos subdirectory
    then 
        ./dumpProcess.sh -bp $blockchainDirPath -dp $(pwd)/dumped_files -bdvp $(pwd)/necessary_programs/bitcoin
    else
        ./dumpProcess.sh -bp $blockchainDirPath -dp $(pwd)/dumped_files
    fi
fi

# create users graph from the dumped files
./usersGraph.sh ./dumped_files ./contractions

rm -rf ./dumped_files
