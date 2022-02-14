#!/usr/bin/env bats

# data for this test taken partially from Wikipedia example: https://en.wikipedia.org/wiki/Singular_value_decomposition/, Example section
# TODO: arrange more readible test!
@test "timeSeries: sample data" {
    snapshotsPath=./tst/snapshots
    timeSeriesPath=./tst/time_series

    rm -rf $snapshotsPath
    mkdir $snapshotsPath
    rm -rf $timeSeriesPath
    
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

    [ $(grep -c "0.05" $timeSeriesPath/component_1.dat) -eq 1 ]
    [ $(grep -c "0.54" $timeSeriesPath/component_1.dat) -eq 1 ]
    [ $(grep -c "0.67" $timeSeriesPath/component_1.dat) -eq 1 ]
    [ $(grep -c "0.187" $timeSeriesPath/component_1.dat) -eq 1 ]
    [ $(wc -l < $timeSeriesPath/component_1.dat) -eq 4 ]
    [ $(grep -c "0.26" $timeSeriesPath/component_2.dat) -eq 1 ]
    [ $(grep -c "0.09" $timeSeriesPath/component_2.dat) -eq 1 ]
    [ $(grep -c "0.02" $timeSeriesPath/component_2.dat) -eq 1 ]
    [ $(grep -c "0.14" $timeSeriesPath/component_2.dat) -eq 1 ]
    [ $(wc -l < $timeSeriesPath/component_2.dat) -eq 4 ]
    [ $(grep -c "0.61" $timeSeriesPath/component_3.dat) -eq 1 ]
    [ $(grep -c "0.20" $timeSeriesPath/component_3.dat) -eq 1 ]
    [ $(grep -c "0.14" $timeSeriesPath/component_3.dat) -eq 1 ]
    [ $(grep -c "0.26" $timeSeriesPath/component_3.dat) -eq 1 ]
    [ $(wc -l < $timeSeriesPath/component_3.dat) -eq 4 ]
    
    rm -rf $snapshotsPath
    rm -rf $timeSeriesPath
}
