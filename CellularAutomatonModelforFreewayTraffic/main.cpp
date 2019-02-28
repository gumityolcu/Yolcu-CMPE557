#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <math.h>
#include <time.h>


#define OPEN_BOUNDARY_CUTOFF 3
#define OPEN_BOUNDARY_VELOCITY 0
#define INITIAL_VELOCITY 0


/*
 *
 * PARAMETERS:
 *
 * 1-) int L: Number of sites
 * 2-) bool B: true for periodic boundary conditions, false for open boundary conditions
 * 3-) double p: Random slow down probability
 * 4-) int V: Vmax
 * 5-) double d: Density (Number of Cars/L)
 * 6-) int T: Duration or number of iterations of the simulation
 *
 */
using namespace std;
vector<string> simulate(int L, bool B, double p, int V, double d, int startT, int T);
vector<string> simulate_refined(int L, bool B, int V, double d, int startT, int T);
vector<int> conj_speed(vector<string> tl);
double probability(double x);
int main() {
    //vector<string> out=simulate(100,true,0.1,6,0.17,1000,300);
    //vector<string> out=simulate(100,true,0.3,6,0.12,1000,300);
    //vector<string> out=simulate(100,false,0.3,6,0.12,1000,300);
    //vector<string> out=simulate_refined(100,false,6,0.7,1000,300);
    vector<string> out;
    string s="-";
    while(s[0]!='0')
    {

        out=simulate(12,true,0.1,6,0.6,1000,50);
        s=out[0][0];
    }
    for(int j=0;j<out.size();j++)
    {
        cout<<"t = "<<j<<"\t\t"<<out[j]<<endl;
    }
    conj_speed(out);
    return 0;
}

vector<string> simulate(int L, bool B, double p, int V, double d, int startT, int T)
{
    vector<string> ret;
    srand(time(NULL));
    int seed=rand();
    uniform_real_distribution<double> unifd(0,1);
    default_random_engine rd;
    rd.seed(seed);
    uniform_int_distribution<int> unifi(0,L);
    default_random_engine ri;
    seed=rand();
    ri.seed(seed);
    int N=L*d;
    int reh[L]; // Current road
    int road[L]; // Next road
    if((L<1)||(p<0)||(p>1)||(d<0)||(d>1)||(V<1))
    {
        return ret;
    }
    for(int i=0;i<L;i++)
    {
        reh[i]=-1;//indicates empty site
        road[i]=-1;
    }

    //Initialise reh
    int i=0;
    ri.seed(420);
    while(i<N)
    {
        int rand_i=unifi(ri);
        if(reh[rand_i]==-1)
        {
            reh[rand_i]=INITIAL_VELOCITY;
            i++;
        }
    }

    //Start simulation
    int t=0;
    while(t<startT+T)
    {
        // Perform transformation on road based on reh
        for(i=0;i<L;i++)
        {
            if(reh[i]!=-1)
            {
                int d=1;
                int v=reh[i];
                bool f=false;
                while((i+d<L)&&!f)
                {
                    if(reh[i+d]!=-1)
                    {
                        f=true;
                    }
                    else
                    {
                        d++;
                    }
                }
                if(!f)
                {
                    if(B)
                    {
                        int c=0;
                        while((c<L)&!f)
                        {
                            if(reh[c]==-1)
                            {
                                d++;
                                c++;
                            }
                            else
                            {
                                f=true;
                            }

                        }
                    }
                }


                if(d>v+1)
                {
                    if(v<V)
                    {
                        v++;
                    }
                }
                else
                {
                    if(d<=v)
                    {
                        v=d-1;
                    }
                }
                double rand_d=unifd(rd);
                if(rand_d<p)
                {
                    if(v>0)
                    {
                        v--;
                    }
                }
                reh[i]=v;
                //Update road
                if(i+v>L-1)
                {
                    if(B)
                    {
                        road[(i+v)%L]=v;
                    }
                }
                else
                {
                    road[(i+v)]=v;
                }
                if(v!=0)
                {
                    road[i]=-1;
                }
                if(!B)
                {
                    for (int h = 0; h < OPEN_BOUNDARY_CUTOFF; h++)
                    {
                        road[L - h] = -1;
                    }
                    if(road[0]==-1)
                    {
                        road[0]=OPEN_BOUNDARY_VELOCITY;
                    }
                }
            }

        }
        //Update reh and ret
        string s="";
        for(i=0;i<L;i++)
        {
            if(t>startT)
            {
                if(reh[i]==-1)
                {
                    s=s+"*";
                }
                else
                {
                    s=s+to_string(reh[i]);
                }
            }
            reh[i]=road[i];
        }
        if(s!="")
        {
            ret.push_back(s);
        }
        t++;
    }
    return ret;
}

