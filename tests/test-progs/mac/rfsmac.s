# void rfsmac_s(float *output)
.global rfsmac_s

rfsmac_s:

    .insn r 0x53, 0x0, 0x38, ft0, x0, x0

    fsw ft0, 0(a0)

    ret
