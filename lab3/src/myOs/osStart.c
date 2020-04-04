extern int myPrintk(int color,const char *format, ...);
extern void clear_screen(void);
extern void myMain(void);
extern void init8253(void);
extern void enable_interrupt(void);
extern void init8259A(void);
extern void setWallClock(int h, int m, int s);
void osStart(void)
{
	// TODO 请填写代码
	clear_screen();
	setWallClock(23,59,30);
	init8259A();
	init8253();
	enable_interrupt();
	
	myMain();
	while(1);
}
