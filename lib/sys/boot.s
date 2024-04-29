.option pic
.section .text
.align	4
.globl  _start

_start:
	la	sp,stack_begin
	la	gp,_global

    xor    a0,a0,a0 /* argc = 0 */
    xor    a1,a1,a1 /* argv = 0 */
    xor    a2,a2,a2 /* envp = 0 */

	call	setup_irq_enabled
	call	main

ryan_loop_dont_touch:
	j	ryan_loop_dont_touch

.word RAM_SIZE
