#include <iostream>
#include "Agent.h"

using namespace std;
using namespace Eigen;

int main()
{
    uniform_int_distribution<int> unif(0,N-1);
    default_random_engine rnd;
    srand(time(NULL));
    int seed=rand();
    rnd.seed(seed);
    vector<Agent> agents;
    agents.resize(N);
    for(int j=0;j<4000;j++)
    {
        for(int i=0;i<N;i++)
        {
            int k=i;
            while(k==i)
            {
                k=unif(rnd);
            }
            //i=0;k=1;
            cout<<i<<" speaking to "<<k;
            agents[i].speak(agents[k],rnd);
        }
    }
    int avgFreq=0;
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
    cout<<"Min Util: "<<(double)minFreq/(double)(N*m*M)<<endl<<endl<<endl<<endl<<endl;

    for(int ag=0;ag<agents.size();ag++)
    {
        agents[ag].generateMatrix();
        cout<<endl<<agents[ag].A<<endl<<endl;
    }
    return 0;
}