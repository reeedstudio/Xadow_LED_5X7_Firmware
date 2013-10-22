/*
  Xadow_LED.h
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

#ifndef __XADOW_LED__
#define __XADOW_LED__

#include "Xadow_LED_dfs.h"

class Xadow_LED{

public:
    uchar disp_dta[7];
    int ctrl_bit;

public:

    void begin();
    void io_init();
    void set_n(uchar n, uchar st);
    void set_n(uchar dta);
    void set_p(uchar p, uchar st);
    void set_p(uchar dta);
    
    void getMatrix(uchar *matrix, char asc);
    
    // for debug
    void setDispDta(uchar *dta);
   
};

extern Xadow_LED xadowLed;

#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/