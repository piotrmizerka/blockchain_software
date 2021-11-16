#pragma once

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <time.h>

using namespace std;

// Headers of Piotr Stanczyk
#define VAR(v,n) decltype(n) v=(n)
#define FOREACH(i,c)for(VAR(i,(c).begin());i!=(c).end();++i)
