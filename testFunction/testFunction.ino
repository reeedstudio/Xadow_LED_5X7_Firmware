#include <Wire.h>
#include <Streaming.h>

#define CMD_SETMODE     0x01
#define CMD_SLEEP       0x02
#define CMD_DISPMATRIX  0x03
#define CMD_DISPCHAR    0x04
#define CMD_DISPSTR     0x05
#define CMD_DISPOINT    0x06

#define STR_ONCE        0x01
#define STR_LOOP        0x02

#define DIR_NORMAL      0x00
#define DIR_LEFT        0x01
#define DIR_RIGHT       0x02
#define DIR_DOWN        0x03



int len=8;

unsigned char dta[50] = {0x03, 
                         0b10000,
                         0b01000,
                         0b10000,
                         0b00000,
                         0b00000,
                         0b00000,
                         0b00000,
                         };
                         
                         
                         
unsigned char dta2[5] = {

0b1000000,
0b0100000,
0b1000000,
0b0000000,
0b0000001,
};

void sendString(unsigned char cycle, int ts, char *str)
{
    int lend = strlen(str);
    
    Wire.beginTransmission(4); // transmit to device #4
    
    Wire.write(CMD_DISPSTR);
    
    Wire.write(cycle);
    Wire.write(ts>>8);
    Wire.write(ts);
    Wire.write(lend);
    for(int i=0; i<lend; i++)
    Wire.write(str[i]);        // sends five bytes
    Wire.endTransmission();    // stop transmitting
}


void sendPoint()
{
    Wire.beginTransmission(4); // transmit to device #4
    
    for(int i=0; i<len; i++)
    Wire.write(dta[i]);        // sends five bytes
    
    Wire.endTransmission();    // stop transmitting
}

void sendPoint2()
{
    Wire.beginTransmission(4); // transmit to device #4
    
    Wire.write(CMD_DISPMATRIX);
    for(int i=0; i<5; i++)
    Wire.write(dta2[i]);        // sends five bytes
    
    Wire.endTransmission();    // stop transmitting
}

void sendChar(char c)
{
    Wire.beginTransmission(4); // transmit to device #4
    
    Wire.write(CMD_DISPCHAR);
    Wire.write(c);        // sends five bytes
    
    Wire.endTransmission();    // stop transmitting
}

void setDir(unsigned char dir)
{

    Wire.beginTransmission(4); // transmit to device #4

    Wire.write(CMD_SETMODE);
    Wire.write(dir);
    
    Wire.endTransmission();    // stop transmitting
}

void setPoint(int x, int y, int state)
{
    Wire.beginTransmission(4); // transmit to device #4

    Wire.write(CMD_DISPOINT);
    Wire.write(x);
    Wire.write(y);
    Wire.write(state);
    Wire.endTransmission();    // stop transmitting
}


void disPoint()
{
    setPoint(0, 0, 1);
    delay(200);
    setPoint(1, 1, 1);
}

void dispCmd()
{
    cout << endl << endl;
    
    cout << "***********************************" << endl;
    
    cout << "1: display matrix" << endl;
    cout << "2: string once" << endl;
    cout << "3: string loop" << endl;
    cout << "4: display char" << endl;
    cout << "5: set direction: DIR_DOWN" << endl;
    cout << "6: set direction: DIR_NORMAL" << endl;
    cout << "7: set direction: DIR_RIGHT" << endl;
    cout << "8: set direction: DIR_LEFT" << endl;
    cout << "9: display matrix: DIRRIGHT" << endl;
    cout << "A: set point test" << endl;
    cout << "S: goto sleed" << endl;
    cout << "W: wake up" << endl;
    cout << "***********************************" << endl << endl;
}

void gotoSleep()
{
    Wire.beginTransmission(4); // transmit to device #4
    Wire.write(CMD_SLEEP);
    Wire.endTransmission();    // stop transmitting
}

void wakeUp()
{
    Wire.beginTransmission(4); // transmit to device #4
    Wire.write(0x55);
    Wire.endTransmission();    // stop transmitting
}

void setup()
{
    Serial.begin(115200);
    Serial.println("hello world");
    
    Wire.begin();
    
    
    dispCmd();

}

void dtaProc(unsigned char cmd)
{
    switch(cmd)
    {
        case '1':
        
        cout << "dispoint" << endl;
        sendPoint();
        
        break;
        case '2':
        
        cout << "string once" << endl;
        sendString(STR_ONCE, 400, "hello world");   // once
        break;
        
        case '3':
        
        cout << "string loop" << endl;
        sendString(STR_LOOP, 400, "hello world");   // once
        break;
        
        case '4':
        
        cout << "disp char" << endl;
        sendChar('A');
        break;
        
        
        case '5':
        
        cout << "set direction: DIR DOWN" << endl;
        setDir(DIR_DOWN);
        
        break;
        
        case '6':
        
        cout << "set direction: DIR NORMAL" << endl;
        setDir(DIR_NORMAL);
        
        break;
        
        case '7':
        
        cout << "set direction: DIR RIGHT" << endl;
        setDir(DIR_RIGHT);
        
        break;
        
        case '8':
        
        cout << "set direction: DIR LEFT" << endl;
        setDir(DIR_LEFT);
        
        break;
        
        
        case '9':
        
        cout << "matrix - dir right" << endl;
        
        sendPoint2();
        
        break;
        
        case 'A':
        
        cout << "test set point" << endl;
        
        
        disPoint();
        
        break;
        
        case 'S':
        
        cout << "goto sleep" <<endl; 
        gotoSleep();
        break;
        
        case 'W':
        
        cout << "wake up" << endl;
        wakeUp();
        break;
        
        default:
        ;
    }
    
    
    dispCmd();
}

void loop()
{

    while(Serial.available())
    {
        unsigned char c = Serial.read();
        Serial.write(c);
        dtaProc(c);
    }
    delay(1);
}

