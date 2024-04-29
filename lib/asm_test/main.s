.option pic
.section .text
.align	4
.globl  main


main:
    li  t0,0x186A0
    mul a0,t0,t0
    mulh a1,t0,t0

loop1:
    j loop1
