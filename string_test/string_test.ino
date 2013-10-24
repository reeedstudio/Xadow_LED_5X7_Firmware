// test
#include <Xadow_LED_dfs.h>
#include <Streaming.h>
#include <Xadow_LED.h>
#include <TimerOne.h>
#include <font.h>


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
    
    unsigned char matrix[154];
    unsigned int matrix_i[7];
    
    memset(matrix, 0, 147);
    
    for(int i=0; i<len; i++)
    {
        xadowLed.getMatrix(&matrix[7*(i+1)], str[i]);
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
            delay(ts/5);
            
            for(int m=0; m<7; m++)
            {
                matrix_i[m] <<= 1;
            }
        }
    } 
}


void dispStringSlide(uchar cycle, int ts, int len_, uchar *str)
{

    while(1)
    {

        cout << "cycle = " << cycle << endl;
        cout << "ts = " << ts << endl;
        cout << "len = " << len_ << endl;
        
        
        int len = len_;
        if(len > 20)return ;
        
        unsigned char matrix[154];
        unsigned int matrix_i[7];
        
        memset(matrix, 0, 147);
        
        for(int i=0; i<len; i++)
        {
            xadowLed.getMatrix(&matrix[7*(i+1)], str[i]);
        }
        
        len++;
        
        for(int i=0; i<len-1; i++)
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
                
               /* if(cmd_get)
                {
                    cmd_get = 0;
                    return ;
                }*/
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


void setup()
{
    Serial.begin(115200);
    xadowLed.begin();
}

void loop()
{

    
    //dispStringSlide("hello world!!", 500);
    
    dispStringSlide(1, 500, 11, (unsigned char *)"hello world");
    
}