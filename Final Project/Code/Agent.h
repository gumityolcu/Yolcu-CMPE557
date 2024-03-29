//
// Created by rehrev on 18.04.2019.
//

#ifndef LANGUAGEDYNAMICS_AGENT_H
#define LANGUAGEDYNAMICS_AGENT_H

#include <Eigen/Dense>
#include <vector>
#include <random>
#include <time.h>
//#define m 1// Memory size
//#define l 2 // Word length
//#define M 50// Number of meanings


typedef struct _Word
{
    unsigned int word;
    int freq;
}Word;

class Agent {
public:
    Eigen::MatrixXi A;
    bool matrixReady=false;
    int* memoryCount;
    unsigned int** memory;
    int* memoryIndex;
    static std::vector<Word> dictionary;
    static int unders;
    static int M;
    static int m;
    static int deleted;
    static unsigned int W;
    static bool Model;

    void generateMatrix();
    Agent();
    ~Agent();
    bool speak(Agent& a, std::default_random_engine& rnd);
    int listen(int meaning, unsigned int s, std::default_random_engine rnd);
    void updateMemory(int meaning, unsigned int word);
    void deleteFromMemory(int meaning, unsigned int word);
    void makeUpWord(unsigned int* word, std::default_random_engine rnd);
    double calculateDistinctiveness();
    double calculateConsistency();
    static void addToDictionary(unsigned int word)
    {
        bool f=false;
        for(int i=0;((i<dictionary.size())&&(!f));i++)
        {
            if(dictionary[i].word==word)
            {
                f=true;
                dictionary[i].freq++;
            }
        }
        if(!f)
        {
            Word w;
            w.word=word;
            w.freq=1;
            dictionary.push_back(w);
        }
        return;
    }
    static void removeFromDictionary(unsigned int word)
    {
        if(word==0)
        {
            return;
        }
        bool f=false;
        for(int i=0;((!f)&&(i<dictionary.size()));i++)
        {
            if(dictionary[i].word==word)
            {
                f=true;
                dictionary[i].freq--;
                if(dictionary[i].freq==0)
                {
                    dictionary.erase(dictionary.begin()+i);
                    Agent::deleted++;
                }
            }
        }
        return;
    }

    static void setParameters(int MM,int mm, int WW, bool model)
    {
        M=MM;m=mm;W=WW;Model=model;
        dictionary.resize(0);
        return;
    }
};

Eigen::MatrixXd normaliseByRow(Eigen::MatrixXi mat);
#endif //LANGUAGEDYNAMICS_AGENT_H
