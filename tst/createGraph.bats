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
    echo "1 1	5	5000	111111111" >> $txEdgesTimesPath
    echo "2 2	5	1000	111111111" >> $txEdgesTimesPath
    echo "2 2	5	333.33333333333331	111111111" >> $txEdgesTimesPath
    echo "2 2	6	666.66666666666663	111111111" >> $txEdgesTimesPath
    echo "2 3	5	666.66666666666663	111111111" >> $txEdgesTimesPath
    echo "2 3	6	1333.3333333333333	111111111" >> $txEdgesTimesPath
    echo "3 3	6	2000	999999999" >> $txEdgesTimesPath
    echo "3 4	6	6000	999999999" >> $txEdgesTimesPath
    echo "4 5	6	7000	999999999" >> $txEdgesTimesPath
    
    chmod +x ./createGraph.sh

    # test a way of D. Kondor - wit complete bipartite graphs for each transaction
    ./createGraph.sh -cap $contractedAddressesPath -tetp $txEdgesTimesPath -ugp $usersGraphPath -cs 0

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

    # test our way - with only one edge for each transaction output
    txinPath=./tst/temp_data/txin.dat
    txoutPath=./tst/temp_data/txout.dat
    
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

    ./createGraph.sh -cap $contractedAddressesPath -tetp $txEdgesTimesPath -ugp $usersGraphPath -cs 1 -tip $txinPath -top $txoutPath

    [ $(grep -c "1 5 6000 111111111" $usersGraphPath) -eq 1 ]
    [ $(grep -c "1 5 1000 111111111" $usersGraphPath) -eq 1 ]
    [ $(grep -c "1 6 2000 111111111" $usersGraphPath) -eq 1 ]
    [ $(grep -c "1 6 8000 999999999" $usersGraphPath) -eq 1 ]
    [ $(grep -c "5 6 7000 999999999" $usersGraphPath) -eq 1 ]
    [ $(wc -l < $usersGraphPath) -eq 5 ]

    rm -rf ./tst/temp_data
}
