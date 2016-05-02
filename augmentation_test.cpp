#include "augmentation_test.h"

/*
    This function prints in format required for plotting curve in Octave.
*/
void printPDFForOctave1(vector<double> vec)
{
    freopen("out.m","w",stdout);
    cout<<"x = [\n";
    for(int i=0;i<vec.size();i++)
    {
        if(vec[i] != 0)
        {
            cout<<i<<",\n";
        }
    }
    cout<<"];\n";

    cout<<"y = [\n";
    for(int i=0;i<vec.size();i++)
    {
        if(vec[i] != 0)
        {
            cout<<vec[i]<<",\n";
        }
    }
    cout<<"];\n";
    fclose(stdout);

    /*
    for(int i=0;i<vec.size();i++)
    {
        if(vec[i] != 0)
        {
            cout<<i<<" "<<vec[i]<<endl;
        }
    }
    */
}

/*
    This maps each graph in the sample to its frequency.
    Used to print K-most frequency graphs.
*/
map<Graph,int> mapGraphToFrequency;


/*
    This function prints K-most frequent graphs.
    Uses "mapGraphToFrequency"
*/
void printKMostFrequentGraphs()
{
    int K = 5;
    map<Graph,int>::iterator itr;
    int totalGraphCount = 0;
    for(itr = mapGraphToFrequency.begin();itr!=mapGraphToFrequency.end();++itr)
    {
        totalGraphCount += itr->second;
    }

    for(int i=1;i<=K;++i)
    {
        if(mapGraphToFrequency.empty())
        {
            break;
        }
        int maxFrequency = 0;
        for(itr = mapGraphToFrequency.begin();itr!=mapGraphToFrequency.end();++itr)
        {
            maxFrequency = max(maxFrequency,itr->second);
        }
        for(itr = mapGraphToFrequency.begin();itr!=mapGraphToFrequency.end();++itr)
        {
            if(itr->second == maxFrequency)
            {
                cout << "--> Graph " << i << " Frequency Ratio: " << (maxFrequency*1.0)/totalGraphCount;
                cout << "(" << maxFrequency << ")" << endl;
                (itr->first).printGraph();
                cout << "*******************" << endl << endl;
                mapGraphToFrequency.erase(itr);
                break;
            }
        }
    }
}

void printObservedGraph(vector<vector<int> > adjacencyMatrix)
{
    int N = adjacencyMatrix.size()-1;
    int E = 0;
    for(int i=1;i<=N;++i)
    {
        for(int j=1;j<=N;++j)
        {
            E += (adjacencyMatrix[i][j]&1);
        }
    }
    E /= 2;
    cout << " N: " << N << " observed Edges: " << E << endl;
    for(int i=1;i<=N;++i)
    {
        for(int j=1;j<=N;++j)
        {
            if(adjacencyMatrix[i][j] > 1)
            {
                cout << (adjacencyMatrix[i][j]-2) << " ";
            }
            else
            {
                cout << "- ";
            }
        }
        cout << endl;
    }
}
/*
    Generates a random graph with edge probability selection probability equal to p.
    This is used as sample to pass to data augmentation function.
*/
vector<vector<int> > generateRandomSample(int N,
                                          int probabilityObserved,
                                          int probabilityEdgeSelection)
{
    vector<vector<int> > adjacencyMatrix;
    adjacencyMatrix.resize(N+1);
    for(int i=1;i<=N;++i)
    {
        adjacencyMatrix[i].resize(N+1);
        adjacencyMatrix[i][i] = 2;
        for(int j=1;j<i;++j)
        {
            if(rand()%100 <= probabilityObserved)
            {
                /*
                    This edge has been observed it means.
                */
                if(rand()%100 <= probabilityEdgeSelection)
                {
                    adjacencyMatrix[i][j] = 3;
                }
                else
                {
                    adjacencyMatrix[i][j] = 2;
                }
            }
            else
            {
                /*
                    This edge has been observed it means.
                */
                adjacencyMatrix[i][j] = 0;
            }
            adjacencyMatrix[j][i] = adjacencyMatrix[i][j];
        }
    }
    return adjacencyMatrix;
}

vector<vector<int> > generateSampleWithTwoUnobservedTies(int N,int probabilityEdgeSelection)
{
    vector<vector<int> > adjacencyMatrix;
    adjacencyMatrix.resize(N+1);
    int cnt = 0;
    for(int i=1;i<=N;++i)
    {
        adjacencyMatrix[i].resize(N+1);
        adjacencyMatrix[i][i] = 2;
        for(int j=1;j<i;++j)
        {
            if(rand()%100 <= probabilityEdgeSelection)
            {
                adjacencyMatrix[i][j] = 3;
            }
            else
            {
                adjacencyMatrix[i][j] = 2;
            }
            if(cnt < 2 && rand()%100 <= 5)
            {
                cnt++;
                adjacencyMatrix[i][j] = 0;
            }
            adjacencyMatrix[j][i] = adjacencyMatrix[i][j];
        }
    }
    return adjacencyMatrix;
}

