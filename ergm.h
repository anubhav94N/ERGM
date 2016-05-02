#include "features.h"
#pragma once
class ERGM
{
    public:
    int N;

    Star2 star2;
    Star3 star3;
    Clique3 clique3;
    Ties ties;

    bool sampledUsingMCMC;

	vector<Graph> graphSamples;
    vector<Graph> augmentedGraphs;

    ERGM();
    ERGM(int);
    double getProbability(Graph);

    /*
        Functions to generate graph according to given probability distribution function
    */
    void generateAllGraphs();
    void getGraphSamples();
    void getGraphSamplesMCMC(int);
    void runDataAugmentationMCMC(vector<vector<int> >,int);
    vector<vector<int> > MarkovChainStep(vector<vector<int> >);

    /*
        This function removes repetition in the sampled graphs. Improves accuracy of the result.
    */
    void removeRepetition(vector<Graph>&);

    /*
        These functions are used to perform different types of analyses on the given ERGM model.
    */
    void expectedEdgeCountDataAugmentation();
    vector<double > computeTiesProbabilityDistributionDataAugmentation();
    vector<double > computeTiesProbabilityDistribution();
    vector<double > computeStar2ProbabilityDistribution();
    vector<double > computeStar3ProbabilityDistribution();
    vector<double > computeClique3ProbabilityDistribution();
    vector<double > computeDiameterProbabilityDistribution();
    vector<double > computeMaxCliqueSizeProbabilityDistribution();
    vector<double > computeExpectedDegreesOfSeparation();
    vector<double > computeExpectedDegreeDistribution();
    vector<double > probabilityDistributionDiameterGivenTies(int minBound, int maxBound);
    vector<double > probabilityDistributionMaxCliqueSizeGivenTies(int minBound, int maxBound);
    vector<double > probabilityDistributionClique3GivenStar2(int minBound, int maxBound);
};
