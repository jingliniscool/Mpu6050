void Enable_FIFO() {
  Wire.beginTransmission(MPU);       // Enable FIFO
  Wire.write(0x23); 
  Wire.endTransmission();
  Wire.requestFrom(MPU, 1, true);
  FIFO_enable = Wire.read();
  Wire.beginTransmission(MPU);      
  Wire.write(0x23); 
  Wire.write(FIFO_enable|0x08);  
  Wire.endTransmission(true);
}

void CycleMode_Set() {
  Wire.beginTransmission(MPU);       // Setting to Cycle mode
  Wire.write(0x6B); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 1, true);
  Cycle_mode = Wire.read();
  Wire.write((Cycle_mode|0x28)&0xBE);
  Wire.endTransmission(true);
}

void Frenquency_Set() {
    Wire.beginTransmission(MPU);       // Setting to Wake up frenquency into 1.25Hz
  Wire.write(0x6C); 
  Wire.endTransmission();
  Wire.requestFrom(MPU, 1, true);
  Frenquency = Wire.read(); 
  Wire.beginTransmission(MPU); 
  Wire.write(0x6C); 
  Wire.write((Frenquency|0x07)&0x3E);
  Wire.endTransmission(true); 
}

void Interrupt_Enable() {
  Wire.beginTransmission(MPU);       // interrupt enable 
  Wire.write(0x38); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 1, true);
  Ix = Wire.read();
  Wire.beginTransmission(MPU);       
  Wire.write(0x38); 
  Wire.write(Ix|0x10);
  Wire.endTransmission(true);  
}
