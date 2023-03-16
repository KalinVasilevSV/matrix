// C standard includes
#include <stdio.h>

// OpenCL includes
#include <CL/opencl.h>

void device_spec(cl_device_id device){

}

int main()
{
    cl_int CL_err = CL_SUCCESS;
    cl_uint numPlatforms = 0;
    cl_platform_id platforms[2];

    CL_err = clGetPlatformIDs( 2, platforms, &numPlatforms );

    cl_int cpu_err=CL_SUCCESS;
    cl_int gpu_err=CL_SUCCESS;
    cl_uint num_cpu_devices_returned, num_gpu_devices_returned;
    cl_device_id devices[2];

    gpu_err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &devices[0], &num_gpu_devices_returned);
    cpu_err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_CPU, 1, &devices[1], &num_cpu_devices_returned);


    if (CL_err == CL_SUCCESS)
        printf("%u platform(s) found\n", numPlatforms);
    else
        printf("No platforms found, clGetPlatformIDs(%i)\n", CL_err);

    if (cpu_err == CL_SUCCESS)
        printf("%u CPU devices(s) found\n", num_cpu_devices_returned);
    else
        printf("No CPUs found, clGetDeviceIDs(%i)\n", cpu_err);

    if (gpu_err == CL_SUCCESS)
        printf("%u GPU devices(s) found\n", num_gpu_devices_returned);
    else
        printf("No GPUs found, clGetDeviceIDs(%i)\n", gpu_err);


//    cl::Device device =  platforms;

    return 0;
}
