#include <iostream>
#include <mpi.h>

#define MCW MPI_COMM_WORLD
using namespace std;

int main(int argc, char **argv){
    int rank;
    int size;
    int data;

    MPI_Init(&argc, &argv);

    //returns my rank in the MCW.
    MPI_Comm_rank(MCW, &rank);
    //how many things there are in this communicator.
    MPI_Comm_size(MCW, &size);
    if(rank == 0){
        data = rand()%100;
        cout << "Process " << rank << "gerated " << data << "\n";

        MPI_Send(&rank, 1, MPI_INT, (rank+1)%size, 0, MCW);
       
        MPI_Recv(&data, 1, MPI_INT, MPI_ANY_SOURCE, 0, MCW, MPI_STATUS_IGNORE);
    }


        MPI_Recv(&data, 1, MPI_INT, MPI_ANY_SOURCE, 0, MCW, MPI_STATUS_IGNORE);
    cout << "I am " << rank << " of " << size << "; got a message from " << data <<endl;

    MPI_Finalize();
    return 0;
}
