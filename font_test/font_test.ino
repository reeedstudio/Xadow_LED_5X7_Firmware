// test
#include <Xadow_LED_dfs.h>
#include <Streaming.h>
#include <Xadow_LED.h>
#include <TimerOne.h>
#include <font.h>

    
void dispChar(char asc)
{
    unsigned char disp[7];
    xadowLed.getMatrix(disp, asc);
    xadowLed.setDispDta(disp);
}

void setup()
{
    Serial.begin(115200);
    xadowLed.begin();
}

void loop()
{

    for(int i='a'; i<='z'; i++)
    {
    
        dispChar(i);
    
        delay(1000);
    }
    
}