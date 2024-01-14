
struct Registry {

  float humidity = 0;
  float temperature = 0;
  uint16_t ppm = 0; 

  bool fanTrigger = false;
  uint8_t fanLevel = 0;
  unsigned long fanTriggerTime = 0;
  unsigned long fanAdjustTime = 0;
  unsigned long fanSwitchTime = 0;

  bool printToSerial();
};

// free RAM check for debugging. SRAM for ATmega328p = 2048Kb.
int availableMemory() {
    // Use 1024 with ATmega168
    int size = 2048;
    byte *buf;
    while ((buf = (byte *) malloc(--size)) == NULL);
        free(buf);
    return size;
}

bool Registry::printToSerial() {
  Serial.print("wt=");
  Serial.print(millis());

  Serial.print("\th=");
  Serial.print(this->humidity);

  Serial.print("\tt=");
  Serial.print(this->temperature);

  Serial.print("\tc=");
  Serial.print(this->ppm);

  Serial.print("\ttr=");
  Serial.print(this->fanTrigger);

  Serial.print("\tl=");
  Serial.print((int)this->fanLevel);

  Serial.print("\ttt=");
  Serial.print(this->fanTriggerTime);

  Serial.print("\tst=");
  Serial.print(this->fanSwitchTime);

  Serial.print("\tat=");
  Serial.print(this->fanAdjustTime);

  Serial.print("\tm=");
  Serial.print(availableMemory());

  Serial.println();

  return true;
}



