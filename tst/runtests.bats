#!/usr/bin/env bats

@test "users graph script unit tests" {
    bats ./tst/txEdges.bats
    bats ./tst/txTimes.bats
    bats ./tst/txEdgesTimes.bats
    bats ./tst/contractedAddresses.bats
    bats ./tst/createGraph.bats
}

@test "long-term subraph, snapshots, and time series unit tests" {
    bats ./tst/longTermSubgraph.bats
    bats ./tst/snapshots.bats
    bats ./tst/timeSeries.bats
}

@test "integration tests" {
    bats ./tst/integrationTests.bats
}
