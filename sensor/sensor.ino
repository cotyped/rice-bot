
#define OUTPUT_SENSITIVITY 100
#define BAUD 9600

#define READ_PERIOD_LENGTH 4000

#define INPUT_PIN A1

static inline void send_data(double vpp, double amps, double watts, double maxVp, double minVp, double duration) {
  double values[] = {vpp, amps, watts, maxVp, minVp, duration};
  for(unsigned i = 0; i < sizeof(values)/sizeof(double); ++i) {
    Serial.print(values[i]);
    if(i < sizeof(values)/sizeof(double) - 1) {
      Serial.print(",");
    }
  }
  Serial.println();
}

void setup() {
  Serial.begin(BAUD);
}

double getVPP(double *mxvp, double *mnvp) {
  double maxVp = 0;
  double minVp = 1024;
  // Sample for one second to get accurate values
  double start = millis();
  while((millis() - start) < READ_PERIOD_LENGTH) {
    double reading = analogRead(INPUT_PIN);
    if(reading > maxVp) {
      maxVp = reading;
    }
    if (reading < minVp) {
      minVp = reading;
    }
  }
  *mxvp = maxVp;
  *mnvp = minVp;
  return ((maxVp - minVp) * 2.77453) / 1024.0;
}

void loop() {
  double maxVp = 0;
  double minVp = 0;
  double vpp = getVPP(&maxVp, &minVp);
  double vRMS = (vpp / 2.0) * 0.707;
  double ampsRMS = (vRMS * 1000) / OUTPUT_SENSITIVITY;
  double wattsRMS = ampsRMS * 120.0;
  send_data(vpp, ampsRMS, wattsRMS, maxVp, minVp, READ_PERIOD_LENGTH);
}
