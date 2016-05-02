#include "ThetaDistribution.h"


#define INF 1000
#define CONST_E  2.7182818284

FactorDistribution::FactorDistribution()
{
    TiesMean = 0, Clique3Mean = 0  , Star2Mean = 0, Star3Mean = 0;
    TiesVar = 100, Clique3Var = 100, Star2Var = 100 , Star3Var = 100;
    TiesMin = -50, Clique3Min = -50 , Star2Min = -50 , Star3Min = -50 ;
    TiesMax = 50, Clique3Max = 50, Star2Max = 50, Star3Max = 50;
}
double FactorDistribution::GenerateSampleNormalDistribution(double mean,double variance,double lowerLimit , double upperLimit)
{
    bool chck = false;

    if(lowerLimit > upperLimit)
    {
        return -1; // Not Possible
    }

    while(!chck)
    {
        //cout<<"Err";
        double u = rand() % INF;
        u -=  INF/2 ;
        u /=  INF/2 ;

        double v = rand() % INF;
        v -=  INF/2 ;
        v /=  INF/2 ;

        //cout<<u<<" "<<v<<endl;

        double w = pow(u, 2) + pow(v, 2);
        if (w < 1.0)
        {
            //cout<<"Err";
            double z = sqrt((-2 * log(w)) / w);
            double res = u*z;
            res *= sqrt(variance);
            res += mean;
            //cout<<res<<" ";
            if(res <= upperLimit && res >= lowerLimit)
            {
                return res;
            }
        }
        //chck = true;
    }
    return mean;
}

vector<double> FactorDistribution::GenerateParametersNormalDistribution()
{
    vector<double> res;
    res.push_back(FactorDistribution::GenerateSampleNormalDistribution(TiesMean,TiesVar,TiesMin,TiesMax));
    res.push_back(FactorDistribution::GenerateSampleNormalDistribution(Clique3Mean,Clique3Var,Clique3Min,Clique3Max));
    res.push_back(FactorDistribution::GenerateSampleNormalDistribution(Star2Mean,Star2Var,Star2Min,Star2Max));
    res.push_back(FactorDistribution::GenerateSampleNormalDistribution(Star3Mean,Star3Var,Star3Min,Star3Max));

    return res;
}

double FactorDistribution::computeProbabilityNormalDistribution(double x , double mean, double variance)
{
    double res = 1.0;
    res /= sqrt(2*M_PI*variance);
    res *= pow(CONST_E , (-1*(x-mean)*(x-mean))/(2*variance));
    return res;
}

double FactorDistribution::computeProbabilityParameters(vector<double> parameters)
{
    double res = 1.0;
    res *= computeProbabilityNormalDistribution(parameters[0],TiesMean,TiesVar);
    res *= computeProbabilityNormalDistribution(parameters[1],Clique3Mean,Clique3Var);
    res *= computeProbabilityNormalDistribution(parameters[2],Star2Mean,Star2Var);
    res *= computeProbabilityNormalDistribution(parameters[3],Star3Mean,Star3Var);
    return res;
}
