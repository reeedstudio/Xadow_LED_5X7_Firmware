// control led

#include <Streaming.h>
#include <TimerOne.h>


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
                        

int ctrl_bit = 0;

uchar disp_dta[7] =               // display string
{
    0b10100,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00011,
};

void io_init()
{
    DDRB |= BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4);
    DDRC |= BIT(0)|BIT(1)|BIT(2)|BIT(3);
    DDRD |= BIT(4)|BIT(5)|BIT(6);
    
    PORTB &= ~(BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4));
    PORTC &= ~(BIT(0)|BIT(1)|BIT(2)|BIT(3));
    PORTD &= ~(BIT(4)|BIT(5)|BIT(6));
}

void set_n(uchar n, uchar st)               // negetive
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

void set_n(uchar dta)
{
    PORTC &= 0b11110000;
    PORTC |= dta&(0b00001111);
    
    PORTD &= 0b11101111;
    PORTD |= dta&(0b00010000);
}

void set_p(uchar p, uchar st)               // positive
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

void set_p(uchar dta)
{

    PORTB &= 0b11100000;
    PORTD &= 0b10011111;
    
    PORTB |= dta&(0b00011111);
    PORTD |= dta&(0b01100000);
}

void timerIsr()
{
    LOW_P_ALL();
    set_n(~disp_dta[6-ctrl_bit]);
    set_p(BIT(ctrl_bit++));
    
    ctrl_bit = ctrl_bit>6 ? 0 : ctrl_bit;
}

void setup()
{   
    Serial.begin(115200);
    io_init();
    cout << "hello world" << endl;
	
    HIGH_P_ALL();
    HIGH_N_ALL();
    
    Timer1.initialize(1000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
    Timer1.attachInterrupt( timerIsr ); // attach the service routine here
}

void loop()
{
    delay(1000);
    
}