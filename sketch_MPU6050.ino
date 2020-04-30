#include <Wire.h>
#include <LowPower.h>
const int MPU = 0x68; // MPU6050 I2C address
byte Acc;
byte FIFO_enable,Frenquency,Cycle_mode,Px,Ix;
byte f,s;
short c;
float AccX, AccY, AccZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float elapsedTime, currentTime, previousTime;
float GyroX, GyroY, GyroZ;
float roll, pitch, yaw;
int h = 0;
void setup() {
  Serial.begin(19200);
  Wire.begin();                      // Initialize comunication
  
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // reset 
  Wire.write(0x00);                  
  Wire.endTransmission(true);       

  Wire.beginTransmission(MPU);       // Enable FIFO
  Wire.write(0x23); 
  Wire.endTransmission();
  Wire.requestFrom(MPU, 1, true);
  FIFO_enable = Wire.read();
  Wire.beginTransmission(MPU);      
  Wire.write(0x23); 
  Wire.write(FIFO_enable|0x08);  
  Wire.endTransmission(true);
//  Serial.print("FIFO_enable: ");
//  Serial.println(FIFO_enable);
//
//
  Wire.beginTransmission(MPU);       // Setting to Cycle mode
  Wire.write(0x6B); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 1, true);
  Cycle_mode = Wire.read();
  Wire.write((Cycle_mode|0x28)&0xBE);
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU);       // Setting to Wake up frenquency into 1.25Hz
  Wire.write(0x6C); 
  Wire.endTransmission();
  Wire.requestFrom(MPU, 1, true);
  Frenquency = Wire.read(); 
  Wire.beginTransmission(MPU); 
  Wire.write(0x6C); 
  Wire.write((Frenquency|0x07)&0x3E);
  Wire.endTransmission(true);  

  Wire.beginTransmission(MPU);       // interrupt enable 
  Wire.write(0x38); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 1, true);
  Ix = Wire.read();

  Wire.beginTransmission(MPU);       
  Wire.write(0x38); 
  Wire.write(Ix|0x10);
  Wire.endTransmission(true);  
 
  
//  Wire.beginTransmission(MPU);       // count the number of bytes stored in the FIFO buffer
//  Wire.write(0x72); 
//  Wire.endTransmission(false);
//  Wire.requestFrom(MPU, 1, true);
//  f = Wire.read();
//  Wire.endTransmission(true);
//
//  Wire.beginTransmission(MPU);       // count the number of bytes stored in the FIFO buffer
//  Wire.write(0x73); 
//  Wire.endTransmission(false);
//  Wire.requestFrom(MPU, 1, true);
//  s = Wire.read();
//  Wire.endTransmission(true);
//
//  c = (f << 8 | s) ;
//  Serial.print("c: ");
//  Serial.println(c);
//        
//  delay(20);
//
//  Wire.beginTransmission(MPU);       
//  Wire.write(0x75); 
//  Wire.endTransmission();
//  Wire.requestFrom(MPU, 1, true);
//  Px = Wire.read();
//  Serial.print("Px: ");
//  Serial.println(Px);

}
//
//  Wire.beginTransmission(MPU);       // count the number of bytes stored in the FIFO buffer
//  Wire.write(0x72); 
//  Wire.endTransmission(false);
//  Wire.requestFrom(MPU, 2, true);
//  c = (Wire.read() << 8 | Wire.read());
//  Wire.endTransmission(true);
//        Serial.print("c: ");
//        Serial.println(c);

void loop() {
  Wire.beginTransmission(MPU);       // count the number of bytes stored in the FIFO buffer
  Wire.write(0x72); 
  Wire.endTransmission();
  Wire.requestFrom(MPU, 2, true);
  while(Wire.available() != 2);
  byte c1 = Wire.read();
  byte c2 = Wire.read();
  c = ((short)c1 << 8 )| (short)c2;
  
  Serial.print("c: ");
  Serial.println(c);

    while (h < c)
       {
         Wire.beginTransmission(MPU);
         Wire.write(0x74);
         Wire.endTransmission();
         Wire.requestFrom(MPU, 1, true);
         Acc = (Acc << 8 | Wire.read()) ;
         h++;
         if ( Acc % 3 == 0)
         {
         AccX = Acc;
         }

         if ( Acc % 3 == 1)
         {
         AccY = Acc;
         }
         if ( Acc % 3 == 2)
         {
         AccZ = Acc;
         }
//  Serial.print("AccX: ");
//  Serial.println(AccX);
//
//  Serial.print("AccY: ");
//  Serial.println(AccY);
//
//  Serial.print("AccZ: ");
//  Serial.println(AccZ);
      }   
    
//    while(1);

  
  // Calculating Roll and Pitch from the accelerometer data
  float(Acc);
  accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) - 0.58; // AccErrorX ~(0.58) See the calculate_IMU_error()custom function for more details
  accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) + 1.58; // AccErrorY ~(-1.58)
  // === Read gyroscope data === //
  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds
  Wire.beginTransmission(MPU);
  Wire.write(0x43); // Gyro data first register address 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 4 registers total, each axis value is stored in 2 registers
  GyroX = (Wire.read() << 8 | Wire.read()) / 131.0; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
  GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;
//  Serial.print(GyroX);
//  Serial.print("/");
//  Serial.print(GyroY);
//  Serial.print("/");
//  Serial.println(GyroZ);
  // Correct the outputs with the calculated error values
  GyroX = GyroX + 0.56; // GyroErrorX ~(-0.56)
  GyroY = GyroY - 2; // GyroErrorY ~(2)
  GyroZ = GyroZ + 0.79; // GyroErrorZ ~ (-0.8)
  // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
  gyroAngleX = gyroAngleX + GyroX * elapsedTime; // deg/s * s = deg
  gyroAngleY = gyroAngleY + GyroY * elapsedTime;
  yaw =  yaw + GyroZ * elapsedTime;
  // Complementary filter - combine acceleromter and gyro angle values
  roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
  pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;
  
  // Print the values on the serial monitor
  Serial.print(roll);
  Serial.print("/");
  Serial.print(pitch);
  Serial.print("/");
  Serial.println(yaw);


//
//
//    Wire.beginTransmission(MPU);       // test if FIFO is FUll
//    Wire.write(0x3A); 
//    Wire.endTransmission(false);
//    Wire.requestFrom(MPU, 1, true);
//    Px = Wire.read();
//    Wire.endTransmission(true);
// 
//    if((Px&0x08)>1)                    //FIFO is FULL
//       {
//        Serial.print("Acc: ");
//        Serial.println(Acc);
//       }
    
}
