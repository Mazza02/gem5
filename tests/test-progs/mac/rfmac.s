# void rfmac_s(float *output, float input, float filter)
.global rfmac_s

rfmac_s:

    .insn r 0x53, 0x0, 0x24, ft0, fa4, fa5
    
    ret
