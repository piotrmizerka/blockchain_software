#!/bin/bash

# In order to sort txin.dat and txout.dat by txidies, first give the necessary permissions to sortTx.sh with the command "chmod +x sortTx.sh" and then run "./sortTx.sh path_to_txin.dat path_to_txout.dat" (assuming you are in the same directory as the sortTx.sh script file).

# Display messages if txin.dat or txout.dat is not sorted
sort -c -n -k 1 -k 2 -k 3 -k 4 -k 5 -k 6 $1
sort -c -n -k 1 -k 2 -k 3 -k 4 -k 5 -k 6 $2

sort -n -k 1 -k 2 -k 3 -k 4 -k 5 -k 6 -o $1 $1
sort -n -k 1 -k 2 -k 3 -k 4 -k 5 -k 6 -o $2 $2
