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

#ifndef __I2C_H
#define __I2C_H

#include <AT89X51.H>
#include <INTRINS.H>

typedef unsigned char ubyte;

/*
****************************************************
*       I2C Bus Configuration
*
****************************************************
*/

#define sda P1_0
#define scl P1_1
#define HOLD _nop_(); _nop_(); _nop_()

void send_stop();
void master(ubyte);
void send_byte(ubyte);
void send_data(ubyte, ubyte, ubyte, ubyte *);
ubyte recv_byte(ubyte);
void recv_data(ubyte, ubyte, ubyte, ubyte *);

#endif