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
#include <Streaming.h>

#include "Xadow_LED.h"
#include "font.h"

void timerIsr()
{
    xadowLed.timer_();
}

void Xadow_LED::timer_()
{
    LOW_P_ALL();
    xadowLed.set_n(~xadowLed.disp_dta[6-xadowLed.ctrl_bit]);
    xadowLed.set_p(BIT(xadowLed.ctrl_bit++));
    xadowLed.ctrl_bit = xadowLed.ctrl_bit>6 ? 0 : xadowLed.ctrl_bit;
}

void Xadow_LED::begin()
{
    ctrl_bit = 0;
    cmd_get  = 0;
    dirDisp  = DIR_NORMAL;

    clear();
    
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

void Xadow_LED::putIntMatrix(unsigned int *matrix)
{
    unsigned char mat[7];
    for(int i=0; i<7; i++)
    {
        mat[i] = matrix[i]>>8;
    }
    setDispDta(mat);  
    if(DIR_DOWN != dirDisp)return ;
    matrixRev();

}

void Xadow_LED::dispChar(char c)
{
    if(DIR_NORMAL != dirDisp && DIR_DOWN != dirDisp)return ;
    getMatrix(disp_dta, c);
    setDispDta(disp_dta);
    if(DIR_DOWN != dirDisp)return ;
    matrixRev();
}

void Xadow_LED::dispStringSlide(uchar cycle, int ts, int len_, uchar *str)
{
    if(DIR_NORMAL != dirDisp && DIR_DOWN != dirDisp)return ;
    
    cmd_get = 0;
    while(1)
    {

        int len = len_;
        
        if(len > 20)return ;
        
        unsigned char matrix[154];
        unsigned int matrix_i[7];
        
        memset(matrix, 0, 147);
        
        for(int i=0; i<len; i++)
        {
            getMatrix(&matrix[7*(i+1)], str[i]);
        }
        
        len++;
        
        for(int i=0; i<len; i++)
        {
            for(int j=0; j<7; j++)
            {
                matrix_i[j] = matrix[7*i+j];
                matrix_i[j] <<= 8;
                matrix_i[j] += matrix[7*(i+1)+j]<<2;
            }
            
            for(int k=0; k<6; k++)
            {
                putIntMatrix(matrix_i);
                
                if(cmd_get)
                {
                    cmd_get = 0;
                    return ;
                }
                delay(ts/5);
                
                for(int m=0; m<7; m++)
                {
                    matrix_i[m] <<= 1;
                }
            }
        } 
        
        if (STR_ONCE == cycle)return;
    }
}

uchar Xadow_LED::byteRev(uchar dta)
{
    uchar tmp = 0;  

    for(int i=0; i<5; i++)
    {
        if(dta & BIT(i))
        {
            tmp += BIT(4-i);
        }
    }
    return tmp;
}

void Xadow_LED::matrixRev()
{
    
    
    uchar mat_tmp[7];
    
    for(int i=0; i<7; i++)
    {
        mat_tmp[i] = disp_dta[i];
    }

    for(int i=0; i<7; i++)
    {
        disp_dta[i] = byteRev(mat_tmp[6-i]);
    }
}

void Xadow_LED::dispMatrix(uchar *mat)
{

    if(DIR_DOWN == dirDisp)
    {
        for(int i=0; i<7; i++)
        {
            disp_dta[i] = mat[i];
        }
        
        matrixRev();
    }
    else if(DIR_NORMAL == dirDisp)
    {
        for(int i=0; i<7; i++)
        {
            disp_dta[i] = mat[i];
        }
    }
    else if(DIR_RIGHT == dirDisp)
    {
        for(int i=0; i<7; i++)
        {
            for(int j=0; j<5; j++)
            {
                if(mat[j] & BIT(i))
                {
                    disp_dta[6-i] += BIT(j);
                }
            }
        }
    }
    else if(DIR_LEFT == dirDisp)
    {
        for(int i=0; i<7; i++)
        {
            for(int j=0; j<5; j++)
            {
                if(mat[j] & BIT(i))
                {
                    disp_dta[6-i] += BIT(j);
                }
            }
        }
        
        matrixRev();
    } 
    
}

void Xadow_LED::setPoint(uchar x, uchar y, uchar state)
{

    if(DIR_DOWN == dirDisp)
    {
        disp_dta[6-y] &= ~BIT(x);
        disp_dta[6-y] |= state ? BIT(x) : 0;
    }
    else if(DIR_NORMAL == dirDisp)
    {
        disp_dta[y] &= ~BIT(4-x);
        disp_dta[y] |= state ? BIT(4-x) : 0;
    }
    else if(DIR_RIGHT == dirDisp)
    {
        //disp_dta[x] &= ~BIT(7-y);
        //disp_dta[x] |= state ? BIT(7-y) : 0;
    }
    else if(DIR_LEFT == dirDisp)
    {
        //disp_dta[7-x] &= ~BIT(y);
        //disp_dta[7-x] |= state ? BIT(y) : 0;
    } 
}

Xadow_LED xadowLed;


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/