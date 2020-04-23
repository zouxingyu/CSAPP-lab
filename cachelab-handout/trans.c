/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
	if(N==32){
		for(int j=0;j<N;j+=8){
			for(int ii=0;ii<M;ii+=8){
				for(int i=ii;i<ii+8;i++){
					int a0=A[i][j];
					int a1=A[i][j+1];
					int a2=A[i][j+2];
					int a3=A[i][j+3];
					int a4=A[i][j+4];
					int a5=A[i][j+5];
					int a6=A[i][j+6];
					int a7=A[i][j+7];
					B[j][i]=a0;
					B[j+1][i]=a1;
					B[j+2][i]=a2;
					B[j+3][i]=a3;
					B[j+4][i]=a4;
					B[j+5][i]=a5;
					B[j+6][i]=a6;
					B[j+7][i]=a7;
				}
			}
		}
	}
	if(N==64){
		int a0,a1,a2,a3,a4,a5,a6,a7;
		for(int i=0;i<N;i+=8){
			for(int j=0;j<M;j+=4){
				for(int l=0;l<8;l+=2){
					a0=A[i+l][j];
					a1=A[i+l][j+1];
					a2=A[i+l][j+2];
					a3=A[i+l][j+3];
					a4=A[i+l+1][j];
					a5=A[i+l+1][j+1];
					a6=A[i+l+1][j+2];
					a7=A[i+l+1][j+3];
					B[j][i+l]=a0;
					B[j+1][i+l]=a1;
					B[j+2][i+l]=a2;
					B[j+3][i+l]=a3;
					B[j][i+l+1]=a4;
					B[j+1][i+l+1]=a5;
					B[j+2][i+l+1]=a6;
					B[j+3][i+l+1]=a7;
				}
			}
		}
	}
	if(N==67){
		int ii, jj, i, j, val0, val1, val2, val3, val4, val5, val6, val7;
		for(ii = 0; ii + 16 < N; ii += 16)
			for(jj = 0; jj + 16 < M; jj += 16)
			{
				for(i = ii; i < ii + 16; i++)
				{
					val0 = A[i][jj + 0];
					val1 = A[i][jj + 1];
					val2 = A[i][jj + 2];
					val3 = A[i][jj + 3];
					val4 = A[i][jj + 4];
					val5 = A[i][jj + 5];
					val6 = A[i][jj + 6];
					val7 = A[i][jj + 7];
					B[jj + 0][i] = val0;
					B[jj + 1][i] = val1;
					B[jj + 2][i] = val2;
					B[jj + 3][i] = val3;
					B[jj + 4][i] = val4;
					B[jj + 5][i] = val5;
					B[jj + 6][i] = val6;
					B[jj + 7][i] = val7;

					val0 = A[i][jj + 8];
					val1 = A[i][jj + 9];
					val2 = A[i][jj + 10];
					val3 = A[i][jj + 11];
					val4 = A[i][jj + 12];
					val5 = A[i][jj + 13];
					val6 = A[i][jj + 14];
					val7 = A[i][jj + 15];
					B[jj + 8][i] = val0;
					B[jj + 9][i] = val1;
					B[jj + 10][i] = val2;
					B[jj + 11][i] = val3;
					B[jj + 12][i] = val4;
					B[jj + 13][i] = val5;
					B[jj + 14][i] = val6;
					B[jj + 15][i] = val7;

				}
			}
		for(i = ii; i < N; i++)
			for(j = 0; j < M; j++)
				B[j][i] = A[i][j];
		for(i = 0; i < ii; i++)
			for(j = jj; j < M; j++)
				B[j][i] = A[i][j];
	}
	
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

