#!/usr/bin/env bats

@test "usersGraph: sample data" {
    rm -rf ./tst/temp_data
    mkdir ./tst/temp_data

    dumpedDirPath=./tst/temp_data/dumped_dir
    contractionsDirPath=./tst/temp_data/contractions
    
    rm -rf $dumpedDirPath
    rm -rf $contractionsDirPath
    mkdir $dumpedDirPath
    mkdir $contractionsDirPath
    
    # arranging sample data
    touch $dumpedDirPath/txin.dat
    echo "1	0	0	0	1	5000" >> $dumpedDirPath/txin.dat
    echo "1	0	0	0	2	1000" >> $dumpedDirPath/txin.dat
    echo "2	0	0	0	2	1000" >> $dumpedDirPath/txin.dat
    echo "2	0	0	0	3	2000" >> $dumpedDirPath/txin.dat
    echo "3	0	0	0	3	2000" >> $dumpedDirPath/txin.dat
    echo "3	0	0	0	4	6000" >> $dumpedDirPath/txin.dat
    echo "4	0	0	0	5	7000" >> $dumpedDirPath/txin.dat 
    touch $dumpedDirPath/txout.dat
    echo "1	0	5	6000" >> $dumpedDirPath/txout.dat
    echo "2	0	5	1000" >> $dumpedDirPath/txout.dat
    echo "2	0	6	2000" >> $dumpedDirPath/txout.dat
    echo "3	0	6	8000" >> $dumpedDirPath/txout.dat
    echo "4	0	6	7000" >> $dumpedDirPath/txout.dat
    touch $dumpedDirPath/tx.dat
    echo "1	10	2	1" >> $dumpedDirPath/tx.dat
    echo "2	10	2	2" >> $dumpedDirPath/tx.dat
    echo "3	20	2	1" >> $dumpedDirPath/tx.dat
    echo "4	20	1	1" >> $dumpedDirPath/tx.dat
    touch $dumpedDirPath/bh.dat
    echo "10	hash_not_important	111111111	2" >> $dumpedDirPath/bh.dat
    echo "20	hash_not_important	999999999	2" >> $dumpedDirPath/bh.dat
    
    chmod +x ./usersGraph.sh

    # creating users graph - the way of D. Kondor
    ./usersGraph.sh $dumpedDirPath $contractionsDirPath 0

    # testing whether the created graph is correct
    [ $(grep -c "1 5 5000 111111111" $contractionsDirPath/users_graph.dat) -eq 1 ]
    [ $(grep -c "1 5 1000 111111111" $contractionsDirPath/users_graph.dat) -eq 1 ]
    [ $(grep -c "1 5 333 111111111" $contractionsDirPath/users_graph.dat) -eq 1 ]
    [ $(grep -c "1 6 667 111111111" $contractionsDirPath/users_graph.dat) -eq 1 ]
    [ $(grep -c "1 5 667 111111111" $contractionsDirPath/users_graph.dat) -eq 1 ]
    [ $(grep -c "1 6 1333 111111111" $contractionsDirPath/users_graph.dat) -eq 1 ]
    [ $(grep -c "1 6 2000 999999999" $contractionsDirPath/users_graph.dat) -eq 1 ]
    [ $(grep -c "1 6 6000 999999999" $contractionsDirPath/users_graph.dat) -eq 1 ]
    [ $(grep -c "5 6 7000 999999999" $contractionsDirPath/users_graph.dat) -eq 1 ]
    [ $(wc -l < $contractionsDirPath/users_graph.dat) -eq 9 ]

    # creating users graph - the way in our article
    ./usersGraph.sh $dumpedDirPath $contractionsDirPath 1

    # testing whether the created graph is correct
    [ $(grep -c "1 5 6000 111111111" $contractionsDirPath/users_graph.dat) -eq 1 ]
    [ $(grep -c "1 5 1000 111111111" $contractionsDirPath/users_graph.dat) -eq 1 ]
    [ $(grep -c "1 6 2000 111111111" $contractionsDirPath/users_graph.dat) -eq 1 ]
    [ $(grep -c "1 6 8000 999999999" $contractionsDirPath/users_graph.dat) -eq 1 ]
    [ $(grep -c "5 6 7000 999999999" $contractionsDirPath/users_graph.dat) -eq 1 ]
    [ $(wc -l < $contractionsDirPath/users_graph.dat) -eq 5 ]

    rm -rf ./tst/temp_data
}

@test "main: sample run" {
    skip "Provide blockchain directory path in the test's code to run this test" # comment this and substitute the parameter below with appropriate values if you want the integration test to run

    blockchainDirPath=/home/piotr/Desktop/bitcoin/blockchain_data

    ./main.sh -bp $blockchainDirPath -bn 40000 -mran 1 -mid 1 -mtn 1 -sdp 30 -ewp w -cn 3

    # users' graph and its long-term subgraph
    [ $(wc -l < ./contractions/contracted_addresses.dat) -ge 1 ]
    [ $(wc -l < ./contractions/long_term_subgraph.dat) -ge 1 ]
    [ $(wc -l < ./contractions/users_graph.dat) -ge 1 ]

    # snapshots of the long-term subgraph
    [ $(ls ./snapshots_number | wc -l) -ge 10 ]
    [ $(ls ./snapshots_value | wc -l) -ge 10 ]

    # time series with PCA
    [ $(ls ./time_series_number | wc -l) -eq 3 ]
    [ $(ls ./time_series_value | wc -l) -eq 3 ]
    [ $(wc -l < ./time_series_number/component_1.dat) -ge 12 ]
    [ $(wc -l < ./time_series_value/component_1.dat) -ge 12 ]

    # statistics
    [ $(wc -l < ./control_statistics/most_represented_users_statistics.txt) -ge 1 ]
    [ $(wc -l < ./control_statistics/active_subgraph_statistics.txt) -ge 1 ]
    [ $(wc -l < ./control_statistics/long_term_subgraph_statistics.txt) -ge 1 ]

    rm -rf ./contractions
    rm -rf ./snapshots_number ./snapshots_value
    rm -rf ./time_series_number ./time_series_value
    rm -rf ./control_statistics/*.txt
}

