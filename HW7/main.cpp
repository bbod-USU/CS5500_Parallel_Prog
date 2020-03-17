#include<iostream>
#include<mpi.h>
#include<stdlib.h>
#include<time.h>
#include<queue>
#define MCW MPI_COMM_WORLD
#define GENERIC 0
#define SPECIAL 1
#define JOB 1
#define TOKEN 2

using namespace std;

int main(int argc, char **argv) {
    int rank, size;
    int jobFlag, tokenFlag;
    int tasksCreated = 0;
    int tasksCompleted = 0;
    int taskLimit;
    int tokenValue = SPECIAL;
    int myToken = SPECIAL;
    int newTask;
    bool hasToken = false;
    queue<int> myTasks;
    MPI_Status status;
    MPI_Request request;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MCW, &rank);
    MPI_Comm_size(MCW, &size);

    srand(time(NULL)+rank);
    taskLimit = rand() % 1025 + 1024; //Set the limit to some number between 1024 and 2048

    if(rank == 0) {
        hasToken = true;
        int job = rand() % 3 + 1;
        tasksCreated++;
        MPI_Send(&job, 1, MPI_INT, rand() % 4, JOB, MCW); //Send out the initial job
    }

    while(true) {
        MPI_Iprobe(MPI_ANY_SOURCE, JOB, MCW, &jobFlag, &status); //Check to see if a job is available
        if(jobFlag) {
            MPI_Irecv(&newTask, 1, MPI_INT, MPI_ANY_SOURCE, JOB, MCW, &request);
            myTasks.push(newTask);
        }
        if(newTask == -1) { //If the stop signal is received, send my info to process 0 and break out of the loop
            MPI_Send(&tasksCreated, 1, MPI_INT, 0, size-1, MCW);
            MPI_Send(&tasksCompleted, 1, MPI_INT, 0, size, MCW);
            break;
        }
        if(myTasks.size() > 16) { //If my task queue is greater than size 16, send off 2 tasks
            for(int i = 0; i < 2; i++) {
                int task = myTasks.front();
                myTasks.pop();
                int dest = rand() % 4;
                while(dest == rank) {dest = rand() % size;}
                if(dest < rank) { myToken = GENERIC;}
                MPI_Send(&task, 1, MPI_INT, dest, JOB, MCW);
            }
        }
        if(!myTasks.empty()) {
            int num = myTasks.front();
            myTasks.pop();
            num^3-num^4;
            tasksCompleted++;
        }
        if(tasksCreated < taskLimit) {
            for(int i = 0; i < rand() % size; i++) {
                myTasks.push(rand() % size);
                tasksCreated++;
            }
        }
        if(myTasks.empty()) { //Check the token information
            if(hasToken) {
                string tmp =  "";
                if(tokenValue == GENERIC)
                    tmp = "Generic";
                else
                    tmp = "Special";


                cout << "My Rank: " << rank << "     My token: " << tmp << endl;
                int dest = rank + 1 < size ? rank + 1 : 0;
                if(rank == 0) { tokenValue = SPECIAL;}
                MPI_Send(&tokenValue, 1, MPI_INT, dest, TOKEN, MCW);
                hasToken = false;
                myToken = SPECIAL;
            }
            else {
                int src = rank - 1 >= 0 ? rank - 1 : size - 1;
                MPI_Iprobe(src, TOKEN, MCW, &tokenFlag, &status);
                if(tokenFlag) {
                    MPI_Irecv(&tokenValue, 1, MPI_INT, src, TOKEN, MCW, &request);
                    hasToken = true;
                    if(rank == 0 && tokenValue == SPECIAL) {
                        int globalStop = -1;
                        for(int i = 1; i < size; i++) {
                            MPI_Send(&globalStop, 1, MPI_INT, i, JOB, MCW);
                        }
                        break;
                    }
                    if(myToken == GENERIC) {
                        tokenValue = GENERIC;
                    }
                }
            }
        }
    }
//Print info after run.
    if(rank == 0) {
        int created;
        int completed;
        cout<<"Rank 0 created "<<tasksCreated<<" tasks and completed "<<tasksCompleted<<" tasks"<<endl;
        for(int i = 0; i < size; i++) {
            MPI_Recv(&created, 1, MPI_INT, i, size -1, MCW, MPI_STATUS_IGNORE);
            MPI_Recv(&completed, 1, MPI_INT, i, size, MCW, MPI_STATUS_IGNORE);
            cout<<"Rank "<<i<<" created "<<created<<" tasks and completed "<<completed<<" tasks"<<endl;
            tasksCreated += created;
            tasksCompleted += completed;
        }
        cout<<"Tasks Created: "<<tasksCreated<<endl;
        cout<<"Tasks Completed: "<<tasksCompleted<<endl;
    }


    MPI_Finalize();
    return 0;
}