#include <fstream>
#include <iostream>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

#define MCW MPI_COMM_WORLD

using namespace std;

struct Matrix{
  int M, N;
  double** matrix;
};

//Problem size
const int N = 10;
vector<Matrix> matricies;
//global variables
double A[N][N];
double B[N][N];
double AB[N][N];
double AB_serial[N][N];

void fill_matrices();
void print_matrix(double mat[][N]);
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
  if(!rank)
    read_in_matrices();
  MPI_Finalize();
}


void read_in_matrices(){
  for(int i = 0; i < 2; i++) {
    Matrix* matrix = new Matrix;
    ifstream f("Matrix.txt");
    f >> matrix->M >> matrix->N;
    // Allocate memory
    matrix->matrix = new double *[matrix->M];
    for (int i = 0; i < matrix->M; ++i)
      matrix->matrix[i] = new double[N];

    for (int i = 0; i < matrix->M; i++)
      for (int j = 0; j < matrix->N; j++)
        f >> matrix->matrix[i][j];
    matricies.push_back(*matrix);
  }
}
//Function to fill matrices at random
void fill_matrices(){
  srand(time(NULL));

  for(int i = 0; i <N; i++){
    for (int j = 0; j < N; j++){
      A[i][j] = rand() %4;
      B[i][j] = rand() %4;
    }
  }
}
//Function to print matrix
void print_matrix(double mat[][N]){
  for (int i = 0; i < N; i++){
    for (int j = 0; j<N; j++){
      cout << mat[i][j] << " ";
    }
    cout << endl;
  }
  cout << endl;
}
//Serial version of solution
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
  int interval = N/size;
  int remainder = N%size;
  //Record start time
  MPI_Barrier(MCW);
  time1 = MPI_Wtime();

  //Rank 0 fills the matrices and computes the remainder
  if(!rank){
    fill_matrices();
    compute_interval(size*interval,remainder);
  }

  //Broadcast Matrix B and scatter relevant portions of Matrix A
  MPI_Bcast(B,N*N,MPI_DOUBLE,0,MCW);
  MPI_Scatter(A,interval*N,MPI_DOUBLE,A[rank*interval],interval*N,MPI_DOUBLE,0,MCW);

  //Each processor cumputes the interval they are responsible for
  compute_interval(rank*interval,interval);

  //Gather results
  MPI_Gather(AB[rank*interval],interval*N,MPI_DOUBLE,AB,interval*N,MPI_DOUBLE,0,MCW);

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
    cout << "gives matrix AB:" << endl;
    print_matrix(AB);
    cout << "serial version gives:" << endl;
    print_matrix(AB_serial);
  }
}
