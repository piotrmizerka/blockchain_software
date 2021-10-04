#include "stdafx.h"

Graph::Graph(vector<int> V, vector<Edge> E, int transactionsNumber, int usersNumber)
{
    if (E.size() > 0 && V.size() == 0)
    {
        set <int> verticesx;
        FOREACH(edge, E)
        {
            verticesx.insert(edge->u);
            verticesx.insert(edge->v);
        }
        vertices.clear();
        FOREACH(v, verticesx)vertices.push_back(*v);
    }
    else vertices = V;
    edges = E;
    transactionsNumber = transactionsNumber;
    usersNumber = usersNumber;
}

void Graph::saveGraph(string path)
{
    FILE* save;
    save = fopen(path.c_str(), "w");
    FOREACH(edge,edges)fprintf(save, "%d %d %lld %d\n", edge->u, edge->v, 
                               (long long)(edge->weight), edge->time);
    fclose(save);
}