#include "ergm.h"

#define SAMPLE_SIZE 100
#define MAXEDGES 2500

double beta[MAXEDGES][MAXEDGES];

void precomputeBetaFn()
{
    beta[1][1] = 1;
    for(int i=1;i<MAXEDGES;++i)
    {
        for(int j=1;j<MAXEDGES;++j)
        {
            if(i==1 && j==1) continue;
            if(i>1) beta[i][j] = beta[i-1][j]*(i-1)/(i+j-1);
            else beta[i][j] = beta[i][j-1]*(j-1)/(i+j-1);
        }
    }
}

ERGM::ERGM(){};

ERGM::ERGM(int _N)
{
    this->N = _N;
    ties.factor = 0;
    star2.factor = 0;
    star3.factor = 0;
    clique3.factor = 0;
    graphSamples.clear();
    augmentedGraphs.clear();
    sampledUsingMCMC = 0;
    precomputeBetaFn();
}

double ERGM::getProbability(Graph graph)
{
    if(sampledUsingMCMC)
    {
        /*
            We consider all graphs equally probable here, as MCMC already samples based on PDF.
        */
        //cout << " here " << endl;
        return 1;
    }
    double ret = (ties.factor * ties.count(graph)) / ((N*(N-1))/2);
    ret += (star2.factor * star2.count(graph)) / ((N*(N-1)*(N-2))/2);
    ret += star3.factor * star3.count(graph) / ((N*(N-1)*(N-2)*(N-3))/6);
    ret += clique3.factor * clique3.count(graph) / ((N*(N-1)*(N-2))/6);
    //cout << " ret: " << ret << endl;
    return exp(ret);
}

void ERGM::generateAllGraphs()
{
    graphSamples.clear();
    if(N>7)
    {
        printf("N is too large, cannot generate all possible graphs");
        return;
    }

    /*
        In this case, we have generated all 2^(nC2) possible graphs for the given node size.
        No need to remove repetitions in this case, as there won't be any.
    */

    int numberOfEdges = (N*(N-1))/2;
    vector<vector<int> > adjacencyMatrix;
    for(int g=0;g<(1<<numberOfEdges);++g)
    {
        //cout << "g: " <<g << endl;
        int edgeNumber = 0;
        adjacencyMatrix.resize(N+1);
        for(int i=1;i<=N;++i)
            adjacencyMatrix[i].resize(N+1);
        /*
        cout << obj.adjacencyMatrix[1][2] << " " << obj.adjacencyMatrix[1][3] << " " ;
        cout << obj.adjacencyMatrix[2][3] << endl;
        */
        for(int i=1;i<=N;++i)
        {
            for(int j=i+1;j<=N;++j)
            {
                if((g>>edgeNumber)&1)
                {
                    adjacencyMatrix[i][j] = 1;
                    adjacencyMatrix[j][i] = 1;
                    //cout << edgeNumber << " " << i << "," << j << endl;
                }
                edgeNumber++;
            }
        }
        Graph obj(N);
        obj.makeNewGraph(adjacencyMatrix);
        /*cout << obj.adjacencyMatrix[1][2] << " " << obj.adjacencyMatrix[1][3] << " " ;
        cout << obj.adjacencyMatrix[2][3] << endl;
        */
        graphSamples.push_back(obj);
    }
}

void ERGM::getGraphSamples()
{
    graphSamples.clear();
    for(int p=0;p<=100;p++)
    {
        for(int i=1;i<=SAMPLE_SIZE;i++)
        {
            Graph obj(N);
            obj.makeNewRandomGraph(p);
            graphSamples.push_back(obj);
        }
    }
}

void ERGM::getGraphSamplesMCMC(int stepCount)
{
    graphSamples.clear();
    int randomProbability = rand()%101;
    Graph obj(N);
    obj.makeNewRandomGraph(randomProbability);
    for(int iter=0;iter<stepCount;iter++)
    {
        vector<vector<int> > adjacencyMatrix;
        adjacencyMatrix.resize(N+1);
        for(int i=1;i<=N;++i)
        {
            adjacencyMatrix[i].resize(N+1);
            for(int j=1;j<=N;++j)
            {
                adjacencyMatrix[i][j] = obj.adjacencyMatrix[i][j];
            }
        }
        vector<vector<int> > nextAdjacencyMatrix = MarkovChainStep(adjacencyMatrix);
        obj.makeNewGraph(nextAdjacencyMatrix);
        graphSamples.push_back(obj);
        sampledUsingMCMC = 1;
        //obj.printGraph();
    }
}

