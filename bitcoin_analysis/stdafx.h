// stdafx.h: dołącz plik do standardowych systemowych plików dołączanych,
// lub specyficzne dla projektu pliki dołączane, które są często wykorzystywane, ale
// są rzadko zmieniane
//

#pragma once

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: W tym miejscu odwołaj się do dodatkowych nagłówków wymaganych przez program
//Jeff Chastine
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cmath>
#include <map>
#include <algorithm>
#include <cstdio>
#include <string>
#include <stdio.h>
#include <queue>
#include <stack>
#include <time.h>
#include <omp.h>
#include <concurrent_vector.h>
#include <ctime>
#include <stdlib.h> 
#include <sys/stat.h>
#include <direct.h>

#define PI 3.14159265
#define EPS 0.00000001

using namespace std;

// Naglowki Piotra Stanczyka
#define VAR(v,n) decltype(n) v=(n)
#define FOREACH(i,c)for(VAR(i,(c).begin());i!=(c).end();++i)

#include "auxiliaryFunctions.h"
#include "Graph.h"
#include "TransactionHypergraph.h"
#include "FileParser.h"
#include "AddressesContractor.h"
#include "auxiliaryRenderingFunctions.h"
#include "GraphVisualizer.h"
#include "GraphTest.h"
#include "TransactionHypergraphTest.h"
#include "AddressesContractorTest.h"