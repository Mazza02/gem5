#include <stdio.h>
#include <riscv_vector.h>

//vint32m1_t vaddi_vv(vint32m1_t vs1, vint32m1_t vs2, int vl_count);

/*static inline vint32m1_t vaddi_vv(vint32m1_t vs1, vint32m1_t vs2, int vl_count, int scalar_m) {
    vint32m1_t result_vec;

    asm volatile (
        "vsetvli t0, %3, e32, m1, ta, ma;" 

        "vadd.vv %[result], %[in_v1], %[in_v2];" 

        "vadd.vx %[result], %[result], %4;" 

        : [result] "=vr" (result_vec) 
        
        : [in_v1] "vr" (vs1), 
          [in_v2] "vr" (vs2), 
          "r" (vl_count),
          "r" (scalar_m)
        
        : "t0" 
    );

    return result_vec;
}*/

int add_increment(int a, int b);

void vector_add(int *a, int *b, int *c, int n) {

    for (size_t i = 0; i < n; i++) {

        size_t vl = __riscv_vsetvl_e32m1(n - i);

        vint32m1_t va = __riscv_vle32_v_i32m1(&a[i], vl);
        vint32m1_t vb = __riscv_vle32_v_i32m1(&b[i], vl);

        vint32m1_t vec_c = __riscv_vadd_vv_i32m1(va, vb, vl);

        __riscv_vse32_v_i32m1(&c[i], vec_c, vl);
    }
}

/*void vector_add_increment_extension(int *a, int *b, int *c, int n) {
    
    size_t i = 0;
    
    for (size_t i = 0; i < n; i++) {

        size_t vl = __riscv_vsetvl_e32m1(n - i);

        vint32m1_t va = __riscv_vle32_v_i32m1(&a[i], vl);
        vint32m1_t vb = __riscv_vle32_v_i32m1(&b[i], vl);

        vint32m1_t vec_c = vaddi_vv(va, vb, vl); 

        __riscv_vse32_v_i32m1(&c[i], vec_c, vl);


    }
}*/

void add_increment_basic(int *a, int *b, int *c, int n) {

    for (size_t i = 0; i < n; i++) {
        c[i] = add_increment(a[i], b[i]);
    }
}

void add_golden(int *a, int *b, int *c, int n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

int main() {
    const int N = 63;
    int a[N], b[N], c[N], vector_normal[N], incremented_normal[N], normal[N], incremented_vector[N];
    

    for (size_t i = 0; i < N; i++) {
        a[i] = i;
        b[i] = 2 * i;
        vector_normal[i] = 0;
        incremented_normal[i] = 0;
        normal[i] = 0;
    }

    vector_add(a, b, vector_normal, N);

    add_golden(a, b, normal, N);

    add_increment_basic(a, b, incremented_normal, N);

    //vector_add_increment_extension(a, b, incremented_vector, N);

    for (size_t i = 0; i < N; i++){
        printf("normal addition[%zu] = %d\n", i, normal[i]);
        printf("incremented normal addition[%zu] = %d\n", i, incremented_normal[i]);
        printf("vector addition[%zu] = %d\n", i, vector_normal[i]);
        //printf("incremented vector addition[%zu] = %d\n", i, incremented_vector[i]);
    }
        


    return 0;
}

// re-create convolution code from r extension paper

// call-stack trace in tandem with interactive debugging (USE BREAKPOINTS)
