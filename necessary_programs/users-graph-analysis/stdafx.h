#pragma once

#include <iostream>
#include <set>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>

using namespace std;

// Headers of Piotr Stanczyk, see (in Polish): https://www-users.mat.umk.pl/~stencel/acm/algorytmika_praktyczna.pdf
#define VAR(v,n) decltype(n) v=(n)
#define FOREACH(i,c)for(VAR(i,(c).begin());i!=(c).end();++i)

#include "Graph.h"