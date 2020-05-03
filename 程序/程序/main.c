#include<reg51.h>
#include<intrins.h>
#define uint unsigned int 
#define uchar unsigned char
//time and alock setting, global
uchar hours;
uchar mins;
uchar seconds;

uchar alock_hours;
uchar alock_mins;
uchar alock_seconds;

sbit button1 = P1^0;
sbit button2 = P1^1;
sbit button3 = P1^2;
sbit led = P1^7;
uchar num = 0;
uchar code table1[] =
{0xc0,0xf9,0xa4,0xb0,

0x99,0x92,0x82,0xf8,
0x80,0x90,0x88,0x83,
0xc6,0xa1,0x86,0x8e};
void display_time();
void init_time(){
	hours = mins = seconds = alock_hours = alock_mins = alock_seconds =  0;
	hours = 12;
	mins = 34;
	seconds = 50;
	alock_hours = 12;
	alock_mins = 35;
	alock_seconds  = 11;
}
void delay(uint xms)
{
  uint i,j;
  for(i = 0; i<xms; ++i)
    for(j = 0; j<110; ++j);
}
void initT0()
{
  TMOD = 0x01;
  TH0 = 0xD8;
  TL0 = 0xF0;
  EA = 1;
  ET0 = 1;
  TR0 = 1;
}
void initEx0() {
  EA = 1;	 
  EX0 = 1;	 
  IT0 = 1;	
  EX1 = 1;  
  IT1 = 1;	
	PT0 = 1;
	PX0 = 1;
}
void t0Intr() interrupt 1//change time
{
  TH0 = 0xD8;
  TL0 = 0xF0;
  
  num++;
  if(num == 100)
  {
    num = 0;
    seconds = seconds + 1;
		if(seconds == 60){
			seconds = 0;
			mins = mins + 1;
			if(mins == 60){
				mins = 0;
				hours = hours + 1;
				if(hours == 24){
					hours = 0;
				}
			}
		}
  }
}
void ex0_intr() interrupt 0 { //int0 ,set time
  uchar i,wei=0x20;
	uchar n = 0x00;
	uchar dp = 0;
	uchar flash = 0;
	uchar cnt = 0;
	uchar res;
  while(1)
  {
		wei=0x20;
    for(i=0;i<6;i++)
    {		
	  //P0=table1[i];
		dp = 0;
		if(i == 0){
			n = seconds%10;
			dp = 1;
		}else if(i == 1){
			n = seconds/10;
		}else if(i == 2){
			n = mins%10;
			dp = 1;
		}else if(i == 3){
			n = mins/10;
		}else if(i == 4){
			n = hours%10;
			dp = 1;
		}else if(i == 5){
			n = hours/10;
		}else{
			n = 0xff;
		}
		P0=0xff;		
		if( dp == 1){
			res = table1[n] ^ 0x80;
		}else{
			res = table1[n];
		}
		if(i /2 == flash){
			cnt = cnt + 1;
			if(cnt == 200){
				cnt = 0;
			}
			if(cnt>=0 && cnt <100){
				res = res | 0xff;
			}else{
				res = res | 0x00;
			}
		}
		P0 = res;
	  P2=wei;
		wei=_cror_(wei,1);
	  delay(2);
    }
		if(button1 == 0){
			delay(20);
			if(button1 == 0){
				break;
			}
		}
		if(button2 == 0){
			delay(20);
			if(button2 == 0){
				flash = flash+1;
				if(flash == 3){
					flash = 0;
				}
				while(button2 == 0){
					//fangzhi chongfu guanjian caozuo  
				}
			}
		}
		if(button3 ==0){
			delay(20);
			if(button3 == 0){
				if(flash ==0){
					seconds = seconds + 1;
					if(seconds == 60){
						seconds = 0;
						mins = mins + 1;
						if(mins == 60){
							mins = 0;
							hours = hours + 1;
							if(hours == 24){
								hours = 0;
							}
						}
					}
				}else if(flash == 1){
					mins = mins + 1;
					if(mins == 60){
						mins = 0;
						hours = hours +1;
						if(hours == 24){
							hours = 0;
						}
					}
				}else{
					hours = hours + 1;
					if(hours == 24){
						hours = 0;
					}
				}
				while(button3 == 0);
			}
		}
  }		
}
void ex1_intr() interrupt 2 { //int1,set alock
  uchar i,wei=0x20;
	uchar n = 0x00;
	uchar dp = 0;
	uchar flash = 0;
	uchar cnt = 0;
	uchar res;
  while(1)
  {
		wei=0x20;
    for(i=0;i<6;i++)
    {		
	  //P0=table1[i];
		dp = 0;
		if(i == 0){
			n = alock_seconds%10;
			dp = 1;
		}else if(i == 1){
			n = alock_seconds/10;
		}else if(i == 2){
			n = alock_mins%10;
			dp = 1;
		}else if(i == 3){
			n = alock_mins/10;
		}else if(i == 4){
			n = alock_hours%10;
			dp = 1;
		}else if(i == 5){
			n = alock_hours/10;
		}else{
			n = 0xff;
		}
		P0=0xff;		
		if( dp == 1){
			res = table1[n] ^ 0x80;
		}else{
			res = table1[n];
		}
		if(i /2 == flash){
			cnt = cnt + 1;
			if(cnt == 200){
				cnt = 0;
			}
			if(cnt>=0 && cnt <100){
				res = res | 0xff;
			}else{
				res = res | 0x00;
			}
		}
		P0 = res;
	  P2=wei;
		wei=_cror_(wei,1);
	  delay(2);
    }
		if(button1 == 0){
			delay(20);
			if(button1 == 0){
				break;
			}
		}
		if(button2 == 0){
			delay(20);
			if(button2 == 0){
				flash = flash+1;
				if(flash == 3){
					flash = 0;
				}
				while(button2 == 0){
					//fangzhi chongfu guanjian caozuo  
				}
			}
		}
		if(button3 ==0){
			delay(20);
			if(button3 == 0){
				if(flash ==0){
					alock_seconds = alock_seconds + 1;
					if(alock_seconds == 60){
						alock_seconds = 0;
						alock_mins = alock_mins + 1;
						if(alock_mins == 60){
							alock_mins = 0;
							alock_hours = alock_hours + 1;
							if(alock_hours == 24){
								alock_hours = 0;
							}
						}
					}
				}else if(flash == 1){
					alock_mins = alock_mins + 1;
					if(alock_mins == 60){
						alock_mins = 0;
						alock_hours = alock_hours +1;
						if(alock_hours == 24){
							alock_hours = 0;
						}
					}
				}else{
					alock_hours = alock_hours + 1;
					if(alock_hours == 24){
						alock_hours = 0;
					}
				}
				while(button3 == 0);
			}
		}
  }		  
}

