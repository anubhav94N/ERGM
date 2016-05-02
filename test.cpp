#include "test.h"

/*
    This function prints in format required for plotting curve in Octave.
*/
void printPDFForOctave(vector<double> vec)
{
    /*
    freopen("out.m","w",stdout);
    cout<<"x = [ ";
    for(int i=0;i<vec.size();i++)
    {
        if(vec[i] != 0)
        {
            cout<<i<<", ";
        }
    }
    cout<<"];\n";

    cout<<"y = [ ";
    for(int i=0;i<vec.size();i++)
    {
        if(vec[i] != 0)
        {
            cout<<vec[i]<<", ";
        }
    }
    cout<<"];\n";
    fclose(stdout);
    */

    freopen("out.m","w",stdout);
    for(int i=0;i<vec.size();i++)
    {
        if(vec[i] != 0)
        {
            cout<<"("<<i<<","<<vec[i]<<")";
        }
    }
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
    This function tests the most generic form of exponential random Graph
*/
void testTiesDistributionClassicalRandomGraph(int N)
{
    ERGM ergm(N);
    ergm.ties.factor = 1;
    ergm.star2.factor = 1;
    ergm.star3.factor = -1;
    ergm.clique3.factor = 1;
    ergm.getGraphSamples();
    ergm.removeRepetition(ergm.graphSamples);

    /*ergm.getGraphSamplesMCMC(20000);
    cerr<<ergm.graphSamples.size()<<endl;
*/
    vector<double > vec;
    vec = ergm.computeTiesProbabilityDistribution();

    printPDFForOctave(vec);
}

/*
    This function tests ties probablity distribution
    such that graph has higher tendency for 2star and low tendency for 3star.

    The graph is concave in nature.
*/
void testTiesDistributionContrainedRandomGraph(int N)
{
    ERGM ergm(N);
    ergm.ties.factor = 0;
    ergm.star2.factor = 90;
    ergm.star3.factor = -100;
    ergm.clique3.factor = 0;

    ergm.getGraphSamples();
    ergm.removeRepetition(ergm.graphSamples);

    /*ergm.getGraphSamplesMCMC(20000);
    cerr<<ergm.graphSamples.size()<<endl;
*/
    vector<double > vec;

    vec = ergm.computeTiesProbabilityDistribution();

    printPDFForOctave(vec);
}

/*
    This function tests ties probablity distribution
    of totally random graph

    probability of having x edges is propotional to nCx.
*/
void testTiesDistributionTotallyRandomGraph(int N)
{
    ERGM ergm(N);
    ergm.ties.factor = 1;
    ergm.star2.factor = 1;
    ergm.star3.factor = 1;
    ergm.clique3.factor = 1;
    /*
    ergm.getGraphSamples();
    ergm.removeRepetition(ergm.graphSamples);
    */
    ergm.getGraphSamplesMCMC(20000);

    vector<double > vec;
    vec = ergm.computeTiesProbabilityDistribution();

    printPDFForOctave(vec);
}

/*
    This function tests MaxCliqueSize probablity distribution
    of a constrained graph with high tendency for ties
    and low for clique3

    probability maxcliquesize = 2 is very close to 1
*/
void testMaxCliqueSizeDistributionConstrainedRandomGraph(int N)
{
    ERGM ergm(N);
    ergm.ties.factor = 1;
    ergm.star2.factor = 0;
    ergm.star3.factor = 0;
    ergm.clique3.factor = -10000;
    ergm.getGraphSamples();
    //ergm.getGraphSamplesMCMC(10);
    ergm.removeRepetition(ergm.graphSamples);

    vector<double > vec;
    vec = ergm.computeMaxCliqueSizeProbabilityDistribution();

    printPDFForOctave(vec);
}

/*
    This function tests MaxCliqueSize probablity distribution
    of a constrained graph with low tendency for ties
    and low for clique3

    probability maxcliquesize = 2 is very close to 1
*/
void testMaxCliqueSizeDistributionConstrainedRandomGraph1(int N)
{
    ERGM ergm(N);
    ergm.ties.factor = -1000;
    ergm.star2.factor = 0;
    ergm.star3.factor = 0;
    ergm.clique3.factor = -10000;
    ergm.getGraphSamples();
    //ergm.getGraphSamplesMCMC(10);
    ergm.removeRepetition(ergm.graphSamples);

    vector<double > vec;
    vec = ergm.computeMaxCliqueSizeProbabilityDistribution();

    printPDFForOctave(vec);
}


/*
    This function tests Diameter probablity distribution
    of constrained random graph
*/
void testDiameterDistributionConstrainedRandomGraph(int N)
{
    ERGM ergm(N);
    ergm.ties.factor = 1;
    ergm.star2.factor = 1;
    ergm.star3.factor = 1;
    ergm.clique3.factor = -10000000000;
    ergm.getGraphSamples();
    //ergm.getGraphSamplesMCMC(10);
    ergm.removeRepetition(ergm.graphSamples);

    vector<double > vec;
    vec = ergm.computeDiameterProbabilityDistribution();

    printPDFForOctave(vec);
}

/*
    This function tests clique3 given star2 probablity distribution
    of constrained random graph

    very less triangles as upper bound is 6
*/
void testClique3GivenStar2ProbabilityDistribution(int N)
{
    ERGM ergm(N);
    ergm.ties.factor = 1;
    ergm.star2.factor = 1;
    ergm.star3.factor = 1;
    ergm.clique3.factor = 1;
    ergm.getGraphSamples();
    //ergm.getGraphSamplesMCMC(10);
    ergm.removeRepetition(ergm.graphSamples);

    vector<double > vec;
    vec = ergm.probabilityDistributionClique3GivenStar2(0,20);

    printPDFForOctave(vec);
}

/*
    This function tests Maxclique size given ties probablity distribution
    of constrained random graph with low tendency for triangle

    as number of ties are high maxima at 3.

*/
void testProbabilityDistributionMaxCliqueSizeGivenTies(int N)
{
    ERGM ergm(N);
    ergm.ties.factor = 1;
    ergm.star2.factor = 1;
    ergm.star3.factor = 1;
    ergm.clique3.factor = -10000;
    ergm.getGraphSamples();
    //ergm.getGraphSamplesMCMC(10);
    ergm.removeRepetition(ergm.graphSamples);

    vector<double > vec;
    vec = ergm.probabilityDistributionMaxCliqueSizeGivenTies(100,200);

    printPDFForOctave(vec);
}

