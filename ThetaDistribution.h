#include "ergm.h"
#pragma once
class FactorDistribution
{
    public:
    double TiesMean , Clique3Mean  , Star2Mean , Star3Mean ;
    double TiesVar , Clique3Var , Star2Var  , Star3Var ;
    double TiesMin , Clique3Min  , Star2Min  , Star3Min ;
    double TiesMax , Clique3Max , Star2Max , Star3Max ;

    FactorDistribution();

    double GenerateSampleNormalDistribution(double mean , double variance,double llimit,double ulimit);
    vector<double> GenerateParametersNormalDistribution();
    double computeProbabilityNormalDistribution(double x , double mean , double variance);
    double computeProbabilityParameters(vector<double> parameters);

};
