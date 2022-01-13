#!/usr/bin/env bats

@test "txTimes: sample data" {
    txinPath=./tst/txin.dat
    txoutPath=./tst/txout.dat
    contractionDataPath=./tst/contractions
    connectedComponentsPath=$contractionDataPath/contracted_addresses.dat

    rm -rf $txinPath
    rm -rf $txoutPath
    rm -rf $contractionDataPath
    mkdir $contractionDataPath
    
    touch $txinPath
    echo "1	0	0	0	1	5000" >> $txinPath
    echo "1	0	0	0	2	1000" >> $txinPath
    echo "2	0	0	0	2	1000" >> $txinPath
    echo "2	0	0	0	3	2000" >> $txinPath
    echo "3	0	0	0	3	2000" >> $txinPath
    echo "3	0	0	0	4	6000" >> $txinPath
    echo "4	0	0	0	5	7000" >> $txinPath
    
    touch $txoutPath
    echo "1	0	5	6000" >> $txoutPath
    echo "2	0	5	1000" >> $txoutPath
    echo "2	0	6	2000" >> $txoutPath
    echo "3	0	6	8000" >> $txoutPath
    echo "4	0	6	7000" >> $txoutPath
    
    ./contractedAddresses.sh $txinPath $txoutPath $contractionDataPath $connectedComponentsPath

    [ $(grep -c "1	1" $connectedComponentsPath) -eq 1 ]
    [ $(grep -c "2	1" $connectedComponentsPath) -eq 1 ]
    [ $(grep -c "3	1" $connectedComponentsPath) -eq 1 ]
    [ $(grep -c "4	1" $connectedComponentsPath) -eq 1 ]
    [ $(grep -c "5	5" $connectedComponentsPath) -eq 1 ]
    [ $(grep -c "6	6" $connectedComponentsPath) -eq 1 ]
    [ $(wc -l < $connectedComponentsPath) -eq 6 ]

    rm -rf $txinPath
    rm -rf $txoutPath
    rm -rf $contractionDataPath
}
