#include "pch.h"
#ifdef _WIN32
using namespace Windows::Devices::Gpio;
using namespace Windows::Foundation;
#endif //_WIN32

#ifdef ARDUINO
#include "Arduino.h"
#else
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#endif //ARDUINO

#ifdef __arm__
#include "stdlib.h"
#include "stdint.h"
#include "wiringPi.h"
#endif //__arm__

namespace MetecDriverLibrary
{
#ifdef _WIN32
void digitalWrite(Windows::Devices::Gpio::GpioPin ^pin, uint8_t val) {
    pin->Write(val ? GpioPinValue::High : GpioPinValue::Low);
}

void delay(unsigned long ms) {
    Sleep(ms);
}

uint8_t digitalRead(Windows::Devices::Gpio::GpioPin ^pin) {
    return pin->Read() == GpioPinValue::High ? 1 : 0;
}
#endif //_WIN32

void MetecDriver::writeAllCell() {
    for (uint8_t i = 0; i < _cellSize; i++) {
        digitalWrite(_din_pin, bitRead(_cells[i], 6) ? 0 : 1); digitalWrite(_clk_pin, 1); digitalWrite(_clk_pin, 0);
        digitalWrite(_din_pin, bitRead(_cells[i], 7) ? 0 : 1); digitalWrite(_clk_pin, 1); digitalWrite(_clk_pin, 0);
        digitalWrite(_din_pin, bitRead(_cells[i], 2) ? 0 : 1); digitalWrite(_clk_pin, 1); digitalWrite(_clk_pin, 0);
        digitalWrite(_din_pin, bitRead(_cells[i], 1) ? 0 : 1); digitalWrite(_clk_pin, 1); digitalWrite(_clk_pin, 0);
        digitalWrite(_din_pin, bitRead(_cells[i], 0) ? 0 : 1); digitalWrite(_clk_pin, 1); digitalWrite(_clk_pin, 0);
        digitalWrite(_din_pin, bitRead(_cells[i], 5) ? 0 : 1); digitalWrite(_clk_pin, 1); digitalWrite(_clk_pin, 0);
        digitalWrite(_din_pin, bitRead(_cells[i], 4) ? 0 : 1); digitalWrite(_clk_pin, 1); digitalWrite(_clk_pin, 0);
        digitalWrite(_din_pin, bitRead(_cells[i], 3) ? 0 : 1); digitalWrite(_clk_pin, 1); digitalWrite(_clk_pin, 0);
    }
    digitalWrite(_strobe_pin, 1);
    digitalWrite(_strobe_pin, 0);
    digitalWrite(_din_pin, 0);
}

void MetecDriver::setCellNoDelay(uint8_t position, uint8_t value) {
    _cells[position] = value;
}

void MetecDriver::clear() {
    for (uint8_t i = _cellSize; i-- > 0;) {
        setCellNoDelay(i, NOTHING);
    }
    writeAllCell();
}

void MetecDriver::full() {
    for (uint8_t i = _cellSize; i-- > 0;) {
        setCellNoDelay(i, FULL);
    }
    writeAllCell();
}

void MetecDriver::setCell(uint8_t position, uint8_t value) {
    _cells[position] = value;
    if (_wait != 0) {
        writeAllCell();
        delay(_wait);
    }
}

#ifdef _WIN32
void MetecDriver::writeCells(const Array<double>^ pattern, uint8_t max_charac, bool reverse) {
#else
void MetecDriver::writeCells(uint16_t *pattern, uint8_t max_charac, bool reverse) {
#endif //_WIN32
    uint8_t max = _cellSize;

    if (max_charac < max) {
        max = max_charac;
    }

    for (uint8_t i = 0; i < max; i++) {
        if (!reverse) {
            writeSingleCell(_cellSize - 1 - i, pattern[i]);
        }
        else {
            writeSingleCell(i, pattern[i]);
        }
    }
    if (max < _cellSize) {
        for (uint8_t i = max; i < _cellSize; i++) {
            if (!reverse) {
                setCell(_cellSize - 1 - i, NOTHING);
            }
            else {
                setCell(i, NOTHING);
            }
        }
    }
}

void MetecDriver::writeSingleCell(uint8_t position, uint16_t pattern) {
    if (pattern >= MIN_BRAILLE && pattern <= MAX_BRAILLE) {
        setCell(position, braille_db[pattern - MIN_BRAILLE]);
    }
    else {
        setCell(position, NOTHING);
    }
}


MetecDriver::MetecDriver(uint8_t cellCount,
                         uint8_t on_pin,
                         uint8_t din_pin,
                         uint8_t strobe_pin,
                         uint8_t clk_pin,
                         uint8_t dout_pin) {
    _cellSize = cellCount;
    _cells = (uint8_t *)malloc(cellCount * sizeof(uint8_t));
    _keys = (uint8_t *)malloc(cellCount * sizeof(uint8_t));

    _on_pin_val = on_pin;
    _din_pin_val = din_pin;
    _strobe_pin_val = strobe_pin;
    _clk_pin_val = clk_pin;
    _dout_pin_val = dout_pin;
}

void MetecDriver::init() {
#ifdef _WIN32
    GpioController ^controller = GpioController::GetDefault();
    _din_pin = controller->OpenPin(_din_pin_val);
    _din_pin->Write(GpioPinValue::Low);
    _din_pin->SetDriveMode(GpioPinDriveMode::Output);
    _strobe_pin = controller->OpenPin(_strobe_pin_val);
    _strobe_pin->Write(GpioPinValue::Low);
    _strobe_pin->SetDriveMode(GpioPinDriveMode::Output);
    _clk_pin = controller->OpenPin(_clk_pin_val);
    _clk_pin->Write(GpioPinValue::Low);
    _clk_pin->SetDriveMode(GpioPinDriveMode::Output);
    _dout_pin = controller->OpenPin(_dout_pin_val);
    _dout_pin->SetDriveMode(GpioPinDriveMode::Input);
    _on_pin = controller->OpenPin(_on_pin_val);
    _on_pin->Write(GpioPinValue::Low); //enable module
    _on_pin->SetDriveMode(GpioPinDriveMode::Output);
#else
    _on_pin = _on_pin_val;
    _din_pin = _din_pin_val;
    _strobe_pin = _strobe_pin_val;
    _clk_pin = _clk_pin_val;
    _dout_pin = _dout_pin_val;
    pinMode(_on_pin, OUTPUT);
    pinMode(_din_pin, OUTPUT);
    pinMode(_strobe_pin, OUTPUT);
    pinMode(_clk_pin, OUTPUT);
    pinMode(_dout_pin, INPUT);
#endif //_WIN32
    clear();
}

MetecDriver::~MetecDriver() {
    free(_cells);
    free(_keys);
}

void MetecDriver::enableModule() {
    digitalWrite(_on_pin, 0);
}

void MetecDriver::disableModule() {
    digitalWrite(_on_pin, 1);
}

void MetecDriver::checkButton() {
    digitalWrite(_strobe_pin, 0);
    digitalWrite(_strobe_pin, 1);
    digitalWrite(_clk_pin, 1);
    delay(10);
    uint8_t changed = 0;
    uint8_t value;
    uint8_t position = 0;
    for (uint8_t i = _cellSize; i-- > 0;) {
        uint8_t val = 0;
        for (uint8_t j = 0; j < 2; j++) {
            digitalWrite(_clk_pin, 0); value = digitalRead(_dout_pin); digitalWrite(_clk_pin, 1);
            val += (value << (4 * j));
        }
        if (val != _keys[i]) {
            changed = 1;
            position = i;
            _keys[i] = val;
        }
    }

    digitalWrite(_clk_pin, 0);
    digitalWrite(_strobe_pin, 0);

    if (changed == 1) {
#ifdef _WIN32
        _btn_position = position;
        _btn_state = (_keys[position] == 16) ? ButtonState::Pushed : ButtonState::Released;
        _btn_update = true;
#else
        btn_position = position;
        btn_state = (_keys[position] == 16) ? ButtonState::Pushed : ButtonState::Released;
        btn_update = true;
#endif
    }
}

void MetecDriver::setWait(uint8_t wait) {
    _wait = wait;
}

uint8_t MetecDriver::getWait() {
    return _wait;
}
}