void ERGM::runDataAugmentationMCMC(vector<vector<int> > adjacencyMatrix,int stepCount)
{

    augmentedGraphs.clear();
    int N = adjacencyMatrix.size()-1;
    Graph obj(N);
    for(int iter=0;iter<stepCount;iter++)
    {
        vector<vector<int> > nextAdjacencyMatrix = MarkovChainStep(adjacencyMatrix);
        obj.makeNewGraph(nextAdjacencyMatrix);
        augmentedGraphs.push_back(obj);
        adjacencyMatrix = nextAdjacencyMatrix;
    }
}

vector<vector<int> > ERGM::MarkovChainStep(vector<vector<int> > adjacencyMatrix)
{
    vector<vector<int> > nextAdjacencyMatrix;
    double probability;
    probability = 0;
    int presentEdges,absentEdges;
    presentEdges = 0;
    absentEdges = 0;
    for(int i=1;i<=N;++i)
    {
        for(int j=i+1;j<=N;++j)
        {
            if(adjacencyMatrix[i][j]%2==1)
            {
                ++presentEdges;
            }
            else
            {
                ++absentEdges;
            }
        }
    }
    probability = 1 + rand()%3;
    //cout << " Probability for beta: " << probability << endl;
    int roundedProbability = probability;
    nextAdjacencyMatrix = adjacencyMatrix;
    for(int i=1;i<=N;++i)
    {
        for(int j=i+1;j<=N;++j)
        {
            if(nextAdjacencyMatrix[i][j] < 2)
            {
                /*
                    This is an unobserved tie
                */
                if(rand()%1000 <= roundedProbability)
                {
                    /*
                        Flip the value
                    */
                    nextAdjacencyMatrix[i][j] = 1-nextAdjacencyMatrix[i][j];
                }
                else
                {
                    /*
                        Do not flip the value, nothing to be done here
                    */
                    ;
                }
                nextAdjacencyMatrix[j][i] = nextAdjacencyMatrix[i][j];
            }
        }
    }
    Graph obj(N);
    Graph nextObj(N);
    obj.makeNewGraph(adjacencyMatrix);
    nextObj.makeNewGraph(nextAdjacencyMatrix);
    double currentProbability = getProbability(obj);
    double nextProbability = getProbability(nextObj);
    //cout << " obj <<<<<<<<<" << endl;
    //obj.printGraph();
    //cout << " Pr: " << currentProbability << endl;
    //cout << " nextObj <<<<<<<<<" << endl;
    //nextObj.printGraph();
    //cout << " Pr: " << nextProbability << endl;
    double probabilityRatio = nextProbability/currentProbability;
    probabilityRatio = min(1.0,probabilityRatio);
    int roundedRatio = probabilityRatio * 10000;
    if(rand()%10000 <= roundedRatio)
    {
        /*
            Change adjacency matrix to new one.
        */
        return nextAdjacencyMatrix;
    }
    else
    {
        /*
            Keep the previous adjacency Matrix.
        */
        return adjacencyMatrix;
    }
}


void ERGM::removeRepetition(vector<Graph>& graphs)
{
    vector<Graph> finalSamples;
    finalSamples.clear();
    map<Graph,bool > mapGraphToBool;

    for(int i=0;i<graphs.size();i++)
    {
        if(mapGraphToBool.find(graphs[i]) != mapGraphToBool.end())continue;
        mapGraphToBool[graphs[i]] = 1;
        finalSamples.push_back(graphs[i]);
    }

    graphs = finalSamples;
}

