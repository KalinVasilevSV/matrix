#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

long size;

//void Multiply(double *A, double *B, double *C, const long size) // C = A*B
//{
//  double (*A1)[size][size];
//  A1 = (double (*) [size][size])&A;
//
//  double (*B1)[size][size];
//  B1 = (double (*) [size][size])&B;
//
//  double (*C1)[size][size];
//  C1 = (double (*) [size][size])&C;
//
//
//  #pragma omp parallel for
//  for (long i=0; i < size; i++){
//     if(omp_get_thread_num()==0 && i==0)
//          printf("allocated threads = %d \n",omp_get_num_threads());
//     #pragma omp parallel for
//     for (long j=0; j < size; j++) {
//       *C1[i][j] = 0.0;
//       #pragma omp parallel for
//       for (long k=0; k < size; k++)
//         *C1[i][j] += *A1[i][k] * *B1[k][j];
//     }
//    }
//}

//void PrintMatrix(double *A, const long size)
//{
//  printf("MATRIX SIZE: %ld \n",size);
//
//  if(size > 30){
//    printf("Skipping print due to size (>30)...\n");
//    return;
//  }
//  for (long i = 0; i < size; i++) {
//    for (long j = 0; j < size; j++)
//      printf("%f ", A[i*size+j]);
//    printf("\n");
//  }
//}

//void Test()
//{
//  double start, end;
//  long size = 3;
//
//  double A[size][size];
//  double B[size][size];
//  double C[size][size];
//
//  A[0][0] = 14.0; A[0][1] = 9.0;  A[0][2] = 3.0;
//  A[1][0] = 2.0;  A[1][1] = 11.0; A[1][2] = 15.0;
//  A[2][0] = 0.0;  A[2][1] = 12.0; A[2][2] = 17.0;
//
//  B[0][0] = 12.0; B[0][1] = 25.0; B[0][2] = 5.0;
//  B[1][0] = 9.0;  B[1][1] = 10.0; B[1][2] = 0.0;
//  B[2][0] = 8.0;  B[2][1] = 5.0;  B[2][2] = 1.0;
//
//  start = omp_get_wtime();
//  Multiply((double*)&A, (double*)&B, (double*)&C, size);
//  end = omp_get_wtime();
//
//  PrintMatrix((double*)&C, size);
//  printf("Compute time: %f \n",end-start);
//}

int main(int argc, char *argv[])
{
  //used for timing
  double start, end;

  //get number of system processors
  int nprocs = omp_get_num_procs();
  printf("system procs = %d \n",nprocs);

  //request 1 thread per system processor
  omp_set_num_threads(nprocs);

//  if (argc == 1) {
//    Test();
//  } else {
    size = atol(argv[1]);
    printf("size=%ld \n", size);
    double A[size][size];
    double B[size][size];
    double C[size][size];

    double sum;
    start = omp_get_wtime();
//    Multiply((double*)&A, (double*)&B, (double*)&C, size);
  #pragma omp parallel for
  for (long i=0; i < size; i++){
     sum = 0;
     if(omp_get_thread_num()==0 && i==0)
          printf("allocated threads = %d \n",omp_get_num_threads());
     for (long j=0; j < size; j++) {
       for (long k=0; k < size; k++)
         sum += (A[i][k] * B[k][j]);
       C[i][j] = sum;
     }
  }
    end = omp_get_wtime();

//    PrintMatrix(*C, size);
    printf("Compute time: %f \n",end-start);

    delete[] C;
    delete[] B;
    delete[] A;
//  }
}
