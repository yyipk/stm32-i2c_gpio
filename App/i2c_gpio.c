#include "i2c_gpio.h"

static void __delay_us (uint16_t us)
{
	I2C_GPIO_PLATFORM_DELAY_US(us);
}

//产生IIC起始信号
void i2c_start(void)
{
	SDA_OUT();  
	Set_IIC_SDA;	  	  
	Set_IIC_SCL;
	__delay_us(4);
 	Clr_IIC_SDA;
	__delay_us(4);
	Clr_IIC_SCL;
}	  

//产生IIC停止信号
void i2c_stop(void)
{
	SDA_OUT();//sda线输出
	Clr_IIC_SCL;
	Clr_IIC_SDA;//STOP:when CLK is high DATA change form low to high
 	__delay_us(4);
	Set_IIC_SCL; 
	Set_IIC_SDA;//发送I2C总线结束信号
	__delay_us(4);							   	
}

//等待应答信号到来
uint8_t i2c_ack_wait(void)
{
	uint16_t ucErrTime=0;
	SDA_IN();
	Set_IIC_SDA;__delay_us(1);	   
	Set_IIC_SCL;__delay_us(1);	 
	while(READ_SDA) {
		ucErrTime++;
		if(ucErrTime>3000) {
			i2c_stop();
			return 1;
		}
	}
	Clr_IIC_SCL;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void i2c_ack(void)
{
	Clr_IIC_SCL;
	SDA_OUT();
	Clr_IIC_SDA;
	__delay_us(2);
	Set_IIC_SCL;
	__delay_us(2);
	Clr_IIC_SCL;
}
//不产生ACK应答		    
void i2c_nack(void)
{
	Clr_IIC_SCL;
	SDA_OUT();
	Set_IIC_SDA;
	__delay_us(2);
	Set_IIC_SCL;
	__delay_us(2);
	Clr_IIC_SCL;
}					 				 


//IIC发送一个字节	  
void i2c_byte_send(uint8_t txd)
{                        
    uint8_t t;   
	  SDA_OUT(); 	    
    Clr_IIC_SCL;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
			if ((txd&0x80)>>7) {
				Set_IIC_SDA;
			} else {
				Clr_IIC_SDA;
			}
			txd<<=1; 	  
			__delay_us(2);   //对TEA5767这三个延时都是必须的
			Set_IIC_SCL;
			__delay_us(2); 
			Clr_IIC_SCL;	
			__delay_us(2);
    }	 
} 	   

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t i2c_byte_read(uint8_t ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
	
  for(i=0;i<8;i++ ) {
		Clr_IIC_SCL; 
		__delay_us(2);
		Set_IIC_SCL;
		receive<<=1;
		if(READ_SDA) {
			receive++;  
		} 
		__delay_us(1); 
	}					 
	
  if (!ack) {
		i2c_nack();//发送nACK
	} else {
		i2c_ack(); //发送ACK  
	}
	
  return receive;
}

void i2c_gpio_init ()
{
    i2c_gpio_platform_init();
}



























