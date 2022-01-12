#!/usr/bin/env bats

# Source: https://github.com/bats-core/bats-core#testing
# In order to perform bash tests:
# (1) install repo above by cloning it and runing "sudo ./install.sh" in the main rep's directory,
# for details, see https://bats-core.readthedocs.io/en/latest/installation.html 
# (2) run "bats path-to-your-test-file"
# @test is works the same as @testset in Julia ;)

# test change to ignore

@test "addition using bc" {
  result="$(echo 2+2 | bc)"
  [ "$result" -eq 4 ]

  result2="$(echo 2+2 | bc)"
  [ "$result2" -eq 2 ]
}

@test "addition using dc" {
  result="$(echo 2 2+p | dc)"
  [ "$result" -eq 4 ]
}