#define PWMA 3
#define DIRA 12
#define BrakeA 9

#define PWMB 11
#define DIRB 13
#define BrakeB 8

#define PWM_MIN 140
#define PWM_MAX 255

void initMotors()
{
    pinMode(DIRA, OUTPUT);
    pinMode(DIRB, OUTPUT);

    pinMode(BrakeA, OUTPUT);
    pinMode(BrakeB, OUTPUT);

    pinMode(PWMA, OUTPUT);
    pinMode(PWMB, OUTPUT);

    digitalWrite(BrakeA, HIGH); //Disengage the Brake for Channel A
    digitalWrite(BrakeB, HIGH); //Disengage the Brake for Channel B
}

void backward()
{
    digitalWrite(DIRA, HIGH);  //Establishes forward direction of Channel A
    digitalWrite(BrakeA, LOW); //Disengage the Brake for Channel A
    //analogWrite(PWMA, pwm); //Spins the motor on Channel A at full speed

    digitalWrite(DIRB, HIGH);  //Establishes forward direction of Channel B
    digitalWrite(BrakeB, LOW); //Disengage the Brake for Channel B
                               //analogWrite(PWMB, pwm); //Spins the motor on Channel B at full speed
}

void forward()
{
    digitalWrite(DIRA, LOW);   //Establishes backward direction of Channel A
    digitalWrite(BrakeA, LOW); //Disengage the Brake for Channel A
    //analogWrite(PWMA, pwm); //Spins the motor on Channel A at full speed

    digitalWrite(DIRB, LOW);   //Establishes backward direction of Channel B
    digitalWrite(BrakeB, LOW); //Disengage the Brake for Channel B
                               //analogWrite(PWMB, pwm); //Spins the motor on Channel B at full speed
}

void left()
{
    digitalWrite(DIRA, HIGH);  //Establishes backward direction of Channel A
    digitalWrite(BrakeA, LOW); //Disengage the Brake for Channel A
    //analogWrite(PWMA, pwm); //Spins the motor on Channel A at full speed

    digitalWrite(DIRB, LOW);   //Establishes backward direction of Channel B
    digitalWrite(BrakeB, LOW); //Disengage the Brake for Channel B
                               //analogWrite(PWMB, pwm); //Spins the motor on Channel B at full speed
}

void right()
{
    digitalWrite(DIRA, LOW);   //Establishes backward direction of Channel A
    digitalWrite(BrakeA, LOW); //Disengage the Brake for Channel A
    //analogWrite(PWMA, pwm); //Spins the motor on Channel A at full speed

    digitalWrite(DIRB, HIGH);  //Establishes backward direction of Channel B
    digitalWrite(BrakeB, LOW); //Disengage the Brake for Channel B
                               //analogWrite(PWMB, pwm); //Spins the motor on Channel B at full speed
}

void stop()
{
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);

    digitalWrite(BrakeA, HIGH); //Engage the Brake for Channel A
    digitalWrite(BrakeB, HIGH); //Engage the Brake for Channel B
}