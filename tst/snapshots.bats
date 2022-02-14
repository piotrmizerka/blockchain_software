#!/usr/bin/env bats

@test "snapshots: sample data" {
    longTermSubgraphPath=./tst/long_term_subgraph.dat
    snapshotsPath=./tst/snapshots

    rm -rf $longTermSubgraphPath
    rm -rf $snapshotsPath
    
    # 1640995611 is on 2022.01.01
    touch $longTermSubgraphPath
    echo "1 5 5000 1640995611" >> $longTermSubgraphPath
    echo "1 5 1000 1640995611" >> $longTermSubgraphPath
    echo "1 5 333 1641082011" >> $longTermSubgraphPath
    echo "1 6 667 1641082011" >> $longTermSubgraphPath
    echo "1 5 667 1641168411" >> $longTermSubgraphPath
    echo "1 6 1333 1641168411" >> $longTermSubgraphPath
    echo "1 6 2000 1641254811" >> $longTermSubgraphPath
    echo "1 6 6000 1641254811" >> $longTermSubgraphPath
    echo "5 6 7000 1641341211" >> $longTermSubgraphPath
    
    chmod +x ./snapshots.sh
    
    ./snapshots.sh -ltsp $longTermSubgraphPath -sdp 1 -ewp n -sp $snapshotsPath
    [ $(grep -c "1 5 2" $snapshotsPath/2022_01_01.dat) -eq 1 ]
    [ $(grep -c "1 6 0" $snapshotsPath/2022_01_01.dat) -eq 1 ]
    [ $(grep -c "5 6 0" $snapshotsPath/2022_01_01.dat) -eq 1 ]
    [ $(wc -l < $snapshotsPath/2022_01_01.dat) -eq 3 ]
    [ $(grep -c "1 5 1" $snapshotsPath/2022_01_02.dat) -eq 1 ]
    [ $(grep -c "1 6 1" $snapshotsPath/2022_01_02.dat) -eq 1 ]
    [ $(grep -c "5 6 0" $snapshotsPath/2022_01_02.dat) -eq 1 ]
    [ $(wc -l < $snapshotsPath/2022_01_02.dat) -eq 3 ]
    [ $(grep -c "1 5 1" $snapshotsPath/2022_01_03.dat) -eq 1 ]
    [ $(grep -c "1 6 1" $snapshotsPath/2022_01_03.dat) -eq 1 ]
    [ $(grep -c "5 6 0" $snapshotsPath/2022_01_03.dat) -eq 1 ]
    [ $(wc -l < $snapshotsPath/2022_01_02.dat) -eq 3 ]
    [ $(grep -c "1 5 0" $snapshotsPath/2022_01_04.dat) -eq 1 ]
    [ $(grep -c "1 6 2" $snapshotsPath/2022_01_04.dat) -eq 1 ]
    [ $(grep -c "5 6 0" $snapshotsPath/2022_01_04.dat) -eq 1 ]
    [ $(wc -l < $snapshotsPath/2022_01_02.dat) -eq 3 ]
    [ $(grep -c "1 5 0" $snapshotsPath/2022_01_05.dat) -eq 1 ]
    [ $(grep -c "1 6 0" $snapshotsPath/2022_01_05.dat) -eq 1 ]
    [ $(grep -c "5 6 1" $snapshotsPath/2022_01_05.dat) -eq 1 ]
    [ $(wc -l < $snapshotsPath/2022_01_02.dat) -eq 3 ]

    ./snapshots.sh -ltsp $longTermSubgraphPath -sdp 1 -ewp w -sp $snapshotsPath
    [ $(grep -c "1 5 6000" $snapshotsPath/2022_01_01.dat) -eq 1 ]
    [ $(grep -c "1 6 0" $snapshotsPath/2022_01_01.dat) -eq 1 ]
    [ $(grep -c "5 6 0" $snapshotsPath/2022_01_01.dat) -eq 1 ]
    [ $(wc -l < $snapshotsPath/2022_01_01.dat) -eq 3 ]
    [ $(grep -c "1 5 333" $snapshotsPath/2022_01_02.dat) -eq 1 ]
    [ $(grep -c "1 6 667" $snapshotsPath/2022_01_02.dat) -eq 1 ]
    [ $(grep -c "5 6 0" $snapshotsPath/2022_01_02.dat) -eq 1 ]
    [ $(wc -l < $snapshotsPath/2022_01_02.dat) -eq 3 ]
    [ $(grep -c "1 5 667" $snapshotsPath/2022_01_03.dat) -eq 1 ]
    [ $(grep -c "1 6 1333" $snapshotsPath/2022_01_03.dat) -eq 1 ]
    [ $(grep -c "5 6 0" $snapshotsPath/2022_01_03.dat) -eq 1 ]
    [ $(wc -l < $snapshotsPath/2022_01_02.dat) -eq 3 ]
    [ $(grep -c "1 5 0" $snapshotsPath/2022_01_04.dat) -eq 1 ]
    [ $(grep -c "1 6 8000" $snapshotsPath/2022_01_04.dat) -eq 1 ]
    [ $(grep -c "5 6 0" $snapshotsPath/2022_01_04.dat) -eq 1 ]
    [ $(wc -l < $snapshotsPath/2022_01_02.dat) -eq 3 ]
    [ $(grep -c "1 5 0" $snapshotsPath/2022_01_05.dat) -eq 1 ]
    [ $(grep -c "1 6 0" $snapshotsPath/2022_01_05.dat) -eq 1 ]
    [ $(grep -c "5 6 7000" $snapshotsPath/2022_01_05.dat) -eq 1 ]
    [ $(wc -l < $snapshotsPath/2022_01_02.dat) -eq 3 ]

    rm -rf $longTermSubgraphPath
    rm -rf $snapshotsPath
}
