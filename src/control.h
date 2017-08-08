void calculateSiganls () {
    // create local variables to hold a local copies of the channel inputs
    // these are declared static so that thier values will be retained
    // between calls to loop.
    static uint16_t unThrottleIn;
    static uint16_t unSteeringIn;

    // local copy of update flags
    static uint8_t bUpdateFlags;

    // check shared update flags to see if any channels have a new signal
    if (bUpdateFlagsShared)
    {
        noInterrupts(); // turn interrupts off quickly while we take local copies of the shared variables
        pulse_time = millis();
        // take a local copy of which channels were updated in case we need to use this in the rest of loop
        bUpdateFlags = bUpdateFlagsShared;

        // in the current code, the shared values are always populated
        // so we could copy them without testing the flags
        // however in the future this could change, so lets
        // only copy when the flags tell us we can.

        if (bUpdateFlags & THROTTLE_FLAG)
        {
            unThrottleIn = unThrottleInShared;
            //Serial.println(unThrottleIn);
        }

        if (bUpdateFlags & STEERING_FLAG)
        {
            unSteeringIn = unSteeringInShared;
            //Serial.println(unSteeringIn);
        }

        // clear shared copy of updated flags as we have already taken the updates
        // we still have a local copy if we need to use it in bUpdateFlags
        bUpdateFlagsShared = 0;

        interrupts(); // we have local copies of the inputs, so now we can turn interrupts back on
                      // as soon as interrupts are back on, we can no longer use the shared copies, the interrupt
                      // service routines own these and could update them at any time. During the update, the
                      // shared copies may contain junk. Luckily we have our local copies to work with :-)
    }

    // do any processing from here onwards
    // only use the local values unAuxIn, unThrottleIn and unSteeringIn, the shared
    // variables unAuxInShared, unThrottleInShared, unSteeringInShared are always owned by
    // the interrupt routines and should not be used in loop

    if (gMode == MODE_RUN)
    {
        // we are checking to see if the channel value has changed, this is indicated
        // by the flags. For the simple pass through we don't really need this check,
        // but for a more complex project where a new signal requires significant processing
        // this allows us to only calculate new values when we have new inputs, rather than
        // on every cycle.
        if (bUpdateFlags & THROTTLE_FLAG)
        {
            // A good idea would be to check the before and after value,
            // if they are not equal we are receiving out of range signals
            // this could be an error, interference or a transmitter setting change
            // in any case its a good idea to at least flag it to the user somehow
            unThrottleIn = constrain(unThrottleIn, TRC_MIN, TRC_MAX);

            if (unThrottleIn > (TRC_CENTER + ERROR_center))
            {
                gThrottle = map(unThrottleIn, (TRC_CENTER + ERROR_center), TRC_MAX, PWM_MIN, PWM_MAX);
                gThrottleDirection = DIRECTION_FORWARD;
            }
            else if (unThrottleIn < (TRC_CENTER - ERROR_center))
            {
                gThrottle = map(unThrottleIn, TRC_MIN, (TRC_CENTER - ERROR_center), PWM_MAX, PWM_MIN);
                gThrottleDirection = DIRECTION_REVERSE;
            }
            else
            {
                gThrottleDirection = DIRECTION_STOP;
                gThrottle = 0;
            }

            if (gThrottle < IDLE_MAX)
            {
                gGear = GEAR_IDLE;
            }
            else
            {
                gGear = GEAR_FULL;
            }

            //Serial.println(gGear);
        }

        if (bUpdateFlags & STEERING_FLAG)
        {
            uint8_t throttleLeft = gThrottle;
            uint8_t throttleRight = gThrottle;

            gDirection = gThrottleDirection;

            // see previous comments regarding trapping out of range errors
            // this is left for the user to decide how to handle and flag
            unSteeringIn = constrain(unSteeringIn, SRC_MIN, SRC_MAX);

            // if idle spin on spot
            switch (gGear)
            {
            case GEAR_IDLE:
                if (unSteeringIn > (SRC_CENTER + RC_DEADBAND))
                {
                    gDirection = DIRECTION_ROTATE_RIGHT;
                    // use steering to set throttle
                    throttleRight = throttleLeft = map(unSteeringIn, SRC_CENTER, SRC_MAX, PWM_MIN, PWM_MAX);
                }
                else if (unSteeringIn < (SRC_CENTER - RC_DEADBAND))
                {
                    gDirection = DIRECTION_ROTATE_LEFT;
                    // use steering to set throttle
                    throttleRight = throttleLeft = map(unSteeringIn, SRC_MIN, SRC_CENTER, PWM_MAX, PWM_MIN);
                }
                break;
            // if not idle proportionally restrain inside track to turn vehicle around it
            case GEAR_FULL:
                if (unSteeringIn > (SRC_CENTER + RC_DEADBAND))
                {
                    throttleLeft = map(unSteeringIn, SRC_CENTER, SRC_MAX, gThrottle, PWM_MIN);
                }
                else if (unSteeringIn < (SRC_CENTER - RC_DEADBAND))
                {
                    throttleRight = map(unSteeringIn, SRC_MIN, SRC_CENTER, PWM_MIN, gThrottle);
                }

                break;
            }

            analogWrite(PWMB, throttleLeft);
            analogWrite(PWMA, throttleRight);
        }
    }

    if ((gDirection != gOldDirection) || (gGear != gOldGear))
    {
        gOldDirection = gDirection;
        gOldGear = gGear;

        stop();

        //Serial.print(gDirection);

        switch (gDirection)
        {
        case DIRECTION_FORWARD:
            forward();
            break;
        case DIRECTION_REVERSE:
            backward();
            break;
        case DIRECTION_ROTATE_RIGHT:
            right();
            break;
        case DIRECTION_ROTATE_LEFT:
            left();
            break;
        case DIRECTION_STOP:
            stop();
            break;
        }
    }

    bUpdateFlags = 0;
}