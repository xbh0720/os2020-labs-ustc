.globl start
.section .multiboot_header:
        /* magic */
        .long    0x1BADB002
        /* flags */
        .long    0
        /* checksum */
        .long   -(0x1BADB002 + 0)
.text
.code32
start:
	call _start
	hlt

