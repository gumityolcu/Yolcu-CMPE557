#include <iostream>
#include <vector>
#include <random>
#include <time.h>
#define INITIAL_SIZE 100
#define P 0.8

using namespace std;

vector<vector<bool>> RunElementaryCellularAutomaton(int W, vector<bool> init, int N);
bool C(vector<bool> c, int i);
bool getValue(int W, int b);

int main()
{
    srand(time(NULL));
    uniform_real_distribution<double> unif(0,1);
    default_random_engine re;
    re.seed(rand());
    vector<bool> c;
    vector<vector<bool>> h;
    for(int j=0;j<INITIAL_SIZE;j++)
    {
        if(unif(re)<P)
        {
            c.push_back(true);
        }
        else
        {
            c.push_back(false);
        }
    }
    h=RunElementaryCellularAutomaton(42,c,50);
    for(int i=0;i<h.size();i++)
    {
        for(int j=0;j<h[i].size();j++)
        {
            if(h[i][j])
            {
                cout<<"o";
            }
            else
            {
                cout<<"-";
            }
        }
        cout<<endl;
    }

    return 0;
}

bool C(vector<bool> c, int i)
{
    if((i<0)||(i>c.size()-1))
    {
        return false;
    }
    return c[i];
}

bool getValue(int W, int b)
{
    if(b==3)
    {
        int k=7;
    }
    W=W>>b;
    return ((W&1)==1);
}


vector<vector<bool>> RunElementaryCellularAutomaton(int W, vector<bool> c, int N)
{
    int x, cur=0, next=1,originOfNext, aliveCount=0;
    bool n1,n2,n3;
    vector<vector<bool>> temp(2);
    vector<vector<bool>> ret(0);
    W=W%256;
    if(W==255)
    {
        cout<<"There are no quiescent states. Can't simulate."<<endl;
        return ret;
    }
    if(getValue(W,0))
    {
        W=((W&128)>>7)|((W&64)>>5)|((W&32)>>3)|((W&16)>>1)|((W&8)*2)|((W&4)*8)|((W&2)*32)|((W&1)*128);
        cout<<"Taking 1 as quiescent state (-) or equivalently W="<<W<<endl;
        for(int l=0;l<c.size();l++)
        {
            c[l]=!c[l];
        }
    }
    else
    {
        cout<<"Taking 0 as quiescent state (-)"<<endl;
    }
    ret.push_back(c);
    temp[0]=c;
    temp[1]=c;
    for(int a=0;a<c.size();a++)
    {
        if(c[a])
        {
            aliveCount++;
        }
    }
    for(int i=0;(i<N)&&(aliveCount>0);i++)
    {
        /*for(int u=0;u<temp[cur].size();u++)
        {
            if(temp[cur][u])
            {
                cout<<"o";
            }
            else
            {
                cout<<"-";
            }
        }
         cout<<endl;*/
        originOfNext=0;
        for(int j=-1;j<(signed)temp[cur].size()+1;j++)
        {
            n1=C(temp[cur],j-1);
            n2=C(temp[cur],j);
            n3=C(temp[cur],j+1);

            x=0;

            if(n1)
            {
                x=x+4;
            }
            if(n2)
            {
                x=x+2;
            }
            if(n3)
            {
                x=x+1;
            }
            if((j>-1)&&(j<temp[cur].size()))
            {
                bool old, n;
                old=temp[cur][j];
                n=getValue(W,x);
                if(!old&&n)
                {
                    aliveCount++;
                }
                else if(old&&!n)
                {
                    aliveCount--;
                }
                temp[next][originOfNext+j]=n;
            }
            else if(j==temp[cur].size())
            {
                if(getValue(W,x))
                {
                    for(int k=0;k<ret.size();k++)
                    {
                        ret[k].push_back(false);
                    }
                    temp[next].push_back(getValue(W,x));
                    aliveCount++;
                }
            }
            else
            {
                if(getValue(W,x))
                {
                    for(int k=0;k<ret.size();k++)
                    {
                        ret[k].insert(ret[k].begin(),false);
                    }
                    temp[next].insert(temp[next].begin(),getValue(W,x));
                    aliveCount++;
                    originOfNext++;
                }
            }
        }
        ret.push_back(temp[next]);
        temp[cur].resize(temp[next].size());
        cur=1-cur;
        next=1-next;
    }
    return ret;
}
