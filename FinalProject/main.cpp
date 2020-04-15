#include <fstream>
#include <iostream>
#include <mpi.h>
#include <vector>
#include "Serialize_Deserialize_Vector.h"

#define MCW MPI_COMM_WORLD

using namespace std;

int N;
ofstream myfile;
std::vector<std::vector<double> > A = std::vector<std::vector<double> >(0, std::vector<double>(0));
std::vector<std::vector<double> > B = std::vector<std::vector<double> >(0, std::vector<double>(0));
std::vector<std::vector<double> > AB = std::vector<std::vector<double> >(0, std::vector<double>(0));
std::vector<std::vector<double> > AB_serial = std::vector<std::vector<double> >(0, std::vector<double>(0));


void print_matrix(std::vector<std::vector<double> > mat);
void serial_version();
void compute_interval(int start, int interval);
void multiplyMatrix(int rank, int size);
void read_in_matrices();
void print_help();
void determinant();
std::vector<std::vector<double> > read_in_single_matrix();
void reduced_row();


int main(int argc, char** argv) {
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MCW, &rank);
  MPI_Comm_size(MCW, &size);
  if(!rank)
    myfile.open ("results.txt");

  bool done = false;
  while (!done) {
    char user_input;
    if (!rank) {
      bool user_input_complete = false;
      while (!user_input_complete) {
        cout << "Enter your selection: \n\t 1=Matrix Multiplication \n\t 2=Reduced Row \n\t 3=Matrix Determinent \n\t h=help \n\t q=Exit"
             << endl;
        cin >> user_input;
        if (user_input == 'h')
          print_help();
        else if (user_input == '1' || user_input == '2' || user_input == '3' ||
                 user_input == 'q')
          user_input_complete = true;
      }
    }
    MPI_Barrier(MCW);
    MPI_Bcast(&user_input, 1, MPI_INT, 0, MCW );
    if (user_input == '1')
      multiplyMatrix(rank, size);
    else if (user_input == '2'){
      if(!rank)
        reduced_row();
    }
    else if (user_input == '3') {
      if (!rank)
        determinant();
    }
    else if (user_input == 'q')
      done = true;
  }
  myfile.close();
  MPI_Finalize();
  return 0;
}
void reduced_row() {
  cout << "Reduced Row: " << endl;
  myfile << "Reduced Row: " << endl;
  std::vector<std::vector<double> > M = read_in_single_matrix();
  int n = M.size();
  const double SMALL = 1.0E-30;

  for (int i = 0; i < n - 1; i++) {
    int r = i;
    double maxM = abs(M[i][i]);
    for (int k = i + 1; k < n; k++) {
      double val = abs(M[k][i]);
      if (val > maxM) {
        r = k;
        maxM = val;
      }
    }
    if (r != i)
      for (int j = i; j < n; j++)
        swap(M[i][j], M[r][j]);

    double pivot = M[i][i];
    if (abs(pivot) < SMALL) {
      print_matrix(M);
      return;
    }

    for (int rows = i + 1; rows < n; rows++) {
      double multiple = M[rows][i] / pivot;
      for (int j = i; j < n; j++)
        M[rows][j] -= multiple * M[i][j];
    }
  }
  print_matrix(M);
}
std::vector<std::vector<double> > read_in_single_matrix() {
  ifstream f("Matrix.txt");
  f >> N;
  // Allocate memory
  A.resize(N);
  for (int i = 0; i < N; ++i)
    A[i].resize(N);
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      f >> A[i][j];
  return A;
}

