//
// Created by rehrev on 18.04.2019.
//

#include "Agent.h"

bool isvowel(char c)
{
    if((c!='a')&&(c!='e')&&(c!='i')&&(c!='o')&&(c!='u')&&(c!='\0'))
    {
        return false;
    }
    return true;
}

bool streq(char* s1, char* s2)
{
    return (strcmp(s1,s2)==0);
}

std::string to_string(char* c)
{
    std::string s="";
    for(int i=0;i<l;i++)
    {
        s=s+c[i];
    }
    if(!isvowel(s[0]))
    {
        int a;
        a=5;
    }
    return s;
}


std::vector<Word> Agent::dictionary;
int Agent::unders=0;

Agent::Agent()
{
    for(int i=0;i<M;i++)
    {
        for(int j=0;j<m;j++)
        {
            strncpy(this->memory[i][j],"",l);
        }
        this->memoryIndex[i]=0;
        this->memoryCount[i]=0;
    }
    this->dictionary.resize(0);
}

Agent::~Agent()
{

}

void Agent::makeUpWord(char* word, std::default_random_engine rnd)
{
    char vowel[]={'a','e','i','o','u'};
    char ot[]={'b','c','d','f','g','h','j','k','l','m','n','p','q','r','s','t','v','w','x','y','z'};
    std::uniform_int_distribution<int> unifV(0,sizeof(vowel)-1);
    std::uniform_int_distribution<int> unifO(0,sizeof(ot)-1);
    for(int i=0;i<l;i++)
    {
        if(i%2==0)
        {
            word[i]=vowel[unifV(rnd)];
        }
        else
        {
            word[i]=ot[unifO(rnd)];
        }
    }
    word[l]='\0';
    return;
}
void Agent::updateMemory(int meaning, char* word)
{
    if(!isvowel(this->memory[meaning][this->memoryIndex[meaning]][0]))
    {
        int x;
        x=5;
        x++;
    }
    removeFromDictionary(to_string(this->memory[meaning][this->memoryIndex[meaning]]));
    strncpy(this->memory[meaning][this->memoryIndex[meaning]],word, l);
    if(!isvowel(word[0]))
    {
        int x=5;
        x++;
    }
    addToDictionary(to_string(word));
    this->memoryIndex[meaning]=(this->memoryIndex[meaning]+1)%m;
    if(this->memoryCount[meaning]<m-1)
    {
        this->memoryCount[meaning]++;
    }
    return;
}

bool Agent::speak(Agent& a, std::default_random_engine rnd)
{
    std::uniform_int_distribution<int> unifM(0,M-1);
    int meaning=unifM(rnd);
    //meaning = 3;
    char s[l+1];
    if(this->memoryCount[meaning]==0)
    {
        makeUpWord(s,rnd);
        if(!isvowel(s[0]))
        {
            int a=4;
            a++;
        }
        updateMemory(meaning, s);
    }
    else
    {
        std::uniform_int_distribution<int> unif(0,this->memoryCount[meaning]-1);
        int r=unif(rnd);
        strncpy(s,this->memory[meaning][r],l);
        if(!isvowel(s[0]))
        {
            int it=0;
            it++;
        }
    }
    if(!isvowel(s[0]))
    {
        int it=0;
        it++;
    }
    bool ret=a.listen(meaning,s, rnd);
    if(ret)
    {
        this->updateMemory(meaning,s);
    }

    for(int a=0;a<dictionary.size();a++)
    {
        if(!isvowel(dictionary[a].word[0]))
        {
            int sss=0;
            sss++;
        }
    }
    return ret;
}

bool Agent::listen(int meaning, char* s, std::default_random_engine rnd)
{
    bool understood=false;
    std::vector<int> prob;
    prob.resize(0);
    for(int i=0;i<M;i++)
    {
        for(int j=0;j<this->memoryCount[i];j++)
        {
            if(streq(this->memory[i][j],s))
            {
                prob.push_back(i);
            }
        }
    }
    if(prob.size()>0)
    {
        std::uniform_int_distribution<int> unif(0,prob.size()-1);
        int r=unif(rnd);//randomise
        if(prob[r]==meaning)
        {
            understood=true;
            unders++;
        }
    }
    if(!isvowel(s[0]))
    {
        int sasd=0;
        sasd++;
    }
    this->updateMemory(meaning,s);
    return understood;
}