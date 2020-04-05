int hh=0, mm=0, ss=0, ms=0;
unsigned char* vgabuffer1=(unsigned char*)0x000B8000;
//由count对时间进行维护
void setWallClock(int h, int m, int s)
{
	hh=h;
	mm=m;
	ss=s;
}

void getWallClock(int *h, int *m, int *s)
{
        *h = hh;
        *m = mm;
        *s = ss;
}

void setWallClockHook(void){
//更新时间
    ss++;
    if(ss>=60){
         mm++;
         if(mm>=60){
	hh++;
                if(hh>=24)  hh=0;
                mm=0;}
         ss=0;}
//在右下角显示时间
vgabuffer1[4000-16]=(char)(hh/10)+'0';
vgabuffer1[4000-14]=(char)(hh%10)+'0';
vgabuffer1[4000-12]=':';
vgabuffer1[4000-10]=(char)(mm/10)+'0';
vgabuffer1[4000-8]=(char)(mm%10)+'0';
vgabuffer1[4000-6]=':';
vgabuffer1[4000-4]=(char)(ss/10)+'0';
vgabuffer1[4000-2]=(char)(ss%10)+'0';

}
