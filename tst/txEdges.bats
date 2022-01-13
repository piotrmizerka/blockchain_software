#!/usr/bin/env bats

# Source: https://github.com/bats-core/bats-core#testing
# In order to perform bash tests:
# (1) install repo above by cloning it and runing "sudo ./install.sh" in the main rep's directory,
# for details, see https://bats-core.readthedocs.io/en/latest/installation.html 
# (2) run "bats path-to-your-test-file"
# Remarks:
# @test works similar to @testset in Julia ;)
# [expression_to_test] works similar to @test expression_to_test in Julia

@test "txEdges: sample data" {
    rm -rf ./tst/txedges.dat
    rm -rf ./tst/txin.dat
    rm -rf ./tst/txout.dat
    txinPath=./tst/txin.dat
    touch $txinPath
    echo "1	0	0	0	1	5000" >> $txinPath
    echo "1	0	0	0	2	1000" >> $txinPath
    echo "2	0	0	0	2	1000" >> $txinPath
    echo "2	0	0	0	3	2000" >> $txinPath
    echo "3	0	0	0	3	2000" >> $txinPath
    echo "3	0	0	0	4	6000" >> $txinPath
    echo "4	0	0	0	5	7000" >> $txinPath
    txoutPath=./tst/txout.dat
    touch $txoutPath
    echo "1	0	5	6000" >> $txoutPath
    echo "2	0	5	1000" >> $txoutPath
    echo "2	0	6	2000" >> $txoutPath
    echo "3	0	6	8000" >> $txoutPath
    echo "4	0	6	7000" >> $txoutPath
    txedgesPath=./tst/txedges.dat
    ./txEdges.sh $txinPath $txoutPath $txedgesPath

    [ $(grep -c "1	1	5	5000" $txedgesPath) -eq 1 ]
    [ $(grep -c "1	2	5	1000" $txedgesPath) -eq 1 ]
    [ $(grep -c "2	2	5	333.33333333333331" $txedgesPath) -eq 1 ]
    [ $(grep -c "2	2	6	666.66666666666663" $txedgesPath) -eq 1 ]
    [ $(grep -c "2	3	5	666.66666666666663" $txedgesPath) -eq 1 ]
    [ $(grep -c "2	3	6	1333.3333333333333" $txedgesPath) -eq 1 ]
    [ $(grep -c "3	3	6	2000" $txedgesPath) -eq 1 ]
    [ $(grep -c "3	4	6	6000" $txedgesPath) -eq 1 ]
    [ $(grep -c "4	5	6	7000" $txedgesPath) -eq 1 ]
    [ $(wc -l < $txedgesPath) -eq 9 ]

    rm -rf ./tst/txedges.dat
    rm -rf ./tst/txin.dat
    rm -rf ./tst/txout.dat
}
