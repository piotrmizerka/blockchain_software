#pragma once

struct Edge
{
    int u, v, time;
    double weight;
};

bool operator < (const Edge &P,const Edge &Q);

int maxValue(vector <int> vect);

class Graph
{
    // parameters
    vector <int> vertices;
    vector <Edge> edges;
    int transactionsNumber = 3000000;
    int usersNumber = 1500000; // max users number bound

public:
    //functionalities
    void saveGraph(string path);

    // contructor
    Graph(vector <int> V = vector<int>(0), vector<Edge> E = vector<Edge>(0),
          int transactionsNumber = 3000000, int usersNumber = 1500000);
};
