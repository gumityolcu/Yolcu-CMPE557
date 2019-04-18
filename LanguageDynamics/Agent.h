//
// Created by rehrev on 18.04.2019.
//

#ifndef LANGUAGEDYNAMICS_AGENT_H
#define LANGUAGEDYNAMICS_AGENT_H

#include <Eigen/Dense>
#include <vector>
#include <random>
#include <time.h>

#define m 20 // Memory size
#define l 2 // Word length
#define M 40// Number of meanings
#define N 20 // Number of agents

typedef struct _Word
{
    std::string word;
    int freq;
}Word;

class Agent {
public:
    Eigen::MatrixXi A;
    bool matrixReady=false;
    int memoryCount[M];
    char memory[M][m][l+1];
    int memoryIndex[M];
    static std::vector<Word> dictionary;
    static int unders;
    void generateMatrix();

    Agent();
    ~Agent();
    bool speak(Agent& a, std::default_random_engine rnd);
    bool listen(int meaning, char* s, std::default_random_engine rnd);
    void updateMemory(int meaning, char* word);
    void makeUpWord(char * word, std::default_random_engine rnd);
    static void addToDictionary(std::string word)
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

    static void removeFromDictionary(std::string word)
    {
        if(word=="")
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
                }
            }
        }
        return;
    }
};


#endif //LANGUAGEDYNAMICS_AGENT_H
