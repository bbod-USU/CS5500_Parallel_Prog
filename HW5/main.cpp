#include <iostream>
#include <random>
#include <mpi.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctime>
#define MCW MPI_COMM_WORLD
using namespace std; 
int main(int argc, char **argv){ int rank, size;
    int data;
    int dest;
    int N = 6;
    MPI_Init(&argc, &argv); MPI_Comm_rank(MCW, &rank); 
    MPI_Comm_size(MCW, &size);

    int slice= N/size;
    int* C=(int*)malloc(sizeof(int) *(N/size));
    bool finished;
    //random number generator 
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist5(1,5); // distribution in range [1, 6]
    
    //initialize array
    int **strip;
    int **world, *worldData;
    if(rank == 0){
        worldData = (int *)malloc(sizeof(int)*N*N);
        world = (int **)malloc(sizeof(int*) * N);
        for(int i = 0; i < N; i++) {
            world[i] = &(worldData[i*N]);
        }

        //Sets up world
        for(int i = 0; i < N; i++){
            for( int j =0; j < N; j++){
                if(dist5(rng) == 1){
                    world[i][j] = 1;  
                }else{
                    world[i][j] = 0;
                }
                cout << world[i][j] << " ";;
            }
            cout << endl;
        }
        cout << endl << endl;
        finished = false;
    }
    MPI_Barrier(MCW);
   


    do{
        //MPI_Bcast (*world, N, MPI_INT, 0, MCW);

        int from = rank * N/size;
        int to = ((rank+1) * N/size);

        //Do Computations.
        for(int i = 0; i < N; i++){
            // Scatter the random numbers to all processes
            MPI_Scatter (worldData, 1, MPI_INT, C, 1, MPI_INT, 0, MCW);
            for(int j = from; j < to; j++){
                cout << "From " << from<<" TO: " << to  <<" process: " << rank <<" j: " << j << " i: " << i<< endl;
                int neighbors = 0;
                //Above 3 cells
               if(i - 1 >= 0)
                    if(world[i-1][j]==1)
                        neighbors++;
                    if(j-1 >= 0)
                        if(world[i-1][j-1]==1)
                            neighbors++;
                    if(j+1 < N)
                        if(world[i-1][j+1]==1)
                            neighbors++;
                //Below 3 Cells
                if(i + 1 < N)
                    if(world[i+1][j]==1)
                        neighbors++;
                    if(j-1 >= 0)
                        if(world[i+1][j-1]==1)
                            neighbors++;
                    if(j+1 < N)
                        if(world[i+1][j+1]==1)
                            neighbors++;
                //To the side of us same row            
                if(j + 1 < N)
                    if(world[i][j+1]==1)
                        neighbors++;
                if(j - 1 >= 0)
                    if(world[i][j-1]==1)
                        neighbors++;
                //If neighbors are to many or few i die.
                if(neighbors > 3 || neighbors <2)
                    C[0]=0;
                //If my neighbors = 2/3 I live, or if I am dead and neighbors = 3 I come alive.
                else if(neighbors == 3){
                    C[0]=1;
                }else{
                    C[0]=world[i][j];
                }
            }
            MPI_Barrier(MCW);
            //Gather all of it back up.
            MPI_Gather (&C[0], N/size, MPI_INT, worldData, N/size, MPI_INT, 0, MCW);
            

        }
        if(rank == 0){
            for(int x = 0; x < N; x++){
                for( int y = 0; y < N; y++){
                    cout << world[x][y] << " ";
                   }
                   cout << endl;
            }
            cout << endl << endl << endl;
            finished = true;
            for( int i = 0; i<N*N; i++){
                if(worldData[i]!=0)
                    finished = false;
            }
        }
    }while(!finished);
    
    MPI_Finalize();
}
