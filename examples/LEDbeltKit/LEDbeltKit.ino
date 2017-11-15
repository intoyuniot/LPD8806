/*
该例程展示如何控制LPD8806 RGB LED 灯条
*/
#include <LPD8806.h>
#include <math.h>

const uint8_t dataPin = D0;
const uint8_t clockPin = D1;

// 第一个参数为灯条像素点的个数
LPD8806 strip = LPD8806(32, dataPin, clockPin);

void setup()
{
    // 启动LED 灯条
    strip.begin();

    // 更新灯条 ,上电默认关闭
    strip.show();
}

// function prototypes, do not remove these!
void colorChase(uint32_t c, uint8_t wait);
void colorWipe(uint32_t c, uint8_t wait);
void dither(uint32_t c, uint8_t wait);
void scanner(uint8_t r, uint8_t g, uint8_t b, uint8_t wait);
void wave(uint32_t c, int cycles, uint8_t wait);
void rainbowCycle(uint8_t wait);
uint32_t Wheel(uint16_t WheelPos);

void loop()
{
    // colorChase效果
    colorChase(strip.Color(127,127,127), 20); // 白色
    colorChase(strip.Color(127,0,0), 20);     // 红色
    colorChase(strip.Color(127,127,0), 20);   // 黄色
    colorChase(strip.Color(0,127,0), 20);     //  绿色
    colorChase(strip.Color(0,127,127), 20);   // 蓝绿色
    colorChase(strip.Color(0,0,127), 20);     // 蓝色
    colorChase(strip.Color(127,0,127), 20);   // 品红色

    colorWipe(strip.Color(127,0,0), 20);      //红色
    colorWipe(strip.Color(0, 127,0), 20);     // 绿色
    colorWipe(strip.Color(0,0,127), 20);      // 蓝色
    colorWipe(strip.Color(0,0,0), 20);        // 黑色

    // dither效果
    dither(strip.Color(0,127,127), 50);       // 蓝绿色, 慢
    dither(strip.Color(0,0,0), 15);           // 黑色, 快
    dither(strip.Color(127,0,127), 50);       // 品红色, 慢
    dither(strip.Color(0,0,0), 15);           // 黑色, 快
    dither(strip.Color(127,127,0), 50);       // 黄色, 慢
    dither(strip.Color(0,0,0), 15);           // 黑色, 快

    // scanner 效果
    scanner(127,0,0, 30);        // 红色, 慢
    scanner(0,0,127, 15);        // 蓝色, 快

    // 波动效果
    wave(strip.Color(127,0,0), 4, 20);
    wave(strip.Color(0,0,100), 2, 40);

    // 循环显示彩红效果
    rainbowCycle(0);

    // 重新开始先清屏
    for (int i=0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, 0);
    }
}

// 彩虹显示效果
void rainbowCycle(uint8_t wait)
{
    uint16_t i, j;

    for (j=0; j < 384 * 5; j++)
    {
        for (i=0; i < strip.numPixels(); i++)
        {
            strip.setPixelColor(i, Wheel(((i * 384 / strip.numPixels()) + j) % 384));
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

    for (i=0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, 0);
    }

    for (i=0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
        strip.setPixelColor(i, 0);
    }
    strip.show();
}

// 灯条将按有序颜色递变的方式显示。
void dither(uint32_t c, uint8_t wait)
{
    int hiBit = 0;
    int n = strip.numPixels() - 1;
    for(int bit=1; bit < 0x8000; bit <<= 1)
    {
        if(n & bit) hiBit = bit;
    }

    int bit, reverse;
    for(int i=0; i<(hiBit << 1); i++)
    {
        reverse = 0;
        for(bit=1; bit <= hiBit; bit <<= 1)
        {
            reverse <<= 1;
            if(i & bit) reverse |= 1;
        }
        strip.setPixelColor(reverse, c);
        strip.show();
        delay(wait);
    }
    delay(250);
}

// 显示效果
void scanner(uint8_t r, uint8_t g, uint8_t b, uint8_t wait)
{
    int i, j, pos, dir;

    pos = 0;
    dir = 1;

    for(i=0; i<((strip.numPixels()-1) * 8); i++)
    {
        strip.setPixelColor(pos - 2, strip.Color(r/4, g/4, b/4));
        strip.setPixelColor(pos - 1, strip.Color(r/2, g/2, b/2));
        strip.setPixelColor(pos, strip.Color(r, g, b));
        strip.setPixelColor(pos + 1, strip.Color(r/2, g/2, b/2));
        strip.setPixelColor(pos + 2, strip.Color(r/4, g/4, b/4));

        strip.show();
        delay(wait);

        for(j=-2; j<= 2; j++)
        strip.setPixelColor(pos+j, strip.Color(0,0,0));

        pos += dir;
        if(pos < 0)
        {
            pos = 1;
            dir = -dir;
        }
        else if(pos >= strip.numPixels())
        {
            pos = strip.numPixels() - 2;
            dir = -dir;
        }
    }
}

// 波动效果
#define PI 3.14159265
void wave(uint32_t c, int cycles, uint8_t wait)
{
    float y;
    byte  r, g, b, r2, g2, b2;

    // 分解颜色值
    g = (c >> 16) & 0x7f;
    r = (c >>  8) & 0x7f;
    b =  c        & 0x7f;

    for(int x=0; x<(strip.numPixels()*5); x++)
    {
        for(int i=0; i<strip.numPixels(); i++)
        {
            y = sin(PI * (float)cycles * (float)(x + i) / (float)strip.numPixels());
            if(y >= 0.0)
            {
                // 波峰颜色为白色
                y  = 1.0 - y; // 转换 Y to 0.0 (顶部) to 1.0 (中间)
                r2 = 127 - (byte)((float)(127 - r) * y);
                g2 = 127 - (byte)((float)(127 - g) * y);
                b2 = 127 - (byte)((float)(127 - b) * y);
            }
            else
            {
                //波谷颜色为黑色
                y += 1.0; // 转换 Y to 0.0 (底部) to 1.0 (中间)
                r2 = (byte)((float)r * y);
                g2 = (byte)((float)g * y);
                b2 = (byte)((float)b * y);
            }
            strip.setPixelColor(i, r2, g2, b2);
        }
        strip.show();
        delay(wait);
    }
}

/* 辅助函数*/
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
