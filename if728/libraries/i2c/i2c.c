/*
***************************************************
*
*               I2C Library
*
*        For Keil C, with features of bus fault
*        detection, no acknowledgement feature,
*        and I2C busy feature.
*
*        I2C Data rate: 62,361 Bps
*
*        Written By: Ajay Bhargav
*        E-mail:     bhargav.ajay@gmail.com
*        website:    www.rickeyworld.tk
*
*  NOTE: It may or maynot be used with multi-master
*        Multislave confuguration
*        
*        Tested with multi-slave configuration
*
****************************************************
*  Having a doubt..?
*  post it on the rickey's forum @ Rickey's World
*  Visit www.rickeyworld.tk
*
****************************************************
*/

#include "I2C.H"

bit i2c_busy, no_ack, bus_fault;

ubyte bdata a;

sbit LSB=a ^ 0;
sbit MSB=a ^ 7;

/*
************************************
*       Sending Stop Condition
************************************
*/

void send_stop()
{
	sda = 0;
	scl = 1;
	HOLD;
	sda = 1;
	i2c_busy = 0;
}

/*
************************************
*      I2C Start Condition
*
*     NOTE: need to use it.
************************************
*/

void master(ubyte slave_addr)
{
	i2c_busy = 1;
	no_ack = 0;
	bus_fault = 0;
	if(!scl||!sda)
		bus_fault = 1;
	else
	{
		sda = 0;
		HOLD;
		scl = 0;
		HOLD;
		send_byte(slave_addr);
	}
}

/*
************************************
*     Sending a byte on I2C Bus
************************************
*/

void send_byte(ubyte i2c_data)
{
	ubyte i;
	a=i2c_data;
	for(i=0;i<8;i++)
	{
		scl=0;
		HOLD;
		sda=MSB;
		a<<=1;
		_nop_();
		scl=1;
		HOLD;
		scl=0;
	}
	sda = 1;
	scl = 1;
	HOLD;
	if(!sda)
	{
		scl = 0;
		HOLD;
	}
	else
	{
		no_ack = 1;
		scl = 0;
		HOLD;
	}
}

/*
****************************************************
*       Sending data on I2C bus
*
*   Usage:
*         send_data(0xD0,0x10, 0x20, send_buffer)
*
*         0XD0: Slave address, Must me with write bit
*         0x10: Starting address, or sub-address
*         0x20: number of bytes to send
*         send_buffer: adress of the buffer pointer
*
*****************************************************
*/

void send_data(ubyte slave_add, ubyte sub_add, ubyte bytes, ubyte *data_in)
{
	master(slave_add);
	send_byte(sub_add);
	if(no_ack)
		send_stop();
	else
	{
		for(bytes;bytes>0;bytes--)
		{
			send_byte(*data_in++);
			if(no_ack)
				break;
		}
		send_stop();
	}
}

/*
*********************************************
*      Recieve a single byte from I2C Bus
*
*    Note: if you are going to recieve
*          a single byte then the passing
*          argument should be 1.
*
*********************************************
*/

ubyte recv_byte(ubyte cnt)
{
	ubyte i,rcv_data;
	for(i=0;i<8;i++)
	{
		sda=1;
		scl=1;
		HOLD;
		LSB=sda;
		if(i<7)
			a<<=1;
		HOLD;
		scl=0;
		HOLD;
	}
	if(cnt==1)
		sda = 1;
	else
		sda = 0;
	scl =1;
	HOLD;
	scl = 0;
	sda = 1;
	HOLD;
	rcv_data = a;
	return rcv_data;
}

/*
****************************************************
*       Recieving bulk data on I2C bus
*
*   Usage:
*         recv_data(0xD0,0x10, 0x20, send_buffer)
*
*         0XD0: Slave address, Must me with write bit
*         0x10: Starting address, or sub-address
*         0x20: number of bytes to recieve
*         send_buffer: adress of the recieve buffer
*                      pointer
*
*****************************************************
*/

void recv_data(ubyte slave_add, ubyte sub_add, ubyte byte_cnt, ubyte *recv_buf)
{
	ubyte i;
	scl = sda = 1;
	master(slave_add);
	send_byte(sub_add);
	slave_add+=1;
	scl = sda = 1;
	master(slave_add);
	if(no_ack)
	{
		send_stop();
		goto exit;
	}
	for(i=0;i<byte_cnt;i++)
		recv_buf[i]=recv_byte(byte_cnt-i);
	send_stop();
exit:;
}