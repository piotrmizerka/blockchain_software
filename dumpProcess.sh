#!/bin/bash

# Source used for this script: https://github.com/dkondor/bitcoin 

# This script generates the dumped files as specified at https://github.com/dkondor/bitcoin 
# (with prev_i column from txin.dat file erased).

# The script requires the following 2 parameters:
#
# (1) -bp|--blockchainDirPath - the path to the folder containing blockchain data files,
# (2) -dp|--dumpedDirPath - the path to the folder where the dumped files will be stored.
#
# It is also possible to specify the following two additional parameters:
# (3) -bn|--blocksNumber - number of blocks to consider
# (4) -bdvp|--bitcoinDumpVersionPath - the path to the bitcoin dump version from the repository: https://github.com/dkondor/bitcoin  

# Sample usage: if the two additional parameters weren't specified:
#   ./dumpProcess.sh -bp blockchainDirPath -dp dumpedDirPath

# Sample usage: if the two additional parameters were specifed:
#	./dumpProcess.sh -bp blockchainDirPath -dp dumpedDirPath -bn blocksNumber -bdvp bitcoinDumpVersionPath


# Check if the appropriate dependencies are installed
dependenciesCheck()
{
    if ! command -v autoconf &> /dev/null
    then
        sudo apt-get install autoconf
    fi
    if [ ! command -v build-essential &> /dev/null ] || [ ! command -v libtool &> /dev/null ] || [ ! command -v autotools-dev &> /dev/null ] ||
       [ ! command -v automake &> /dev/null ] || [ ! command -v pkg-config &> /dev/null ] || [ ! command -v bsdmainutils &> /dev/null ] || [ ! command -v python3 &> /dev/null ]
    then
        sudo apt-get install build-essential libtool autotools-dev automake pkg-config bsdmainutils python3
    fi
    if [ ! command -v libssl-dev &> /dev/null ] || [ ! command -v libevent-dev &> /dev/null ] || [ ! command -v libboost-system-dev &> /dev/null ] ||
       [ ! command -v libboost-filesystem-dev &> /dev/null ] || [ ! command -v libboost-chrono-dev &> /dev/null ] || 
       [ ! command -v libboost-test-dev &> /dev/null ] || [ ! command -v libboost-thread-dev &> /dev/null ]
    then
        sudo apt-get install libssl-dev libevent-dev libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-test-dev libboost-thread-dev
    fi
}

fetchBitcoinDumpVersion()
{
    cd ./necessary_programs/
    rm -rf ./bitcoin
    git clone https://github.com/dkondor/bitcoin.git #git@github.com:dkondor/bitcoin.git
    cd bitcoin

    ./autogen.sh
    ./contrib/install_db4.sh `pwd`
    export BDB_PREFIX=$(pwd)/db4
    ./configure BDB_LIBS="-L${BDB_PREFIX}/lib -ldb_cxx-4.8" BDB_CFLAGS="-I${BDB_PREFIX}/include"
    make
}

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
        shift
        shift
        ;;
        -bn|--blocksNumber)
        blocksNumber="$2"
        shift
        shift
        ;;
        -bdvp|--bitcoinDumpVersionPath)
        bitcoinDumpVersionPath="$2"
        shift
        shift
        ;;
        *)    # unknown option
        POSITIONAL+=("$1") # save it in an array for later
        shift
        ;;
    esac
done

dependenciesCheck

# Checking if the Bitcoin dump version directory exists - if so, the Bitcoin dump version
# is assumed to be installed there. This is not checked, however.
if [ -d "./necessary_programs/bitcoin" ]
then
    bitcoinDumpVersionPath=./necessary_programs/bitcoin
fi

# Dumping files from the blockchain
echo "Dumping files from the downloaded blockchain..."
if [[ ! -z "$blocksNumber" ]]
then
    if [[ ! -z "$bitcoinDumpVersionPath" ]]
    then
        $bitcoinDumpVersionPath/src/bitcoind -datadir=$blockchainDirPath -DUMP -DUMP_outdir=$dumpedDirPath -DUMP_bmax=$blocksNumber
    else
        fetchBitcoinDumpVersion
        src/bitcoind -datadir=$blockchainDirPath -DUMP -DUMP_outdir=$dumpedDirPath -DUMP_bmax=$blocksNumber
        cd ..
        cd ..
    fi
else # dump the whole downloaded blockchain if the number of blocks number hasn't been specified
    if [[ ! -z "$bitcoinDumpVersionPath" ]]
    then
        $bitcoinDumpVersionPath/src/bitcoind -datadir=$blockchainDirPath -DUMP -DUMP_outdir=$dumpedDirPath
    else
        fetchBitcoinDumpVersion
        src/bitcoind -datadir=$blockchainDirPath -DUMP -DUMP_outdir=$dumpedDirPath
        cd ..
        cd ..
    fi
fi
