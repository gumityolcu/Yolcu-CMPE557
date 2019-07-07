#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <Eigen/Dense>

using namespace std;

int main(int argc, char** argv) {
    if(argc<2)
    {
        cout<<"Run program as : DataAnalyser <mode> ..."<<endl;
        cout<<"Set mode=0 for calculating results for one replication as : DataAnalyser 0 <input_file_name>"<<endl;
        cout<<"Set mode=1 for calculating average of results from one machine as : DataAnalyser 1 <machine_id> <number_of_replications> <N> <M> <W> <m> <IT>"<<endl;
        cout<<"Set mode=2 for calculating average of results from different machines as : DataAnalyser 2 ..."<<endl;
    }
    string args="";
    for(int i=1;i<argc;i++)
    {
        args=args+argv[i];
        args=args+" ";
    }
    istringstream str;
    str.str(args);
    int id,numOfReplications,N,M,W,m,IT;

}