void print_help(){
  cout << "Inputs-"
          "\n\t<1> Matrix Multiplication"
            "\n\t\t Matrix Multiplication runs both a parallel and serial implementation timing both results for comparison."
            "\n\t\t The read files first line should be the dimensions of the matricies being read and formatted as below:"
            "\n\t\t\t 3"
            "\n\t\t\t 1 2 3"
            "\n\t\t\t 4 5 6"
            "\n\t\t\t 7 8 9"
            "\n"
            "\n\t\t\t 9 8 7"
            "\n\t\t\t 6 5 4"
            "\n\t\t\t 3 2 1"
          "\n\n\t<2> Reduced Row"
            "\n\t\t Reduced row will preform row swaps to get the matrix into reduced row format."
              "\n\t\t The read files first line should be the dimensions of the matrix being read and formatted as below:"
              "\n\t\t\t 3"
              "\n\t\t\t 1 2 3"
              "\n\t\t\t 4 5 6"
              "\n\t\t\t 7 8 9"
          "\n\n\t<3> Matrix Determinent"
            "\n\t\t The Matrix determinant will be calculated by placing the matrix in reduced row format and then multiplying the pivot points."
              "\n\t\t The read files first line should be the dimensions of the matrix being read and formatted as below:"
              "\n\t\t\t 3"
              "\n\t\t\t 1 2 3"
              "\n\t\t\t 4 5 6"
              "\n\t\t\t 7 8 9"
          <<endl << endl;
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
void print_matrix(std::vector<std::vector<double> >mat) {
  for (int i = 0; i < N; i++){
    for (int j = 0; j<N; j++){
      cout << mat[i][j] << " ";
      myfile << mat[i][j] << " ";
    }
    cout << endl;
    myfile << endl;
  }
  myfile << endl;
  cout << endl;
}
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
  double time1,time2,time3;
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
  MPI_Barrier(MCW);

  int interval = N / size;
  int remainder = N % size;
  MPI_Bcast(&interval, 1, MPI_INT, 0, MCW);
  MPI_Bcast(&remainder, 1, MPI_INT, 0, MCW);
  // Record start time
  MPI_Barrier(MCW);
  time1 = MPI_Wtime();
  if (!rank) {
    compute_interval(size * interval, remainder);
  }

  std::vector<double> tmpB = std::vector<double>(N*N);

  tmpB = Serialize_Deserialize_Vector::Serialize(B);
  MPI_Bcast(&tmpB[0],N*N,MPI_DOUBLE,0,MCW);
  if(rank)
    B=Serialize_Deserialize_Vector::Deserialize(tmpB);
  std::vector<double> tmpA = std::vector<double>(N*N);
  tmpA = Serialize_Deserialize_Vector::Serialize(A);
  MPI_Bcast(&tmpA[0], N*N, MPI_DOUBLE, 0, MCW);
  if(rank)
    A=Serialize_Deserialize_Vector::Deserialize(tmpA);

  compute_interval(rank*interval,interval);

  std::vector<double> tmp_AB = Serialize_Deserialize_Vector::Serialize(AB);
  MPI_Gather(&tmp_AB[rank * interval*N], interval * N, MPI_DOUBLE, &tmp_AB[rank*interval*N],
             interval * N, MPI_DOUBLE, 0, MCW);
  AB=Serialize_Deserialize_Vector::Deserialize(tmp_AB);
  //Record parallel finish time
  MPI_Barrier(MCW);
  time2 = MPI_Wtime();

  if (!rank){
    serial_version();
    time3 = MPI_Wtime();
    cout << size << " processors computed in time: " << time2-time1 << endl;
    cout << "Serial version computed in time: " << time3-time2 << endl;
    cout << "Matrix A: " << endl;
    myfile << size << " processors computed in time: " << time2-time1 << endl;
    myfile << "Serial version computed in time: " << time3-time2 << endl;
    myfile << "Matrix A: " << endl;
    print_matrix(A);
    cout << "multiplied Matrix B:" << endl;
    myfile << "multiplied Matrix B:" << endl;
    print_matrix(B);
    cout << "serial version gives:" << endl;
    myfile << "serial version gives:" << endl;
    print_matrix(AB_serial);
    cout << "gives matrix AB:" << endl;
    myfile << "gives matrix AB:" << endl;
    print_matrix(AB);

  }
}

void determinant()
{
  std::vector<std::vector<double> > M = read_in_single_matrix();
  int n = M.size();
  const double SMALL = 1.0E-30;
  double det = 1;

  for ( int i = 0; i < n - 1; i++ )
  {
    int r = i;
    double maxM = abs( M[i][i] );
    for ( int k = i + 1; k < n; k++ )
    {
      double val = abs( M[k][i] );
      if ( val > maxM )
      {
        r = k;
        maxM = val;
      }
    }
    if ( r != i )
    {
      for ( int j = i; j < n; j++ ) swap( M[i][j], M[r][j] );
      det = -det;
    }

    double pivot = M[i][i];
    if ( abs( pivot ) < SMALL ) {
      cout << "Determinant = " << M[0][0] << " for matrix: " <<endl;
      myfile << "Determinant = " << M[0][0] << " for matrix: " <<endl;
      print_matrix(M);
      return;
    }
    for ( int rows = i + 1; rows < n; rows++ )
    {
      double multiple = M[rows][i] / pivot;
      for ( int j = i; j < n; j++ ) M[rows][j] -= multiple * M[i][j];
    }
    det *= pivot;
  }

  det *= M[n-1][n-1];
  cout << "Determinant = " << det<< " for matrix: " <<endl;
  myfile << "Determinant = " << det<< " for matrix: " <<endl;
  print_matrix(M);
}
