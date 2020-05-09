extern void outb (unsigned short int port_to, unsigned char value);
extern unsigned char inb(unsigned short int port_from);
//#define vga_base  0x3C0
unsigned char* vgabuffer=(unsigned char*)0x000B8000;	//创建一个指针指向vga显存地址
int pos=0;		//pos指示当前屏幕输出位置，vga尺寸为25*80
//清屏，即输出黑底无字
void clear_screen(void){
        void setcursor(int x,int y);
        int i,j;
        for(i=0;i<25;i++)
	for(j=0;j<80;j++){
		pos=i*80+j;
		vgabuffer[pos*2+1]=0x9;
		vgabuffer[pos*2]=' ';//显示黑底无字
	}
         pos=0;
	setcursor(pos/80,pos%80);
}
//向vga端口输出字符串
void append2screen(char *str,int color){
        void setcursor(int x,int y);
        int i=0,j=0;
        while(str[i]){
	if(str[i]=='\n'||str[i]=='\r'){	//对换行符进行处理
	        pos=pos+80-pos%80;	//pos移向下一行
	        i++;
	}
	else {
	        vgabuffer[2*pos+1]=(unsigned char)color;
	        vgabuffer[2*pos]=str[i];	//vga显示字符由两个字节，前一个代表字符本身，
                        i++;			//后一个表示颜色
	        pos++;
	}
	if(pos>=1920){		//实现滚屏
	        pos-=80;
	        for(j=0;j<3680;j++)		//所有元素上移一行
		vgabuffer[j]=vgabuffer[j+160];
	        for(j=0;j<80;j++){		//将最后一行清除
		vgabuffer[2*(pos+j)]=' ';
		vgabuffer[2*(pos+j)+1]=0x9;}
	}
        }
        setcursor(pos/80,pos%80);
}
//设置光标位置
void setcursor(int x,int y){
	int _pos=x*80+y;
	outb(0x3D4,0xE);		//通过向寄存器14，15写入值设置光标位置
	outb(0x3D5,(_pos>>8)&0xff);	//向寄存器14写入地址高八位
	outb(0x3D4,0xF);
	outb(0x3D5,_pos&0xff);	//向寄存器15写入地址低八位
}
//在左下角输出
void putchars(char*s,int color){
int i=0;
while(s[i]){
vgabuffer[3840+2*i]=s[i];
vgabuffer[3841+2*i]=color;
i++;}
}