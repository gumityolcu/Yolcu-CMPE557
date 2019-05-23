#include <iostream>
#include "Agent.h"

#define AVERAGE_IT 1
#define SKIP 1
#define RANGE 100
using namespace std;
using namespace Eigen;

typedef struct SimRes
{
    double PopulationFitness=0;
    int DictionarySize=0;
    double AverageUtilisation=0;
    double MaximumUtilisation=0;
    double MinimumUtilisation=0;
}SimulationResults;

double calculateFitnessBetweenAgents(Agent& a1, Agent& a2);
double calculatePopulationFitness(vector<Agent>& population, int N);
MatrixXd normaliseByRow(MatrixXi mat);
SimulationResults runSimulation(int NN, int MM, int mm, int WW, int IT);

int main()
{
    string matlabM="M=[1:"+to_string(SKIP)+":"+to_string(RANGE)+"];";
    string matlabFit="Fit=[";
    string matlabDict="Dict=[";
    string matlabAvgUtil="AvgUtil=[";
    string matlabMaxUtil="MaxUtil=[";
    string matlabMinUtil="MinUtil=[";
    for(int i=1;i<RANGE+1;i=i+SKIP)
    {
        SimulationResults r,avg;
        for(int j=0;j<AVERAGE_IT;j++)
        {
            r=runSimulation(10,50,i,1000,4000);
            avg.MinimumUtilisation+=r.MinimumUtilisation;
            avg.MaximumUtilisation+=r.MaximumUtilisation;
            avg.AverageUtilisation+=r.AverageUtilisation;
            avg.DictionarySize+=r.DictionarySize;
            avg.PopulationFitness+=r.PopulationFitness;
        }
        avg.MinimumUtilisation/=AVERAGE_IT;
        avg.MaximumUtilisation/=AVERAGE_IT;
        avg.AverageUtilisation/=AVERAGE_IT;
        avg.DictionarySize/=AVERAGE_IT;
        avg.PopulationFitness/=AVERAGE_IT;
        cout<<Agent::deleted<<" i: "<<i<<"\tFitness:"<<avg.PopulationFitness<<"\tDictionary Size:"<<avg.DictionarySize<<"\tAverage Utilisation:"<<avg.AverageUtilisation<<"\tMaximum Utilisation:"<<avg.MaximumUtilisation<<"\tMinimum Utilisation:"<<avg.MinimumUtilisation<<endl;
        Agent::deleted=0;
        matlabFit+=to_string(avg.PopulationFitness);
        matlabDict+=to_string(avg.DictionarySize);
        matlabAvgUtil+=to_string(avg.AverageUtilisation);
        matlabMaxUtil+=to_string(avg.MaximumUtilisation);
        matlabMinUtil+=to_string(avg.MinimumUtilisation);
        if(i<RANGE)
        {
            matlabFit=matlabFit+", ";
            matlabDict=matlabDict+", ";
            matlabAvgUtil=matlabAvgUtil+", ";
            matlabMaxUtil=matlabMaxUtil+", ";
            matlabMinUtil=matlabMinUtil+", ";
        }
        else
        {
            matlabFit+="];";
            matlabDict+="];";
            matlabAvgUtil+="];";
            matlabMaxUtil+="];";
            matlabMinUtil+="];";
        }
    }
    string matlab=matlabM+'\n'+matlabFit+'\n'+matlabDict+'\n'+matlabAvgUtil+'\n'+matlabMaxUtil+'\n'+matlabMinUtil+'\n';
    matlab+="subplot(3,2,1);plot(M,Fit,'-o');\n";
    matlab+="subplot(3,2,2);plot(M,Dict,'-o');\n";
    matlab+="subplot(3,2,3);plot(M,AvgUtil,'-o');\n";
    matlab+="subplot(3,2,4);plot(M,MaxUtil,'-o');\n";
    matlab+="subplot(3,2,5);plot(M,MinUtil,'-o');\n";
    cout<<matlab;
    return 0;
}

