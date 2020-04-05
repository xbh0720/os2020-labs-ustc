// NOTE: 以下框架仅供参考。可以推翻重写。
#include<string.h>
#define WID 20
extern int myPrintk(int color,const char *format, ...);
extern unsigned char uart_get_char(void);
// 命令处理函数
/*int cmd_handler(int, char **);
int help_handler(int, char **);

// 帮助处理函数
void help_help(void);
*/
char argv[WID][WID];
int argc;
int cmd_handler(void);
int help_handler(void);
void help_help(void);
//初始化命令以及命令处理函数
struct command {
	char *cmd;
	int (*func)(void);
	void (*help_func)(void);
	char *desc;
} cmds[] = {
	{"cmd", cmd_handler, NULL, "list all commands"},
	{"help", help_handler, help_help, "help [cmd]"},
	{"",NULL, NULL,""}
};


// help 的帮助
void help_help(void)
{
	myPrintk(0x6,"%s\n",cmds[1].desc);
}

// help 命令处理函数
int help_handler(void)
{
    int i=0;
    int flag=0;
    if(strcmp(argv[1],"")==0){   help_help();     flag=1;}//若没有指定命令，则调用help的help
    else 
	{
	while(strcmp(cmds[i].cmd,"")){	//寻找指定命令
		if(strcmp(cmds[i].cmd,argv[1])==0){  //若找到指定命令则输出描述信息
		      flag=1;
		      myPrintk(0x6,"%s\n",cmds[i].desc);
		break;}
		i++;
	}
}
 	if(!flag)myPrintk(0x6,"unknown cmd!\n"); //无法识别的指令


	return 0;
}

// cmd 命令处理函数
int cmd_handler(void)
{
	int i=0;
	char str1[]="cmd name",str2[]="descriptiom";
	myPrintk(0x6,"%s\n",cmds[i].desc);
	myPrintk(0x7,"%-20s%-20s\n",str1,str2);
//输出所有指令
	while(strcmp(cmds[i].cmd,"")){
		myPrintk(0x6,"%-20s%-20s\n",cmds[i].cmd,cmds[i].desc);

		i++;

	}
	return 0;
}

void startShell(void)
{
   char c,str[400];
   int i,j,k,flag=0;
   while(1){
//初始化argc，argv，以及命令合法标志flag
        argc=0;flag=0;
        for(i=0;i<WID;i++){strcpy(argv[i],"");}
        myPrintk(0x2,"@xubihan >:");
        i=j=k=0;
//逐个显示输入的字符，并将其储存在str中
        while((c=uart_get_char())!='\r'){str[k++]=c;myPrintk(0x6,"%c",c);}
        str[k]='\0';
        k=0;
//将输入的字符串保存至argv[][]中，并得到参数个数argc
        while(str[k]){
            while(str[k]==' ')k++; 
            if(str[k]){   argc++;
                while(str[k]!=' '&&str[k]){ 
                    argv[i][j++]=str[k];k++;}
                argv[i][j]='\0';
                if(str[k]==' '){j=0;i++;}
             }
         }
         myPrintk(0x6,"\n"); 
//当输入命令非空时
         if(argc==1||argc==2){
                  i=0;
                  //判断输入命令是否是合法命令
                  while(strcmp(cmds[i].cmd,"")){
                           if(strcmp(cmds[i].cmd,argv[0])==0){
                                    flag=1;
                                    cmds[i].func();//调用相应命令处理程序
                                    break;} 
                           i++;
                  }
                  if(!flag)myPrintk(0x6,"unknown cmd!\n");//该命令非法，无法识别	
         }
    }
}
//argc，argv处理

