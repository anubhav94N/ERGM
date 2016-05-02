#include "graph.h"
#include "mcqd/mcqd.h"

Graph::Graph(){};

Graph::Graph(int _N)
{
    this->N = _N;
    this->adjacencyMatrix.resize(N+1);
    for(int i=1;i<=N;++i)
        adjacencyMatrix[i].resize(N+1);
    this->adjacencyList.resize(N+1);
    E = 0;
}

void Graph::buildAdjacencyList()
{
    E = 0;
    for(int i=1;i<=N;i++)
    {
        adjacencyList[i].clear();
        for(int j=1;j<=N;j++)
        {
            if(adjacencyMatrix[i][j])
            {
                adjacencyList[i].push_back(j);
                E++;
            }
        }
    }
    E = E/2;
}

void Graph::makeNewGraph(vector<vector<int> > _adjacencyMatrix)
{
    for(int i=1;i<=N;++i)
    {
        for(int j=1;j<=N;++j)
        {
            this->adjacencyMatrix[i][j] = (_adjacencyMatrix[i][j]&1);
        }
    }
    buildAdjacencyList();
}

void Graph::makeNewRandomGraph(int p) // p is percentage of edges ie probablity * 100
{
    adjacencyMatrix.resize(N+1);
    for(int i=1;i<=N;++i)
    {
        adjacencyMatrix[i].resize(N+1);
        for(int j=1;j<i;++j)
        {
            adjacencyMatrix[i][j] = adjacencyMatrix[j][i];
        }
        for(int j=i+1;j<=N;++j)
        {
            if(rand()%100 < p)
            {
                adjacencyMatrix[i][j] = 1;
            }
            else
            {
                adjacencyMatrix[i][j] = 0;
            }
            //cout<<adjacencyMatrix[j][i]<<" ";
        }
        //cout<<endl;
    }

    buildAdjacencyList();
}

int Graph::Diameter()
{
    int ret = 1;
    vector<int > separation;
    separation = degreesOfSeparation();

    for(ret=1;ret<N;ret++)
    {
        if(separation[ret] == 0)break;
    }
    return ret-1;
}

vector<int> Graph::degreesOfSeparation()
{
    vector<int> ret(N);
    queue<int> Q;
    bool vis[N+1];
    int distance[N+1];

    for(int i=0;i<N;i++)
    {
        ret[i] = 0;
    }

    for(int i=1;i<=N;i++)
    {
        memset(vis,0,sizeof(vis));
        memset(distance,0,sizeof(distance));
        Q.push(i);
        vis[i] = 1;//cout<<"aman\n";
        while(!Q.empty())
        {
            int now = Q.front();
            Q.pop();
            ret[distance[now]]++;
            for(int j=0;j<adjacencyList[now].size();++j)
            {
                int next = adjacencyList[now][j];
                if(vis[next]) continue;
                Q.push(next);
                distance[next] = distance[now] + 1;
                vis[next] = 1;
            }
        }//cout<<"aman\n";
    }
    return ret;
}

vector<int> Graph::degreeDistribution()
{
    vector<int> ret;
    ret.resize(N);
    for(int i=1;i<=N;i++)
    {
        //cout<<"aman\n";
        ret[adjacencyList[i].size()]++;
    }
    return ret;
}

int Graph::maxCliqueSize()
{
    bool **conn;
    conn = new bool*[N];
    for (int i=0; i < N; i++)
    {
        conn[i] = new bool[N];
    }
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            //cout<<i<<" "<<j<<endl;
            conn[i][j] = adjacencyMatrix[i+1][j+1];
            //cout<<i<<" "<<j<<endl;
            //cout<<conn[i][j]<<" ";
        }
        //cout<<endl;
    }
    Maxclique m(conn, N);

    int *qmax; int qsize;
    m.mcqdyn(qmax, qsize);
    return qsize;
}

void Graph::printGraph() const
{
    cout << "N: " << N << " E: " << E << endl;
    for(int i=1;i<=N;++i)
    {
        for(int j=1;j<=N;++j)
        {
            cout << adjacencyMatrix[i][j] << " ";
        }
        cout << endl;
    }
}

/*int main()
{
    Graph g(100);
    g.makeNewRandomGraph(100);
    cout<<g.maxCliqueSize()<<" "<<g.Diameter()<<endl;
}*/
