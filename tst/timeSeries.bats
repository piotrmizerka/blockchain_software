#!/usr/bin/env bats

# data for this test taken partially from Wikipedia example: https://en.wikipedia.org/wiki/Singular_value_decomposition/, Example section
# TODO: arrange more readible test!
@test "timeSeries: sample data" {
    rm -rf ./tst/temp_data
    mkdir ./tst/temp_data

    snapshotsPath=./tst/temp_data/snapshots
    timeSeriesPath=./tst/temp_data/time_series

    mkdir $snapshotsPath
    
    touch $snapshotsPath/snapshot_1.dat
    echo "x x 1" >> $snapshotsPath/snapshot_1.dat
    echo "x x 0" >> $snapshotsPath/snapshot_1.dat
    echo "x x 0" >> $snapshotsPath/snapshot_1.dat
    echo "x x 0" >> $snapshotsPath/snapshot_1.dat
    echo "x x 2" >> $snapshotsPath/snapshot_1.dat
    touch $snapshotsPath/snapshot_2.dat
    echo "x x 0" >> $snapshotsPath/snapshot_2.dat
    echo "x x 0" >> $snapshotsPath/snapshot_2.dat
    echo "x x 3" >> $snapshotsPath/snapshot_2.dat
    echo "x x 0" >> $snapshotsPath/snapshot_2.dat
    echo "x x 0" >> $snapshotsPath/snapshot_2.dat
    touch $snapshotsPath/snapshot_3.dat
    echo "x x 0" >> $snapshotsPath/snapshot_3.dat
    echo "x x 0" >> $snapshotsPath/snapshot_3.dat
    echo "x x 0" >> $snapshotsPath/snapshot_3.dat
    echo "x x 4" >> $snapshotsPath/snapshot_3.dat
    echo "x x 0" >> $snapshotsPath/snapshot_3.dat
    touch $snapshotsPath/snapshot_4.dat
    echo "x x 0" >> $snapshotsPath/snapshot_4.dat
    echo "x x 2" >> $snapshotsPath/snapshot_4.dat
    echo "x x 0" >> $snapshotsPath/snapshot_4.dat
    echo "x x 0" >> $snapshotsPath/snapshot_4.dat
    echo "x x 0" >> $snapshotsPath/snapshot_4.dat
    
    chmod +x ./timeSeries.sh
    ./timeSeries.sh -sp $snapshotsPath -tsp $timeSeriesPath -cn 3

    [ $(wc -l < $timeSeriesPath/component_1.dat) -eq 4 ]
    [ $(wc -l < $timeSeriesPath/component_2.dat) -eq 4 ]
    [ $(wc -l < $timeSeriesPath/component_3.dat) -eq 4 ]
    
    rm -rf ./tst/temp_data
}
