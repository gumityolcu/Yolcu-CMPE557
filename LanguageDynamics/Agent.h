//
// Created by rehrev on 18.04.2019.
//

#ifndef LANGUAGEDYNAMICS_AGENT_H
#define LANGUAGEDYNAMICS_AGENT_H

#include <Eigen/Dense>
#include <vector>

#define m 20 // Memory size
#define l 5 // Word length
#define M 5 // Number of meanings
#define N 10 // Number of agents

class Agent {
    //Eigen::MatrixXi A;
    char memory[M][m][l+1];
    int memoryIndex[M];
    static std::vector<char[l+1]> words;

    Agent();
    ~Agent();
    void incrementMemoryIndex(int i);
    bool speak(int meaning, Agent& a);
    bool listen(char* s, int meaning);
};


#endif //LANGUAGEDYNAMICS_AGENT_H