void ERGM::expectedEdgeCountDataAugmentation()
{
    double ret = 0,total = 0;
    for(int i=0;i<augmentedGraphs.size();i++)
    {
        ret += ties.count(augmentedGraphs[i]);
    }
    total = augmentedGraphs.size();
    if(total == 0)total = 1;
    ret/=total;
    cout<<"Expected Number of edges after Data Augmentation is : "<<ret<<endl<<endl;
}

vector<double > ERGM::computeTiesProbabilityDistributionDataAugmentation()
{
    vector<double> ret;
    ret.resize((N*(N-1))/2+1);
    double total = 0;
    for(int i=0;i<augmentedGraphs.size();i++)
    {
        int cnt = ties.count(augmentedGraphs[i]);
        ret[cnt]++;
    }
    total = augmentedGraphs.size();
    if(total == 0)total = 1;
    for(int i=0;i<ret.size();i++)
    {
        ret[i] /= total;
    }
    return ret;
}


vector<double > ERGM::computeTiesProbabilityDistribution()
{
    vector<double > ret;
    ret.resize((N*(N-1))/2 + 1);
    for(int i=0;i<graphSamples.size();i++)
    {
        int cnt = ties.count(graphSamples[i]);
        ret[cnt] += getProbability(graphSamples[i]);
    }
    double total = 0;
    for(int i=0;i<ret.size();i++)
    {
        total += ret[i];
    }
    if(total == 0)total = 1;
    for(int i=0;i<ret.size();i++)
    {
        ret[i]/=total;
    }
    return ret;
}

vector<double > ERGM::computeStar2ProbabilityDistribution()
{
    vector<double > ret;
    ret.resize((N*(N-1)*(N-2))/2 + 1);
    for(int i=0;i<graphSamples.size();i++)
    {
        int cnt = star2.count(graphSamples[i]);
        ret[cnt] += getProbability(graphSamples[i]);
    }
    double total = 0;
    for(int i=0;i<ret.size();i++)
    {
        total += ret[i];
    }
    if(total == 0)total = 1;
    for(int i=0;i<ret.size();i++)
    {
        ret[i]/=total;
    }
    return ret;
}

vector<double > ERGM::computeStar3ProbabilityDistribution()
{
    vector<double > ret;
    ret.resize((N*(N-1)*(N-2)*(N-3))/6 + 1);
    for(int i=0;i<graphSamples.size();i++)
    {
        int cnt = star3.count(graphSamples[i]);
        ret[cnt] += getProbability(graphSamples[i]);
    }
    double total = 0;
    for(int i=0;i<ret.size();i++)
    {
        total += ret[i];
    }
    if(total == 0)total = 1;
    for(int i=0;i<ret.size();i++)
    {
        ret[i]/=total;
    }
    return ret;
}

vector<double > ERGM::computeClique3ProbabilityDistribution()
{
    vector<double > ret;
    ret.resize((N*(N-1)*(N-2))/6 + 1);
    for(int i=0;i<graphSamples.size();i++)
    {
        int cnt = clique3.count(graphSamples[i]);
        ret[cnt] += getProbability(graphSamples[i]);
    }
    double total = 0;
    for(int i=0;i<ret.size();i++)
    {
        total += ret[i];
    }
    if(total == 0)total = 1;
    for(int i=0;i<ret.size();i++)
    {
        ret[i]/=total;
    }
    return ret;
}

vector<double > ERGM::computeDiameterProbabilityDistribution()
{
    vector<double > ret;
    ret.resize(N);
    for(int i=0;i<graphSamples.size();i++)
    {
        int cnt = graphSamples[i].Diameter();
        ret[cnt] += getProbability(graphSamples[i]);
    }
    double total = 0;
    for(int i=0;i<ret.size();i++)
    {
        total += ret[i];
    }
    if(total == 0)total = 1;
    for(int i=0;i<ret.size();i++)
    {
        ret[i]/=total;
    }
    return ret;
}

