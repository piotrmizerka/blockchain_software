#!/usr/bin/env bats

@test "usersGraph: sample data" {
    dumpedDirPath=./tst/dumped_dir
    contractionsDirPath=./tst/contractions
    
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
    
    # creating users graph
    chmod +x ./usersGraph.sh
    ./usersGraph.sh $dumpedDirPath $contractionsDirPath

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

    rm -rf $dumpedDirPath
    rm -rf $contractionsDirPath
}

@test "main: sample run" {
    # skip # comment this and substitute the parameter below with appropriate values if you want the integration test to run

    blockchainDirPath=/home/piotr/Desktop/bitcoin/blockchain_data

    ./main.sh -bp $blockchainDirPath -bn 40000 -mran 10 -mid 1 -mtn 2 -sdp 1 -ewp w -cn 3

    # users' graph and its long-term subgraph
    [ $(wc -l < ./contractions/contracted_addresses.dat) -ge 1 ]
    [ $(wc -l < ./contractions/long_term_subgraph.dat) -ge 1 ]
    [ $(wc -l < ./contractions/users_graph.dat) -ge 1 ]

    # snapshots of the long-term subgraph
    [ $(ls ./snapshots | wc -l) -ge 10 ]

    # time series with PCA
    [ $(ls ./time_series | wc -l) -eq 3 ]
    [ $(wc -l < ./time_series/component_1.dat) -ge 4000 ]

    rm -rf ./contractions
    rm -rf ./snapshots
    rm -rf ./time_series
}

