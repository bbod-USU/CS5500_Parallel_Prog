
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
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MCW, &rank); 
    MPI_Comm_size(MCW, &size); 
    data = rank;

    MPI_Gather(&data,1,MPI_INT,bigData,1,MPI_INT,0,MCW);
    if(rank == 0)for(int i=0;i<size;++i)cout<<bigData[i];
    cout<<endl;




    MPI_Finalize();

    return 0;
}

