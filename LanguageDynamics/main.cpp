#include <iostream>
#include "Agent.h"

using namespace std;
using namespace Eigen;

double calculateFitnessBetweenAgents(Agent& a1, Agent& a2);
double calculatePopulationFitness(vector<Agent>& population, int N);
MatrixXd normaliseByRow(MatrixXi mat);
double runSimulation(int NN, int MM, int mm, int WW, int IT);

int main()
{
    string matlab="M=[1:100];F=[";
    for(int i=1;i<101;i++)
    {
        double r=runSimulation(20,50,i,105,4000);
        cout<<"i: "<<i<<"  "<<r<<endl;
        matlab+=to_string(r);
        if(i<100)
        {
            matlab=matlab+", ";
        }
        else
        {
            matlab+="];";
        }
    }
    matlab+="plot(M,F)";
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

double runSimulation(int N, int MM, int mm, int WW, int IT)
{
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
    return calculatePopulationFitness(agents,N);
    /*int avgFreq=0;
    int maxFreq=Agent::dictionary[0].freq;
    int maxind=0, minind=0;
    int minFreq=Agent::dictionary[0].freq;
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
    cout<<"Dict size: "<<Agent::dictionary.size()<<endl;
    cout<<"Average freq: "<<avgFreq<<endl;
    cout<<"Average utilisation: "<<(double)avgFreq/(double)(N*m*M)<<endl;
    cout<<"Max freq: "<<maxFreq<<"  "<<Agent::dictionary[maxind].word<<endl;
    cout<<"Max Utilisation: "<<(double)maxFreq/(double)(N*m*M)<<endl;
    cout<<"Min freq: "<<minFreq<<"  "<<Agent::dictionary[minind].word<<endl;
    cout<<"Min Util: "<<(double)minFreq/(double)(N*m*M)<<endl<<endl<<endl<<endl<<endl;*/
}