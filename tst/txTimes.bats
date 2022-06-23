#!/usr/bin/env bats

@test "txTimes: sample data" {
    rm -rf ./tst/temp_data
    mkdir ./tst/temp_data

    txPath=./tst/temp_data/tx.dat
    bhPath=./tst/temp_data/bh.dat
    txTimesPath=./tst/temp_data/tx_times.dat
    
    touch $txPath
    echo "1	10	2	1" >> $txPath
    echo "2	10	2	2" >> $txPath
    echo "3	20	2	1" >> $txPath
    echo "4	20	1	1" >> $txPath
    
    touch $bhPath
    echo "10	hash_not_important	111111111	2" >> $bhPath
    echo "20	hash_not_important	999999999	2" >> $bhPath
    
    ./txTimes.sh $txPath $bhPath $txTimesPath

    [ $(grep -c "1	111111111" $txTimesPath) -eq 1 ]
    [ $(grep -c "2	111111111" $txTimesPath) -eq 1 ]
    [ $(grep -c "3	999999999" $txTimesPath) -eq 1 ]
    [ $(grep -c "4	999999999" $txTimesPath) -eq 1 ]
    [ $(wc -l < $txTimesPath) -eq 4 ]

    rm -rf ./tst/temp_data
}
