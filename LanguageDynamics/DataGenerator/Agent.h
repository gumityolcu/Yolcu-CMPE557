//
// Created by rehrev on 18.04.2019.
//

#ifndef LANGUAGEDYNAMICS_AGENT_H
#define LANGUAGEDYNAMICS_AGENT_H

#include <Eigen/Dense>
#include <vector>
#include <random>

/*
 * Structure Word is used for keeping track of the words used by the population during the simulation
 * A vector of Words is the dictionary. When memories are updated, the dictionary is updated accordingly.
 */

typedef struct _Word
{
    unsigned int word;
    int freq;
}Word;

class Agent {
public:
    // Association matrix that will be computed at the end of the simulation
    Eigen::MatrixXi A;
    // Boolean indicating if the association matrix is computed. This should be checked before accessing A.
    bool matrixReady=false;

    // Array of M integers holding the number of words in the memory for each meaning
    int* memoryCount;
    // Array of M arrays of m unsigned integers. Unsigned integers represent symbols or words. This is the memory of the agent
    unsigned int** memory;
    // Array of integers holding the index of memory that will be overwritten in the next write operation for each meaning
    int* memoryIndex;

    static std::vector<Word> dictionary;
    // # of meanings
    static int M;
    // # of memory sites per meaning
    static int m;
    // # of distinct words, UINT_MAX for infinity
    static unsigned int W;

    Agent();
    ~Agent();
    // Compute association matrices from current memory
    void generateMatrix();
    // Speak with another agent
    bool speak(Agent& a, std::default_random_engine& rnd);
    // Listen another agent. Notice that we don't need to know anything about the speaker agent, we return the success value, true of false
    bool listen(int meaning, unsigned int s, std::default_random_engine& rnd);
    void updateMemory(int meaning, unsigned int word);
    void makeUpWord(unsigned int* word, std::default_random_engine& rnd);

    static void addToDictionary(unsigned int word)
    {
        // For this function, word is always greater than 0
        bool f=false;
        for(int i=0;((i<dictionary.size())&&(!f));i++)
        {
            // If the word is found in dictionary
            if(dictionary[i].word==word)
            {
                f=true;
                // Increment the frequency
                dictionary[i].freq++;
            }
        }
        // If it is not found in the dictionary
        if(!f)
        {
            // Add it
            Word w;
            w.word=word;
            w.freq=1;
            dictionary.push_back(w);
        }
        return;
    }

    static void removeFromDictionary(unsigned int word)
    {
        // Initially, when the memory is half empty, this function will be called with word=0
        // 0 is not a word but a placeholder for empty memory sites so nothing is done
        if(word==0)
        {
            return;
        }

        bool f=false;
        for(int i=0;((!f)&&(i<dictionary.size()));i++)
        {
            // If the word is found
            if(dictionary[i].word==word)
            {
                f=true;
                // Decrement frequency
                dictionary[i].freq--;
                // If the word is not used
                if(dictionary[i].freq==0)
                {
                    // Delete it
                    dictionary.erase(dictionary.begin()+i);
                }
            }
        }
        return;
    }

    static void setParameters(int MM,int mm, int WW)
    {
        M=MM;m=mm;W=WW;
        dictionary.resize(0);
        return;
    }
};
Eigen::MatrixXd normaliseByRow(Eigen::MatrixXi mat);
#endif //LANGUAGEDYNAMICS_AGENT_H
