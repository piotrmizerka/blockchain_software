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
    if(edges.size()>0)
    {
        FILE* save;
        save = fopen(path.c_str(), "w");
        int edgesSize = edges.size();
        // last edges is repeated when read from users graph
        for(int i=0;i<edgesSize-1;i++)fprintf(save, "%d %d %lld %d\n", edges[i].u, edges[i].v, 
                                              (long long)(edges[i].weight), edges[i].time);
        fclose(save);
    }
}