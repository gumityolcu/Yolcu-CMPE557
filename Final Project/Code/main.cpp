#include <iostream>
#include "Agent.h"

#define AVERAGE_IT 10
#define SKIP 1
#define RANGE 60
#define SUCCESS 0.95
#define MODEL 0
using namespace std;
using namespace Eigen;

typedef struct SimRes
{
    double PopulationFitness=0;
    int DictionarySize=0;
    double Distinctiveness=0;
    double Consistency=0;
    double AverageUtilisation=0;
    double MaximumUtilisation=0;
    double MinimumUtilisation=0;
}SimulationResults;


double calculateFitnessBetweenAgents(Agent& a1, Agent& a2);
double calculatePopulationFitness(vector<Agent>& population, int N);
double calculatePopulationDistinctiveness(vector<Agent>& agents, int N);
double calculatePopulationConsistency(vector<Agent>& agents, int N);
SimulationResults runSimulationForIterations(int NN, int MM, int mm, int WW, int IT);
int runSimulationUntilEmergence(int N, int MM, int mm, int WW, int MAX_IT);
vector<int> runSimulationTimeLapse(int N, int MM, int mm, int WW, int IT);
int main()
{
    string matlabM="M=[1:"+to_string(SKIP)+":"+to_string(RANGE)+"];";
    string matlabFit="Fit=[";
    string matlabDict="Dict=[";
    string matlabAvgUtil="AvgUtil=[";
    string matlabMaxUtil="MaxUtil=[";
    string matlabMinUtil="MinUtil=[";
    string matlabDistinct="Distinct=[";
    string matlabConsistent="Consistent=[";
    for(int i=1;i<RANGE+1;i=i+SKIP)
    {
        SimulationResults r,avg;
        for(int j=0;j<AVERAGE_IT;j++)
        {
            r=runSimulationForIterations(20,20,i,INT_MAX,1000);
            avg.MinimumUtilisation+=r.MinimumUtilisation;
            avg.MaximumUtilisation+=r.MaximumUtilisation;
            avg.AverageUtilisation+=r.AverageUtilisation;
            avg.DictionarySize+=r.DictionarySize;
            avg.PopulationFitness+=r.PopulationFitness;
            avg.Distinctiveness+=r.Distinctiveness;
            avg.Consistency+=r.Consistency;
        }
        avg.MinimumUtilisation/=AVERAGE_IT;
        avg.MaximumUtilisation/=AVERAGE_IT;
        avg.AverageUtilisation/=AVERAGE_IT;
        avg.DictionarySize/=AVERAGE_IT;
        avg.PopulationFitness/=AVERAGE_IT;
        avg.Distinctiveness/=AVERAGE_IT;
        avg.Consistency/=AVERAGE_IT;
        cout<<" i: "<<i<<"\tDeleted:"<<Agent::deleted<<"\tFitness:"<<avg.PopulationFitness<<"\tDictionary Size:"<<avg.DictionarySize<<"\tAverage Utilisation:"<<avg.AverageUtilisation<<"\tMaximum Utilisation:"<<avg.MaximumUtilisation<<"\tMinimum Utilisation:"<<avg.MinimumUtilisation<<"\tDistinctiveness:"<<avg.Distinctiveness<<endl<<"\tConsistency:"<<avg.Consistency<<endl;
        Agent::deleted=0;
        matlabFit+=to_string(avg.PopulationFitness);
        matlabDict+=to_string(avg.DictionarySize);
        matlabAvgUtil+=to_string(avg.AverageUtilisation);
        matlabMaxUtil+=to_string(avg.MaximumUtilisation);
        matlabMinUtil+=to_string(avg.MinimumUtilisation);
        matlabDistinct+=to_string(avg.Distinctiveness);
        matlabConsistent+=to_string(avg.Consistency);

        if(i<RANGE)
        {
            matlabFit+=", ";
            matlabDict+=", ";
            matlabAvgUtil+=", ";
            matlabMaxUtil+=", ";
            matlabMinUtil+=", ";
            matlabDistinct+=", ";
            matlabConsistent+=", ";
        }
        else
        {
            matlabFit+="];";
            matlabDict+="];";
            matlabAvgUtil+="];";
            matlabMaxUtil+="];";
            matlabMinUtil+="];";
            matlabDistinct+="];";
            matlabConsistent+="];";
        }
    }
    string matlab=matlabM+'\n'+matlabFit+'\n'+matlabDict+'\n'+matlabAvgUtil+'\n'+matlabMaxUtil+'\n'+matlabMinUtil+'\n'+matlabDistinct+'\n'+matlabConsistent+'\n';
    matlab+="subplot(4,2,1);plot(M,Fit,'-o');\n";
    matlab+="subplot(4,2,2);plot(M,Dict,'-o');\n";
    matlab+="subplot(4,2,3);plot(M,AvgUtil,'-o');\n";
    matlab+="subplot(4,2,4);plot(M,MaxUtil,'-o');\n";
    matlab+="subplot(4,2,5);plot(M,MinUtil,'-o');\n";
    matlab+="subplot(4,2,6);plot(M,Distinct,'-o');\n";
    matlab+="subplot(4,2,7);plot(M,Consistent,'-o');\n";
    cout<<matlab;


    /*string matlabM="M=[1:"+to_string(SKIP)+":"+to_string(RANGE)+"];";
    string matlabN="N=[";


    for(int i=1;i<RANGE+1;i=i+SKIP)
    {
        double iterations=0;
        for(int j=0;j<AVERAGE_IT;j++) {
            int ret;
            ret = runSimulationUntilEmergence(10, 10, i, 150,500);
            iterations += ret;
        }
        iterations/=AVERAGE_IT;
        matlabN+=to_string(iterations);
        cout<<"i: "<<i<<" "<<iterations<<endl;
        if(i<RANGE)
        {
            matlabN+=", ";
        }
        else
        {
            matlabN+="];";
        }
    }
    string matlab=matlabM+'\n'+matlabN+";\n";
    matlab+="subplot(4,2,1);plot(M,Fit,'-o');\n";
    cout<<matlab;*/


    /*for(int i=0;i<15;i++)
    {
        double iterations=0;
        for(int j=0;j<AVERAGE_IT;j++) {
            int ret;
            ret = runSimulationUntilEmergence(10, 10, 30, 150,500);
            cout<<ret<<"   ";
            iterations += ret;
        }
        iterations/=AVERAGE_IT;
        cout<<iterations<<endl;
    }*/


    /*string matlabN="N=[";
    vector<int> ret;
    ret = runSimulationTimeLapse(20, 20, 8,INT_MAX,4000);
    for(int j=0;j<ret.size();j++)
    {
        matlabN+=to_string(ret[j]);
        if(j<ret.size()-1)
        {
            matlabN+=", ";
        }
    }
    matlabN+="];";

    string matlab=matlabN+";\n";
    matlab+="plot(N,'-');\n";
    cout<<matlab;*/


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
        for(int j=i+1;j<N;j++)
        {
            double temfit=calculateFitnessBetweenAgents(population[i],population[j]);
            //cout<<"Fitness of "<<i<<" and "<<j<<": "<<temfit<<endl;
            fitness+=temfit;
        }
    }
    fitness=2*(fitness/(N*(N-1)));
    //cout<<"Population fitness : "<<fitness<<endl;
    return fitness;
}


