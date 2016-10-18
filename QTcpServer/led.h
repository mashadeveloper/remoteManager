#ifndef LED_H
#define LED_H

#include <QString>
#include <QList>

class Led
{
public:
    explicit Led();
    explicit Led(QString ledState, QString ledColor, int ledRate);
    QString ledState() const;
    void setLedState(const QString &ledState);

    QString ledColor() const;
    void setLedColor(const QString &ledColor);

    int ledRate() const;
    void setLedRate(int ledRate);

    QList<QString> colorList() const;

private:
    QString _ledState;
    QString _ledColor;
    QList<QString> _colorList;
    int _ledRate;
};

#endif // LED_H
