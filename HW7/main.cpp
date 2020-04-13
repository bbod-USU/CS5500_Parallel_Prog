#include<iostream>
#include<mpi.h>
#include<cstdlib>
#include<ctime>
#include<queue>

#define MCW MPI_COMM_WORLD
#define GENERIC 0
#define SPECIAL 1
#define JOB 1
#define ALLSTOP (-1)
#define TOKEN 2

using namespace std;

int main(int argc, char **argv) {
    int rank, size;
    int jobFlag = 0;
    int tokenFlag = 0;
    int tasksCreated = 0;
    int tasksCompleted = 0;
    int taskLimit;
    int tokenValue = SPECIAL;
    int myToken = SPECIAL;
    int newTask = 0;
    bool hasToken = false;
    queue<int> myTasks;
    MPI_Status status;
    MPI_Request request;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MCW, &rank);
    MPI_Comm_size(MCW, &size);

    srand(static_cast<unsigned int>(time(nullptr) + rank));
    taskLimit = rand() % 1025 + 1024;

    if (!rank) {
        hasToken = true;
        int job = rand() % size;
        tasksCreated++;
        MPI_Send(&job, 1, MPI_INT, rand() % size, JOB, MCW);
    }

    while (true) {
        MPI_Iprobe(MPI_ANY_SOURCE, JOB, MCW, &jobFlag, &status);
        if (jobFlag == 1) {
            MPI_Irecv(&newTask, 1, MPI_INT, MPI_ANY_SOURCE, JOB, MCW, &request);
            myTasks.push(newTask);
        }
        if (newTask == ALLSTOP) {
            MPI_Send(&tasksCreated, 1, MPI_INT, 0, size - 1, MCW);
            MPI_Send(&tasksCompleted, 1, MPI_INT, 0, size, MCW);
            break;
        }
        if (myTasks.size() > 16) {
            for (int i = 0; i < 2; i++) {
                int task = myTasks.front();
                myTasks.pop();
                int dest = rank;
                while (dest == rank) { dest = rand() % size; }
                if (dest < rank) { myToken = GENERIC; }
                MPI_Send(&task, 1, MPI_INT, dest, JOB, MCW);
            }
        }
        if (!myTasks.empty()) {
          int num;
          num = myTasks.front();
          myTasks.pop();
            num ^ 3 - num ^ 4;
            tasksCompleted++;
        }
        if (tasksCreated < taskLimit) {
            for (int i = 0; i < rand() % size; i++) {
                myTasks.push(rand() % size);
                tasksCreated++;
            }
        }
        if (myTasks.empty()) {
            if (hasToken) {
                string tmp;
                if (tokenValue == GENERIC)
                    tmp = "Generic";
                else
                    tmp = "Special";


                cout << "My Rank: " << rank << "  My token: " << tmp << endl;
                int dest = rank + 1 < size ? rank + 1 : 0;
                if (!rank)
                  tokenValue = SPECIAL;
                MPI_Send(&tokenValue, 1, MPI_INT, dest, TOKEN, MCW);
                hasToken = false;
                myToken = SPECIAL;
            } else {
                int src = 0;
                if(rank - 1 >= 0)
                    src = rank - 1;
                else
                    src = size - 1;
                MPI_Iprobe(src, TOKEN, MCW, &tokenFlag, &status);
                if (tokenFlag == 1) {
                    MPI_Irecv(&tokenValue, 1, MPI_INT, src, TOKEN, MCW, &request);
                    hasToken = true;
                    if (!rank && tokenValue) {
                        int stop = ALLSTOP;
                        for (int i = 1; i < size; i++) {
                            MPI_Send(&stop, 1, MPI_INT, i, JOB, MCW);
                        }
                        break;
                    }
                    tokenValue = (myToken == GENERIC) ? GENERIC : SPECIAL;
                }
            }
        }
    }
//Print info after run.
    if (!rank) {

        cout << "Rank 0--Total tasks created: " << tasksCreated << " Total Tasks completed: " << tasksCompleted <<  endl;
        for (int i = 1; i < size; i++) {
            int created;
            int completed;
            MPI_Recv(&completed, 1, MPI_INT, i, size, MCW, MPI_STATUS_IGNORE);
            MPI_Recv(&created, 1, MPI_INT, i, size - 1, MCW, MPI_STATUS_IGNORE);
            cout << "Rank "<<i<<"--Total tasks created: " << created << " Total Tasks completed: " << completed <<  endl;
            tasksCreated += created;
            tasksCompleted += completed;
        }
        cout << "Total Tasks Created: " << tasksCreated << endl;
        cout << "Total Tasks Completed: " << tasksCompleted << endl;
    }


    MPI_Finalize();
    return 0;
}