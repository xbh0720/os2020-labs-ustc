extern int myPrintk(int color,const char *format, ...);
extern void clear_screen(void);
extern void myMain(void);

void osStart(void){
clear_screen();
//操作系统各个功能模块的初始化
myPrintk(0x2,"START RUNNING userApp......\n");
myMain();
myPrintk(0x2, "STOP RUNNING userApp......ShutDown\n");
while(1);
}