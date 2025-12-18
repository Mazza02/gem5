#include <stdio.h>
#include <riscv_vector.h>

int x[10] = {1,2,3,4,5,6,7,8,9,0};
int y[10] = {0,9,8,7,6,5,4,3,2,1};
int z[10];

void vec_add_rvv(int *a, int *b, int *c, size_t n) {

size_t vl;
   vint32m2_t va, vb, vc;
   for (;vl = vsetvl_e32m2 (n);n -= vl) {
     vb = vle32_v_i32m2 (b);
     vc = vle32_v_i32m2 (c);
     va = vadd_vv_i32m2 (vb, vc);
     vse32_v_i32m2 (a, va);
     a += vl;
     b += vl;
     c += vl;
   }
}



int main()
{
  int i;
  vec_add_rvv(z, x, y, 10);
  for (i=0; i<10; i++)
     printf ("%d ", z[i]);
  printf("\n");
  return 0;
}
