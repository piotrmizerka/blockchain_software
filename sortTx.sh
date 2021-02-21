#!/bin/bash

# Display messages if txin.dat or txout.dat is not sorted
sort -c -n -k 1 -k 2 -k 3 -k 4 -k 5 -k 6 $1
sort -c -n -k 1 -k 2 -k 3 -k 4 -k 5 -k 6 $2

sort -n -k 1 -k 2 -k 3 -k 4 -k 5 -k 6 -o $1 $1
sort -n -k 1 -k 2 -k 3 -k 4 -k 5 -k 6 -o $2 $2
