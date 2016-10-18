#include "led.h"

Led::Led()
{
    _colorList.append("red");
    _colorList.append("green");
    _colorList.append("blue");
    _ledState.append("off");
    _ledColor.append("Led has no color");
    _ledRate = 0;
}

Led::Led(QString ledState, QString ledColor, int ledRate)
{
    _ledState = ledState;
    _ledColor = ledColor;
    _ledRate = ledRate;
}

QString Led::ledState() const
{
    return _ledState;
}

void Led::setLedState(const QString &ledState)
{
    _ledState = ledState;
}

QString Led::ledColor() const
{
    return _ledColor;
}

void Led::setLedColor(const QString &ledColor)
{
    _ledColor = ledColor;
}

int Led::ledRate() const
{
    return _ledRate;
}

void Led::setLedRate(int ledRate)
{
    _ledRate = ledRate;
}

QList<QString> Led::colorList() const
{
    return _colorList;
}
