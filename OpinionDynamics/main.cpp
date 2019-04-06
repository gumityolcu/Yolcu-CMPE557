#include <iostream>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <random>
#include <string>
#include <time.h>

#define D 3 // dimensions
#define N 30 // Grid size = N x N
#define RATE1 0.3
#define RATE2 0.25
#define IT 10000 // number of iterations
#define PIXELSPERCELL 25
#define THETA1 0.1
#define THETA2 0.0001

using namespace Eigen;
using namespace cv;
using namespace std;

typedef Matrix<double,D,1> View;
typedef Matrix<View,N,N> Grid;

uniform_real_distribution<double> unifd(-1,1);
uniform_real_distribution<double> unifp(0,1);
uniform_int_distribution<int> unifi(0,N-1);
default_random_engine rnd;

void Init_Unif_Rnd(Grid& grid);
void interact(Grid& grid, int x, int y);
double distance(View v1, View v2);
void converse(Grid& grid, int x1, int y1, int x2, int y2);
void printGrid(Grid grid);
void show3DGrid(Grid grid, string name);
void show2DGrid(Grid grid, string name);
void show1DGrid(Grid grid, string name);
void showGrid(Grid grid, string name);
int interactions;

int main()
{
    char c=32;
    int t=time(NULL);
    srand(time(NULL));
    int seed=rand();
    rnd.seed(seed);
    Grid grid;
    // Uniform random initialisation
    Init_Unif_Rnd(grid);
    int count=0;
    int iteration=0;
    bool in=true;
    while(in)
    {
        interactions = 0;
        for (int z = 0; z < iteration; z++) {
            for (int i = 0; i < N * N; i++) {
                int x = unifi(rnd);
                int y = unifi(rnd);
                //cout<<"n = "<<count<<endl;
                //cout<<"x = "<<x<<"  y = "<<y<<endl;
                interact(grid, x, y);
            }
            count++;
            cout << count << endl;
        }
        showGrid(grid, "n");
        c = waitKey(0);
        if (c == 32)
        {
            iteration = 1;
        }
        else if (c == 113)
        {
            in=false;
        }
        else if(c==122)
        {
            iteration = 300;
        }
        else if(c==120)
        {
            iteration=5000;
        }
        else
        {
            Yirmibin=20000;
        }
    }
    showGrid(grid, "n");
    cout<<count*N*N<<endl;
    waitKey(0);
    return 0;
}

void show3DGrid(Grid grid, string name)
{
    Mat M(N*PIXELSPERCELL,N*PIXELSPERCELL,CV_8UC3,Scalar(128,128,128));
    namedWindow(name,WINDOW_AUTOSIZE);
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
                for(int k=0;k<PIXELSPERCELL;k++)
                {
                    for(int l=0;l<PIXELSPERCELL;l++)
                    {
                        for(int dim=0;dim<3;dim++)
                        {
                            M.at<Vec3b>(i*PIXELSPERCELL+k,j*PIXELSPERCELL+l)[dim]=(grid(i,j)(dim)+1)*128;
                        }
                    }
                }
        }
    }
    imshow(name, M);
    return;
}

void show2DGrid(Grid grid, string name)
{
    Mat M(N*PIXELSPERCELL,N*PIXELSPERCELL,CV_8UC3,Scalar(128,128,0));
    namedWindow(name,WINDOW_AUTOSIZE);
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            for(int k=0;k<PIXELSPERCELL;k++)
            {
                for(int l=0;l<PIXELSPERCELL;l++)
                {
                    for(int dim=0;dim<D;dim++)
                    {
                        M.at<Vec3b>(i*PIXELSPERCELL+k,j*PIXELSPERCELL+l)[dim]=(grid(i,j)(dim)+1)*128;
                    }
                }
            }
        }
    }
    imshow(name, M);
    return;
}

void show1DGrid(Grid grid, string name)
{
    Mat M(N*PIXELSPERCELL,N*PIXELSPERCELL,CV_8UC1,128);
    namedWindow(name,WINDOW_AUTOSIZE);
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            for(int k=0;k<PIXELSPERCELL;k++)
            {
                for(int l=0;l<PIXELSPERCELL;l++)
                {
                    M.at<char>(i*PIXELSPERCELL+k,j*PIXELSPERCELL+l)=(grid(i,j)(0)+1)*128;
                }
            }
        }
    }
    imshow(name, M);
    return;
}

