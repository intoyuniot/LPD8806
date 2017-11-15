/*
该例程展示如何控制LPD8806 RGB LED 灯条
*/

#include <LPD8806.h>

// 灯条像素点个数
int nLEDs = 32;

int dataPin  = D0;
int clockPin = D1;


LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

//输入一个0-384数字获取一个颜色值
//颜色值从红-绿-蓝-黑-红分布
uint32_t Wheel(uint16_t WheelPos)
{
    byte r, g, b;
    switch(WheelPos / 128)
    {
        case 0:
            r = 127 - WheelPos % 128; // 红色下调
            g = WheelPos % 128;       // 绿色上调
            b = 0;                    // 蓝色关闭
            break;
        case 1:
            g = 127 - WheelPos % 128; // 绿色下调
            b = WheelPos % 128;       // 蓝色上调
            r = 0;                    // 红色关闭
            break;
        case 2:
            b = 127 - WheelPos % 128; // 蓝色下调
            r = WheelPos % 128;       // 红色上调
            g = 0;                    // 绿色关闭
            break;
    }
    return(strip.Color(r,g,b));
}

//彩虹效果
void rainbowCycle(uint8_t wait)
{
    uint16_t i, j;

    for (j=0; j < 384 * 5; j++)
    {
        for (i=0; i < strip.numPixels(); i++)
        {
            strip.setPixelColor(i, Wheel( ((i * 384 / strip.numPixels()) + j) % 384) );
        }
        strip.show();
        delay(wait);
    }
}

// 逐个填充给定的颜色
void colorWipe(uint32_t c, uint8_t wait)
{
    int i;

    for (i=0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

// 逐个点亮灯条像素点
void colorChase(uint32_t c, uint8_t wait)
{
    int i;

    // 关闭所有像素点
    for(i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);

    // 每次显示一个像素点
    for(i=0; i<strip.numPixels(); i++)
    {
        strip.setPixelColor(i, c);
        strip.show();
        strip.setPixelColor(i, 0);
        delay(wait);
    }

    strip.show();
}

//爬行灯效果
void theaterChase(uint32_t c, uint8_t wait)
{
    for (int j=0; j<10; j++)
    {
        for (int q=0; q < 3; q++)
        {
            for (int i=0; i < strip.numPixels(); i=i+3)
            {
                strip.setPixelColor(i+q, c);
            }
            strip.show();

            delay(wait);

            for (int i=0; i < strip.numPixels(); i=i+3)
            {
                strip.setPixelColor(i+q, 0);
            }
        }
    }
}

//带彩虹效果的爬行灯
void theaterChaseRainbow(uint8_t wait)
{
    for (int j=0; j < 384; j++)
    {     // cycle all 384 colors in the wheel
        for (int q=0; q < 3; q++)
        {
            for (int i=0; i < strip.numPixels(); i=i+3)
            {
                strip.setPixelColor(i+q, Wheel( (i+j) % 384));
            }
            strip.show();

            delay(wait);

            for (int i=0; i < strip.numPixels(); i=i+3)
            {
                strip.setPixelColor(i+q, 0);
            }
        }
    }
}
//彩虹效果
void rainbow(uint8_t wait)
{
    int i, j;

    for (j=0; j < 384; j++)
    {
        for (i=0; i < strip.numPixels(); i++)
        {
            strip.setPixelColor(i, Wheel( (i + j) % 384));
        }
        strip.show();
        delay(wait);
    }
}

void setup()
{
    strip.begin();
    strip.show();
}


void loop()
{
    // colorChase 效果
    colorChase(strip.Color(127, 127, 127), 50); // 白色
    colorChase(strip.Color(127,   0,   0), 50); // 红色
    colorChase(strip.Color(127, 127,   0), 50); // 黄色
    colorChase(strip.Color(  0, 127,   0), 50); // 绿色
    colorChase(strip.Color(  0, 127, 127), 50); // 蓝绿色
    colorChase(strip.Color(  0,   0, 127), 50); // 蓝色
    colorChase(strip.Color(127,   0, 127), 50); // 紫色

    // theaterChase 效果
    theaterChase(strip.Color(127, 127, 127), 50); // 白色
    theaterChase(strip.Color(127,   0,   0), 50); // 红色
    theaterChase(strip.Color(127, 127,   0), 50); // 黄色
    theaterChase(strip.Color(  0, 127,   0), 50); // 绿色
    theaterChase(strip.Color(  0, 127, 127), 50); // 蓝绿色
    theaterChase(strip.Color(  0,   0, 127), 50); // 蓝色
    theaterChase(strip.Color(127,   0, 127), 50); // 紫色

    // colorWipe 效果
    colorWipe(strip.Color(127,   0,   0), 50);  // 红色
    colorWipe(strip.Color(  0, 127,   0), 50);  // 绿色
    colorWipe(strip.Color(  0,   0, 127), 50);  // 蓝色

    rainbow(10);
    rainbowCycle(0);
    theaterChaseRainbow(50);
}
