#!/usr/bin/env bats

@test "createGraph: sample data" {
    usersGraphPath=./tst/users_graph.dat
    contractedAddressesPath=./tst/contracted_addresses.dat
    longTermSubgraphPath=./tst/long_term_subgraph.dat

    rm -rf $usersGraphPath
    rm -rf $contractedAddressesPath
    rm -rf $longTermSubgraphPath
    
    touch $usersGraphPath
    echo "1 5 5000 0" >> $usersGraphPath
    echo "1 5 1000 0" >> $usersGraphPath
    echo "1 5 333 0" >> $usersGraphPath
    echo "1 6 667 86401" >> $usersGraphPath
    echo "1 5 667 0" >> $usersGraphPath
    echo "1 6 1333 86401" >> $usersGraphPath
    echo "1 6 2000 86401" >> $usersGraphPath
    echo "1 6 6000 86401" >> $usersGraphPath
    echo "5 6 7000 864002" >> $usersGraphPath
    
    touch $contractedAddressesPath
    echo "1	1" >> $contractedAddressesPath
    echo "2	1" >> $contractedAddressesPath
    echo "3	1" >> $contractedAddressesPath
    echo "4	1" >> $contractedAddressesPath
    echo "5	5" >> $contractedAddressesPath
    echo "6	6" >> $contractedAddressesPath
    
    chmod +x ./longTermSubgraph.sh

    ./longTermSubgraph.sh -mran 0 -mid 9 -mtn 0 -ugp $usersGraphPath -cap $contractedAddressesPath -ltsp $longTermSubgraphPath
    [ $(grep -c "5 6 7000 864002" $longTermSubgraphPath) -eq 1 ]
    [ $(wc -l < $longTermSubgraphPath) -eq 1 ]
    rm -rf $longTermSubgraphPath

    ./longTermSubgraph.sh -mran 3 -mid 10 -mtn 5 -ugp $usersGraphPath -cap $contractedAddressesPath -ltsp $longTermSubgraphPath
    [ ! -f $longTermSubgraphPath ]
    rm -rf $longTermSubgraphPath

    ./longTermSubgraph.sh -mran 0 -mid 1 -mtn 5 -ugp $usersGraphPath -cap $contractedAddressesPath -ltsp $longTermSubgraphPath
    [ $(grep -c "1 5 5000 0" $longTermSubgraphPath) -eq 1 ]
    [ $(grep -c "1 5 1000 0" $longTermSubgraphPath) -eq 1 ]
    [ $(grep -c "1 5 333 0" $longTermSubgraphPath) -eq 1 ]
    [ $(grep -c "1 6 667 86401" $longTermSubgraphPath) -eq 1 ]
    [ $(grep -c "1 5 667 0" $longTermSubgraphPath) -eq 1 ]
    [ $(grep -c "1 6 1333 86401" $longTermSubgraphPath) -eq 1 ]
    [ $(grep -c "1 6 2000 86401" $longTermSubgraphPath) -eq 1 ]
    [ $(grep -c "1 6 6000 86401" $longTermSubgraphPath) -eq 1 ]
    [ $(grep -c "5 6 7000 864002" $longTermSubgraphPath) -eq 1 ]
    [ $(wc -l < $longTermSubgraphPath) -eq 9 ]

    rm -rf $usersGraphPath
    rm -rf $contractedAddressesPath
    rm -rf $longTermSubgraphPath
}
