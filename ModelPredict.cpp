#include "ModelPredict.h"

ERGMPredict::ERGMPredict(int numbernodes)
{
    N = numbernodes;
    ParameterDistribution = FactorDistribution();
    parametersSamples.clear();
    baseParameters = ParameterDistribution.GenerateParametersNormalDistribution();
    observedGraph = Graph(N);
    observedGraph.makeNewRandomGraph(90);
    observedGraph.printGraph();
    baseModel = createNewERGM(N,baseParameters);
    srand(time(NULL));
}
ERGM ERGMPredict::createNewERGM(int N,vector<double> parameters)
{
    ERGM ergm(N);
    ergm.ties.factor = parameters[0];
    ergm.clique3.factor = parameters[1];
    ergm.star2.factor = parameters[2];
    ergm.star3.factor = parameters[3];

    return ergm;
}

pair< vector<double> , Graph> ERGMPredict::MarkovChainNextStep(pair< vector<double> , Graph> prevState)
{
    vector<double> nextTheta = ParameterDistribution.GenerateParametersNormalDistribution();
    double probability;
    probability = (rand() % 100) + 1;
    int roundedProbability = probability;
    Graph nextAuxVal(N);
    nextAuxVal.makeNewRandomGraph(roundedProbability);

    ERGM currentModel= createNewERGM(N,prevState.first);
    ERGM nextModel = createNewERGM(N,nextTheta);

    double nextProbability = nextModel.getProbability(observedGraph) * baseModel.getProbability(nextAuxVal) * currentModel.getProbability(prevState.second) * ParameterDistribution.computeProbabilityParameters(nextTheta);
    double currentProbability = currentModel.getProbability(observedGraph) * baseModel.getProbability(prevState.second) * nextModel.getProbability(nextAuxVal) * ParameterDistribution.computeProbabilityParameters(prevState.first);
    double probabilityRatio = nextProbability/currentProbability;
    probabilityRatio = min(1.0,probabilityRatio);
    int roundedRatio = probabilityRatio * 10000;
    if(rand()%10000 <= roundedRatio)
    {
        /*
            Change state to new one.
        */
        return make_pair(nextTheta,nextAuxVal);
    }
    else
    {
        /*
            Keep the previous state.
        */
        return prevState;
    }
}

void ERGMPredict::getParametersSamplesMCMC(int maxIterations)
{
    parametersSamples.clear();
    pair<vector<double>,Graph> currState;
    currState.first = ParameterDistribution.GenerateParametersNormalDistribution();
    int randomProbability = rand()%101;
    Graph obj(N);
    obj.makeNewRandomGraph(randomProbability);
    currState.second = obj;
    for(int iter=0;iter<maxIterations;iter++)
    {
        pair<vector<double>,Graph> nextState = MarkovChainNextStep(currState);
        parametersSamples.push_back(nextState);
        currState = nextState;

    }
}

void ERGMPredict::printParametersSamples()
{
    for(int j=0;j<parametersSamples.size();j++)
    {
        for(int x=0;x<(parametersSamples[j].first).size();x++)
        {
            cout<<(parametersSamples[j].first)[x]<<" ";
        }
        cout<<endl;
    }
}

vector<double> ERGMPredict::predictParameters()
{
    map< vector<double> , int> mp;
    vector<double> res;
    int mx = -1;
    for(int j=0;j<parametersSamples.size();j++)
    {
        mp[(parametersSamples[j].first)]++;
        if(mp[(parametersSamples[j].first)] > mx)
        {
            res = (parametersSamples[j].first);
            mx = mp[(parametersSamples[j].first)];
        }

    }
    return res;
}

