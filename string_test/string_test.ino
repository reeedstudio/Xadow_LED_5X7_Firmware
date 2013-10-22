// test
#include <Xadow_LED_dfs.h>
#include <Streaming.h>
#include <Xadow_LED.h>
#include <TimerOne.h>
#include <font.h>



void dispChar(char asc)
{
    unsigned char disp[7];
    for(int i=0; i<7; i++)
    {
        int tmp = (asc-32)*7+i;
        disp[i] = pgm_read_byte(&Font5x7[tmp]); 
    }
    
    xadowLed.setDispDta(disp);
}

void dispCharSlide(char asc, int ts)
{
    unsigned char disp[7];
    for(int i=0; i<7; i++)
    {
        int tmp = (asc-32)*7+i;
        disp[i] = pgm_read_byte(&Font5x7[tmp]); 
    }
    
    
    for(int i=0; i<5; i++)
    {
    
        for(int j=0; j<7; j++)
        {
            disp[j] = disp[j]<<1;
        }
        
        xadowLed.setDispDta(disp);
        
        delay(ts/5);
    }    
}


void putIntMatrix(unsigned int *matrix)
{
    unsigned char mat[7];
    for(int i=0; i<7; i++)
    {
        mat[i] = matrix[i]>>8;
    }
    
    xadowLed.setDispDta(mat);
}

void dispStringSlide(char *str, int ts)
{

    uchar len = strlen(str);
    
    if(len > 20)return ;
    
    unsigned char matrix[140];
    unsigned int matrix_i[7];
    
    for(int i=0; i<len; i++)
    {
        xadowLed.getMatrix(&matrix[7*i], str[i]);
    }
    
    for(int i=0; i<len; i++)
    {

        for(int j=0; j<7; j++)
        {
            matrix_i[j] = matrix[7*i+j];
            matrix_i[j] <<= 8;
            matrix_i[j] += matrix[7*(i+1)+j];
        }
        
        for(int k=0; k<8; k++)
        {
            putIntMatrix(matrix_i);
            delay(ts/5);
            
            for(int m=0; m<7; m++)
            {
                matrix_i[m] <<= 1;
            }
        }
    }
}


void setup()
{
    Serial.begin(115200);
    xadowLed.begin();
}

void loop()
{

    
    dispStringSlide("hello world!!", 800);
    
}