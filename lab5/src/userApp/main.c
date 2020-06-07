#include "../myOS/userInterface.h"   //interface from kernel

#include "shell.h"

extern void myTsk0(void);
extern void myTsk1(void);
extern void myTsk2(void);
extern void startShell(void);
extern void tskEnd(void);
extern int createTsk(void (*tskBody)(void));
extern void memTestCaseInit(void);
// init task 入口
void myMain(void) {

     clear_screen();
myPrintf(0x5,"*****      start       *********\n");
     createTsk(myTsk0);
     createTsk(myTsk1);
     createTsk(myTsk2);

	initShell();
     memTestCaseInit();
	createTsk(startShell);
     tskEnd();
//myPrintf(0x5,"444\n");
}
