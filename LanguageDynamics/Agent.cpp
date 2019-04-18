//
// Created by rehrev on 18.04.2019.
//

#include "Agent.h"

Agent::Agent()
{
    for(int i=0;i<M;i++)
    {
        for(int j=0;i<m;j++)
        {
            strncpy(this->memory[i][j],"",l);
        }
        this->memoryIndex[i]=0;
    }
    this->words.resize(0);
}

Agent::~Agent()
{

}

void Agent::incrementMemoryIndex(int i)
{
    this->memoryIndex[i]=(this->memoryIndex[i]+1)%m;
}

bool Agent::speak(int meaning, Agent& a)
{
    char s[l+1];
    //select word
    bool ret=a.listen(s,meaning);
    if(ret)
    {
        //update
    }
    return ret;
}

bool Agent::listen(char* s, int meaning)
{
    bool understood;
    //try to understand
    if(understood)
    {
        //update
    }
    else
    {
        //update
    }
    return understood;
}

bool streq(char* s1, char* s2)
{
    return (strcmp(s1,s2)==0);
}
