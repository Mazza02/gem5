#include <stdio.h>
#include <math.h>
#include <riscv_vector.h>

float vec1[150];
float vec2[150];
float vec3[150];

int main(){

for (int i = 0; i < 150; i++)
{
    vec1[i] = i + 2;
    vec2[i] = i;
    vec3[i] = vec1[i] + vec2[i];
}
printf("vec3[25] = %f\n", vec3[25]);
return 0;
}
