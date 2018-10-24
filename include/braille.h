/************************************************************************************
 * The MIT License (MIT)                                                            *
 *                                                                                  *
 * Copyright (c) 2018 Bertrand Martel                                               *
 *                                                                                  * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy     * 
 * of this software and associated documentation files (the "Software"), to deal    * 
 * in the Software without restriction, including without limitation the rights     * 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell        * 
 * copies of the Software, and to permit persons to whom the Software is            * 
 * furnished to do so, subject to the following conditions:                         * 
 *                                                                                  * 
 * The above copyright notice and this permission notice shall be included in       * 
 * all copies or substantial portions of the Software.                              * 
 *                                                                                  * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR       * 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,         * 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE      * 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER           * 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,    * 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN        * 
 * THE SOFTWARE.                                                                    * 
 ************************************************************************************/
/**
    braille.h
    
    Metec driver for Metec Braille-line 20 Cell standard
    @author Bertrand Martel
    @version 1.0
*/
#ifndef braille_h
#define braille_h

#ifdef ARDUINO
#include "Arduino.h"
#elif __arm__
#include "stdint.h"
#endif

#define NOTHING                   0xFF
#define FULL                      0x00
#define DEFAULT_TIME_BETWEEN_CELL 30

#define MIN_BRAILLE 0x2800
#define MAX_BRAILLE 0x28FF

#ifdef _WIN32
using namespace Platform;
#include <collection.h>
#define PUBLIC public
#else
#define PUBLIC
#endif //_WIN32

