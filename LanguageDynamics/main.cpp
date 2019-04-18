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
    for(int j=0;j<40;j++)
    {
        for(int i=0;i<N;i++)
        {
            int k=i;
            while(k==i)
            {
                k=unif(rnd);
            }
            //i=0;k=1;
            agents[i].speak(agents[k],rnd);
        }
    }
    cout<<Agent::dictionary.size();
    return 0;
}