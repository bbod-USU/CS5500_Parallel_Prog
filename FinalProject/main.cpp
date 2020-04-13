#include <fstream>
#include <iostream>
#include <mpi.h>
#include <vector>
#include "Serialize_Deserialize_Vector.h"

#define MCW MPI_COMM_WORLD

using namespace std;
using sdv = Serialize_Deserialize_Vector;

//Problem size
int N;
//global variables
std::vector<std::vector<double> > A = std::vector<std::vector<double> >(0, std::vector<double>(0));
std::vector<std::vector<double> > B = std::vector<std::vector<double> >(0, std::vector<double>(0));
std::vector<std::vector<double> > AB = std::vector<std::vector<double> >(0, std::vector<double>(0));
std::vector<std::vector<double> > AB_serial = std::vector<std::vector<double> >(0, std::vector<double>(0));


void print_matrix(std::vector<std::vector<double> > mat);
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
  A.resize(N);
  for (int i = 0; i < N; ++i)
    A[i].resize(N);
  B.resize(N);
  for (int i = 0; i < N; ++i)
    B[i].resize(N);
  AB.resize(N);
  for (int i = 0; i < N; ++i)
    AB[i].resize(N);
  AB_serial.resize(N);
  for (int i = 0; i < N; ++i)
    AB_serial[i].resize(N);
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
void print_matrix(std::vector<std::vector<double> >mat) {
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
  if(rank) {
    A.resize(N);
    for (int i = 0; i < N; ++i)
      A[i].resize(N);
    B.resize(N);
    for (int i = 0; i < N; ++i)
      B[i].resize(N);
    AB.resize(N);
    for (int i = 0; i < N; ++i)
      AB[i].resize(N);
  }
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
  std::vector<double> tmpB = std::vector<double>(N*N);

  tmpB = Serialize_Deserialize_Vector::Serialize(B);
  MPI_Bcast(&tmpB[0],N*N,MPI_DOUBLE,0,MCW);
  if(rank)
    B=Serialize_Deserialize_Vector::Deserialize(tmpB);
  //print_matrix(B);
  std::vector<double> tmpA = std::vector<double>(N*N);
  tmpA = Serialize_Deserialize_Vector::Serialize(A);
  MPI_Bcast(&tmpA[0], N*N, MPI_DOUBLE, 0, MCW);
//  MPI_Scatter(&tmpA[0], interval * N, MPI_DOUBLE, &tmpA[rank * interval], interval * N,
//              MPI_DOUBLE, 0, MCW);
  if(rank)
    A=Serialize_Deserialize_Vector::Deserialize(tmpA);
  //print_matrix(A);

  //Each processor cumputes the interval they are responsible for
  compute_interval(rank*interval,interval);

  //Gather results
  auto tmp_AB = Serialize_Deserialize_Vector::Serialize(AB);
//  cout << "RANK: " << rank << " INTERVAL: "<< interval << " rank*N " << rank*N << " tmp_AB.size() "<<tmp_AB.size() << " " ;
//  std::vector<double> tmp(interval*N);
//  for(int i = (rank * interval*N),j=0; i < (rank * interval*N)+N; i++, j++){
//    tmp[j]=tmp_AB[i];
//    cout << tmp[j]<<" ";
//  }
//  cout << endl;
//  std::vector<double> test(N*N);
  MPI_Gather(&tmp_AB[rank * interval*N], interval * N, MPI_DOUBLE, &tmp_AB[rank*interval*N],
             interval * N, MPI_DOUBLE, 0, MCW);
  AB=Serialize_Deserialize_Vector::Deserialize(tmp_AB);
  if(!rank)
    print_matrix(AB);
  //Record parallel finish time
  MPI_Barrier(MCW);

  time2 = MPI_Wtime();

  if (!rank){
    cout << "made it here!!" <<endl;
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
