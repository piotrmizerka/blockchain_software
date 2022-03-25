#!/usr/bin/env bats

@test "snapshots: sample data" {
    rm -rf ./tst/temp_data
    mkdir ./tst/temp_data

    longTermSubgraphPath=./tst/temp_data/long_term_subgraph.dat
    snapshotsPath=./tst/temp_data/snapshots
    blockhashPath=./tst/temp_data/bh.dat
    
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

    # 1640995611 is on 2022.01.01
    touch $blockhashPath
    echo "1 hash_not_important 1458931501 999" >> $blockhashPath
    echo "2 hash_not_important 1640995611 999" >> $blockhashPath
    echo "3 hash_not_important 1641427200 999" >> $blockhashPath # 1641427200 = 2022.01.06, 00:00:00
    
    chmod +x ./snapshots.sh
    
    ./snapshots.sh -ltsp $longTermSubgraphPath -sdp 1 -ewp n -sp $snapshotsPath -bhp $blockhashPath -bd "2021_12_29"
    [ ! -f $snapshotsPath/2021_12_28.dat ]
    [ -f $snapshotsPath/2021_12_29.dat ]
    [ -f $snapshotsPath/2022_01_06.dat ]
    [ ! -f $snapshotsPath/2022_01_07.dat ]
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

    ./snapshots.sh -ltsp $longTermSubgraphPath -sdp 1 -ewp w -sp $snapshotsPath -bhp $blockhashPath -bd "2021_12_29"
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

    rm -rf ./tst/temp_data
}
