// Constants
const int DIRECTION_UP = 0;
const int DIRECTION_DOWN = 1;
const int STATE_AUTO = 2;
const int STATE_ON = 3;
const int STATE_OFF = 4;
const int DURATION_TICK_COUNT = 10;  // 1 minute
const int PIN_LEDS[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
const int LED_SIZE = 10;
// const int PIN_SENSOR_BOTTOM = 12;
const int PIN_SENSOR_TOP = 13;


// Variables
int previousDirection = DIRECTION_UP;
int state = STATE_AUTO;
int tickCount = 0;
bool isLedsOn = false;

void setup() {
  Serial.begin(115200);

  // Setup LEDS.
  for (int i = 0; i < LED_SIZE; i++) {
    pinMode(PIN_LEDS[i], OUTPUT);
  }

  // Setup movement sensors.
  pinMode(PIN_SENSOR_BOTTOM, INPUT);
  pinMode(PIN_SENSOR_TOP, INPUT);

  // Leds Checks
  turnOnLeds(previousDirection);
  turnOffLeds(previousDirection);

  //setting state
  setState(state);
}

void loop() {
  if (state == STATE_AUTO) {
    bool isSensorTopMoved = (digitalRead(PIN_SENSOR_TOP) == HIGH);
    bool isSensorBottomMoved = (digitalRead(PIN_SENSOR_BOTTOM) == HIGH);

    log("tickCount: ", tickCount);

      if (isSensorTopMoved) {
      if (isLedsOn) {
        updateLastMoved();
        updatePreviousDirection(DIRECTION_DOWN);
        log("Sensor Top Moved");
      } else {
        turnOnLeds(DIRECTION_DOWN);
      }
      log("");
    }
    else if (isSensorBottomMoved) {
      if (isLedsOn) {
        updateLastMoved();
        updatePreviousDirection(DIRECTION_UP);
        log("Sensor Bottom Moved");
      } else {
        turnOnLeds(DIRECTION_UP);
      }
      log("");
    }
    else {
      if (isTimeUp() && isLedsOn) {
        log("Time up, tick count: ", tickCount);
        turnOffLeds(previousDirection);
        log("");
      }
    }
    if (isLedsOn) {
      tickCount++;
    }
  } else if (state == STATE_ON) {
    turnOnLeds(DIRECTION_UP);
  } else if (state == STATE_OFF) {
    turnOffLeds(DIRECTION_UP);
  }
  delay(250);
}

void updateLastMoved() {
  tickCount = 0;
}

bool isTimeUp() {
  return tickCount >= DURATION_TICK_COUNT;
}

void updatePreviousDirection(int direction) {
  previousDirection = direction;
}

void turnOnLeds(int direction) {
  log("Turning leds on direction: ", direction);
  if (direction == DIRECTION_UP) {
    // Turn on leds from bottom to top
    for (int i = 0; i < LED_SIZE; i++) {
      digitalWrite(PIN_LEDS[i], LOW);
      delay(200);
    }
  } else {
    // Turn on leds from top to bottom
    for (int i = LED_SIZE - 1; i >= 0; i--) {
      digitalWrite(PIN_LEDS[i], LOW);
      delay(200);
    }
  }

  updatePreviousDirection(direction);
  updateLastMoved();
  isLedsOn = true;
}

void turnOffLeds(int direction) {
  log("Turning leds off direction: ", direction);
  if (direction == DIRECTION_UP) {
    // Turn off leds from bottom to top
    for (int i = 0; i < LED_SIZE; i++) {
      digitalWrite(PIN_LEDS[i], HIGH);
      delay(200);
    }
  } else {
    // Turn off leds from top to bottom
    for (int i = LED_SIZE - 1; i >= 0; i--) {
      digitalWrite(PIN_LEDS[i], HIGH);
      delay(200);
    }
  }
  updateLastMoved();
  isLedsOn = false;
}

void setState(int tmpState) {
  state = tmpState;
}

void log(String str) {
  Serial.println(str);
}
void log(String str, long i) {
  Serial.println(str + i);
}