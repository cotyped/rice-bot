#define OUTPUT_SENSITIVITY 100
#define BAUD 9600
#define READ_LENGTH 5000

#define COMM_START "rc_start"
#define INPUT_PIN A1

static inline void debug(double vpp, double amps, double watts)
{
  Serial.print("Voltage: ");
  Serial.println(vpp, 3);
  Serial.print("Amps: ");
  Serial.println(amps, 3);
  Serial.print("Power: ");
  Serial.println(watts, 3);
}

void setup()
{
  Serial.begin(BAUD);
  Serial.println(COMM_START);
}

double getVPP()
{
  double maxVp = 0;
  double minVp = 1024;
  // Sample for one second to get accurate values
  double start = millis();
  while ((millis() - start) < READ_LENGTH)
  {
    double reading = analogRead(INPUT_PIN);
    if (reading > maxVp)
    {
      maxVp = reading;
    }
    if (reading < minVp)
    {
      minVp = reading;
    }
  }
  // Constants are emperically determined
  Serial.println(maxVp);
  Serial.println(minVp);
  return ((maxVp - minVp) * 2.77453) / 1024.0;
}

void loop()
{
  double vpp = getVPP();
  double vRMS = (vpp / 2.0) * 0.707;
  double ampsRMS = (vRMS * 1000) / OUTPUT_SENSITIVITY;
  double wattsRMS = ampsRMS * 120.0;
  debug(vpp, ampsRMS, wattsRMS);
}