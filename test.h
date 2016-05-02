#include "ergm.h"

/*
    This function prints in format required for plotting curve in Octave.
*/
void printPDFForOctave(vector<double> vec);


/*
    This function tests the most generic form of exponential random Graph
*/
void testTiesDistributionClassicalRandomGraph(int);


/*
    This function tests ties probablity distribution
    such that graph has higher tendency for 2star and low tendency for 3star.

    The graph is concave in nature.
*/
void testTiesDistributionContrainedRandomGraph(int);

/*
    This function tests ties probablity distribution
    of totally random graph

    probability of having x edges is propotional to nCx.
*/
void testTiesDistributionTotallyRandomGraph(int);

/*
    This function tests MaxCliqueSize probablity distribution
    of a constrained graph with high tendency for ties
    and low for clique3

    probability maxcliquesize = 2 is very close to 1
*/
void testMaxCliqueSizeDistributionConstrainedRandomGraph(int);

/*
    This function tests MaxCliqueSize probablity distribution
    of a constrained graph with low tendency for ties
    and low for clique3

    probability maxcliquesize = 2 is very close to 1
*/
void testMaxCliqueSizeDistributionConstrainedRandomGraph1(int);


/*
    This function tests Diameter probablity distribution
    of constrained random graph
*/
void testDiameterDistributionConstrainedRandomGraph(int);

/*
    This function tests clique3 given star2 probablity distribution
    of constrained random graph

    very less triangles as upper bound is 6
*/
void testClique3GivenStar2ProbabilityDistribution(int);


/*
    This function tests Maxclique size given ties probablity distribution
    of constrained random graph with low tendency for triangle

    as number of ties are high maxima at 3.

*/
void testProbabilityDistributionMaxCliqueSizeGivenTies(int);

