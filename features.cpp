#include "features.h"

long long Ties::count(Graph G)
{
    long long ret = 0;
    for(int i=1;i<=G.N;i++)
    {
        ret += G.adjacencyList[i].size();
    }
    return ret/2;
}

long long Star2::count(Graph G)
{
    long long ret = 0,count = 0;
    for(int i=1;i<=G.N;++i)
    {
        count = G.adjacencyList[i].size();
        ret += (count*(count-1))/2;
    }
    return ret;
}

long long Star3::count(Graph G)
{
    long long ret = 0,count = 0;
    for(int i=1;i<=G.N;++i)
    {
        count = G.adjacencyList[i].size();
        ret += (count * (count - 1) * (count - 2)) / 6;
    }
    return ret;
}

long long Clique3::count(Graph G)
{
    long long ret = 0;
    for(int i=1;i<=G.N;++i)
    {
        for(int j=0;j<G.adjacencyList[i].size();j++)
        {
            for(int k=j+1;k<G.adjacencyList[i].size();k++)
            {
                int next1 = G.adjacencyList[i][j];
                int next2 = G.adjacencyList[i][k];
                if(G.adjacencyMatrix[next1][next2])
                {
                    ++ret;
                }
            }
        }
    }
    return ret/3;
}

