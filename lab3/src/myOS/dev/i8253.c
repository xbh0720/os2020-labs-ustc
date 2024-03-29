extern unsigned char inb(unsigned short int port_from);
extern void outb (unsigned short int port_to, unsigned char value);
void init8253(void)
{	
	int cut=11932;     //设定时钟中断的频率为100HZ，得到分频参数
	outb(0x43,0x34);
	outb(0x40,cut&0xff);// 将分频参数写入0x40端口先低八位后高八位
	outb(0x40,(cut>>8)&0xff);
	outb(0x21,inb(0x21&0xfe));  //通过8259控制，允许时钟中断，取消屏蔽
}
