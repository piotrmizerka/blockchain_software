#!/usr/bin/env bats

# Source: https://github.com/bats-core/bats-core#testing
# In order to perform bash tests:
# (1) install repo above by cloning it and runing "sudo ./install.sh" in the main rep's directory,
# for details, see https://bats-core.readthedocs.io/en/latest/installation.html 
# (2) run "bats path-to-your-test-file"

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
