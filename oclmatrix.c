#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <CL/cl.h>

void Test();
void TestDevices(cl_int CL_err,cl_uint num_devices_returned);
void PrintMatrix(double *A, const long size);

void Multiply(double *A, double *B, double *C, const long size) // C = A*B
{
  double sum = 0;
  #pragma omp parallel for shared(A,B,C) firstprivate(sum)
  for (long i=0; i < size; i++){
    for (long j=0; j < size; j++) {
        sum=0;
      for (long k=0; k < size; k++){
        sum += A[i*size+k] * B[j+k*size];
      }
      C[i*size+j] = sum;
    }
  }
}

//kernel void
//mult(global const double *A, global const double *B, global const double *C, global const long size){
//;
//}

int main(int argc, char *argv[])
{
  //used for timing
  double start, end;

  //get number of system processors
  int nprocs = omp_get_num_procs();
  printf("system procs = %d \n",nprocs);

  //request 1 thread per system processor
  omp_set_num_threads(nprocs*2);

//  if (argc == 1) {
//    Test();
//  } else {

//    long size = atol(argv[1]);
    long size = 1000;

//    global_dim[3] = {size*size,1,1};

cl_uint num_gpu_devices_returned;
  cl_device_id gpu_device;

  cl_int CL_err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &gpu_device, &num_gpu_devices_returned);

  //Test found GPUs
  TestDevices(CL_err,num_gpu_devices_returned);

  //Get Device name
  cl_device_info gpu_device_info = CL_DEVICE_NAME;
  char device_name[sizeof(char)*100];
  clGetDeviceInfo(gpu_device,gpu_device_info,sizeof(device_name),&device_name,NULL);
  printf("device name = %s\n",device_name);

  //create context
  cl_context context = clCreateContext(0,1,&gpu_device,NULL,NULL,CL_err);

  //Get Supported images
  cl_mem_flags flags = CL_MEM_READ_WRITE;
  cl_mem_object_type type = CL_MEM_OBJECT_IMAGE2D; // may not support 1D ?
  cl_image_format sup_formats[sizeof(cl_image_format)*100];
  cl_uint num_formats;
  clGetSupportedImageFormats(context,flags,type,10,&sup_formats,&num_formats);

  printf("num supported formats = %d \n", num_formats);

  //create queue
  cl_command_queue gpu_queue;
  gpu_queue = clCreateCommandQueue(context,gpu_device,0,&CL_err);

  cl_image_format format;
  format.image_channel_data_type = CL_FLOAT;
  format.image_channel_order = CL_RGBA;
  size_t image_width=size;
  size_t image_height=size;

    printf("size=%ld \n", size);
    double *A[size*size];
//    double *B[size*size];
//    double *C[size*size];

  cl_mem input_image;
  input_image = clCreateImage2D(context,CL_MEM_READ_ONLY,&format,image_width,image_height,0,NULL,&CL_err);

  cl_mem output_image;
  output_image = clCreateImage2D(context,CL_MEM_WRITE_ONLY,&format,image_width,image_height,0,NULL,&CL_err);

  printf("input image creation error code = %d \n",CL_err);
  printf("output image creation error code = %d \n",CL_err);

  cl_mem input_buffer;
  input_buffer = clCreateBuffer(context,CL_MEM_READ_ONLY,sizeof(cl_float)*4*image_width*image_height,NULL,&CL_err);

  cl_mem output_buffer;
  output_buffer = clCreateBuffer(context,CL_MEM_WRITE_ONLY,sizeof(cl_float)*4*image_width*image_height,NULL,&CL_err);


//    printf("size=%ld \n", size);
//    double *A[size*size];
//    double *B[size*size];
//    double *C[size*size];

    start = omp_get_wtime();
//    Multiply((double*)A, (double*)B, (double*)C, size);
    end = omp_get_wtime();

//    PrintMatrix((double*)C, size);
    printf("Compute time: %f \n",end-start);

//    free(*C);
//    free(*B);
//    free(*A);
//  }
}

// Functions' definitions

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

void TestDevices(cl_int CL_err, cl_uint num_devices_returned)
{
  if (CL_err == CL_SUCCESS)
    printf("%u GPU devices(s) found\n", num_devices_returned);
  else
    printf("No GPUs found, clGetDeviceIDs(%i)\n", CL_err);
}
