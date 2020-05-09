extern void putchars(char*s,int color);
void ignoreIntBody(void)
{
	putchars("Unknown interrupt",0x4); //在vga左下角输出内容用于处理除时钟中断外的其他中断
}