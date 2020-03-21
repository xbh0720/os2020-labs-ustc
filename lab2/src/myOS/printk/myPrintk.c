#include "vsprintf.c"
extern void append2screen(char *str,int color);
extern void uart_put_chars(char *str);
char kBuf[400];  //TODO: fix me
char uBuf[400];  //TODO: fix me
int myPrintk(int color,char*format,...){ 
	int n;
	va_list args;
	va_start(args,format);
	n=vsprintf(kBuf,format,args);
	va_end(args);
	uart_put_chars(kBuf);
	append2screen(kBuf,color);
	return n;
}
int myPrintf(int color, char *format,...){
	int n;
	va_list args;
	va_start(args, format);
	n=vsprintf(uBuf,format,args);
	va_end(args);
	uart_put_chars(uBuf);
	append2screen(uBuf,color);
	return n;
}