Xadow_LED_5X7_Firmware
======================

这是Xadow 5X7 LED 固件的Sketchbook，Sketchbook的使用，参考：http://www.seeedstudio.com/wiki/How_To_Use_Sketchbook

器件地址：0x04

寄存器地址：

1. 设置模式

     SETMODE：0x01
     最后两位表示显示屏的方向：
     00：NORMAL
     01：LEFT
     10：RIGHT
     11：DOWN

2. 睡眠

     SLEEP: 0x02
     
     只要发送0x02，屏进入睡眠模式

3. 显示点阵
     DISPMATRIX: 0x03
     如果direction 为NORMAL和DOWN， 后面跟着7个字节
     否则后面跟着5个字节。

4. 显示字符
     DISPCHAR: 0x04
     后面跟着一个字符

5. 显示字符串

     DISPCHAR：0x05
     后面跟着显示模式：
     0x01: 单次显示
     0x02: 循环显示

     接着两个字节，表示每个字显示的时间， 单位ms

     接着LEN，表示字符串长度, LEN < 20

     接着字符串字符

6. 显示单个点
     DISPOINT: 0x06

     后面跟着x, y, dta
     dta: 1 亮
            0 咩   
     