namespace MetecDriverLibrary
{
PUBLIC enum class ButtonState {Released = 0, Pushed = 1};

#ifdef _WIN32
public ref class MetecDriver sealed {
#else
class MetecDriver {
#endif //_WIN32

public:
    MetecDriver(uint8_t cellCount,
                       uint8_t on_pin,
                       uint8_t din_pin,
                       uint8_t strobe_pin,
                       uint8_t clk_pin,
                       uint8_t dout_pin);

    void init();

    void clear();

    void full();

#ifdef _WIN32
    void writeCells(const Array<uint16_t>^ pattern, uint8_t max_charac, bool reverse);
#else
    void writeCells(uint16_t* pattern, uint8_t max_charac, bool reverse);
#endif //_WIN32

    void writeSingleCell(uint8_t position, uint16_t pattern);

    void enableModule();

    void disableModule(); 

    void setWait(uint8_t wait);
    uint8_t getWait();
    
    void checkButton();

#ifdef _WIN32
    property uint8_t btn_position
    {
        uint8_t get() { return _btn_position; }
    }
    property ButtonState btn_state
    {
        ButtonState get() { return _btn_state; }
    }
    property bool btn_update
    {
        bool get() { return _btn_update; }
        void set(bool value) { _btn_update = value; }
    }
#else
    uint8_t btn_position;
    ButtonState btn_state;
    bool btn_update;
    ~MetecDriver();
#endif //_WIN32

private:
#ifdef _WIN32
    ~MetecDriver();
#endif
    void setCell(uint8_t position, uint8_t value);
    void setCellNoDelay(uint8_t position, uint8_t value);
    void writeAllCell();

    uint8_t _on_pin_val = 0;
    uint8_t _din_pin_val = 0;
    uint8_t _strobe_pin_val = 0;
    uint8_t _clk_pin_val = 0;
    uint8_t _dout_pin_val = 0;
    uint8_t _wait = DEFAULT_TIME_BETWEEN_CELL;

#ifdef _WIN32
    Windows::Devices::Gpio::GpioPin ^_on_pin;
    Windows::Devices::Gpio::GpioPin ^_din_pin;
    Windows::Devices::Gpio::GpioPin ^_strobe_pin;
    Windows::Devices::Gpio::GpioPin ^_clk_pin;
    Windows::Devices::Gpio::GpioPin ^_dout_pin;
#else
    uint8_t _on_pin = 0;
    uint8_t _din_pin = 0;
    uint8_t _strobe_pin = 0;
    uint8_t _clk_pin = 0;
    uint8_t _dout_pin = 0;
#endif //_WIN32

    uint8_t _cellSize = 0;
    uint8_t* _cells = 0;
    uint8_t* _keys = 0;

#ifdef _WIN32
    uint8_t _btn_position = 0;
    ButtonState _btn_state = ButtonState::Released;
    bool _btn_update;
#endif
};

const uint16_t braille_db[256] = {
    0xFF, // 0x2800  ⠀BRAILLE PATTERN BLANK
    0xF7, // 0x2801 ⠁ BRAILLE PATTERN DOTS-1
    0xEF, // 0x2802 ⠂ BRAILLE PATTERN DOTS-2
    0xE7, // 0x2803 ⠃ BRAILLE PATTERN DOTS-12
    0xDF, // 0x2804 ⠄ BRAILLE PATTERN DOTS-3
    0xD7, // 0x2805 ⠅ BRAILLE PATTERN DOTS-13
    0xCF, // 0x2806 ⠆ BRAILLE PATTERN DOTS-23
    0xC7, // 0x2807 ⠇ BRAILLE PATTERN DOTS-123
    0xFE, // 0x2808 ⠈ BRAILLE PATTERN DOTS-4
    0xF6, // 0x2809 ⠉ BRAILLE PATTERN DOTS-14
    0xEE, // 0x280A ⠊ BRAILLE PATTERN DOTS-24
    0xE6, // 0x280B ⠋ BRAILLE PATTERN DOTS-124
    0xDE, // 0x280C ⠌ BRAILLE PATTERN DOTS-34
    0xD6, // 0x280D ⠍ BRAILLE PATTERN DOTS-134
    0xCE, // 0x280E ⠎ BRAILLE PATTERN DOTS-234
    0xC6, // 0x280F ⠏ BRAILLE PATTERN DOTS-1234
    0xFD, // 0x2810 ⠐ BRAILLE PATTERN DOTS-5
    0xF5, // 0x2811 ⠑ BRAILLE PATTERN DOTS-15
    0xED, // 0x2812 ⠒ BRAILLE PATTERN DOTS-25
    0xE5, // 0x2813 ⠓ BRAILLE PATTERN DOTS-125
    0xDD, // 0x2814 ⠔ BRAILLE PATTERN DOTS-35
    0xD5, // 0x2815 ⠕ BRAILLE PATTERN DOTS-135
    0xCD, // 0x2816 ⠖ BRAILLE PATTERN DOTS-235
    0xC5, // 0x2817 ⠗ BRAILLE PATTERN DOTS-1235
    0xFC, // 0x2818 ⠘ BRAILLE PATTERN DOTS-45
    0xF4, // 0x2819 ⠙ BRAILLE PATTERN DOTS-145
    0xEC, // 0x281A ⠚ BRAILLE PATTERN DOTS-245
    0xE4, // 0x281B ⠛ BRAILLE PATTERN DOTS-1245
    0xDC, // 0x281C ⠜ BRAILLE PATTERN DOTS-345
    0xD4, // 0x281D ⠝ BRAILLE PATTERN DOTS-1345
    0xCC, // 0x281E ⠞ BRAILLE PATTERN DOTS-2345
    0xC4, // 0x281F ⠟ BRAILLE PATTERN DOTS-12345
    0xFB, // 0x2820 ⠠ BRAILLE PATTERN DOTS-6
    0xF3, // 0x2821 ⠡ BRAILLE PATTERN DOTS-16
    0xEB, // 0x2822 ⠢ BRAILLE PATTERN DOTS-26
    0xE3, // 0x2823 ⠣ BRAILLE PATTERN DOTS-126
    0xDB, // 0x2824 ⠤ BRAILLE PATTERN DOTS-36
    0xD3, // 0x2825 ⠥ BRAILLE PATTERN DOTS-136
    0xCB, // 0x2826 ⠦ BRAILLE PATTERN DOTS-236
    0xC3, // 0x2827 ⠧ BRAILLE PATTERN DOTS-1236
    0xFA, // 0x2828 ⠨ BRAILLE PATTERN DOTS-46
    0xF2, // 0x2829 ⠩ BRAILLE PATTERN DOTS-146
    0xEA, // 0x282A ⠪ BRAILLE PATTERN DOTS-246
    0xE2, // 0x282B ⠫ BRAILLE PATTERN DOTS-1246
    0xDA, // 0x282C ⠬ BRAILLE PATTERN DOTS-346
    0xD2, // 0x282D ⠭ BRAILLE PATTERN DOTS-1346
    0xCA, // 0x282E ⠮ BRAILLE PATTERN DOTS-2346
    0xC2, // 0x282F ⠯ BRAILLE PATTERN DOTS-12346
    0xF9, // 0x2830 ⠰ BRAILLE PATTERN DOTS-56
    0xF1, // 0x2831 ⠱ BRAILLE PATTERN DOTS-156
    0xE9, // 0x2832 ⠲ BRAILLE PATTERN DOTS-256
    0xE1, // 0x2833 ⠳ BRAILLE PATTERN DOTS-1256
    0xD9, // 0x2834 ⠴ BRAILLE PATTERN DOTS-356
    0xD1, // 0x2835 ⠵ BRAILLE PATTERN DOTS-1356
    0xC9, // 0x2836 ⠶ BRAILLE PATTERN DOTS-2356
    0xC1, // 0x2837 ⠷ BRAILLE PATTERN DOTS-12356
    0xF8, // 0x2838 ⠸ BRAILLE PATTERN DOTS-456
    0xF0, // 0x2839 ⠹ BRAILLE PATTERN DOTS-1456
    0xE8, // 0x283A ⠺ BRAILLE PATTERN DOTS-2456
    0xE0, // 0x283B ⠻ BRAILLE PATTERN DOTS-12456
    0xD8, // 0x283C ⠼ BRAILLE PATTERN DOTS-3456
    0xD0, // 0x283D ⠽ BRAILLE PATTERN DOTS-13456
    0xC8, // 0x283E ⠾ BRAILLE PATTERN DOTS-23456
    0xC0, // 0x283F ⠿ BRAILLE PATTERN DOTS-123456
    0x7F, // 0x2840 ⡀ BRAILLE PATTERN DOTS-7
    0x77, // 0x2841 ⡁ BRAILLE PATTERN DOTS-17
    0x6F, // 0x2842 ⡂ BRAILLE PATTERN DOTS-27
    0x67, // 0x2843 ⡃ BRAILLE PATTERN DOTS-127
    0x5F, // 0x2844 ⡄ BRAILLE PATTERN DOTS-37
    0x57, // 0x2845 ⡅ BRAILLE PATTERN DOTS-137
    0x4F, // 0x2846 ⡆ BRAILLE PATTERN DOTS-237
    0x47, // 0x2847 ⡇ BRAILLE PATTERN DOTS-1237
    0x7E, // 0x2848 ⡈ BRAILLE PATTERN DOTS-47
    0x76, // 0x2849 ⡉ BRAILLE PATTERN DOTS-147
    0x6E, // 0x284A ⡊ BRAILLE PATTERN DOTS-247
    0x66, // 0x284B ⡋ BRAILLE PATTERN DOTS-1247
    0x5E, // 0x284C ⡌ BRAILLE PATTERN DOTS-347
    0x56, // 0x284D ⡍ BRAILLE PATTERN DOTS-1347
    0x4E, // 0x284E ⡎ BRAILLE PATTERN DOTS-2347
    0x46, // 0x284F ⡏ BRAILLE PATTERN DOTS-12347
    0x7D, // 0x2850 ⡐ BRAILLE PATTERN DOTS-57
    0x75, // 0x2851 ⡑ BRAILLE PATTERN DOTS-157
    0x6D, // 0x2852 ⡒ BRAILLE PATTERN DOTS-257
    0x65, // 0x2853 ⡓ BRAILLE PATTERN DOTS-1257
    0x5D, // 0x2854 ⡔ BRAILLE PATTERN DOTS-357
    0x55, // 0x2855 ⡕ BRAILLE PATTERN DOTS-1357
    0x4D, // 0x2856 ⡖ BRAILLE PATTERN DOTS-2357
    0x45, // 0x2857 ⡗ BRAILLE PATTERN DOTS-12357
    0x7C, // 0x2858 ⡘ BRAILLE PATTERN DOTS-457
    0x74, // 0x2859 ⡙ BRAILLE PATTERN DOTS-1457
    0x6C, // 0x285A ⡚ BRAILLE PATTERN DOTS-2457
    0x64, // 0x285B ⡛ BRAILLE PATTERN DOTS-12457
    0x5C, // 0x285C ⡜ BRAILLE PATTERN DOTS-3457
    0x54, // 0x285D ⡝ BRAILLE PATTERN DOTS-13457
    0x4C, // 0x285E ⡞ BRAILLE PATTERN DOTS-23457
    0x44, // 0x285F ⡟ BRAILLE PATTERN DOTS-123457
    0x7B, // 0x2860 ⡠ BRAILLE PATTERN DOTS-67
    0x73, // 0x2861 ⡡ BRAILLE PATTERN DOTS-167
    0x6B, // 0x2862 ⡢ BRAILLE PATTERN DOTS-267
    0x63, // 0x2863 ⡣ BRAILLE PATTERN DOTS-1267
    0x5B, // 0x2864 ⡤ BRAILLE PATTERN DOTS-367
    0x53, // 0x2865 ⡥ BRAILLE PATTERN DOTS-1367
    0x4B, // 0x2866 ⡦ BRAILLE PATTERN DOTS-2367
    0x43, // 0x2867 ⡧ BRAILLE PATTERN DOTS-12367
    0x7A, // 0x2868 ⡨ BRAILLE PATTERN DOTS-467
    0x72, // 0x2869 ⡩ BRAILLE PATTERN DOTS-1467
    0x6A, // 0x286A ⡪ BRAILLE PATTERN DOTS-2467
    0x62, // 0x286B ⡫ BRAILLE PATTERN DOTS-12467
    0x5A, // 0x286C ⡬ BRAILLE PATTERN DOTS-3467
    0x52, // 0x286D ⡭ BRAILLE PATTERN DOTS-13467
    0x4A, // 0x286E ⡮ BRAILLE PATTERN DOTS-23467
    0x42, // 0x286F ⡯ BRAILLE PATTERN DOTS-123467
    0x79, // 0x2870 ⡰ BRAILLE PATTERN DOTS-567
    0x71, // 0x2871 ⡱ BRAILLE PATTERN DOTS-1567
    0x69, // 0x2872 ⡲ BRAILLE PATTERN DOTS-2567
    0x61, // 0x2873 ⡳ BRAILLE PATTERN DOTS-12567
    0x59, // 0x2874 ⡴ BRAILLE PATTERN DOTS-3567
    0x51, // 0x2875 ⡵ BRAILLE PATTERN DOTS-13567
    0x49, // 0x2876 ⡶ BRAILLE PATTERN DOTS-23567
    0x41, // 0x2877 ⡷ BRAILLE PATTERN DOTS-123567
    0x78, // 0x2878 ⡸ BRAILLE PATTERN DOTS-4567
    0x70, // 0x2879 ⡹ BRAILLE PATTERN DOTS-14567
    0x68, // 0x287A ⡺ BRAILLE PATTERN DOTS-24567
    0x60, // 0x287B ⡻ BRAILLE PATTERN DOTS-124567
    0x58, // 0x287C ⡼ BRAILLE PATTERN DOTS-34567
    0x50, // 0x287D ⡽ BRAILLE PATTERN DOTS-134567
    0x48, // 0x287E ⡾ BRAILLE PATTERN DOTS-234567
    0x40, // 0x287F ⡿ BRAILLE PATTERN DOTS-1234567
    0xBF, // 0x2880 ⢀ BRAILLE PATTERN DOTS-8
    0xB7, // 0x2881 ⢁ BRAILLE PATTERN DOTS-18
    0xAF, // 0x2882 ⢂ BRAILLE PATTERN DOTS-28
    0xA7, // 0x2883 ⢃ BRAILLE PATTERN DOTS-128
    0x9F, // 0x2884 ⢄ BRAILLE PATTERN DOTS-38
    0x97, // 0x2885 ⢅ BRAILLE PATTERN DOTS-138
    0x8F, // 0x2886 ⢆ BRAILLE PATTERN DOTS-238
    0x87, // 0x2887 ⢇ BRAILLE PATTERN DOTS-1238
    0xBE, // 0x2888 ⢈ BRAILLE PATTERN DOTS-48
    0xB6, // 0x2889 ⢉ BRAILLE PATTERN DOTS-148
    0xAE, // 0x288A ⢊ BRAILLE PATTERN DOTS-248
    0xA6, // 0x288B ⢋ BRAILLE PATTERN DOTS-1248
    0x9E, // 0x288C ⢌ BRAILLE PATTERN DOTS-348
    0x96, // 0x288D ⢍ BRAILLE PATTERN DOTS-1348
    0x8E, // 0x288E ⢎ BRAILLE PATTERN DOTS-2348
    0x86, // 0x288F ⢏ BRAILLE PATTERN DOTS-12348
    0xBD, // 0x2890 ⢐ BRAILLE PATTERN DOTS-58
    0xB5, // 0x2891 ⢑ BRAILLE PATTERN DOTS-158
    0xAD, // 0x2892 ⢒ BRAILLE PATTERN DOTS-258
    0xA5, // 0x2893 ⢓ BRAILLE PATTERN DOTS-1258
    0x9D, // 0x2894 ⢔ BRAILLE PATTERN DOTS-358
    0x95, // 0x2895 ⢕ BRAILLE PATTERN DOTS-1358
    0x8D, // 0x2896 ⢖ BRAILLE PATTERN DOTS-2358
    0x85, // 0x2897 ⢗ BRAILLE PATTERN DOTS-12358
    0xBC, // 0x2898 ⢘ BRAILLE PATTERN DOTS-458
    0xB4, // 0x2899 ⢙ BRAILLE PATTERN DOTS-1458
    0xAC, // 0x289A ⢚ BRAILLE PATTERN DOTS-2458
    0xA4, // 0x289B ⢛ BRAILLE PATTERN DOTS-12458
    0x9C, // 0x289C ⢜ BRAILLE PATTERN DOTS-3458
    0x94, // 0x289D ⢝ BRAILLE PATTERN DOTS-13458
    0x8C, // 0x289E ⢞ BRAILLE PATTERN DOTS-23458
    0x84, // 0x289F ⢟ BRAILLE PATTERN DOTS-123458
    0xBB, // 0x28A0 ⢠ BRAILLE PATTERN DOTS-68
    0xB3, // 0x28A1 ⢡ BRAILLE PATTERN DOTS-168
    0xAB, // 0x28A2 ⢢ BRAILLE PATTERN DOTS-268
    0xA3, // 0x28A3 ⢣ BRAILLE PATTERN DOTS-1268
    0x9B, // 0x28A4 ⢤ BRAILLE PATTERN DOTS-368
    0x93, // 0x28A5 ⢥ BRAILLE PATTERN DOTS-1368
    0x8B, // 0x28A6 ⢦ BRAILLE PATTERN DOTS-2368
    0x83, // 0x28A7 ⢧ BRAILLE PATTERN DOTS-12368
    0xBA, // 0x28A8 ⢨ BRAILLE PATTERN DOTS-468
    0xB2, // 0x28A9 ⢩ BRAILLE PATTERN DOTS-1468
    0xAA, // 0x28AA ⢪ BRAILLE PATTERN DOTS-2468
    0xA2, // 0x28AB ⢫ BRAILLE PATTERN DOTS-12468
    0x9A, // 0x28AC ⢬ BRAILLE PATTERN DOTS-3468
    0x92, // 0x28AD ⢭ BRAILLE PATTERN DOTS-13468
    0x8A, // 0x28AE ⢮ BRAILLE PATTERN DOTS-23468
    0x82, // 0x28AF ⢯ BRAILLE PATTERN DOTS-123468
    0xB9, // 0x28B0 ⢰ BRAILLE PATTERN DOTS-568
    0xB1, // 0x28B1 ⢱ BRAILLE PATTERN DOTS-1568
    0xA9, // 0x28B2 ⢲ BRAILLE PATTERN DOTS-2568
    0xA1, // 0x28B3 ⢳ BRAILLE PATTERN DOTS-12568
    0x99, // 0x28B4 ⢴ BRAILLE PATTERN DOTS-3568
    0x91, // 0x28B5 ⢵ BRAILLE PATTERN DOTS-13568
    0x89, // 0x28B6 ⢶ BRAILLE PATTERN DOTS-23568
    0x81, // 0x28B7 ⢷ BRAILLE PATTERN DOTS-123568
    0xB8, // 0x28B8 ⢸ BRAILLE PATTERN DOTS-4568
    0xB0, // 0x28B9 ⢹ BRAILLE PATTERN DOTS-14568
    0xA8, // 0x28BA ⢺ BRAILLE PATTERN DOTS-24568
    0xA0, // 0x28BB ⢻ BRAILLE PATTERN DOTS-124568
    0x98, // 0x28BC ⢼ BRAILLE PATTERN DOTS-34568
    0x90, // 0x28BD ⢽ BRAILLE PATTERN DOTS-134568
    0x88, // 0x28BE ⢾ BRAILLE PATTERN DOTS-234568
    0x80, // 0x28BF ⢿ BRAILLE PATTERN DOTS-1234568
    0x3F, // 0x28C0 ⣀ BRAILLE PATTERN DOTS-78
    0x37, // 0x28C1 ⣁ BRAILLE PATTERN DOTS-178
    0x2F, // 0x28C2 ⣂ BRAILLE PATTERN DOTS-278
    0x27, // 0x28C3 ⣃ BRAILLE PATTERN DOTS-1278
    0x1F, // 0x28C4 ⣄ BRAILLE PATTERN DOTS-378
    0x17, // 0x28C5 ⣅ BRAILLE PATTERN DOTS-1378
    0x0F, // 0x28C6 ⣆ BRAILLE PATTERN DOTS-2378
    0x07, // 0x28C7 ⣇ BRAILLE PATTERN DOTS-12378
    0x3E, // 0x28C8 ⣈ BRAILLE PATTERN DOTS-478
    0x36, // 0x28C9 ⣉ BRAILLE PATTERN DOTS-1478
    0x2E, // 0x28CA ⣊ BRAILLE PATTERN DOTS-2478
    0x26, // 0x28CB ⣋ BRAILLE PATTERN DOTS-12478
    0x1E, // 0x28CC ⣌ BRAILLE PATTERN DOTS-3478
    0x16, // 0x28CD ⣍ BRAILLE PATTERN DOTS-13478
    0x0E, // 0x28CE ⣎ BRAILLE PATTERN DOTS-23478
    0x06, // 0x28CF ⣏ BRAILLE PATTERN DOTS-123478
    0x3D, // 0x28D0 ⣐ BRAILLE PATTERN DOTS-578
    0x35, // 0x28D1 ⣑ BRAILLE PATTERN DOTS-1578
    0x2D, // 0x28D2 ⣒ BRAILLE PATTERN DOTS-2578
    0x25, // 0x28D3 ⣓ BRAILLE PATTERN DOTS-12578
    0x1D, // 0x28D4 ⣔ BRAILLE PATTERN DOTS-3578
    0x15, // 0x28D5 ⣕ BRAILLE PATTERN DOTS-13578
    0x0D, // 0x28D6 ⣖ BRAILLE PATTERN DOTS-23578
    0x05, // 0x28D7 ⣗ BRAILLE PATTERN DOTS-123578
    0x3C, // 0x28D8 ⣘ BRAILLE PATTERN DOTS-4578
    0x34, // 0x28D9 ⣙ BRAILLE PATTERN DOTS-14578
    0x2C, // 0x28DA ⣚ BRAILLE PATTERN DOTS-24578
    0x24, // 0x28DB ⣛ BRAILLE PATTERN DOTS-124578
    0x1C, // 0x28DC ⣜ BRAILLE PATTERN DOTS-34578
    0x14, // 0x28DD ⣝ BRAILLE PATTERN DOTS-134578
    0x0C, // 0x28DE ⣞ BRAILLE PATTERN DOTS-234578
    0x04, // 0x28DF ⣟ BRAILLE PATTERN DOTS-1234578
    0x3B, // 0x28E0 ⣠ BRAILLE PATTERN DOTS-678
    0x33, // 0x28E1 ⣡ BRAILLE PATTERN DOTS-1678
    0x2B, // 0x28E2 ⣢ BRAILLE PATTERN DOTS-2678
    0x23, // 0x28E3 ⣣ BRAILLE PATTERN DOTS-12678
    0x1B, // 0x28E4 ⣤ BRAILLE PATTERN DOTS-3678
    0x13, // 0x28E5 ⣥ BRAILLE PATTERN DOTS-13678
    0x0B, // 0x28E6 ⣦ BRAILLE PATTERN DOTS-23678
    0x03, // 0x28E7 ⣧ BRAILLE PATTERN DOTS-123678
    0x3A, // 0x28E8 ⣨ BRAILLE PATTERN DOTS-4678
    0x32, // 0x28E9 ⣩ BRAILLE PATTERN DOTS-14678
    0x2A, // 0x28EA ⣪ BRAILLE PATTERN DOTS-24678
    0x22, // 0x28EB ⣫ BRAILLE PATTERN DOTS-124678
    0x1A, // 0x28EC ⣬ BRAILLE PATTERN DOTS-34678
    0x12, // 0x28ED ⣭ BRAILLE PATTERN DOTS-134678
    0x0A, // 0x28EE ⣮ BRAILLE PATTERN DOTS-234678
    0x02, // 0x28EF ⣯ BRAILLE PATTERN DOTS-1234678
    0x39, // 0x28F0 ⣰ BRAILLE PATTERN DOTS-5678
    0x31, // 0x28F1 ⣱ BRAILLE PATTERN DOTS-15678
    0x29, // 0x28F2 ⣲ BRAILLE PATTERN DOTS-25678
    0x21, // 0x28F3 ⣳ BRAILLE PATTERN DOTS-125678
    0x19, // 0x28F4 ⣴ BRAILLE PATTERN DOTS-35678
    0x11, // 0x28F5 ⣵ BRAILLE PATTERN DOTS-135678
    0x09, // 0x28F6 ⣶ BRAILLE PATTERN DOTS-235678
    0x01, // 0x28F7 ⣷ BRAILLE PATTERN DOTS-1235678
    0x38, // 0x28F8 ⣸ BRAILLE PATTERN DOTS-45678
    0x30, // 0x28F9 ⣹ BRAILLE PATTERN DOTS-145678
    0x28, // 0x28FA ⣺ BRAILLE PATTERN DOTS-245678
    0x20, // 0x28FB ⣻ BRAILLE PATTERN DOTS-1245678
    0x18, // 0x28FC ⣼ BRAILLE PATTERN DOTS-345678
    0x10, // 0x28FD ⣽ BRAILLE PATTERN DOTS-1345678
    0x08, // 0x28FE ⣾ BRAILLE PATTERN DOTS-2345678
    0x00, // 0x28FF ⣿ BRAILLE PATTERN DOTS-12345678
};
}
#endif //braille_h