vector<double > ERGM::computeMaxCliqueSizeProbabilityDistribution()
{
    vector<double > ret;
    ret.resize(N+1);
    for(int i=0;i<graphSamples.size();i++)
    {
        int cnt = graphSamples[i].maxCliqueSize();
        ret[cnt] += getProbability(graphSamples[i]);
    }
    double total = 0;
    for(int i=0;i<ret.size();i++)
    {
        total += ret[i];
    }
    if(total == 0)total = 1;
    for(int i=0;i<ret.size();i++)
    {
        ret[i]/=total;
    }
    return ret;
}

vector<double > ERGM::computeExpectedDegreesOfSeparation()
{
    vector<double > ret;
    ret.resize(N);
    double total = 0;
    for(int i=0;i<graphSamples.size();i++)
    {
        vector<int > separation;
        separation = graphSamples[i].degreesOfSeparation();
        for(int j=0;j<N;j++)
        {
            ret[j] += separation[j]*getProbability(graphSamples[i]);
        }
        total += getProbability(graphSamples[i]);
    }
    if(total == 0)total = 1;
    for(int i=0;i<ret.size();i++)
    {
        ret[i]/=total;
    }
    return ret;
}

vector<double > ERGM::computeExpectedDegreeDistribution()
{
    vector<double > ret;
    ret.resize(N);
    double total = 0;
    for(int i=0;i<graphSamples.size();i++)
    {
        vector<int > Distribution;
        Distribution = graphSamples[i].degreeDistribution();
        for(int j=0;j<N;j++)
        {
            ret[j] += Distribution[j]*getProbability(graphSamples[i]);
            //cout<<ret[j]<<endl;
        }
        total += getProbability(graphSamples[i]);
    }
    if(total == 0)total = 1;
    //cout<<"total "<<total<<endl;
    for(int i=0;i<ret.size();i++)
    {
        ret[i]/=total;
    }
    return ret;
}

vector<double > ERGM::probabilityDistributionDiameterGivenTies(int minBound, int maxBound)
{
    vector<double > ret;
    ret.resize(N);
    double total = 0;
    for(int i=0;i<graphSamples.size();i++)
    {
        int cntDiameter = graphSamples[i].Diameter();
        int cntTies = ties.count(graphSamples[i]);
        if(minBound <= cntTies && cntTies <= maxBound)
        {
            ret[cntDiameter] += getProbability(graphSamples[i]);
        }
    }
    for(int i=0;i<ret.size();i++)
    {
        total += ret[i];
    }
    if(total == 0)total = 1;
    for(int i=0;i<ret.size();i++)
    {
        ret[i]/=total;
    }
    return ret;
}

vector<double > ERGM::probabilityDistributionMaxCliqueSizeGivenTies(int minBound, int maxBound)
{
    vector<double > ret;
    ret.resize(N+1);
    double total = 0;
    for(int i=0;i<graphSamples.size();i++)
    {
        int cntMaxCliqueSize = graphSamples[i].maxCliqueSize();
        int cntTies = ties.count(graphSamples[i]);
        if(minBound <= cntTies && cntTies <= maxBound)
        {
            ret[cntMaxCliqueSize] += getProbability(graphSamples[i]);
        }
    }
    for(int i=0;i<ret.size();i++)
    {
        total += ret[i];
        //cout<<i<<" asd "<<ret[i]<<endl;
    }
    if(total == 0)total = 1;
    for(int i=0;i<ret.size();i++)
    {
        ret[i]/=total;
    }
    return ret;
}


vector<double > ERGM::probabilityDistributionClique3GivenStar2(int minBound, int maxBound)
{
    vector<double > ret;
    ret.resize((N*(N-1)*(N-2))/6 + 1);
    double total = 0;
    for(int i=0;i<graphSamples.size();i++)
    {
        int cntClique3 = clique3.count(graphSamples[i]);
        int cntStar2 = star2.count(graphSamples[i]);
        if(minBound <= cntStar2 && cntStar2 <= maxBound)
        {
            ret[cntClique3] += getProbability(graphSamples[i]);
        }
    }
    for(int i=0;i<ret.size();i++)
    {
        total += ret[i];
    }
    if(total == 0)total = 1;
    //cout<<total<<endl;
    for(int i=0;i<ret.size();i++)
    {
        ret[i]/=total;
    }
    return ret;
}

