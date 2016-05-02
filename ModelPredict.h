#include "ThetaDistribution.h"
#pragma once
class ERGMPredict
{
    public:
    int N;
    FactorDistribution ParameterDistribution;
    vector<double> baseParameters;
    Graph observedGraph;
    ERGM baseModel;

    ERGMPredict(int);
    vector< pair< vector<double> , Graph>  > parametersSamples;
    void getParametersSamplesMCMC(int);
    pair< vector<double> , Graph> MarkovChainNextStep(pair< vector<double> , Graph>);
    ERGM createNewERGM(int N,vector<double> params);
    void printParametersSamples();
    vector<double> predictParameters();
};
