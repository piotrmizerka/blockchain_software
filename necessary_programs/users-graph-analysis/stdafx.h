#pragma once

#include <iostream>
#include <set>
#include <stdio.h>
#include <string>
#include <vector>

#define EPS 0.00000001

using namespace std;

// Headers of Piotr Stanczyk
#define VAR(v,n) decltype(n) v=(n)
#define FOREACH(i,c)for(VAR(i,(c).begin());i!=(c).end();++i)

#include "Graph.h"