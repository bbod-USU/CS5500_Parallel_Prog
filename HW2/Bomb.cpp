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
    int dest;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MCW, &rank); 
    MPI_Comm_size(MCW, &size); 
    srand(rank);

    if(rank == 0){
      data = rand % 20;
      cout << "Timer is set to " << data << endl;
    }
    
    while(1) {
      MPI_Recv(&data, 1, MPI_INT, MPI_ANY_SOURCE, 0, MCW, MPI_STATUS_IGNORE);
      dest = rank++;

      if (!(rand() % 5)) {
        data = -1;
        for (int i = 0; i < size; ++i)
          MPI_Send(&data, 1, MPI_INT, i, 0, MCW);
      }

      if (data != 0) {
        data--;
      } else {
        dest = rank - 1;
        if (dest < 0)
          dest = size - 1;
      }

      MPI_Send(&data, 1, MPI_INT, dest, 0, MCW);

        
    cout<<"rank "<<rank<<" is done.\n";

    MPI_Finalize();

    return 0;
}
}