void printGrid(Grid grid)
{
    for(int x=0;x<N;x++)
    {
        for(int y=0;y<N;y++)
        {
            for(int i=0;i<D;i++)
            {
                //cout<<grid(x,y)(i);
                if(i<D-1)
                {
                    //cout<<",  ";
                }
            }
            if(y<N-1)
            {
                //cout<<" * * * ";
            }
        }
        //cout<<endl;
    }
}

void Init_Unif_Rnd(Grid& grid)
{

    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            for(int k=0;k<D;k++)
            {
                grid(i,j)(k)=unifd(rnd);
            }
        }
    }
    //printGrid(grid);
    return;
}

void showGrid(Grid grid, string name)
{
    int d=D;
    switch(d)
    {
        case 1:
            show1DGrid(grid,name);
            break;

        case 2:
            show2DGrid(grid,name);
            break;

        case 3:
            show3DGrid(grid,name);
            break;

        default:
            break;
    }
    return;
}

double distance(View v1, View v2)
{
    View diff=v1-v2;
    double n=0;
    for(int i=0;i<D;i++)
    {
        n=n+diff(i)*diff(i);
    }
    n=sqrt(n);
    return n;
}

void converse(Grid& grid, int x1, int y1, int x2, int y2)
{
    for(int a=0;a<grid(x1,y1).size();a++)
    {
        //cout<<grid(x1,y1)(a)<<'\t'<<grid(x2,y2)(a)<<endl;
        //cout<<(grid(x1,y1)(a)+1)*128<<'\t'<<(grid(x2,y2)(a)+1)*128<<endl;
    }
    //cout<<endl<<"======="<<endl<<endl;

    grid(x1,y1)=grid(x1,y1)+RATE1*(grid(x2,y2)-grid(x1,y1));
    grid(x2,y2)=grid(x2,y2)-RATE2*(grid(x1,y1)-grid(x2,y2));
    interactions++;
    for(int a=0;a<grid(x1,y1).size();a++)
    {
        //cout<<grid(x1,y1)(a)<<'\t'<<grid(x2,y2)(a)<<endl;
        //cout<<(grid(x1,y1)(a)+1)*128<<'\t'<<(grid(x2,y2)(a)+1)*128<<endl;
    }
    return;
}

void interact(Grid& grid, int x, int y)
{
    //Interacts with boundaries
    double p1,p2,p3,p4;

    if(y<N-1)
    {
        p1=1-distance(grid(x,y),grid(x,y+1))/2;
    }
    else
    {
        p1=-1;
    }
    if(x>0)
    {
        p2=1-distance(grid(x,y),grid(x-1,y))/2;
    }
    else
    {
        p2=-1;
    }
    if(y>0)
    {
        p3=1-distance(grid(x,y),grid(x,y-1))/2;
    }
    else
    {
        p3=-1;
    }
    if(x<N-1)
    {
        p4=1-distance(grid(x,y),grid(x+1,y))/2;
    }
    else
    {
        p4=-1;
    }
    double r=unifp(rnd);
    double p=unifp(rnd);
    if(p<0.25)
    {
        if((r<p1)&&(p1<1-THETA2)&&(p1>THETA1))
        {
            //cout<<"x' = "<<x<<"   y' = "<<y+1<<endl;
            converse(grid,x,y,x,y+1);
        }
    }
   else if(p<0.5)
    {
        if((r<p2)&&(p2<1-THETA2)&&(p2>THETA1))
        {
            //cout<<"x' = "<<x-1<<"   y' = "<<y<<endl;
            converse(grid,x,y,x-1,y);
        }
    }
    else if(p<0.75)
    {
        if((r<p3)&&(p3<1-THETA2)&&(p3>THETA1))
        {
            //cout<<"x' = "<<x<<"   y' = "<<y-1<<endl;
            converse(grid,x,y,x,y-1);
        }
    }
    else
    {
        if ((r < p4)&&(p4<1-THETA2)&&(p4>THETA1))
        {
            //cout<<"x' = "<<x+1<<"   y' = "<<y<<endl;
            converse(grid, x, y, x + 1, y);
        }
    }
    //cout<<"*****"<<endl<<endl;
    return;
}