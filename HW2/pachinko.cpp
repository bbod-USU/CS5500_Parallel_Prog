#include <time.h>
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
    srand(time(NULL));


    if(rank==0){
        for(int i=0;i<size;++i)cout<<" "<<i<<" ";
        cout<<endl;
        
        data=rand() % 20;
        MPI_Send(&data,1,MPI_INT,0,0,MCW);
    }
    
    while(1){
        MPI_Recv(&data,1,MPI_INT,MPI_ANY_SOURCE,0,MCW,MPI_STATUS_IGNORE);
        if(data<0)break;
        sleep(1);
        for(int i=0;i<rank;++i)cout<<"   ";
        cout<< data <<endl;

        if(data != 0){
            data --;
        }else{
            cout << rank << " Is the Loser" << endl;
            cout << endl << endl; 
            for(int i = 0; i < size; i++){
                data=-1;
                for(int i=0;i<size;++i) MPI_Send(&data,1,MPI_INT,i,0,MCW);
            }
            break;
        }

        dest = rand() % size;
        if(dest == rank){
            dest = dest -1;
        }

        MPI_Send(&data,1,MPI_INT,dest,0,MCW);
    }
    cout<<"rank "<<rank<<" is done.\n";

    MPI_Finalize();

    return 0;
}

