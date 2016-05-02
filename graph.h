#include<bits/stdc++.h>
#pragma once
using namespace std;

class Graph
{
    public:
    /*
        Data Members
    */
    int N,E;
    vector<vector<int> > adjacencyMatrix;
    vector<vector<int> > adjacencyList;

    /*
        Member functions
    */
    Graph();
    Graph(int);
    bool operator<(const Graph& g) const
    {
        for(int i=1;i<=N;i++)
        {
            for(int j=1;j<=N;j++)
            {
                if(this->adjacencyMatrix[i][j] != g.adjacencyMatrix[i][j])
                    return (this->adjacencyMatrix[i][j] < g.adjacencyMatrix[i][j]);
            }
        }
        return 0;
    }

    bool operator==(const Graph& g) const
    {
        for(int i=1;i<=N;i++)
        {
            for(int j=1;j<=N;j++)
            {
                if(this->adjacencyMatrix[i][j] != g.adjacencyMatrix[i][j])
                    return 0;
            }
        }
        return 1;
    }

    void makeNewGraph(vector<vector<int> >);
    void makeNewRandomGraph(int p);
    void buildAdjacencyList();

    int Diameter();
    int maxCliqueSize();
    int connectedComponents();
    vector<int> degreesOfSeparation();
    vector<int> degreeDistribution();
    void printGraph() const;
};