double probability(double x)
{
    return 4*x*x*x-6*x*x+3*x;
}

vector<string> simulate_refined(int L, bool B, int V, double d, int startT, int T)
{
    vector<string> ret;
    uniform_real_distribution<double> unifd(0,1);
    default_random_engine rd;
    uniform_int_distribution<int> unifi(0,L);
    default_random_engine ri;
    int N=L*d;
    int reh[L]; // Current road
    int road[L]; // Next road
    if((L<1)||(d<0)||(d>1)||(V<1))
    {
        return ret;
    }
    for(int i=0;i<L;i++)
    {
        reh[i]=-1;//indicates empty site
        road[i]=-1;
    }

    //Initialise reh
    int i=0;
    ri.seed(420);
    while(i<N)
    {
        int rand_i=unifi(ri);
        if(reh[rand_i]==-1)
        {
            reh[rand_i]=INITIAL_VELOCITY;
            i++;
        }
    }

    //Start simulation
    int t=0;
    while(t<startT+T)
    {
        // Perform transformation on road based on reh
        for(i=0;i<L;i++)
        {
            if(reh[i]!=-1)
            {
                int d=1;
                int v=reh[i];
                bool f=false;
                double rand_d=unifd(rd);
                if(rand_d<probability(((double)v)/((double)V)))
                {
                    if(v>0)
                    {
                        v--;
                    }
                }
                while((i+d<L)&&!f)
                {
                    if(reh[i+d]!=-1)
                    {
                        f=true;
                    }
                    else
                    {
                        d++;
                    }
                }
                if(!f)
                {
                    if(B)
                    {
                        int c=0;
                        while((c<L)&!f)
                        {
                            if(reh[c]==-1)
                            {
                                d++;
                                c++;
                            }
                            else
                            {
                                f=true;
                            }

                        }
                    }
                }

                if(d>v+1)
                {
                    if(v<V)
                    {
                        v++;
                    }
                }
                else
                {
                    if(d<=v)
                    {
                        v=d-1;
                    }
                }
                reh[i]=v;
                //Update road
                if(i+v>L-1)
                {
                    if(B)
                    {
                        road[(i+v)%L]=v;
                    }
                }
                else
                {
                    road[(i+v)]=v;
                }
                if(v!=0)
                {
                    road[i]=-1;
                }
                if(!B)
                {
                    for (int h = 0; h < OPEN_BOUNDARY_CUTOFF; h++)
                    {
                        road[L - h] = -1;
                    }
                    if(road[0]==-1)
                    {
                        road[0]=OPEN_BOUNDARY_VELOCITY;
                    }
                }
            }

        }
        //Update reh and ret
        string s="";
        for(i=0;i<L;i++)
        {
            if(t>startT)
            {
                if(reh[i]==-1)
                {
                    s=s+".";
                }
                else
                {
                    s=s+to_string(reh[i]);
                }
            }
            reh[i]=road[i];
        }
        if(s!="")
        {
            ret.push_back(s);
        }
        t++;
    }
    return ret;
}

vector<int> conj_speed(vector<string> tl)
{
    vector<pair<int,int>> conjunctions;
    vector<vector<pair<int,int>>> totalconj(0);
    int firstindex=-1;
    int conjcount;
    bool inconj,startcount;
    for(int t=0;t<tl.size();t++)
    {
        conjcount=0;
        inconj=false;
        conjunctions.resize(0);
        if(tl[t][0]=='0'&&tl[t][tl[t].length()-1]=='0')
        {
            startcount=false;
        }
        else
        {
            startcount=true;
        }
        for(int x=0;x<tl[t].length();x++)
        {
            if(tl[t][x]=='0')
            {
                if(startcount)
                {
                    if(!inconj)
                    {
                        conjcount++;
                        conjunctions.resize(conjcount);
                        conjunctions[conjcount-1].first=x;
                        inconj=true;
                    }
                    if(x==tl[t].length()-1)
                    {
                        if(firstindex!=-1)
                        {
                            conjunctions[conjcount-1].second=firstindex;
                        }
                    }
                }
            }
            else
            {
                if(!startcount)
                {
                    startcount=true;
                    firstindex=x-1;
                }
                else
                {
                    if(inconj)
                    {
                        inconj=false;
                        conjunctions[conjcount-1].second=x-1;
                    }
                }
            }
        }
        totalconj.push_back(conjunctions);
    }

    //You've got history in totalconj, calculate speed
    vector<int> ret;
    return ret;



}