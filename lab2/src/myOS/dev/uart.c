extern unsigned char inb(unsigned short int port_from);
extern void outb(unsigned short int port_to, unsigned char value);
#define uart_base 0x3F8
void uart_put_char(unsigned char c){
	outb(0x3F8,c);	//向串口输出字符
}
unsigned char uart_get_char(void){
	unsigned char c;
	c=inb(uart_base);              //读入字符
	return c;
}
void uart_put_chars(char *str){		//串口输出字符串
        int i=0;
        while(str[i]){
	if(str[i]=='\n'){
		i++;
		uart_put_char(0x0d);
		uart_put_char(0x0a);		//对换行进行处理，输出\r\n
		continue;}
	outb(uart_base,str[i]);	//输出字符
	i++;
	}
}
