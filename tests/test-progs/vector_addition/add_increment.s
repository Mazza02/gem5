    # int add_increment(int vs1, int vs2)

    .globl add_increment
add_increment:
    # if bit shifts not specified, simulator would only operate on zero register (output)
    # opcode, rd, rs1, rs2 -- 10 = a0 register
    #.word (0xe000033 | (10 << 7) | (10 << 15) | (11 << 20))
    .insn r 0x33, 0x1, 0x9, x10, x11, x10
    ret
