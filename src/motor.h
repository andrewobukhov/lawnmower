#define RPWMA 13
#define LPWMA 11
#define RENA 8
#define LENA 7

#define RPWMB 5
#define LPWMB 6
#define RENB 9
#define LENB 10

#define PWM_MIN 140
#define PWM_MAX 255

void initMotors()
{
    pinMode(RPWMA, OUTPUT);
    pinMode(LPWMA, OUTPUT);
    pinMode(RENA, OUTPUT);
    pinMode(LENA, OUTPUT);

    pinMode(RPWMB, OUTPUT);
    pinMode(LPWMB, OUTPUT);
    pinMode(RENB, OUTPUT);
    pinMode(LENB, OUTPUT);

    digitalWrite(RENA, HIGH); 
    digitalWrite(LENA, HIGH); 
    digitalWrite(RENB, HIGH); 
    digitalWrite(LENB, HIGH); 
}

void backward()
{
    analogWrite(LPWMA, 255);
    analogWrite(LPWMB, 255);

    analogWrite(RPWMA, 0);
    analogWrite(RPWMB, 0);
}

void forward()
{
    analogWrite(RPWMA, 255);
    analogWrite(RPWMB, 255);

    analogWrite(LPWMA, 0);
    analogWrite(LPWMB, 0);
}

void left()
{
    analogWrite(LPWMA, 255);
    analogWrite(RPWMB, 255);

    analogWrite(RPWMA, 0);
    analogWrite(LPWMB, 0);
}

void right()
{
    analogWrite(LPWMA, 0);
    analogWrite(RPWMB, 0);

    analogWrite(RPWMA, 255);
    analogWrite(LPWMB, 255);
}

void stop()
{
    analogWrite(RPWMA, 0);
    analogWrite(LPWMA, 0);

    analogWrite(RPWMB, 0);
    analogWrite(LPWMB, 0);

    //digitalWrite(RENA, LOW);
    //digitalWrite(LENA, LOW);

    //digitalWrite(RENB, LOW);
    //digitalWrite(LENB, LOW);
}