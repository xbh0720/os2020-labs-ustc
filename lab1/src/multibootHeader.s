.section.multiboot_header:		/*遵循multiboot启动协议编写multiboot_header*/
        /* magic */
        .long    0x1BADB002
        /* flags */
        .long    0
        /* checksum */
        .long   -(0x1BADB002 + 0);
.globl start			/*声明全局变量start*/

/*output hello word,PB18051098xubihan by vga*/
/*every output 2 characters ,the address +4*/
/*根据vga代码要求将所要输出字符通过mov移到vga显存中*/
start:		/*确定代码入口*/
.text.multiboot_entry:
	movl $0x2f652f68,0xB8000
	movl $0x2f6c2f6c,0xB8004
	movl $0x2f002f6f,0xB8008
	movl $0x2f6f2f77,0xB800c
	movl $0x2f6c2f72,0xB8010
	movl $0x2f2c2f64,0xB8014
	movl $0x2f422f50,0xB8018
	movl $0x2f382f31,0xB801c
	movl $0x2f352f30,0xB8020
	movl $0x2f302f31,0xB8024
	movl $0x2f382f39,0xB8028
	movl $0x2f752f78,0xB802c
	movl $0x2f692f62,0xB8030
	movl $0x2f612f68,0xB8034
	movl $0x2f002f6e,0xB8038

hlt
