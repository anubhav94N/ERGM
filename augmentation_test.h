#include "ergm.h"
#pragma once


/*
    This function prints K-most frequent graphs.
    Uses "mapGraphToFrequency"
*/
void printKMostFrequentGraphs();

void printObservedGraph(vector<vector<int> >);

/*
    Generates a random graph with edge probability selection probability equal to p.
    This is used as sample to pass to data augmentation function.
*/
vector<vector<int> > generateRandomSample(int, int, int);

vector<vector<int> > generateSampleWithTwoUnobservedTies(int,int);

/*
    - 20% graph sampled
    - 100% edges present in the sampled graph
*/
void testForRandomGraph1();
/*
    - 20% graph sampled
    - 50% edges present in the sampled graph
*/
void testForRandomGraph2();

/*
    - 50% graph sampled
    - 50% edges present in the sampled graph
*/
void testForRandomGraph3();

/*
    - All but one edge of the graph have been observed.
    - 50% edges present in the sampled graph.
*/
void testForSpecialGraph();

