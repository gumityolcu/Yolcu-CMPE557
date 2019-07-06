#include <iostream>
#include <fstream>
#include "Agent.h"

using namespace std;
using namespace Eigen;

void runSimulationForIterations(int id, int N, int MM, int mm, int WW, int IT);

int main(int argc, char** argv)
{
    string args="";
    if(argc != 7)
    {
        cout<<"Run the program as :"<<"LanguageDynamics <id> <N> <M> <W> <m> <IT>"<<endl;
        return 1;
    }
    for(int i=1;i<argc;i++)
    {
        args=args+argv[i];
        args=args+" ";
    }
    istringstream str;
    str.str(args);
    int id,N,M,W,m,IT;

    // Parse arguments
    str>>id>>N>>M>>W>>m>>IT;

    // Start simulation
    runSimulationForIterations(id,N,M,W,m,IT);
    return 0;
}

void runSimulationForIterations(int id, int N, int MM, int WW, int mm, int IT)
{
    ofstream file;
    string fname;
    uniform_int_distribution<int> unif(0,N-1);
    default_random_engine rnd;
    srand(time(NULL));
    int seed=rand();
    rnd.seed(seed);

    Agent::setParameters(MM,mm,WW);
    vector<Agent> agents;
    vector<int> dictSize;
    agents.resize(N);
    dictSize.resize(0);

    fname="id="+to_string(id)+"|N="+to_string(N)+"|M="+to_string(MM)+"|m="+to_string(mm)+"|W="+to_string(WW)+"|IT="+to_string(IT);

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
        cout<<"% "<<(int)((double)j/IT*100)<<endl;
        dictSize.push_back(Agent::dictionary.size());
    }

    file.open(fname);
    if(!file)
    {
        cout<<"Couldn't open file!"<<endl;
        return;
    }
    file<<"Parameters"<<endl;
    file<<id<<"\t"<<N<<"\t"<<MM<<"\t"<<mm<<"\t"<<WW<<"\t"<<IT<<endl<<endl;
    file<<"Dictionary size"<<endl;
    for(int i=0;i<IT;i++)
    {
        file<<dictSize[i]<<"\t";
    }
    file<<endl<<endl;
    for(int ag=0;ag<agents.size();ag++)
    {
        file<<"Agent "<<ag<<endl;
        agents[ag].generateMatrix();
        file<<agents[ag].A<<endl<<endl;
    }
    file.close();
    return;
}
