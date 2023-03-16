#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <omp.h>
#include <CL/cl.hpp>

void Multiply(double *A, double *B, double *C, const long size) // C = A*B
{
  int max_threads = omp_get_max_threads();
  printf("max possible threads = %d \n",max_threads);
  omp_set_num_threads(max_threads);

  double sum = 0;
  #pragma omp parallel shared(A,B,C) firstprivate(sum)
  {
   #pragma omp master
   {
     printf("allocated threads = %d \n",omp_get_num_threads());
   }
   #pragma omp for
   for (long i=0; i < size; i++)
     for (long j=0; j < size; j++) {
       sum = 0;
       for (long k=0; k < size; k++){
         sum += A[i*size+k] * B[k*size+j];
       }
       C[i*size+j] = sum;
     }
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
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    if (platforms.empty()){
        std::cerr << "No platforms found!" << std::endl;
        return -1;
    }

    auto platform = platforms.front();
    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

    if (devices.empty()){
        std::cerr << "No devices found!" << std::endl;
        return -1;
    }

        auto device = devices.front();
    auto name = device.getInfo<CL_DEVICE_NAME>();
    auto vendor = device.getInfo<CL_DEVICE_VENDOR>();
    auto version = device.getInfo<CL_DEVICE_VERSION>();
    auto workItems = device.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();
    auto workGroups = device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
    auto computeUnits = device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
    auto globalMemory = device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
    auto localMemory = device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>();

    std::cout << "OpenCL Device Info:"
    << "\nName: " << name
    << "\nVendor: " << vendor
    << "\nVersion: " << version
    << "\nMax size of work-items: (" << workItems[0] << "," << workItems[1] << "," << workItems[2] << ")"
    << "\nMax size of work-groups: " << workGroups
    << "\nNumber of compute units: " << computeUnits
    << "\nGlobal memory size (bytes): " << globalMemory
    << "\nLocal memory size per compute unit (bytes): " << localMemory/computeUnits
    << std::endl;

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
