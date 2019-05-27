//
// Created by rehrev on 18.04.2019.
//

#include "Agent.h"
#include<iostream>

std::vector<Word> Agent::dictionary;
int Agent::M;
int Agent::m;
unsigned int Agent::W;
bool Agent::Model=false;
int Agent::unders=0;
int Agent::deleted=0;

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
            this->memory[i][j]=0;
        }
        this->memoryIndex[i]=0;
        this->memoryCount[i]=0;
    }
    this->dictionary.resize(0);
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

void Agent::makeUpWord(unsigned int* word, std::default_random_engine rnd)
{
    std::uniform_int_distribution<int> unif(1,Agent::W);
    *(word)=unif(rnd);
    return;
}
void Agent::updateMemory(int meaning, unsigned int word)
{
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

void Agent::deleteFromMemory(int meaning, unsigned int word)
{
    int delIndex=this->memoryIndex[meaning];
    int readIndex=this->memoryIndex[meaning];
    int wrIndex=0;
    int* newMemory=new int[m];
    bool found=false;
    while(!found)
    {
        if(this->memory[meaning][delIndex]==word)
        {
            found=true;
            this->memory[meaning][delIndex]=0;
            removeFromDictionary(word);
            this->memoryCount[meaning]--;
        }
        else
        {
            delIndex=(delIndex+1)%m;
        }
    }
    //Rearrange memory
    for(int i=0;i<m;i++)
    {
        newMemory[i]=0;
    }
    if(this->memory[meaning][readIndex]!=0)
    {
        newMemory[wrIndex]=this->memory[meaning][readIndex];
        wrIndex++;
    }
    readIndex=(readIndex+1)%m;
    while(readIndex!=this->memoryIndex[meaning])
    {
        if(this->memory[meaning][readIndex]!=0)
        {
            newMemory[wrIndex]=this->memory[meaning][readIndex];
            wrIndex++;
        }
        readIndex=(readIndex+1)%m;
    }
    int finalIndex=-1;
    for(int i=0;i<m;i++)
    {
        this->memory[meaning][i]=newMemory[i];
        if(newMemory[i]==0)
        {
            if(finalIndex==-1)
            {
                finalIndex=i;
            }
        }
    }
    this->memoryIndex[meaning]=finalIndex;
    return;
}

bool Agent::speak(Agent& a, std::default_random_engine& rnd)
{
    std::uniform_int_distribution<int> unifM(0,M-1);
    int meaning=unifM(rnd);
    //meaning = 3;
    //std::cout<<" meaning: "<<meaning;
    unsigned int s;
    if(this->memoryCount[meaning]==0)
    {
        makeUpWord(&s,rnd);
        updateMemory(meaning, s);
    }
    else
    {
        std::uniform_int_distribution<int> unif(0,this->memoryCount[meaning]-1);
        int r=unif(rnd);
        s=this->memory[meaning][r];
    }
    //std::cout<<" \""<<s<<"\" ";
    bool ret=a.listen(meaning,s, rnd);
    if(ret)
    {
        //std::cout<<"understood"<<std::endl;
        this->updateMemory(meaning,s);
    }
    else
    {
        //std::cout<<"understoodn't"<<std::endl;
        if(Agent::Model)
        {
            this->deleteFromMemory(meaning,s);
        }
    }

    return ret;
}

bool Agent::listen(int meaning, unsigned int s, std::default_random_engine rnd)
{
    bool understood=false;
    std::vector<int> prob;
    prob.resize(0);
    for(int i=0;i<M;i++)
    {
        for(int j=0;j<this->memoryCount[i];j++)
        {
            if(this->memory[i][j]==s)
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
    this->updateMemory(meaning,s);
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

double Agent::calculateDistinctiveness()
{
    if(!this->matrixReady)
    {
        return 0;
    }
    int count=0;
    Eigen::MatrixXd mat=normaliseByRow(this->A.transpose());
    double cumulP=0;
    for(int c=0;c<mat.rows();c++)
    {
        double maxP=0;
        for(int r=0;r<mat.cols();r++)
        {
            if(mat(c,r)>maxP)
            {
                maxP=mat(c,r);
            }
        }
        if(maxP>0)
        {
            count++;
            cumulP=cumulP+maxP;
        }
    }
    return cumulP/count;
}

double Agent::calculateConsistency()
{
    if(!this->matrixReady)
    {
        return 0;
    }
    int count=0;
    Eigen::MatrixXd mat=normaliseByRow(this->A);
    double cumulP=0;
    for(int r=0;r<mat.rows();r++)
    {
        double maxP=0;
        for(int c=0;c<mat.cols();c++)
        {
            if(mat(r,c)>maxP)
            {
                maxP=mat(r,c);
            }
        }
        if(maxP>0)
        {
            count++;
            cumulP=cumulP+maxP;
        }
    }
    return cumulP/count;
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
