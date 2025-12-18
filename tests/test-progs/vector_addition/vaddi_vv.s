    # void vaddi_vv(vint32m1_t *vs1, vint32m1_t *vs2, int vl_count, int scalar_m)

    # a0 = ptr vs1
    # a1 = ptr vs2
    # a2 = vl_count
    # a3 = scalar_m


    .globl vaddi_vv
vaddi_vv:
1:
    vsetvli t0, a2, e32, m1, ta, ma   # t0 = VL

    vle32.v v0, (a0)                  # load vs1
    vle32.v v1, (a1)                  # load vs2
    vadd.vv v2, v0, v1                # v2 = vs1 + vs2
    vadd.vx v2, v2, a3                # v2 = v2 + scalar_m
    vse32.v v2, (a0)                  # store result

    slli t1, t0, 2                    # bytes = VL * 4
    add a0, a0, t1
    add a1, a1, t1
    sub a2, a2, t0                    # remaining -= VL
    bnez a2, 1b

    ret
