#!/usr/bin/env bats

@test "txEdgesTimes: sample data" {
    txTimesPath=./tst/tx_times.dat
    txEdgesPath=./tst/txedges.dat
    txEdgesTimesPath=./tst/tx_edges_times.dat

    rm -rf $txTimesPath
    rm -rf $txEdgesPath
    rm -rf $txEdgesTimesPath
    
    touch $txTimesPath
    echo "1	111111111" >> $txTimesPath
    echo "2	111111111" >> $txTimesPath
    echo "3	999999999" >> $txTimesPath
    echo "4	999999999" >> $txTimesPath
    
    touch $txEdgesPath
    echo "1	1	5	5000" >> $txEdgesPath
    echo "1	2	5	1000" >> $txEdgesPath
    echo "2	2	5	333.33333333333331" >> $txEdgesPath
    echo "2	2	6	666.66666666666663" >> $txEdgesPath
    echo "2	3	5	666.66666666666663" >> $txEdgesPath
    echo "2	3	6	1333.3333333333333" >> $txEdgesPath
    echo "3	3	6	2000" >> $txEdgesPath
    echo "3	4	6	6000" >> $txEdgesPath
    echo "4	5	6	7000" >> $txEdgesPath
    
    chmod +x ./txEdgesTimes.sh
    ./txEdgesTimes.sh $txTimesPath $txEdgesPath $txEdgesTimesPath

    [ $(grep -c "1	5	5000	111111111" $txEdgesTimesPath) -eq 1 ]
    [ $(grep -c "2	5	1000	111111111" $txEdgesTimesPath) -eq 1 ]
    [ $(grep -c "2	5	333.33333333333331	111111111" $txEdgesTimesPath) -eq 1 ]
    [ $(grep -c "2	6	666.66666666666663	111111111" $txEdgesTimesPath) -eq 1 ]
    [ $(grep -c "3	5	666.66666666666663	111111111" $txEdgesTimesPath) -eq 1 ]
    [ $(grep -c "3	6	1333.3333333333333	111111111" $txEdgesTimesPath) -eq 1 ]
    [ $(grep -c "3	6	2000	999999999" $txEdgesTimesPath) -eq 1 ]
    [ $(grep -c "4	6	6000	999999999" $txEdgesTimesPath) -eq 1 ]
    [ $(grep -c "5	6	7000	999999999" $txEdgesTimesPath) -eq 1 ]
    [ $(wc -l < $txEdgesTimesPath) -eq 9 ]

    rm -rf $txTimesPath
    rm -rf $txEdgesPath
    rm -rf $txEdgesTimesPath
}
