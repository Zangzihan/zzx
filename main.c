#include <reg51.h>
#include <stdio.h>
#include "./pca9633/pca9633.h"
#include "./ir/ir.h"
#include "./delay/delay.h"
#include "./uart/uart.h"
unsigned int choose_uart;
unsigned int choose_ir;
unsigned int choose;

void breath_led()
{
	unsigned char addchange = 0x02;
	int i;
  write9633(0xC0, PWM0, 0);//红	  
  write9633(0xC0, PWM1, 0);//绿
  write9633(0xC0, PWM2, 0); //蓝
  write9633(0xC2, PWM2, 0);//	  
  write9633(0xC2, PWM1, 0);
	write9633(0xC2, PWM0, 0);
	write9633(0xC0, GRPPWM, 255);
	write9633(0xC0, GRPPFREQ, 0);
	write9633(0xC2, GRPPWM, 255);
	write9633(0xC2, GRPPFREQ, 0);
  while(1)
  {
	  for ( i= 0; i<255; )
		{
			choose = 0;
			if((choose_uart = uart_rev_deal()) != 0)
			{
				choose = choose_uart;
			}
			if((choose_ir = ir_choose()) != 0)
			{
				choose = choose_ir;
			}
			if(choose == 2 || choose == 3 || choose == 4 || choose == 5 || choose == 6)
			{
				return;
			}
			uart_send_ledState(choose, 0, 0, 0, 0);
		  write9633 (0xC0, addchange, i);	 //绿
			write9633 (0xC2, addchange, i);	
			
			
			i=i+10;
			delay_ms(1);
		}
		for (i= 255; i>0; i--)
		{ 
			choose = 0;
			if((choose_uart = uart_rev_deal()) != 0)
			{
				choose = choose_uart;
			}
			if((choose_ir = ir_choose()) != 0)
			{
				choose = choose_ir;
			}
			if(choose == 2 || choose == 3 || choose == 4 || choose == 5)
			{
				return;
			}
			uart_send_ledState(choose, 0, 0, 0, 0);
		  write9633 (0xC0, (0x06-addchange), i);	 //绿
			write9633 (0xC2, (0x06-addchange), i);	
			delay_ms(1);
	  }
		addchange++;
		if(addchange == 0x05)
		{
			addchange = 0x02;
		}
	}
}
 
void led_color(unsigned char led_adress, unsigned char led_color)
{
		if(led_color == 1)
		{
			write9633(led_adress, PWM0, 100);//红	  
			write9633(led_adress, PWM1, 0);
			write9633(led_adress, PWM2, 0);
			write9633(led_adress, PWM3, 0);
		}
		else if(led_color == 2)
		{
      write9633(led_adress, PWM0, 0);	  
			write9633(led_adress, PWM1, 100);//绿
			write9633(led_adress, PWM2, 0);
			write9633(led_adress, PWM3, 0);
    }
		else if(led_color == 3)
		{
      write9633(led_adress, PWM0, 0);	  
		  write9633(led_adress, PWM1, 0);
			write9633(led_adress, PWM2, 100);//蓝
			write9633(led_adress, PWM3, 0);
    }
}
				
int main()
{
	unsigned char freq = 0;//1s内方波个数，即灯光频率
	unsigned char led_color1 = 1;//led1灯光颜色控制
	unsigned char led_color2 = 1;//led2灯光颜色控制
	unsigned char led_bright = 0xff;//led1方波占空比
	uart_init();
  T0_init();				//定时器0初始化
  INT1_init();			//外部中断1初始化
	pca9633_init();
	while(1)
	{
		choose = 0;
		if((choose_uart = uart_rev_deal()) != 0)
		{
			choose = choose_uart;
		}
		if((choose_ir = ir_choose()) != 0)
		{
			choose = choose_ir;
		}
		uart_send_ledState(choose, led_color1, led_color2, freq, led_bright);
	  switch(choose)
		{
			case 1:
			{
			  write9633(0xC0, PWM0, 100); //红  // 100 数值是灯的亮度 ， 根据要求可以改变 
				write9633(0xC0, PWM1, 0);	 //绿
				write9633(0xC0, PWM2, 100);  
				write9633(0xC2, PWM0, 100); //红	 // 100 数值是灯的亮度 ， 根据要求可以改变 
				write9633(0xC2, PWM1, 0);	 //绿
				write9633(0xC2, PWM2, 100);
				break;
		  }
			case 2:
			{
				write9633(0xC0, GRPPFREQ, freq);
				write9633(0xC2, GRPPFREQ, freq);
        freq = freq + 9;
        if(freq == 45)
        {
          freq = 0;
        }					
        break;				
		  }
			case 3:
      {
				led_color(0xC0, led_color1);
				led_color1++;
				if(led_color1 == 4)
				{
					led_color1 = 1;
				}
        break;				
      }
			case 4:
			{
        led_color(0xC2, led_color2);
				led_color2++;
				if(led_color2 == 4)
				{
					led_color2 = 1;
				}
				break;
		  }
			case 5:
			{
				write9633(0xC0, GRPPWM, led_bright);
				write9633(0xC2, GRPPWM, led_bright);
				if(led_bright == 5)
				{
          led_bright = 0xff;
        }	
        led_bright = led_bright - 50;	
        break;				
      }
			case 6:
			{
			breath_led();
				break;
			}
			default : break;
		}	
	}
	return 0;
}