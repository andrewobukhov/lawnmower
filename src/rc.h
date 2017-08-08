#define SRC_CENTER 1436
#define SRC_MAX 1900
#define SRC_MIN 1100

#define TRC_CENTER 1500
#define TRC_MAX 1900
#define TRC_MIN 1100

#define RC_DEADBAND 50
#define ERROR_center 50
#define pERROR 100

#define GEAR_NONE 1
#define GEAR_IDLE 1
#define GEAR_FULL 2

#define DIRECTION_STOP 0
#define DIRECTION_FORWARD 1
#define DIRECTION_REVERSE 2
#define DIRECTION_ROTATE_RIGHT 3
#define DIRECTION_ROTATE_LEFT 4

#define IDLE_MAX 50
#define MODE_RUN 0

// Assign your channel in pins
#define THROTTLE_IN_PIN 0
#define STEERING_IN_PIN 1

// These bit flags are set in bUpdateFlagsShared to indicate which
// channels have new signals
#define THROTTLE_FLAG 1
#define STEERING_FLAG 2

// holds the update flags defined above
volatile uint8_t bUpdateFlagsShared;

// shared variables are updated by the ISR and read by loop.
// In loop we immediatley take local copies so that the ISR can keep ownership of the
// shared ones. To access these in loop
// we first turn interrupts off with noInterrupts
// we take a copy to use in loop and the turn interrupts back on
// as quickly as possible, this ensures that we are always able to receive new signals
volatile unsigned int unThrottleInShared;
volatile unsigned int unSteeringInShared;

// These are used to record the rising edge of a pulse in the calcInput functions
// They do not need to be volatile as they are only used in the ISR. If we wanted
// to refer to these in loop and the ISR then they would need to be declared volatile
unsigned long ulThrottleStart;
unsigned long ulSteeringStart;

uint8_t gThrottle = 0;
uint8_t gGear = GEAR_NONE;
uint8_t gOldGear = GEAR_NONE;

uint8_t gThrottleDirection = DIRECTION_STOP;
uint8_t gDirection = DIRECTION_STOP;
uint8_t gOldDirection = DIRECTION_STOP;

uint8_t gMode = MODE_RUN;

unsigned long pulse_time;

// simple interrupt service routine
void calcThrottle()
{
    // if the pin is high, its a rising edge of the signal pulse, so lets record its value
    if (digitalRead(THROTTLE_IN_PIN) == HIGH)
    {
        ulThrottleStart = micros();
    }
    else
    {
        // else it must be a falling edge, so lets get the time and subtract the time of the rising edge
        // this gives use the time between the rising and falling edges i.e. the pulse duration.
        unThrottleInShared = (uint16_t)(micros() - ulThrottleStart);
        // use set the throttle flag to indicate that a new throttle signal has been received
        bUpdateFlagsShared |= THROTTLE_FLAG;
    }
}

void calcSteering()
{
    if (digitalRead(STEERING_IN_PIN) == HIGH)
    {
        ulSteeringStart = micros();
    }
    else
    {
        unSteeringInShared = (uint16_t)(micros() - ulSteeringStart);
        bUpdateFlagsShared |= STEERING_FLAG;
    }
}

void initRemoteControl() {
    attachInterrupt(2 /* INT0 = THROTTLE_IN_PIN */, calcThrottle, CHANGE);
    attachInterrupt(3 /* INT1 = STEERING_IN_PIN */, calcSteering, CHANGE);
    pulse_time = millis();
}