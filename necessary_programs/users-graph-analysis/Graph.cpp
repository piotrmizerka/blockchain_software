#include "stdafx.h"

Graph::Graph(vector<int> V, vector<Edge> E, int transactionsNumber, int usersNumber)
{
    if (E.size() > 0 && V.size() == 0)
    {
        vector <bool> verticesIdPresence(usersNumber+1, false);
        FOREACH(edge, E)
        {
            verticesIdPresence[edge->u] = true;
            verticesIdPresence[edge->v] = true;
        }

        vertices.clear();
        for(int i=0;i<=usersNumber;i++)if(verticesIdPresence[i])vertices.push_back(i);
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