double calculateFitnessBetweenAgents(Agent& a1, Agent& a2)
{
    double fitness=0;
    if((!a1.matrixReady)||(!a2.matrixReady))
    {
        return fitness;
    }
    MatrixXd a1E,a2C;
    a1E=normaliseByRow(a1.A);
    //cout<<a1.A<<endl<<endl<<endl<<a2.A<<endl<<endl;
    a2C=normaliseByRow(a2.A.transpose());
    double sum=0;
    for(int r=0;r<a1.A.rows();r++)
    {
        double meanfit=0;
        for(int c=0;c<a1.A.cols();c++)
        {
            meanfit=meanfit+a1E(r,c)*a2C(c,r);
        }
        sum=sum+meanfit;
    }
    fitness=sum/(double)Agent::M;
    return fitness;
}

double calculatePopulationFitness(vector<Agent>& population, int N)
{
    double fitness=0;
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            if(i!=j)
            {
                fitness+=calculateFitnessBetweenAgents(population[i],population[j]);
            }
        }
    }
    fitness=fitness/(N*(N-1));
    return fitness;
}

MatrixXd normaliseByRow(MatrixXi mat)
{
    MatrixXd ret;
    ret.resize(mat.rows(),mat.cols());
    for(int r=0;r<mat.rows();r++)
    {
        int rowsum=0;
        for(int c=0;c<mat.cols();c++)
        {
            rowsum=rowsum+mat(r,c);
        }
        if(rowsum!=0)
        {
            for(int c=0;c<mat.cols();c++)
            {
                ret(r,c)=(double)mat(r,c)/(double)rowsum;
            }
        }
    }
    return ret;
}

SimulationResults runSimulation(int N, int MM, int mm, int WW, int IT)
{
    SimulationResults ret;
    uniform_int_distribution<int> unif(0,N-1);
    default_random_engine rnd;
    srand(time(NULL));
    int seed=rand();
    rnd.seed(seed);
    Agent::setParameters(MM,mm,WW);
    vector<Agent> agents;
    agents.resize(N);
    for(int j=0;j<IT;j++)
    {
        for(int i=0;i<N;i++)
        {
            int k=i;
            while(k==i)
            {
                k=unif(rnd);
            }
            //i=0;k=1;
            //cout<<i<<" speaking to "<<k;
            agents[i].speak(agents[k],rnd);
        }
    }

    for(int ag=0;ag<agents.size();ag++)
    {
        agents[ag].generateMatrix();
        //cout<<endl<<agents[ag].A<<endl<<endl;
    }
    ret.PopulationFitness=calculatePopulationFitness(agents,N);
    ret.DictionarySize=Agent::dictionary.size();
    double avgFreq=0;
    double maxFreq=Agent::dictionary[0].freq;
    int maxind=0, minind=0;
    double minFreq=Agent::dictionary[0].freq;
    for(int i=0;i<Agent::dictionary.size();i++)
    {
        int freq=Agent::dictionary[i].freq;
        if(freq>maxFreq)
        {
            maxFreq=freq;
            maxind=i;
        }
        else if(freq<minFreq)
        {
            minind=i;
            minFreq=freq;
        }
        avgFreq+=freq;
    }
    avgFreq/=Agent::dictionary.size();
    /*cout<<"Dict size: "<<Agent::dictionary.size()<<endl;
    cout<<"Average freq: "<<avgFreq<<endl;
    cout<<"Average utilisation: "<<(double)avgFreq/(double)(N*mm*MM)<<endl;
    cout<<"Max freq: "<<maxFreq<<"  "<<Agent::dictionary[maxind].word<<endl;
    cout<<"Max Utilisation: "<<(double)maxFreq/(double)(N*mm*MM)<<endl;
    cout<<"Min freq: "<<minFreq<<"  "<<Agent::dictionary[minind].word<<endl;
    cout<<"Min Util: "<<(double)minFreq/(double)(N*mm*MM)<<endl<<endl<<endl<<endl<<endl;*/
    ret.AverageUtilisation=avgFreq/(double)(N*mm);
    ret.MaximumUtilisation=maxFreq/(double)(N*mm);
    ret.MinimumUtilisation=minFreq/(double)(N*mm);
    return ret;
}