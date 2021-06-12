#!/bin/bash

# Source used for this script: https://github.com/dkondor/bitcoin 

# This script generates the dumped files as specified at https://github.com/dkondor/bitcoin 
# (with prev_i column from txin.dat file erased).

# The script requires the following 2 parameters:
#
# (1) -bp|--blockchainDirPath - the path to the folder containing blockchain data files
# (2) -dp|--dumpedDirPath - the path to the folder where the dumped files will be stored,
#
# It is also possible to specify the following two additional parameters:
# (3) -bn|--blocksNumber - number of blocks to consider
# (4) -bdvp|--bitcoinDumpVersionPath - the path to the bitcoin dump version from the repository: https://github.com/dkondor/bitcoin  

# If the two additional parameters weren't specified:
#   ./dumpProcess.sh -bp blockchainDirPath -dp dumpedDirPath

# If the two additional parameters were specifed:
#	./dumpProcess.sh -bp blockchainDirPath -dp dumpedDirPath -bn blocksNumber -bdvp bitcoinDumpVersionPath

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
        -dp|--dumpedDirPath)
        dumpedDirPath="$2"
        shift # past argument
        shift # past value
        ;;
        -bn|--blocksNumber)
        blocksNumber="$2"
        shift # past argument
        shift # past value
        ;;
        -bdvp|--bitcoinDumpVersionPath)
        bitcoinDumpVersionPath="$2"
        shift # past argument
        shift # past value
        ;;
        *)    # unknown option
        POSITIONAL+=("$1") # save it in an array for later
        shift # past argument
        ;;
    esac
done

# Dumping files from the blockchain
echo "Dumping files from the downloaded blockchain..."
if [[ ! -z "$blocksNumber" ]]
then
    if [[ ! -z "$bitcoinDumpVersionPath" ]]
    then
        $bitcoinDumpVersionPath/src/bitcoind -datadir=$blockchainDirPath -DUMP -DUMP_outdir=$dumpedDirPath -DUMP_bmax=$blocksNumber
    else
        cd ./necessary_programs/
        git clone git@github.com:dkondor/bitcoin.git
        cd bitcoin
        ./autogen.sh
        ./configure
        make
        src/bitcoind -datadir=$blockchainDirPath -DUMP -DUMP_outdir=$dumpedDirPath -DUMP_bmax=$blocksNumber
        cd ..
        cd ..
    fi
else # dump the whole downloaded blockchain if the number of blocks hasn't been specified
    if [[ ! -z "$bitcoinDumpVersionPath" ]]
    then
        $bitcoinDumpVersionPath/src/bitcoind -datadir=$blockchainDirPath -DUMP -DUMP_outdir=$dumpedDirPath
    else
        cd ./necessary_programs/
        git clone git@github.com:dkondor/bitcoin.git
        cd bitcoin
        ./autogen.sh
        ./configure
        make
        src/bitcoind -datadir=$blockchainDirPath -DUMP -DUMP_outdir=$dumpedDirPath
        cd ..
        cd ..
    fi
fi


