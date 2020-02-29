#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iterator>


//#include "/usr/local/include/mpi.h"
#define MCW MPI_COMM_WORLD

using namespace std;

int main(int argc, char **argv){

    int rank, size;
    int data = [5, 10, 3, 6, 8, 34, 45, 23, 68, 79, 20, 27, 99];
    int arraySize;
    int dest;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MCW, &rank); 
    MPI_Comm_size(MCW, &size); 
    srand(rank);
   
    arraySize = i

    MPI_Finalize();

    return 0;
}


