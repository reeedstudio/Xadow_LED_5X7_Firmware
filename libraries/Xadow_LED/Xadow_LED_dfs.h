/*
  Xadow_LED_dfs.h
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

#ifndef __XADOW_LED_DFS__
#define __XADOW_LED_DFS__


#define uchar unsigned char

#define BIT(X)  (0x01<<X)


#define HIGH_P_ALL()    PORTB |= (BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4));\
                        PORTD |= (BIT(5)|BIT(6))
                    
#define LOW_P_ALL()     PORTB &= ~(BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4));\
                        PORTD &= ~(BIT(5)|BIT(6))

#define HIGH_N_ALL()    PORTC |= (BIT(0)|BIT(1)|BIT(2)|BIT(3));\
                        PORTD |= BIT(4)
                    
#define LOW_N_ALL()     PORTC &= ~(BIT(0)|BIT(1)|BIT(2)|BIT(3));\
                        PORTD &= ~BIT(4)
                        


#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/