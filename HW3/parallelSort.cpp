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
    int data;
    int dest;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MCW, &rank); 
    MPI_Comm_size(MCW, &size); 
    srand(rank);
    int masterArray[size*20];
   

    //if master process create array and scatter:
    if(rank == 0){
        for(int i = 0; i < size * 20; i++){
            masterArray[i] = rand()%100 + 1;
        }
    }

    int subArray[20];
    MPI_Scatter(masterArray, 20, MPI_INT,subArray,20, MPI_INT,0,MCW); 
    vector<int> subVector;
    for(int i = 0; i < 20; i++){
        subVector.push_back(subArray[i]);
    }
    sort(subVector.begin(), subVector.end());
    for(int i = 0; i < 20; i++){
        subArray[i] = subVector[i];
    }

    int mainArray[20*size];
    MPI_Gather(&subArray, 20, MPI_INT, mainArray, 20, MPI_INT, 0, MCW);

    if(rank == 0){
        vector<int> mainVector;
        for(int i = 0; i < 20*size; i++){
            mainVector.push_back(mainArray[i]);
        }
        inplace_merge(mainVector.begin(), mainVector.begin() + 20, mainVector.begin() + 40);
		inplace_merge(mainVector.begin() + 40, mainVector.begin() + 60, mainVector.end());
		inplace_merge(mainVector.begin(), mainVector.begin() + 40, mainVector.end());
        cout << "------Original Array------" << endl;
        for(int i = 0; i < 20*size; i++){
            cout << " " << masterArray[i];
        }
        cout << endl << endl<<"------Sorted Array-----" << endl << endl;
        for(int i = 0; i < 20*size; i++){
            cout <<" " << mainVector[i];
        }
        cout << endl;
        

    }

    MPI_Finalize();

    return 0;
}

