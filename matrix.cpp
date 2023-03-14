#include <stdlib.h>
#include <stdio.h>

void Multiply(double *A, double *B, double *C, const long size) // C = A*B
{
  for (long i=0; i < size; i++)
    for (long j=0; j < size; j++) {
      C[i*size+j] = 0.0;
      for (long k=0; k < size; k++)
        C[i*size+j] += A[i*size+k] * B[k*size+j];
    }
}

void PrintMatrix(double *A, const long size)
{
  for (long i = 0; i < size; i++) {
    for (long j = 0; j < size; j++)
      printf("%f ", A[i*size+j]);
    printf("\n");
  }
}

void Test()
{
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

  Multiply((double*)&A, (double*)&B, (double*)&C, size);

  PrintMatrix((double*)&C, size);
}

int main(int argc, char *argv[])
{
  if (argc == 1) {
    Test();
  } else {
    long size = atol(argv[1]);
    printf("size=%ld\n", size);
    double *A = new double[size*size];
    double *B = new double[size*size];
    double *C = new double[size*size];
    Multiply(A, B, C, size);
    PrintMatrix(C, size);
    delete[] C;
    delete[] B;
    delete[] A;
  }
}
