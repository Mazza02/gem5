#include <stdio.h>
#include <stdlib.h>
//#include <riscv_vector.h>

void fmac_s(float *output, float input, float filter);
void rfmac_s(float input, float filter); 
float rfsmac_s(float *output);



void convolution(float ***Output, float ***Input, float ****Filter, int M, int C, int H, int W, int S, int Hfill, int Wfill) {
    for (size_t i = 0; i < M; i++) {
        for(size_t j = 0; j < H; j += S) {
            for (size_t k = 0; k < W; k += S) {
                for(size_t l = 0; l < C; l++) {
                    for(size_t m = 0; m < Hfill; m++) {
                        for(size_t n = 0; n < Wfill; n++) {
                            fmac_s(&Output[i][j/S][k/S], Input[l][j+m][k+n], Filter[i][l][m][n]);
                    }
                }
            }
        }
    }
}
}


void r_conv(float ***Output, float ***Input, float ****Filter, int M, int C, int H, int W, int S, int Hfill, int Wfill) {
    for (size_t i = 0; i < M; i++) {           // Output Channels
        for(size_t j = 0; j < H; j += S) {     // Height
            for (size_t k = 0; k < W; k += S) { // Width
                for(size_t l = 0; l < C; l++) {          // Input Channels
                    for(size_t m = 0; m < Hfill; m++) {  // Kernel Y
                        for(size_t n = 0; n < Wfill; n++) { // Kernel X
                            rfmac_s(Input[l][j+m][k+n], Filter[i][l][m][n]);
                        }
                    }
                }
                rfsmac_s(&Output[i][j/S][k/S]);
            }
        }
    }
}

void convolution_golden(float ***Output, float ***Input, float ****Filter, int M, int C, int H, int W, int S, int Hfill, int Wfill) {
    for (size_t i = 0; i < M; i++) {
        for(size_t j = 0; j < H; j += S) {
            for (size_t k = 0; k < W; k += S) {
                for(size_t l = 0; l < C; l++) {
                    for(size_t m = 0; m < Hfill; m++) {
                        for(size_t n = 0; n < Wfill; n++) {
                            Output[i][j/S][k/S] += Input[l][j+m][k+n] * Filter[i][l][m][n];
                    }
                }
            }
        }
    }
}
}

int main() {
    const int M = 1;      // output channels
    const int C = 1;      // input channels
    const int H = 5;      // input height
    const int W = 5;      // input width
    const int S = 1;      // stride
    const int Hfill = 3;  // kernel height
    const int Wfill = 3;  // kernel width

    float ***input  = malloc(C * sizeof(float **));
    float ****filter = malloc(M * sizeof(float ***));
    float ***output = malloc(M * sizeof(float **));
    float ***output_golden = malloc(M * sizeof(float **));
    float ***r_output = malloc(M * sizeof(float **));

    for(int c = 0; c < C; c++) {
        input[c] = malloc(H * sizeof(float *));
        for(int i = 0; i < H; i++)
            input[c][i] = malloc(W * sizeof(float));
    }

    for(int m = 0; m < M; m++) {
        filter[m] = malloc(C * sizeof(float **));
        for(int c = 0; c < C; c++) {
            filter[m][c] = malloc(Hfill * sizeof(float *));
            for(int i = 0; i < Hfill; i++)
                filter[m][c][i] = malloc(Wfill * sizeof(float));
        }
    }

    int outH = H / S;
    int outW = W / S;

    output_golden[0] = malloc(outH * sizeof(float *));
    for(int i = 0; i < outH; i++) {
        output_golden[0][i] = calloc(outW, sizeof(float));
    }
    output[0] = malloc(outH * sizeof(float *));
    for(int i = 0; i < outH; i++) {
        output[0][i] = calloc(outW, sizeof(float));
    }
    r_output[0] = malloc(outH * sizeof(float *));
    for(int i = 0; i < outH; i++) {
        r_output[0][i] = calloc(outW, sizeof(float));
    }

    // Init input
    printf("Input:\n");
    for(int i = 0; i < H; i++) {
        for(int j = 0; j < W; j++) {
            input[0][i][j] = i * W + j;
            printf("%4.1f ", input[0][i][j]);
        }
        printf("\n");
    }

    // Init filter (3x3 kernel of 1s)
    for(int i = 0; i < Hfill; i++) {
        for(int j = 0; j < Wfill; j++) {
            filter[0][0][i][j] = 5.0;
        }
    }
    printf("Filter:\n");
    for(int i = 0; i < Hfill; i++) {
        for(int j = 0; j < Wfill; j++) {
            printf("%4.1f ", filter[0][0][i][j]);
        }
        printf("\n");
    }

    convolution_golden(
        output_golden, input, filter,
        M, C, H - Hfill + 1, W - Wfill + 1,
        S, Hfill, Wfill
    );

    printf("\nGolden Output:\n");
    for(int i = 0; i < H - Hfill + 1; i++) {
        for(int j = 0; j < W - Wfill + 1; j++) {
            printf("%7.2f ", output_golden[0][i][j]);
        }
        printf("\n");
    }

    convolution(
        output, input, filter,
        M, C, H - Hfill + 1, W - Wfill + 1,
        S, Hfill, Wfill
    );

    printf("\nFmac Output:\n");
    for(int i = 0; i < H - Hfill + 1; i++) {
        for(int j = 0; j < W - Wfill + 1; j++) {
            printf("%7.2f ", output[0][i][j]);
        }
        printf("\n");
    }

     r_conv(
        r_output, input, filter,
        M, C, H - Hfill + 1, W - Wfill + 1,
        S, Hfill, Wfill
    );

    printf("\nR Extension Output:\n");
    for(int i = 0; i < H - Hfill + 1; i++) {
        for(int j = 0; j < W - Wfill + 1; j++) {
            printf("%7.2f ", r_output[0][i][j]);
        }
        printf("\n");
    }


    //convolution(a, b, r, N);

    //convolution_golden(a, b, c);

    //for (size_t i = 0; i < N; i++){
    //    printf("normal addition[%zu] = %f\n", i, c[i]);
    //    printf("vector addition[%zu] = %f\n", i, r[i]);
    //}
        


    return 0;
}

// re-create convolution code from r extension paper

// call-stack trace in tandem with interactive debugging (USE BREAKPOINTS)
