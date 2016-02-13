Arduino Code 
  #include <Wire.h>
    const int MPU=0x68;  // I2C address of the MPU-6050
    int16_t Tmp,GyX,GyY,GyZ,AcX,AcY,AcZ;
    #include <Servo.h> 
 int d=0;
 int X,Y;
 int sensor0,sensor1,sensor2,sensor3,A,B,C,a;
 #include<LiquidCrystal.h>
  int pushButton = 7; 
 //int buttonState ; 

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  // sets the interfacing pins

    void setup(){
      lcd.begin(16, 2);  // initializes the 16x2 LCD
       digitalWrite(A0,HIGH);
      digitalWrite(A1,HIGH);  
      digitalWrite(A2,HIGH);
      digitalWrite(A3,HIGH);
     
      Wire.begin();
      Wire.beginTransmission(MPU);
      Wire.write(0x6B);  // PWR_MGMT_1 register
      Wire.write(0);     // set to zero (wakes up the MPU-6050)
      Wire.endTransmission(true);
      //pinMode(pushButton, INPUT);
      Serial.begin(9600);
    }
    void loop(){
     {
      
      Wire.beginTransmission(MPU);
      Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      Wire.requestFrom(MPU,14,true);  // request a total of 14 registers
      GyX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
      GyY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
     
     X = map(GyX,-16000,16000,-100,100);
      Y = map(GyY,-16000,16000,-100,100);
     }
     
     {
      sensor0 = analogRead(A0);
      sensor1 = analogRead(A1);
      sensor2 = analogRead(A2);
      sensor3= analogRead(A3);
      A = map(sensor0,470,700 ,0,100);
      B = map(sensor1,410,600,0,100);
      C = map(sensor2,460,720,0,100);
      //buttonState = digitalRead(pushButton);
     }
     
  if(A>35 && B>55 && C>20)
  {
    if(d!=0)
    {
      d=0;
     Serial.println("0");
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("    Clamped");
     
  }   
  }
  
  // print out the state of the but
  else if(sensor3>500)
{
  
    if(d!=a)
    {
      d=a;
     Serial.println("a");
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("   Unclamped");
    }
}
   
  else if (A>75 && B>35 )
  {
  if (d!=8)
  {
    d=8;
  Serial.println("8");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("    Forward");
   }
  
 delay(700); }  
 
  else if(B>35 && C>40){
    if(d!=2){
      d=2;
  Serial.println("2");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("    Backward");
}
delay(700);   }
  else if(C>40){
    if(d!=6){
      d=6;
  Serial.println("6");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("     Right");
}
delay(700);      }
else if(A>75){
  if(d!=4){
    d=4;
Serial.println("4");
lcd.clear();
lcd.setCursor(0,0);
lcd.print("      Left");
}
delay(700);  }
    
else if(X<-70){//DC motor left
    if(d!=51)
    {
      d=1;
  Serial.println("1");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("    DC left");
    }
  delay(700);      }
else if(X>85){  //DC motor right
  if(d!=32){
    d=3;
Serial.println("3");
lcd.clear();
lcd.setCursor(0,0);
lcd.print("    DC right");
      }
delay(700);      }
else if(Y>60){  //servo motor front
  if(d!=9){
    d=9;
Serial.println("9");
lcd.clear();
lcd.setCursor(0,0);
lcd.print("   Arm front");

      }
delay(700);      }
else if(Y<-80){  //servo motor back
  if(d!=7){
    d=7;
Serial.println("7");
lcd.clear();
lcd.setCursor(0,0);
lcd.print("    Arm Back");

      }
delay(700);      }

else if(d!=5){
  d=5;
Serial.println("5");
lcd.clear();
lcd.setCursor(0,0);
lcd.print("      Stop");

}
delay(700);
}
     
    
