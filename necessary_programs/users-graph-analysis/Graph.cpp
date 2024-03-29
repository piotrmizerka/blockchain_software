#include "stdafx.h"

Graph::Graph(vector<int> V, vector<Edge> *E, long long transactionsNumber, int usersNumber)
{
    if ((*E).size() > 0 && V.size() == 0)
    {
        vector <bool> verticesIdPresence(usersNumber+1, false);
        FOREACH(edge, (*E))
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
    if((*edges).size()>0) //TODO: handle edge size = 0!! - make fscanf proper everywhere
    {
        FILE* save;
        save = fopen(path.c_str(), "w");
        for(long long i=0;i<(*edges).size();i++)
        {
            fprintf(
                save, "%d %d %lld %d\n", 
                (*edges)[i].u, 
                (*edges)[i].v, 
                (long long)((*edges)[i].weight), 
                (*edges)[i].time
            );
        }

        fclose(save);
    }
}
