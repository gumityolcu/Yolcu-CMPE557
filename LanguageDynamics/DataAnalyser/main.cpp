#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <Eigen/Dense>
#include <vector>

using namespace std;

typedef struct SimRes{
    double populationFitness=0;

    struct SimRes operator+(const struct SimRes b)
    {
        struct SimRes ret;
        ret.populationFitness=this->populationFitness+b.populationFitness;
        return ret;
    }

    struct SimRes operator/(int b)
    {
        struct SimRes ret;
        ret.populationFitness=this->populationFitness/b;
        return ret;
    }

}SimulationResults;

typedef struct _Agent{
    Eigen::MatrixXi A;
    Eigen::MatrixXd P; // M x W
    Eigen::MatrixXd Q; // W x M


}Agent;

Eigen::MatrixXd normaliseByRow(Eigen::MatrixXi mat);

SimulationResults mode0(string fname);
SimulationResults mode1(int MID, int numOfReplications, int N, int M, int W, int m, int IT);
double calculatePopulationFitness(vector<Agent>& population);
double calculateFitnessBetweenAgents(Agent& a1, Agent& a2);

int main(int argc, char** argv) {
    if(argc<2)
    {
        cout<<"Run program as : DataAnalyser <mode> ..."<<endl;
        cout<<"Set mode=0 for calculating results for one replication as : DataAnalyser 0 <input_file_name>"<<endl;
        cout<<"Set mode=1 for calculating average of results from one machine as : DataAnalyser 1 <id> <number_of_replications> <N> <M> <W> <m> <IT>"<<endl;
        cout<<"Set mode=2 for calculating average of results from different machines as : DataAnalyser 2 ..."<<endl;
    }
    string args="";
    for(int i=1;i<argc;i++)
    {
        args=args+argv[i];
        args=args+" ";
    }
    istringstream str;
    SimulationResults res;
    str.str(args);
    string inputfilename;
    int mode,id,numOfReplications,N,M,W,m,IT;
    // Parse arguments
    str>>mode;
    switch(mode)
    {
        case 0:
            str>>inputfilename;
            res=mode0(inputfilename);
            break;

        case 1:
            if(argc!=9)
            {
                cout<<"For mode=1 run the program as : DataAnalyser 1 <id> <number_of_replications> <N> <M> <W> <m> <IT>"<<endl;
            }
            else
            {
                str>>id>>numOfReplications>>N>>M>>W>>m>>IT;
                res=mode1(id,numOfReplications,N,M,W,m,IT);
            }
            break;

        case 2:
            break;
    }

    cout<<res.populationFitness;
    return 0;
}

SimulationResults mode0(string fname)
{
    SimulationResults ret;

    ifstream file;
    string line;
    file.open(fname);
    if(!file)
    {
        cout << "File " << fname << " can't be opened!" << endl;
        return ret;
    }
    // Get 'Parameters' line
    getline(file,line);
    int MID,RID,N,M,W,m,IT;
    file>>N>>M>>W>>m>>IT;
    // Get newline char
    getline(file,line);
    // Get 'Dictionary Size' line
    getline(file,line);
    vector<int> dictSize(0);
    for(int i=0;i<IT;i++)
    {
        int s;
        file>>s;
        dictSize.push_back(s);
    }
    int cols=dictSize[dictSize.size()-1];
    vector<Agent> agents(0);
    for(int i=0;i<N;i++)
    {
        // Get newline char
        getline(file,line);
        // Get Agent title
        getline(file,line);
        Agent ag;
        ag.A.resize(M,cols);
        for(int r=0;r<M;r++)
        {
            for(int c=0;c<cols;c++)
            {
                int a;
                file>>a;
                ag.A(r,c)=a;
            }
        }
        ag.P=normaliseByRow(ag.A);
        ag.Q=normaliseByRow(ag.A.transpose());
        agents.push_back(ag);
    }
    /*for(int i=0;i<N;i++)
    {
        cout<<agents[i].Q<<endl;
        cout<<endl;
    }*/

    // WE READ THE DATA, CAN START CALCULATING
    ret.populationFitness=calculatePopulationFitness(agents);

    return ret;
}

SimulationResults mode1(int MID, int numOfReplications, int N, int M, int W, int m, int IT)
{
    SimulationResults ret;
    string filename;
    for(int i=0;i<numOfReplications;i++)
    {
        filename="MID="+to_string(MID)+"|RID="+to_string(i)+"|N="+to_string(N)+"|M="+to_string(M)+"|W="+to_string(W)+"|m="+to_string(m)+"|IT="+to_string(IT);
        ret=ret+mode0(filename);
    }
    ret=ret/numOfReplications;
    return ret;

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

double calculateFitnessBetweenAgents(Agent& a1, Agent& a2)
{
    double fitness=0;
    double sum=0;
    int M=a1.A.rows();
    for(int r=0;r<a1.A.rows();r++)
    {
        double meanfit=0;
        for(int c=0;c<a1.A.cols();c++)
        {
            meanfit=meanfit+a1.P(r,c)*a2.Q(c,r);
        }
        sum=sum+meanfit;
    }
    fitness=sum/(double)M;
    return fitness;
}

double calculatePopulationFitness(vector<Agent>& population)
{
    int N=population.size();
    double fitness=0;
    for(int i=0;i<N;i++)
    {
        for(int j=i+1;j<N;j++)
        {
            double temfit=calculateFitnessBetweenAgents(population[i],population[j]);
            //cout<<"Fitness of "<<i<<" and "<<j<<": "<<temfit<<endl;
            fitness+=temfit;
        }
    }
    fitness=2*(fitness/(N*(N-1)));
    //cout<<"Population fitness : "<<fitness<<endl;
    return fitness;
}