void display_time()
{
  uchar i,wei=0x20;
	uchar n = 0x00;
	uchar dp = 0;
	uchar state = 0;
	uchar cnt = 0;
	//if state == 0,normally. if state == 1,led is flash.
  while(1)
  {
		wei=0x20;
    for(i=0;i<6;i++)
    {		
	  //P0=table1[i];
		dp = 0;
		if(i == 0){
			n = seconds%10;
			dp = 1;
		}else if(i == 1){
			n = seconds/10;
		}else if(i == 2){
			n = mins%10;
			dp = 1;
		}else if(i == 3){
			n = mins/10;
		}else if(i == 4){
			n = hours%10;
			dp = 1;
		}else if(i == 5){
			n = hours/10;
		}else{
			n = 0xff;
		}
		P0=0xff;		
		if( dp == 1){
			P0 = table1[n] ^ 0x80;
		}else{
			P0 = table1[n];
		}
	  P2=wei;
		wei=_cror_(wei,1);
	  delay(2);
    }
		if(state == 0){
			led = 1;
		}else{//yao shanliang
			cnt = cnt + 1;
			if(cnt == 20){
				cnt = 0;
			}
			if(cnt>=0 && cnt <5){
				led = 0;
			}else{
				led = 1;
			}
		}
		if(state == 0){
			if(hours == alock_hours && mins == alock_mins && seconds == alock_seconds){
				state = 1;
			}
		}else{
			if(button1 == 1 && button2 == 1 && button3 ==1){
			
			}else{//push any button state = 0;
				state = 0;
				delay(20);
				if(button1 != 1){//zaishui wufenzhong
					alock_mins = alock_mins + 5;
				}
			}
		}
  }
}
void main(){
	init_time();
	initT0();
	initEx0();
	delay(20);
	display_time();
}