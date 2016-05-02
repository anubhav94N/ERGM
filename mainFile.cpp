#include "test.h"
#include "augmentation_test.h"
#include "ModelPredict.h"

int main()
{
    srand(time(NULL));

    /*
        - 20% graph sampled
        - 100% edges present in the sampled graph
    */
    //testForRandomGraph1();

    /*
        - 20% graph sampled
        - 50% edges present in the sampled graph
    */
    //testForRandomGraph2();

    /*
        - 50% graph sampled
        - 50% edges present in the sampled graph
    */
    //testForRandomGraph3();

    /*
        - All but one edge of the graph have been observed.
        - 50% edges present in the sampled graph.
    */
    //testForSpecialGraph();



    /*
        This function tests the most generic form of exponential random Graph
    */
    //testTiesDistributionClassicalRandomGraph(40);


    /*
        This function tests ties probablity distribution
        such that graph has higher tendency for 2star and low tendency for 3star.

        The graph is concave in nature.
    */
    //testTiesDistributionContrainedRandomGraph(40);

    /*
        This function tests ties probablity distribution
        of totally random graph

        probability of having x edges is propotional to nCx.
    */
    //testTiesDistributionTotallyRandomGraph(40);

    /*
        This function tests MaxCliqueSize probablity distribution
        of a constrained graph with high tendency for ties
        and low for clique3

        probability maxcliquesize = 2 is very close to 1
    */
    //testMaxCliqueSizeDistributionConstrainedRandomGraph(40);

    /*
        This function tests MaxCliqueSize probablity distribution
        of a constrained graph with low tendency for ties
        and low for clique3

        probability maxcliquesize = 2 is very close to 1
    */
    testMaxCliqueSizeDistributionConstrainedRandomGraph1(40);


    /*
        This function tests Diameter probablity distribution
        of constrained random graph
    */
    //testDiameterDistributionConstrainedRandomGraph(40);

    /*
        This function tests clique3 given star2 probablity distribution
        of constrained random graph

        very less triangles as upper bound is 6
    */
    //testClique3GivenStar2ProbabilityDistribution(40);


    /*
        This function tests Maxclique size given ties probablity distribution
        of constrained random graph with low tendency for triangle

        as number of ties are high maxima at 3.
    */
    //testProbabilityDistributionMaxCliqueSizeGivenTies(40);



/*

    ERGMPredict testing(10);
    //cout<<"Done\n";
    testing.getParametersSamplesMCMC(100);
    //cout<<"Done\n";
    //testing.printParametersSamples();
    vector<double> chck = testing.predictParameters();
    cout<<"Ties: "<<chck[0]<<"\nStar2: "<<chck[2]<<"\nStar3: "<<chck[3]<<"\nTriangle: "<<chck[1]<<endl;
*/

}
