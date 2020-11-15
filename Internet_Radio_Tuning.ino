const int numReadings = 50;

float readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
float total = 0;                  // the running total
float average = 0;                // the average
const int OUT_PIN = A4;
const int IN_PIN = A0;
const int IN_VOLUME = A1;
const float IN_STRAY_CAP_TO_GND = 24.48;
const float IN_CAP_TO_GND  = IN_STRAY_CAP_TO_GND;
const float R_PULLUP = 34.8;
const int MAX_ADC_VALUE = 1023;
const int tuningBulb = 5;
int station = 0;
int volume = 0;
int previousStation = 0;
void setup()
{
  pinMode(OUT_PIN, OUTPUT);
  pinMode(IN_PIN, OUTPUT);
  pinMode(IN_VOLUME, INPUT);
  pinMode(tuningBulb, OUTPUT);
  digitalWrite(tuningBulb, false);
  Serial.begin(9600);             // initialize serial transmission for debugging
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop()
{

  total = total - readings[readIndex];
  pinMode(IN_PIN, INPUT);
  digitalWrite(OUT_PIN, HIGH);
  int val = analogRead(IN_PIN);
  digitalWrite(OUT_PIN, LOW);

  volume = analogRead(IN_VOLUME);
  //Serial.println(volume);

  if (val < 1000)
  {
    pinMode(IN_PIN, OUTPUT);

    float capacitance = (float)val * IN_CAP_TO_GND / (float)(MAX_ADC_VALUE - val);

    readings[readIndex] = capacitance;
    total = total + readings[readIndex];
    // advance to the next position in the array:
    readIndex = readIndex + 1;

    // if we're at the end of the array...
    if (readIndex >= numReadings) {
      // ...wrap around to the beginning:
      readIndex = 0;
    }

    // calculate the average:
    average = total / numReadings;
    // send it to the computer as ASCII digits
     //Serial.println(average);
    if (readIndex == 49) {
      if (12 < average && average < 18) {
        announceStation(1);
        // Radio Caroline
      }
      else if (23 < average && average < 31) {
        announceStation(2);
        // Radio 6 Music
      }
      else if (38 < average && average < 46) {
        announceStation(3);
        // BBC Radio 1
      }
      else if (58 < average && average < 65) {

        announceStation(4);
        // BBC Radio 2
      }
      else if (72 < average && average < 90) {
        announceStation(5);
        // BBC Radio 4
      }
      else if (127 < average && average < 144) {
        announceStation(6);
        // BBC World Service
      }
      else if (173 < average && average < 187) {
        // Classic FM
        announceStation(7);
      }
      else {

        announceStation(0);
      }

    }
  }

}
void tuningLight(boolean onOff) {
  digitalWrite(tuningBulb, onOff);
}

void announceStation(int staNum) {
  if (previousStation != staNum) {

    station = staNum;

    previousStation = station;
    if (station == 0) {
      tuningLight(false);
    }
    else {
      tuningLight(true);
      //Serial.println(station); // debug purpose
      Serial.write(station); //to send to esp32
    }

  }


}