SimulationResults runSimulationForIterations(int N, int MM, int mm, int WW, int IT)
{
    SimulationResults ret;
    uniform_int_distribution<int> unif(0,N-1);
    default_random_engine rnd;
    srand(time(NULL));
    int seed=rand();
    rnd.seed(seed);
    bool mod=false;
    if(MODEL)
    {
        mod=true;
    }
    Agent::setParameters(MM,mm,WW,mod);
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
        //cout<<"Agent "<<ag<<endl<<agents[ag].A<<endl;
        //cout<<"Dist: "<<agents[ag].calculateDistinctiveness()<<endl;
        //cout<<"Cons: "<<agents[ag].calculateConsistency()<<endl<<endl;
    }
    ret.PopulationFitness=calculatePopulationFitness(agents,N);
    ret.DictionarySize=Agent::dictionary.size();
    ret.Distinctiveness=calculatePopulationDistinctiveness(agents,N);
    ret.Consistency=calculatePopulationConsistency(agents,N);
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
vector<int> runSimulationTimeLapse(int N, int MM, int mm, int WW, int IT)
{
    vector<int> ret;
    uniform_int_distribution<int> unif(0,N-1);
    default_random_engine rnd;
    srand(time(NULL));
    int seed=rand();
    rnd.seed(seed);
    bool mod=false;
    if(MODEL)
    {
        mod=true;
    }
    Agent::setParameters(MM,mm,WW,mod);
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
        ret.push_back(Agent::dictionary.size());
    }
    return ret;
}



int runSimulationUntilEmergence(int N, int MM, int mm, int WW, int MAX_IT)
{
    int IT=0;
    double fit=0;
    uniform_int_distribution<int> unif(0,N-1);
    default_random_engine rnd;
    srand(time(NULL));
    int seed=rand();
    rnd.seed(seed);
    bool mod=false;
    if(MODEL)
    {
        mod=true;
    }
    Agent::setParameters(MM,mm,WW,mod);
    vector<Agent> agents;
    agents.resize(N);
    while((fit<SUCCESS)&&(IT<MAX_IT))
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
        for(int ag=0;ag<agents.size();ag++)
        {
            agents[ag].generateMatrix();
            //cout<<"Agent "<<ag<<endl<<agents[ag].A<<endl;
            //cout<<"Dist: "<<agents[ag].calculateDistinctiveness()<<endl;
            //cout<<"Cons: "<<agents[ag].calculateConsistency()<<endl<<endl;
        }
        fit=calculatePopulationFitness(agents,N);
        IT++;
    }
    if(IT==MAX_IT)
    {
        IT=-1;
    }
    return IT;

}

double calculatePopulationDistinctiveness(vector<Agent>& agents, int N)
{
    double ret=0;
    for(int i=0;i<N;i++)
    {
        ret=ret+agents[i].calculateDistinctiveness();
    }
    return ret/N;

}
double calculatePopulationConsistency(vector<Agent>& agents, int N)
{
    double ret=0;
    for(int i=0;i<N;i++)
    {
        ret=ret+agents[i].calculateConsistency();
    }
    return ret/N;

}