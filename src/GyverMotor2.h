#pragma once
#include <Arduino.h>

#ifndef GMOTOR2_DT
#define GMOTOR2_DT 50  // период таймера ускорения, мс
#endif

enum GM_driver {
    DRIVER2WIRE,            // двухпроводной драйвер с инверсией шим (dir + PWM)
    DRIVER2WIRE_NO_INVERT,  // двухпроводной драйвер без инверсии ШИМ (dir + PWM)
    DRIVER2WIRE_PWM_SPEED,  // двухпроводной драйвер с двумя ШИМ (PWM + PWM) - режим скорости
    DRIVER2WIRE_PWM_POWER,  // двухпроводной драйвер с двумя ШИМ (PWM + PWM) - режим момента
    DRIVER3WIRE,            // трёхпроводной драйвер (dir + dir + PWM)
    RELAY2WIRE,             // реле в качестве драйвера (dir + dir)

    DRIVER2WIRE_PWM,         // синоним DRIVER2WIRE_PWM_SPEED
    DRIVER2WIRE_PWM_INVERT,  // синоним DRIVER2WIRE_PWM_POWER
};

template <GM_driver GM_TYPE, uint8_t GM_RES = 8>
class GMotor2 {
   public:
    // инициализация с указанием пинов
    GMotor2(uint8_t a, uint8_t b, uint8_t c = 0xff) : _pinA(a), _pinB(b), _pinC(c) {
        pinMode(_pinA, OUTPUT);
        pinMode(_pinB, OUTPUT);
        if (_pinC != 0xff) pinMode(_pinC, OUTPUT);
        _setAll(0);
    }

    // установить минимальный ШИМ (умолч. 0)
    void setMinDuty(uint16_t mduty) {
        _mduty = mduty;
        _run(_speed);
    }

    // установить минимальный ШИМ в % (умолч. 0)
    void setMinDutyPerc(uint8_t p) {
        if (p > 100) p = 100;
        setMinDuty(maxDuty * p / 100);
    }

    // реверс направления (умолч. false)
    void reverse(bool rev) {
        _rev = rev;
        _run(_speed);
    }

    // остановка. Если включено ускорение, то плавная
    void stop() {
        setSpeed(0);
    }

    // активный тормоз
    void brake() {
        _setAll(1);
        _speed = 0;
#ifndef GMOTOR2_NO_ACCEL
        _target = 0;
#endif
    }

    // установить скорость (-макс.. макс)
    void setSpeed(int16_t speed) {
        speed = constrain(speed, -maxDuty, maxDuty);
#ifndef GMOTOR2_NO_ACCEL
        _target = speed;
        if (!_ds) _run(speed);
#else
        _run(speed);
#endif
    }

    // установить скорость в процентах (-100.. 100%)
    void setSpeedPerc(int16_t spd) {
        spd = constrain(spd, -100, 100);
        setSpeed(maxDuty * spd / 100);
    }

    // установить deadtime в микросекундах (умолч. 0)
    void setDeadtime(uint16_t us) {
        _deadtime = us;
    }

    // плавное изменение к указанной скорости с ускорением setAccel, вызывать в loop
    void tick() {
#ifndef GMOTOR2_NO_ACCEL
        if (_ds && (_speed != _target) && ((uint8_t)((uint8_t)millis() - _tmr) >= GMOTOR2_DT)) {
            _tmr = millis();
            if (abs(_speed - _target) > _ds) _run(_speed + (_speed < _target ? _ds : -_ds));
            else _run(_target);
        }
#endif
    }

    // установить ускорение в единицах ШИМ в секунду. 0 чтобы отключить
    void setAccel(uint32_t s) {
#ifndef GMOTOR2_NO_ACCEL
        _ds = s * GMOTOR2_DT / 1000;
#endif
    }

    // установить ускорение в процентах в секунду. 0 чтобы отключить
    void setAccelPerc(uint8_t p) {
        if (p > 100) p = 100;
        setAccel(maxDuty * p / 100);
    }

    // получить статус: мотор крутится (1 и -1), мотор остановлен (0)
    int8_t getState() {
        return _speed ? (_speed > 0 ? 1 : -1) : 0;
    }

    // получить текущую скорость мотора в единицах ШИМ
    int16_t getSpeed() {
        return _speed;
    }

    // макс ШИМ при разрешении
    static constexpr int32_t maxDuty = (1 << GM_RES) - 1;

// ============ DEPRECATED ============
#ifndef GMOTOR2_NO_ACCEL
    void smoothMode(bool v) { _ds = v ? 20 : 0; }
    void setSmoothSpeed(uint16_t s) { _ds = s; }
    void setSmoothSpeedPerc(uint8_t p) {
        if (p > 100) p = 100;
        _ds = maxDuty * p / 100;
    }
#endif

   private:
    void _run(int16_t spd) {
        if (!spd) {
            _setAll(0);
            _speed = 0;
            return;
        }

        if (_deadtime && _speed && (_speed > 0) != (spd > 0)) {  // направление изменилось
            _setAll(0);
            delayMicroseconds(_deadtime);
        }
        _speed = spd;

        bool dir = (spd > 0) ^ _rev;
        if (spd < 0) spd = -spd;
        if (_mduty) spd = ((spd * (maxDuty - _mduty)) >> GM_RES) + _mduty;
        if (GM_RES > 8 && spd == 255) ++spd;  // защита от 255 при разрешении > 8 бит

        switch (GM_TYPE) {
            case DRIVER2WIRE:
                digitalWrite(_pinA, !dir);
                analogWrite(_pinB, dir ? spd : (maxDuty - spd));
                break;

            case DRIVER2WIRE_NO_INVERT:
                digitalWrite(_pinA, !dir);
                analogWrite(_pinB, spd);
                break;

            case DRIVER2WIRE_PWM:
            case DRIVER2WIRE_PWM_SPEED:
                if (dir) {
                    digitalWrite(_pinA, 0);
                    analogWrite(_pinB, spd);
                } else {
                    analogWrite(_pinA, spd);
                    digitalWrite(_pinB, 0);
                }
                break;

            case DRIVER2WIRE_PWM_INVERT:
            case DRIVER2WIRE_PWM_POWER:
                if (!dir) {
                    digitalWrite(_pinA, 1);
                    analogWrite(_pinB, maxDuty - spd);
                } else {
                    analogWrite(_pinA, maxDuty - spd);
                    digitalWrite(_pinB, 1);
                }
                break;

            case DRIVER3WIRE:
                digitalWrite(_pinA, !dir);
                digitalWrite(_pinB, dir);
                analogWrite(_pinC, spd);
                break;

            case RELAY2WIRE:
                digitalWrite(_pinA, !dir);
                digitalWrite(_pinB, dir);
                break;
        }
    }

    // установить все пины
    void _setAll(bool val) {
        digitalWrite(_pinA, val);
        digitalWrite(_pinB, val);
        if (_pinC != 0xff) digitalWrite(_pinC, val);
    }

    int16_t _mduty = 0, _speed = 0;
#ifndef GMOTOR2_NO_ACCEL
    int16_t _target = 0, _ds = 0;
    uint8_t _tmr = 0;
#endif
    const uint8_t _pinA, _pinB, _pinC;
    uint8_t _deadtime = 0;
    bool _rev = 0;
};