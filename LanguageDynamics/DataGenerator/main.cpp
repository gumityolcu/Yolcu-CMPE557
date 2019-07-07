#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include "Agent.h"

using namespace std;
using namespace Eigen;

void runSimulationForIterations(int machineID, int replicationID, int N, int MM, int mm, int WW, int IT, default_random_engine& rnd);

int main(int argc, char** argv)
{
    string args="";
    if(argc != 8)
    {
        cout<<"Run the program as :"<<"DataGenerator <id> <# of replications> <N> <M> <W> <m> <IT>"<<endl;
        return 1;
    }
    for(int i=1;i<argc;i++)
    {
        args=args+argv[i];
        args=args+" ";
    }
    istringstream str;
    str.str(args);
    int id,numOfReplications,N,M,W,m,IT;
    default_random_engine rnd;
    srand(time(NULL));
    int seed=rand();
    rnd.seed(seed);

    // Parse arguments
    str>>id>>numOfReplications>>N>>M>>W>>m>>IT;

    // Start simulation
    for(int i=0;i<numOfReplications;i++)
    {
        runSimulationForIterations(id,i,N,M,W,m,IT,rnd);
    }
    return 0;
}

void runSimulationForIterations(int machineID, int replicationID, int N, int MM, int WW, int mm, int IT, default_random_engine& rnd)
{
    ofstream file;
    string fname;
    uniform_int_distribution<int> unif(0,N-1);

    Agent::setParameters(MM,mm,WW);
    vector<Agent> agents;
    vector<int> dictSize;
    agents.resize(N);
    dictSize.resize(0);

    fname="MID="+to_string(machineID)+"|RID="+to_string(replicationID)+"|N="+to_string(N)+"|M="+to_string(MM)+"|W="+to_string(WW)+"|m="+to_string(mm)+"|IT="+to_string(IT);

    for(int j=0;j<IT;j++)
    {
        // In each iteration, every agent speaks once
        for(int i=0;i<N;i++)
        {
            int k=i;
            // Select a different agent to speak to
            while(k==i)
            {
                k=unif(rnd);
            }
            // Speak
            agents[i].speak(agents[k],rnd);
        }
        //cout<<"% "<<(int)((double)j/IT*100)<<endl;
        dictSize.push_back(Agent::dictionary.size());
    }

    file.open(fname);
    if(!file)
    {
        cout<<"Couldn't open file!"<<endl;
        return;
    }
    file<<"Parameters"<<endl;
    file<<N<<"\t"<<MM<<"\t"<<WW<<"\t"<<mm<<"\t"<<IT<<endl;
    file<<"Dictionary size"<<endl;
    for(int i=0;i<IT-1;i++)
    {
        file<<dictSize[i]<<"\t";
    }
    file<<dictSize[dictSize.size()-1];
    file<<endl;
    for(int ag=0;ag<agents.size();ag++)
    {
        file<<"Agent "<<ag<<endl;
        agents[ag].generateMatrix();
        file<<agents[ag].A<<endl;
    }
    file.close();
    return;
}
