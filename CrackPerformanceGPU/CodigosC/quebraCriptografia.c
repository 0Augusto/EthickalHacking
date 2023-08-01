#include <stdio.h>
#include <OpenCL/opencl.h>

// Tamanho das matrizes
#define MATRIX_SIZE 1024
#define MATRIX_SIZE_SQUARE (MATRIX_SIZE * MATRIX_SIZE)

// Função para inicializar as matrizes com valores aleatórios
void initialize_matrices(float* A, float* B) {
    for (int i = 0; i < MATRIX_SIZE_SQUARE; i++) {
        A[i] = (float)(rand() % 100) / 10.0f;
        B[i] = (float)(rand() % 100) / 10.0f;
    }
}

int main() {
    // Matrizes de entrada e saída
    float A[MATRIX_SIZE_SQUARE], B[MATRIX_SIZE_SQUARE], C[MATRIX_SIZE_SQUARE];

    // Inicializa as matrizes com valores aleatórios
    initialize_matrices(A, B);

    // Define as variáveis da plataforma e dispositivo da GPU Apple
    cl_platform_id platform;
    clGetPlatformIDs(1, &platform, NULL);
    cl_device_id device;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    // Cria o contexto da GPU
    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);

    // Cria a fila de comandos para executar o kernel na GPU
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, NULL);

    // Cria os buffers de memória para as matrizes na GPU
    cl_mem bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * MATRIX_SIZE_SQUARE, NULL, NULL);
    cl_mem bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * MATRIX_SIZE_SQUARE, NULL, NULL);
    cl_mem bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * MATRIX_SIZE_SQUARE, NULL, NULL);

    // Copia os dados das matrizes para os buffers na GPU
    clEnqueueWriteBuffer(queue, bufferA, CL_TRUE, 0, sizeof(float) * MATRIX_SIZE_SQUARE, A, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, bufferB, CL_TRUE, 0, sizeof(float) * MATRIX_SIZE_SQUARE, B, 0, NULL, NULL);

    // Carrega o código do kernel para multiplicação de matrizes
    const char* kernelSource = "__kernel void matrix_mult(__global const float* A, __global const float* B, __global float* C, const int size) {\n"
                               "    int i = get_global_id(0);\n"
                               "    int j = get_global_id(1);\n"
                               "    float sum = 0;\n"
                               "    for (int k = 0; k < size; k++) {\n"
                               "        sum += A[i * size + k] * B[k * size + j];\n"
                               "    }\n"
                               "    C[i * size + j] = sum;\n"
                               "}\n";

    // Compila o código do kernel
    cl_program program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    // Cria o kernel a partir do programa
    cl_kernel kernel = clCreateKernel(program, "matrix_mult", NULL);

    // Define os argumentos do kernel
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufferA);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferB);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufferC);
    clSetKernelArg(kernel, 3, sizeof(int), &MATRIX_SIZE);

    // Define as dimensões de trabalho
    size_t globalWorkSize[2] = { MATRIX_SIZE, MATRIX_SIZE };

    // Executa o kernel na GPU
    clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalWorkSize, NULL, 0, NULL, NULL);

    // Espera a conclusão do kernel
    clFinish(queue);

    // Copia o resultado de volta para a CPU
    clEnqueueReadBuffer(queue, bufferC, CL_TRUE, 0, sizeof(float) * MATRIX_SIZE_SQUARE, C, 0, NULL, NULL);

    // Imprime o resultado
    printf("Resultado da multiplicação de matrizes:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%.2f ", C[i * MATRIX_SIZE + j]);
        }
        printf("\n");
    }

    // Libera recursos
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return 0;
}