/*
    - 20% graph sampled
    - 100% edges present in the sampled graph
*/
void testForRandomGraph1()
{
    vector<vector<int> > adjacencyMatrix;
    int probabilityObserved = 20;
    int probabilityEdgeSelection = 100;
    int N = 10;
    adjacencyMatrix = generateRandomSample(N,probabilityObserved,probabilityEdgeSelection);
    ERGM ergm(N);
    ergm.ties.factor = 0;
    ergm.star2.factor = 0;
    ergm.star3.factor = 0;
    ergm.clique3.factor = 0;
    printObservedGraph(adjacencyMatrix);
    ergm.runDataAugmentationMCMC(adjacencyMatrix,20000);
    for(int i=1;i<ergm.augmentedGraphs.size();++i)
    {
        mapGraphToFrequency[ergm.augmentedGraphs[i]]++;
    }
    ergm.expectedEdgeCountDataAugmentation();
    printKMostFrequentGraphs();
}

/*
    - 20% graph sampled
    - 50% edges present in the sampled graph
*/
void testForRandomGraph2()
{
    vector<vector<int> > adjacencyMatrix;
    int probabilityObserved = 20;
    int probabilityEdgeSelection = 50;
    int N = 10;
    adjacencyMatrix = generateRandomSample(N,probabilityObserved,probabilityEdgeSelection);
    ERGM ergm(N);
    ergm.ties.factor = 1;
    ergm.star2.factor = 1;
    ergm.star3.factor = 1;
    ergm.clique3.factor = -10000;
    printObservedGraph(adjacencyMatrix);
    ergm.runDataAugmentationMCMC(adjacencyMatrix,20000);
    for(int i=1;i<ergm.augmentedGraphs.size();++i)
    {
        mapGraphToFrequency[ergm.augmentedGraphs[i]]++;
    }
    ergm.expectedEdgeCountDataAugmentation();
    printKMostFrequentGraphs();
}

/*
    - 50% graph sampled
    - 50% edges present in the sampled graph
*/
void testForRandomGraph3()
{
    vector<vector<int> > adjacencyMatrix;
    int probabilityObserved = 50;
    int probabilityEdgeSelection = 50;
    int N = 40;
    adjacencyMatrix = generateRandomSample(N,probabilityObserved,probabilityEdgeSelection);
    ERGM ergm(N);
    ergm.ties.factor = 0;
    ergm.star2.factor = 90;
    ergm.star3.factor = -100;
    ergm.clique3.factor = 0;
    printObservedGraph(adjacencyMatrix);
    ergm.runDataAugmentationMCMC(adjacencyMatrix,20000);
    for(int i=1;i<ergm.augmentedGraphs.size();++i)
    {
        mapGraphToFrequency[ergm.augmentedGraphs[i]]++;
    }
    ergm.expectedEdgeCountDataAugmentation();
    //vector<double> vec;
    //vec = ergm.computeTiesProbabilityDistributionDataAugmentation();
    //printPDFForOctave1(vec);
    printKMostFrequentGraphs();
}

/*
    - All but one edge of the graph have been observed.
    - 50% edges present in the sampled graph.
*/
void testForSpecialGraph()
{
    vector<vector<int> > adjacencyMatrix;
    int probabilityEdgeSelection = 50;
    int N = 10;
    adjacencyMatrix = generateSampleWithTwoUnobservedTies(N,probabilityEdgeSelection);
    ERGM ergm(N);
    ergm.ties.factor = 100;
    ergm.star2.factor = 100;
    ergm.star3.factor = 100;
    ergm.clique3.factor = 0;
    printObservedGraph(adjacencyMatrix);
    cout << endl << endl;
    ergm.runDataAugmentationMCMC(adjacencyMatrix,20000);
    for(int i=1;i<ergm.augmentedGraphs.size();++i)
    {
        mapGraphToFrequency[ergm.augmentedGraphs[i]]++;
    }
    ergm.expectedEdgeCountDataAugmentation();
    printKMostFrequentGraphs();

    vector<double> probabilities;
    ergm.removeRepetition(ergm.augmentedGraphs);
    double probabilitySum = 0;
    for(int i=0;i<ergm.augmentedGraphs.size();++i)
    {
        probabilities.push_back(ergm.getProbability(ergm.augmentedGraphs[i]));
        probabilitySum += probabilities[i];
    }
    sort(probabilities.begin(),probabilities.end(),std::greater<double>());
    for(int i=0;i<probabilities.size();++i)
    {
        //ergm.augmentedGraphs[i].printGraph();
        cout << "According to ERGM model, expected probability of graph " << i << ": ";
        cout << (probabilities[i]/probabilitySum) << endl;
    }
}


