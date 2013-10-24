// firmware of xadow led 5x7
#include <Xadow_LED.h>
#include <Streaming.h>
#include <Wire.h>
#include <Xadow_LED_dfs.h>
#include <TimerOne.h>
#include <Sleep_n0m1.h>

#define __Debug         1

#if __Debug
#define DBG(X)  Serial.println(X)
#else
#define DBG(X)
#endif

#define ADDRESS     0x04

Sleep sleep;

unsigned char dtaLen = 0;
unsigned char dtaGet[30];
unsigned char cmd_flg = 0;                  // if get right cmd

void cmd_proc(int cmd)
{
    int ts_ = 0;
    
    switch(cmd)
    {
        //***********************set mode*******************************
        case CMD_SETMODE:
        
        xadowLed.clear();
        xadowLed.setDirDisp(dtaGet[1] & 0x03);
        
        break;
        //***********************goto sleep*****************************
        case CMD_SLEEP:
        
        cout << "sleep" << endl;
        
        xadowLed.clear();
        delay(100);
        //sleep.pwrDownMode(); //set sleep mode
        //sleep.sleepInterrupt(0,FALLING); //sleep for: sleepTime
        
        cout << "wake" << endl;
        
        break;
        
        //***********************display matrix*************************
        case CMD_DISPMATRIX:

        DBG("display matrix");
        xadowLed.clear();
        xadowLed.dispMatrix(&dtaGet[1]);
        
        
        break;
        
        //***********************display char***************************
        case CMD_DISPCHAR:
        
        DBG("display char");
        xadowLed.clear();
        xadowLed.dispChar(dtaGet[1]);
        
        break;
        //***********************display string*************************
        case CMD_DISPSTR:
        
        DBG("display string");
        
        ts_ = dtaGet[2];
        ts_ = (ts_<<8) + dtaGet[3];
        
        xadowLed.clear();
        xadowLed.dispStringSlide(dtaGet[1], ts_, dtaGet[4], &dtaGet[5]);
        
        break;
        
        
        //***********************display point*************************
        case CMD_DISPOINT:
        
        DBG("display point");

        xadowLed.setPoint(dtaGet[1], dtaGet[2], dtaGet[3]);
        
        
        break;
        
        //***********************default********************************
        default:
        ;

    }
}

void setup()
{
    Serial.begin(115200);

    cout << "restart!" << endl;
    
    xadowLed.begin();
    Wire.begin(ADDRESS);  
    Wire.onReceive(receiveEvent); // register event
    
}

void loop()
{
    if(cmd_flg)
    {
        cmd_flg = 0;
        dtaLen  = 0;
        cmd_proc(dtaGet[0]);
    }
}

void receiveEvent(int howMany)
{

    while(Wire.available())
    {
        dtaGet[dtaLen++] = Wire.read();
        Serial.write(dtaGet[dtaLen-1]);
    }

    switch(dtaGet[0])
    {
        case CMD_SETMODE:
        
        //cout << ""
        if(dtaLen != 2)goto ERR;
        
        break;
        
        case CMD_SLEEP:
        
        if(dtaLen != 1)goto ERR;
        
        break;
        
        case CMD_DISPMATRIX:
        
        if(DIR_NORMAL == xadowLed.getDirDisp() || DIR_DOWN == xadowLed.getDirDisp())
        {
            if(dtaLen != 8)goto ERR;
        }
        else
        {
            if(dtaLen != 6)goto ERR;
        }
        
        break;
        
        case CMD_DISPCHAR:
        
        if(dtaLen != 2)goto ERR;
        
        break;
        
        case CMD_DISPSTR:
        
        if(dtaLen < 6)goto ERR;
        if(dtaLen != dtaGet[4]+5) goto ERR;

        break;
        
        case CMD_DISPOINT:

        if(4 != dtaLen)goto ERR;    
        //if(dtaGet[1]>7 || dtaGet[2] > 7 || dtaGet[3]>1)goto ERR;
        
        break;
        
        default:
        
        goto ERR;
    }
    
    cmd_flg = 1;
    xadowLed.setCmdGet(1);
    
    return ;
    
ERR:

    dtaLen  = 0;
    cmd_flg = 0;
    //xadowLed.setCmdGet(0);
    DBG("get wrong dta");
    
    
}