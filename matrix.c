#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define CORE 4
#define MAX 4
int step_i = 0;
int matA[MAX][MAX]; 
int matB[MAX][MAX]; 

pthread_t thread[CORE * 2];

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX]; 
int matProductResult[MAX][MAX]; 


void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            matrix[i][j] = rand()%10+1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the sum of the
// values at the coordinates of matA and matB.
void* computeSum(void* args) { // pass in the number of the ith thread
    int i, j;
    //int* core_ptr = (int*)args;
    int core = (long)args;
  
    // Each thread computes 1/4th of matrix addition
    for (int i = core * MAX / 4; i < (core + 1) * MAX / 4; i++) {
  
        for (int j = 0; j < MAX; j++) {
  
            // Compute Sum Row wise
            matSumResult[i][j] = matA[i][j] + matB[i][j];
        }
  
    }
    return NULL;
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the difference of the
// values at the coordinates of matA and matB.
void* computeDiff(void* args) { // pass in the number of the ith thread
    int i, j;
    //int* core_ptr = (int*)args;
    int core = (long)args;
  
    // Each thread computes 1/4th of matrix subtraction
    for (int i = core * MAX / 4; i < (core + 1) * MAX / 4; i++) {
  
        for (int j = 0; j < MAX; j++) {
  
            // Compute Subtract row wise
            matDiffResult[i][j] = matA[i][j] - matB[i][j];
        }
  
    }

    
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the inner product
// of matA and matB.
void* computeProduct(void* args) { // pass in the number of the ith thread
    int b = step_i++;

  
    for (int i = 0; i < MAX; i++) {
  
        for (int j = 0; j < MAX; j++) {
  
            matProductResult[b][i] += matA[b][j] * matB[j][i];
        }
  
    }

}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main() {
    int step = 0;
    srand(time(0));  // Do Not Remove. Just ignore and continue below.
    
    // 0. Get the matrix size from the command line and assign it to MAX
    fillMatrix(matA);
    fillMatrix(matB);
    // 1. Fill the matrices (matA and matB) with random values.
    
    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);
    
    // 3. Create pthread_t objects for our threads.
    
    // 4. Create a thread for each cell of each matrix operation.
    pthread_t thread[4];
    for (int i = 0; i < 4; i++) {
        pthread_create(&thread[i], NULL, &computeSum, (void*)step);
        pthread_create(&thread[i], NULL, &computeDiff, (void*)step);
        pthread_create(&thread[i], NULL, &computeProduct, (void*)step);
        step++;
    }

    for (int i = 0; i < 4 ; i++) {
        pthread_join(thread[i], NULL);
    }
    // You'll need to pass in the coordinates of the cell you want the thread
    // to compute.
    // 
    // One way to do this is to malloc memory for the thread number i, populate the coordinates
    // into that space, and pass that address to the thread. The thread will use that number to calcuate 
    // its portion of the matrix. The thread will then have to free that space when it's done with what's in that memory.
    
    // 5. Wait for all threads to finish.
    
    // 6. Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    return 0;

  
}
