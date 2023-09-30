// Constants
const int DIRECTION_UP = 0;
const int DIRECTION_DOWN = 1;
const int STATE_ON = 3;
const int STATE_OFF = 4;
const int DURATION_TICK_COUNT = 60;  // 15 secs
const int PIN_LEDS[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
const int LED_SIZE = 10;
const int PIN_SENSOR_TOP = 13;

// Variables
int tickCount = 0;
bool isLedsOn = false;

void setup() {
  Serial.begin(115200);

  // Setup LEDS.
  for (int i = 0; i < LED_SIZE; i++) {
    pinMode(PIN_LEDS[i], OUTPUT);
  }

  // Setup movement sensors.
  pinMode(PIN_SENSOR_TOP, INPUT);

  // Leds Checks
  turnOnLeds();
  turnOffLeds();
}

void loop() {
  bool isSensorTopMoved = (digitalRead(PIN_SENSOR_TOP) == HIGH);
  log("tickCount: ", tickCount);

  if (isSensorTopMoved) {
    if (isLedsOn) {
      updateLastMoved();
      log("Sensor Top Moved");
    } else {
      turnOnLeds();
    }
    log("");
  }
  else {
    if (isTimeUp() && isLedsOn) {
      log("Time up, tick count: ", tickCount);
      turnOffLeds();
      log("");
    }
  }
  if (isLedsOn) {
    tickCount++;
  }
  delay(250);
}

void updateLastMoved() {
  tickCount = 0;
}

bool isTimeUp() {
  return tickCount >= DURATION_TICK_COUNT;
}

void turnOnLeds() {
  log("Turning leds on");
  // Turn on leds from bottom to top
  for (int i = 0; i <= LED_SIZE/2; i++) {
    digitalWrite(PIN_LEDS[i], LOW);
    digitalWrite(PIN_LEDS[LED_SIZE-i], LOW);
    delay(200);
  }

  updateLastMoved();
  isLedsOn = true;
}

void turnOffLeds() {
  log("Turning leds off");
  // Turn off leds from bottom to top
  for (int i = 0; i <= LED_SIZE/2; i++) {
    digitalWrite(PIN_LEDS[i], HIGH);
    digitalWrite(PIN_LEDS[LED_SIZE-i], HIGH);
    delay(200);
  }
  updateLastMoved();
  isLedsOn = false;
}

void log(String str) {
  Serial.println(str);
}
void log(String str, long i) {
  Serial.println(str + i);
}