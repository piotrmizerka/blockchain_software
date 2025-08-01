#!/usr/bin/env bats

# data for this test taken partially from Wikipedia example: https://en.wikipedia.org/wiki/Singular_value_decomposition/, Example section
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
    ./timeSeries.sh -sp $snapshotsPath -tsp $timeSeriesPath -cn 3 -nm l2

    [ $(wc -l < $timeSeriesPath/component_1.dat) -eq 4 ]
    [ $(wc -l < $timeSeriesPath/component_2.dat) -eq 4 ]
    [ $(wc -l < $timeSeriesPath/component_3.dat) -eq 4 ]
    
    rm -rf ./tst/temp_data
}

@test "timeSeries: data with sample zero row" {
    rm -rf ./tst/temp_data
    mkdir ./tst/temp_data

    snapshotsPath=./tst/temp_data/snapshots
    timeSeriesPath=./tst/temp_data/time_series

    mkdir $snapshotsPath
    
    touch $snapshotsPath/snapshot_1.dat
    echo "x x 1" >> $snapshotsPath/snapshot_1.dat
    echo "x x 2" >> $snapshotsPath/snapshot_1.dat
    echo "x x 3" >> $snapshotsPath/snapshot_1.dat
    echo "x x 4" >> $snapshotsPath/snapshot_1.dat
    echo "x x 5" >> $snapshotsPath/snapshot_1.dat
    touch $snapshotsPath/snapshot_2.dat
    echo "x x 0" >> $snapshotsPath/snapshot_2.dat
    echo "x x 0" >> $snapshotsPath/snapshot_2.dat
    echo "x x 0" >> $snapshotsPath/snapshot_2.dat
    echo "x x 0" >> $snapshotsPath/snapshot_2.dat
    echo "x x 0" >> $snapshotsPath/snapshot_2.dat
    touch $snapshotsPath/snapshot_3.dat
    echo "x x 6" >> $snapshotsPath/snapshot_3.dat
    echo "x x 7" >> $snapshotsPath/snapshot_3.dat
    echo "x x 8" >> $snapshotsPath/snapshot_3.dat
    echo "x x 9" >> $snapshotsPath/snapshot_3.dat
    echo "x x 10" >> $snapshotsPath/snapshot_3.dat
    touch $snapshotsPath/snapshot_4.dat
    echo "x x 11" >> $snapshotsPath/snapshot_4.dat
    echo "x x 12" >> $snapshotsPath/snapshot_4.dat
    echo "x x 13" >> $snapshotsPath/snapshot_4.dat
    echo "x x 14" >> $snapshotsPath/snapshot_4.dat
    echo "x x 15" >> $snapshotsPath/snapshot_4.dat
    
    chmod +x ./timeSeries.sh
    ./timeSeries.sh -sp $snapshotsPath -tsp $timeSeriesPath -cn 3 -nm l2

    [ $(wc -l < $timeSeriesPath/component_1.dat) -eq 4 ]
    [ $(wc -l < $timeSeriesPath/component_2.dat) -eq 4 ]
    [ $(wc -l < $timeSeriesPath/component_3.dat) -eq 4 ]
    
    # the code below inspired by: https://www.codegrepper.com/code-examples/shell/bash+compare+float+values 

    # check if zero rows manifest in appropriate places in the time series
    [ $(echo | awk "{ print ($(awk 'NR==2' $timeSeriesPath/component_1.dat) == 0.0)?1 : 0 }") -eq 1 ]
    [ $(echo | awk "{ print ($(awk 'NR==2' $timeSeriesPath/component_2.dat) == 0.0)?1 : 0 }") -eq 1 ]
    [ $(echo | awk "{ print ($(awk 'NR==2' $timeSeriesPath/component_3.dat) == 0.0)?1 : 0 }") -eq 1 ]

    # other time series' entries shall be non-zero
    [ $(echo | awk "{ print ($(awk 'NR==1' $timeSeriesPath/component_1.dat) != 0.0)?1 : 0 }") -eq 1 ]
    [ $(echo | awk "{ print ($(awk 'NR==1' $timeSeriesPath/component_2.dat) != 0.0)?1 : 0 }") -eq 1 ]
    [ $(echo | awk "{ print ($(awk 'NR==1' $timeSeriesPath/component_3.dat) != 0.0)?1 : 0 }") -eq 1 ]
    [ $(echo | awk "{ print ($(awk 'NR==3' $timeSeriesPath/component_1.dat) != 0.0)?1 : 0 }") -eq 1 ]
    [ $(echo | awk "{ print ($(awk 'NR==3' $timeSeriesPath/component_2.dat) != 0.0)?1 : 0 }") -eq 1 ]
    [ $(echo | awk "{ print ($(awk 'NR==3' $timeSeriesPath/component_3.dat) != 0.0)?1 : 0 }") -eq 1 ]
    [ $(echo | awk "{ print ($(awk 'NR==4' $timeSeriesPath/component_1.dat) != 0.0)?1 : 0 }") -eq 1 ]
    [ $(echo | awk "{ print ($(awk 'NR==4' $timeSeriesPath/component_2.dat) != 0.0)?1 : 0 }") -eq 1 ]
    [ $(echo | awk "{ print ($(awk 'NR==4' $timeSeriesPath/component_3.dat) != 0.0)?1 : 0 }") -eq 1 ]

    rm -rf ./tst/temp_data
}
