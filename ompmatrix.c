#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

void Multiply(double *A, double *B, double *C, const long size) // C = A*B
{
  double sum = 0;
  #pragma omp parallel for shared(A,B,C) firstprivate(sum)
  for (long i=0; i < size; i++){
//       if(omp_get_thread_num()==0 && i==0)
//          printf("allocated threads = %d \n",omp_get_num_threads());
    for (long j=0; j < size; j++) {
        sum=0;
      for (long k=0; k < size; k++){
        sum += A[i*size+k] * B[j+k*size];
      }
      C[i*size+j] = sum;
    }
  }
}
//       if(omp_get_thread_num()==0 && i==0)
//          printf("allocated threads = %d \n",omp_get_num_threads());

//void v_mult(double *A, double *B, double *C,long i, long j, long size){
//    long spot = i*size*j;
//    long row=i*size,col=j;
//    for(long k=0;k<size;k++,row=i*size+k,col=k*size+j)
//        C[spot] += A[row]*B[col];
//}

//void Multiply(double *A, double *B, double *C, const long size) // C = A*B
//{
//  double sum = 0;
//  #pragma omp parallel
//  {
//  #pragma omp single
//  {
//  for (long i=0; i < size; i++){
//    for (long j=0; j < size; j++) {
//        sum=0;
////      C[i*size+j] = 0.0;
////      #pragma omp parallel for reduction(+:sum)
//      for (long k=0; k < size; k++){
//        #pragma omp task
//        {
//        sum += A[i*size+k] * B[j+k*size];
//        }
//      }
//      C[i*size+j] = sum;
//    }
//    }
//  }
//  }
//}//       if(omp_get_thread_num()==0 && i==0)
//          printf("allocated threads = %d \n",omp_get_num_threads());



void PrintMatrix(double *A, const long size)
{
  printf("MATRIX SIZE: %ld \n",size);

  if(size > 30){
    printf("Skipping print due to size (>30)...\n");
    return;
  }
  for (long i = 0; i < size; i++) {
    for (long j = 0; j < size; j++)
      printf("%f ", A[i*size+j]);
    printf("\n");
  }
}

void Test()
{
  double start, end;
  long size = 3;

  double A[size][size];
  double B[size][size];
  double C[size][size];

  A[0][0] = 14.0; A[0][1] = 9.0;  A[0][2] = 3.0;
  A[1][0] = 2.0;  A[1][1] = 11.0; A[1][2] = 15.0;
  A[2][0] = 0.0;  A[2][1] = 12.0; A[2][2] = 17.0;

  B[0][0] = 12.0; B[0][1] = 25.0; B[0][2] = 5.0;
  B[1][0] = 9.0;  B[1][1] = 10.0; B[1][2] = 0.0;
  B[2][0] = 8.0;  B[2][1] = 5.0;  B[2][2] = 1.0;

  start = omp_get_wtime();
  Multiply((double*)&A, (double*)&B, (double*)&C, size);
  end = omp_get_wtime();

  PrintMatrix((double*)&C, size);
  printf("Compute time: %f \n",end-start);
}

int main(int argc, char *argv[])
{
  //used for timing
  double start, end;

  //get number of system processors
  int nprocs = omp_get_num_procs();
  printf("system procs = %d \n",nprocs);

  //request 1 thread per system processor
  omp_set_num_threads(nprocs*2);

  if (argc == 1) {
    Test();
  } else {
    long size = atol(argv[1]);
    printf("size=%ld \n", size);
    double *A[size*size];
    double *B[size*size];
    double *C[size*size];

    start = omp_get_wtime();
    Multiply((double*)A, (double*)B, (double*)C, size);
    end = omp_get_wtime();

    PrintMatrix((double*)C, size);
    printf("Compute time: %f \n",end-start);

    free(*C);
    free(*B);
    free(*A);
  }
}
