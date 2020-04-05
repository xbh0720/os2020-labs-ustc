//周期性时钟中断
extern void setWallClockHook(void);
extern int myPrintk(int color,const char *format, ...);
int count=0;
void tick(void)
{
	count++;	 //全局变量count在每次tick后+1
//每tick100次几乎经过了1s，用于时钟维护
	if(count==100){
		setWallClockHook();
		count=0;}
}
