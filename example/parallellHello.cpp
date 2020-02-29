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

    //Sends info
        //&rank- location of send buffer.
        //1- how many things in the buffer.
        //MPI_INT- tells what type to turn the data into..
        //(rank+1)%size- return number.
        //0- tag
        //MCW- communicator and other things.
    MPI_Send(&rank, 1, MPI_INT, (rank+1)%size, 0, MCW);
    
    //Recieves information
        //&data where to put the return data.
        //1- how many.
        //MPI_INT- type.
        //MPI_ANY_SOURCE- can come from any process.
        //0- tag.
        //mcw- communicator type
        //MPI_STATUS_IGNORE- 
    MPI_Recv(&data, 1, MPI_INT, MPI_ANY_SOURCE, 0, MCW, MPI_STATUS_IGNORE);

    cout << "I am " << rank << " of " << size << "; got a message from " << data <<endl;

    MPI_Finalize();
    return 0;
}
