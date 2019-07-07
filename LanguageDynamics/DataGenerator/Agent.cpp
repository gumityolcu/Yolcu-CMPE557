//
// Created by rehrev on 18.04.2019.
//

#include "Agent.h"
#include<iostream>

std::vector<Word> Agent::dictionary;
int Agent::M;
int Agent::m;
unsigned int Agent::W;

Agent::Agent()
{
    this->memoryCount=new int[M];
    this->memoryIndex=new int[M];
    this->memory=new unsigned int*[M];
    for(int i=0;i<M;i++)
    {
        this->memory[i]=new unsigned int[m];
        for(int j=0;j<m;j++)
        {
            // 0 stands for "No word present". Thus 0 is not a word or symbol
            this->memory[i][j]=0;
        }
        this->memoryIndex[i]=0;
        this->memoryCount[i]=0;
    }
}

Agent::~Agent()
{
    delete this->memoryCount;
    delete this->memoryIndex;
    for(int i=0;i<M;i++)
    {
        delete this->memory[i];
    }
    delete this->memory;
}

void Agent::makeUpWord(unsigned int* word, std::default_random_engine& rnd)
{
    // 0 is not a word
    std::uniform_int_distribution<unsigned int> unif(1,Agent::W);
    // ???
    *(word)=unif(rnd);
    return;
}

void Agent::updateMemory(int meaning, unsigned int word)
{
    // Remove the overwritten word, notice that if there is no word to be overwritten (i.e. argument to removeFromDictionary is 0) nothing is done
    removeFromDictionary(this->memory[meaning][this->memoryIndex[meaning]]);
    this->memory[meaning][this->memoryIndex[meaning]]=word;
    addToDictionary(word);
    this->memoryIndex[meaning]=(this->memoryIndex[meaning]+1)%m;

    if(this->memoryCount[meaning]<m)
    {
        this->memoryCount[meaning]++;
    }
    return;
}

bool Agent::speak(Agent& a, std::default_random_engine& rnd)
{
    std::uniform_int_distribution<int> unifM(0,M-1);
    // Select a random meaning
    int meaning=unifM(rnd);
    unsigned int s;

    // If there is no word in memory for the meaning
    if(this->memoryCount[meaning]==0)
    {
        // Choose a random word
        makeUpWord(&s,rnd);
        // Add it to memory regardless of success
        updateMemory(meaning, s);
    }
    else
    {
        // If at least one word is found
        std::uniform_int_distribution<int> unif(0,this->memoryCount[meaning]-1);
        // Select a word at random
        int r=unif(rnd);
        s=this->memory[meaning][r];
    }
    // Learn if communication was successful
    bool ret=a.listen(meaning,s, rnd);
    if(ret)
    {
        // If successful, add the word to memory
        this->updateMemory(meaning,s);
    }

    return ret;
}

bool Agent::listen(int meaning, unsigned int s, std::default_random_engine& rnd)
{
    bool understood=false;
    std::vector<int> list;
    list.resize(0);
    // For each meaning
    for(int i=0;i<M;i++)
    {
        for(int j=0;j<this->memoryCount[i];j++)
        {
            // If the word is found in the memory for that meaning
            if(this->memory[i][j]==s)
            {
                // Add it to the list
                list.push_back(i);
            }
        }
    }
    if(list.size()>0)
    {
        std::uniform_int_distribution<int> unif(0,list.size()-1);
        // Select random meaning from list
        int r=unif(rnd);
        // If selected meaning is the same as the intended meaning, it is a success
        if(list[r]==meaning)
        {
            understood=true;
        }
    }
    // Update memory
    this->updateMemory(meaning,s);
    // Return success information
    return understood;
}

void Agent::generateMatrix()
{
    this->A.resize(M,dictionary.size());
    for(int r=0;r<M;r++)
    {
        for(int c=0;c<dictionary.size();c++)
        {
            int cnt=0;
            unsigned int kelime;
            kelime=dictionary[c].word;
            for(int i=0;i<m;i++)
            {
                if(this->memory[r][i]==kelime)
                {
                    cnt++;
                }
            }
            A(r,c)=cnt;
        }
    }
    this->matrixReady=true;
    return;
}

Eigen::MatrixXd normaliseByRow(Eigen::MatrixXi mat)
{
    Eigen::MatrixXd ret;
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
