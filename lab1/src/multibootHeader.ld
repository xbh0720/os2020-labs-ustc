OUTPUT_FORMAT("elf32-i386","elf32-i386","elf32-i386")     /*以elf32-i386格式输出文件*/
OUTPUT_ARCH(i386)    /*建立x86架构下*/
ENTRY(start)     /*代码入口*/

SECTIONS{
   . = 1M;		/*表示在物理内存1M位置开始放代码*/
   .text : {
	*(.multiboot_header)
	. = ALIGN(8);    /*按8个字节对齐存放代码段*/
	*(.text)
    }
}
