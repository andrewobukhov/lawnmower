#include <Arduino.h>
#include <motor.h>
#include <rc.h>
#include <control.h>

void setup()
{
    //Serial.begin(9600);
    
    initMotors();
    initRemoteControl();
}

void loop()
{
    calculateSiganls();
}
