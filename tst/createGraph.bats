#!/usr/bin/env bats

@test "createGraph: sample data" {
    rm -rf ./tst/temp_data
    mkdir ./tst/temp_data

    contractedAddressesPath=./tst/temp_data/contracted_addresses.dat
    txEdgesTimesPath=./tst/temp_data/tx_edges_times.dat
    usersGraphPath=./tst/temp_data/users_graph.dat
    
    touch $contractedAddressesPath
    echo "1 1" >> $contractedAddressesPath
    echo "2 1" >> $contractedAddressesPath
    echo "3 1" >> $contractedAddressesPath
    echo "4 1" >> $contractedAddressesPath
    echo "5 5" >> $contractedAddressesPath
    echo "6 6" >> $contractedAddressesPath
    
    touch $txEdgesTimesPath
    echo "1	5	5000	111111111" >> $txEdgesTimesPath
    echo "2	5	1000	111111111" >> $txEdgesTimesPath
    echo "2	5	333.33333333333331	111111111" >> $txEdgesTimesPath
    echo "2	6	666.66666666666663	111111111" >> $txEdgesTimesPath
    echo "3	5	666.66666666666663	111111111" >> $txEdgesTimesPath
    echo "3	6	1333.3333333333333	111111111" >> $txEdgesTimesPath
    echo "3	6	2000	999999999" >> $txEdgesTimesPath
    echo "4	6	6000	999999999" >> $txEdgesTimesPath
    echo "5	6	7000	999999999" >> $txEdgesTimesPath
    
    chmod +x ./createGraph.sh
    ./createGraph.sh -cap $contractedAddressesPath -tetp $txEdgesTimesPath -ugp $usersGraphPath

    [ $(grep -c "1 5 5000 111111111" $usersGraphPath) -eq 1 ]
    [ $(grep -c "1 5 1000 111111111" $usersGraphPath) -eq 1 ]
    [ $(grep -c "1 5 333 111111111" $usersGraphPath) -eq 1 ]
    [ $(grep -c "1 6 667 111111111" $usersGraphPath) -eq 1 ]
    [ $(grep -c "1 5 667 111111111" $usersGraphPath) -eq 1 ]
    [ $(grep -c "1 6 1333 111111111" $usersGraphPath) -eq 1 ]
    [ $(grep -c "1 6 2000 999999999" $usersGraphPath) -eq 1 ]
    [ $(grep -c "1 6 6000 999999999" $usersGraphPath) -eq 1 ]
    [ $(grep -c "5 6 7000 999999999" $usersGraphPath) -eq 1 ]
    [ $(wc -l < $usersGraphPath) -eq 9 ]

    rm -rf ./tst/temp_data
}
