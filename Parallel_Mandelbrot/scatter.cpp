

#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <stdlib.h>

//#include "/usr/local/include/mpi.h"
#define MCW MPI_COMM_WORLD

using namespace std;

int main(int argc, char **argv){

    int rank, size;
    int data;
    int bigData[16];
    cout << rank;
    if(!rank)cout << endl << rank;for(int i=0;i<16;++i)bigData[i]=i;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MCW, &rank); 
    MPI_Comm_size(MCW, &size); 

    MPI_Scatter(bigData,1,MPI_INT,&data,1,MPI_INT,0,MCW);
    cout<<"rank"<<rank<<" "<<data<<endl;




    MPI_Finalize();

    return 0;
}

