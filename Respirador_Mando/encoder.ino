// ********encoder function
int myEncoderRead() {
  if ((lastEncoderPos[counter] != encoderpos[counter])) {
    encflag[counter] = LOW;
    lastEncoderPos[counter] = encoderpos[counter];
  }
  return (encoderpos[counter]);
}
void asleep() {
  if (PUSHED) {
    last_something = millis();
  }
}
void readEncoders() {
  digitalWrite(28, HIGH);
  byte counter = 0;
  if ( (digitalRead(49) ) != A_set[counter] ) {
    A_set[counter] = !A_set[counter];
    if ( A_set[counter] && !B_set[counter] )  {
      if (millis() - encodertimer > 20) {
        encoderpos[counter] += 1 * sentido_giro_encoder;
      }
      else if ( millis() - encodertimer > 10 ) {
        encoderpos[counter] += 5 * sentido_giro_encoder;
      }
      else {
        encoderpos[counter] += 10 * sentido_giro_encoder;
      }
      encodertimer = millis();
    }
    last_something = millis();
  }
  if ( (digitalRead(50) ) != B_set[counter] ) {
    B_set[counter] = !B_set[counter];
    if ( B_set[counter] && !A_set[counter] ) {
      if (millis() - encodertimer > 20) {
        encoderpos[counter] -= 1 * sentido_giro_encoder;
      }
      else if ( millis() - encodertimer > 10 ) {
        encoderpos[counter] -= 5 * sentido_giro_encoder;
      }
      else {
        encoderpos[counter] -= 10 * sentido_giro_encoder;
      }
      encodertimer = millis();
    }
    last_something = millis();
  }
  set_brightness();
  asleep();
}

void led_on() {
  //digitalWrite(28, LOW);
}
void initEncoders() {
  encodertimer = millis(); // acceleration measurement
  for (byte counter = 0; counter < MAXENCODERS; counter++)
  {
    encstate[counter] = HIGH;
    encflag[counter] = HIGH;
    A_set[counter] = false;
    B_set[counter] = false;
    encoderpos[counter] = 0;
    pinMode(encoderpinA[counter], INPUT_PULLUP);
    pinMode(encoderpinB[counter], INPUT_PULLUP);
    lastEncoderPos[counter] = 1;
  }


  timer3.configure(TC_CLOCK_PRESCALER_DIV1, // prescaler
                   TC_COUNTER_SIZE_16BIT,   // bit width of timer/counter
                   TC_WAVE_GENERATION_MATCH_FREQ // frequency or PWM mode
                  );


  timer3.setCompare(0, 4600);
  timer3.setCallback(true, TC_CALLBACK_CC_CHANNEL0, readEncoders);
  timer3.enable(true);
  newPosition = myEncoderRead();
  oldPosition = newPosition;

}

void set_brightness() {
  if ( brightness != set_brightness0 && startup_led_control == 0 && state_asleep) {
    analogWrite(TFT_LED, 255 - brightness * 25);
    set_brightness0 = brightness;

  }
}
