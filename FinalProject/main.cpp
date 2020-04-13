#include <fstream>
#include <iostream>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

#define MCW MPI_COMM_WORLD

using namespace std;

//Problem size
int N;
//global variables
double **A, **B, **AB, **AB_serial;

void print_matrix(double **mat);
void serial_version();
void compute_interval(int start, int interval);
void multiplyMatrix(int rank, int size);
void read_in_matrices();



int main(int argc, char** argv){
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MCW, &rank);
  MPI_Comm_size(MCW, &size);
  multiplyMatrix(rank, size);
  MPI_Finalize();
}

void read_in_matrices() {
  ifstream f("Matrix.txt");
  f >> N;
  // Allocate memory
  A = new double *[N];
  for (int i = 0; i < N; ++i)
    A[i] = new double[N];
  B = new double *[N];
  for (int i = 0; i < N; ++i)
    B[i] = new double[N];
  AB = new double *[N];
  for (int i = 0; i < N; ++i)
    AB[i] = new double[N];
  AB_serial = new double *[N];
  for (int i = 0; i < N; ++i)
    AB_serial[i] = new double[N];

  // Fill Matricies
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      f >> A[i][j];
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      f >> B[i][j];
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      AB[i][j] = 0;
}

//Function to print matrix
void print_matrix(double **mat) {
  for (int i = 0; i < N; i++){
    for (int j = 0; j<N; j++){
      cout << mat[i][j] << " ";
    }
    cout << endl;
  }
  cout << endl;
}
// Serial multiplication works.
void serial_version(){
  for (int i = 0; i <N; i++){
    for (int j = 0; j < N;j++){
      AB_serial[i][j] = 0;
      for (int k = 0; k <N; k++){
        AB_serial[i][j] += A[i][k]*B[k][j];
      }
    }
  }
}
//Compute interval multiplication
void compute_interval(int start,int interval){
  for(int i = start; i <start+interval;i++){
    for (int j = 0; j <N; j++){
      AB[i][j] = 0;
      for (int k = 0; k <N; k++){
        AB[i][j] += A[i][k]*B[k][j];
      }
    }
  }
}
void multiplyMatrix(int rank, int size){
  //timing variables
  double time1,time2,time3;
  //compute interval size
  //rank 0 responsible for remainder

  //Rank 0 fills the matrices and computes the remainder
  if(!rank){
    read_in_matrices();
  }
  MPI_Bcast(&N, 1, MPI_INT, 0, MCW);
  // Record start time
  MPI_Barrier(MCW);
  time1 = MPI_Wtime();

  int interval = N / size;
  int remainder = N % size;
  MPI_Bcast(&interval, 1, MPI_INT, 0, MCW);
  MPI_Bcast(&remainder, 1, MPI_INT, 0, MCW);

  if (!rank) {
    compute_interval(size * interval, remainder);
  }

  //Broadcast Matrix B and scatter relevant portions of Matrix A
  print_matrix(A);
  MPI_Bcast(B,N*N,MPI_DOUBLE,0,MCW);
  MPI_Scatter(A, interval * N, MPI_DOUBLE, &A[rank * interval], interval * N,
              MPI_DOUBLE, 0, MCW);

  //Each processor cumputes the interval they are responsible for
  compute_interval(rank*interval,interval);

  //Gather results
  MPI_Gather(AB[rank * interval], interval * N, MPI_DOUBLE, &AB[0][0],
             interval * N, MPI_DOUBLE, 0, MCW);

  //Record parallel finish time
  MPI_Barrier(MCW);

  time2 = MPI_Wtime();

  if (!rank){

    //serial computation
    serial_version();

    //Record serial finish time
    time3 = MPI_Wtime();

    //Print times
    cout << "Problem size " << N << endl;
    cout << size << " processors computed in time: " << time2-time1 << endl;
    cout << "Serial version computed in time: " << time3-time2 << endl;
    cout << "Efficiency of: " << (time3-time2)/((time2-time1)*size) << endl;

    //Code to print matrices and results

    cout << "Matrix A: " << endl;
    print_matrix(A);
    cout << "multiplied Matrix B:" << endl;
    print_matrix(B);
    cout << "serial version gives:" << endl;
    print_matrix(AB_serial);
    cout << "gives matrix AB:" << endl;
    print_matrix(AB);
  }
}
