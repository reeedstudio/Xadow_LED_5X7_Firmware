/*
  Xadow_LED.cpp
  2013 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee
  2013-10-21
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Arduino.h>
#include <TimerOne.h>

#include "Xadow_LED.h"
#include "font.h"

void timerIsr()
{
#if 0
    LOW_P_ALL();
    xadowLed.set_n(~BIT(xadowLed.ctrl_bit));
    xadowLed.set_p(xadowLed.disp_dta[4-xadowLed.ctrl_bit]);
    
    xadowLed.ctrl_bit++;
    xadowLed.ctrl_bit = xadowLed.ctrl_bit>4 ? 0 : xadowLed.ctrl_bit;
#else
    LOW_P_ALL();
    xadowLed.set_n(~xadowLed.disp_dta[6-xadowLed.ctrl_bit]);
    xadowLed.set_p(BIT(xadowLed.ctrl_bit++));
    
    xadowLed.ctrl_bit = xadowLed.ctrl_bit>6 ? 0 : xadowLed.ctrl_bit;
    
#endif
}

void Xadow_LED::begin()
{
    ctrl_bit = 0;

#if 0
    uchar dtaTmp[5]=
    {    
        0b0101000,
        0b0100000,
        0b0000000,
        0b0000000,
        0b0000000,
    };
    
    for(int i=0; i<5; i++)
    {
        disp_dta[i] = dtaTmp[i];
    }
    
#else

    uchar dtaTmp[7]=
    {    
        0b01000,
        0b10000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b11000,
    };
    
    for(int i=0; i<7; i++)
    {
        disp_dta[i] = dtaTmp[i];
    }
#endif

    io_init();
    Timer1.initialize(1000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
    Timer1.attachInterrupt( timerIsr ); // attach the service routine here
}

void Xadow_LED::setDispDta(uchar *dta)
{
    
    if(NULL == dta)return;
    
    for(int i=0; i<7; i++)
    {
        disp_dta[i] = dta[i];
    }
}

void Xadow_LED::io_init()
{
    DDRB |= BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4);
    DDRC |= BIT(0)|BIT(1)|BIT(2)|BIT(3);
    DDRD |= BIT(4)|BIT(5)|BIT(6);
    
    PORTB &= ~(BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4));
    PORTC &= ~(BIT(0)|BIT(1)|BIT(2)|BIT(3));
    PORTD &= ~(BIT(4)|BIT(5)|BIT(6));
}

void Xadow_LED::set_n(uchar n, uchar st)               // negetive
{
    
    if(n>4)return;
    
    n = 4-n;
    
    if(st)
    {
        if(n<4)
        {
            PORTC |= BIT(n);
        }
        else
        {
            PORTD |= BIT(n);
        }
        
    }
    else
    {
        if(n<4)
        {
            PORTC &= ~BIT(n);
        }
        else
        {
            PORTD &= ~BIT(n);
        }
    }
}

void Xadow_LED::set_n(uchar dta)
{
    PORTC &= 0b11110000;
    PORTC |= dta&(0b00001111);
    
    PORTD &= 0b11101111;
    PORTD |= dta&(0b00010000);
}

void Xadow_LED::set_p(uchar p, uchar st)               // positive
{
    if(p>6)return;
    
    p = 6-p;

    if(st)
    {
        if(p<5)
        {
            PORTB |= BIT(p);
        }
        else
        {
            PORTD |= BIT(p);
        }
    }
    else
    {
        if(p<5)
        {
            PORTB &= ~BIT(p);
        }
        else
        {
            PORTD &= ~BIT(p);
        }
    }
}

void Xadow_LED::set_p(uchar dta)
{

    PORTB &= 0b11100000;
    PORTD &= 0b10011111;
    
    PORTB |= dta&(0b00011111);
    PORTD |= dta&(0b01100000);
}


void Xadow_LED::getMatrix(uchar *matrix, char asc)
{
    if(NULL == matrix)return;
    
    for(int i=0; i<7; i++)
    {
        int tmp = (asc-32)*7+i;
        matrix[i] = pgm_read_byte(&Font5x7[tmp]); 
    }
}

Xadow_LED xadowLed;


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/