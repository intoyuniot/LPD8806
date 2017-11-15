/*
该例程展示如何控制LPD8806 RGB LED 灯条
*/

#include <LPD8806.h>

//灯条像素点个数
int nLEDs = 160;

int dataPin  = D0;
int clockPin = D1;

LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

// 逐个像素点显示
void colorChase(uint32_t c, uint8_t wait)
{
    int i;

    // 熄灭所有像素点
    for(i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);

    // 逐个像素点显示
    for(i=0; i<strip.numPixels(); i++)
    {
        strip.setPixelColor(i, c);
        strip.show();
        strip.setPixelColor(i, 0);
        delay(wait);
    }

    strip.show();
}

void setup()
{
    strip.begin();

    strip.show();
}

void loop()
{
    colorChase(strip.Color(127,  0,  0), 100); // 红色
    colorChase(strip.Color(  0,127,  0), 100); // 绿色
    colorChase(strip.Color(  0,  0,127), 100); // 蓝色
    colorChase(strip.Color(127,127,127), 100); // 白色
}
