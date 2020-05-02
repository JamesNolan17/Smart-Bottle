////////////////////////////////////////Libraries & Pre Set Up/////////////////////////////////////////////
#include <LiquidCrystal.h>
#include<SoftwareSerial.h>
#include <HX711.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <Arduino.h>

//1206
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Values
int CurrentVol = 0;
int OldVol = 0;
int VolDiff = 0;
int drank = 0;
int goal = 0; //Changed when set
int voltogoal;
int reminder = 0;
int buzzer = 10;
int sendstatus = 0; // 0 for False and 1 for True
//weight sensor
int m = 0;
int n = 0;
int k = 0;
int bottle = 150; //the weight of bottle
const int DT_PIN = 7;
const int SCK_PIN = 6;
char c;
char ble;
HX711 scale;

/////////////////////////////////////////Extra Functions///////////////////////////////////////////////////
void general()
{
  digitalWrite(buzzer,HIGH);
  int voltogoal = goal - drank;
  //Display
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CurrentVol:");
  lcd.setCursor(11,0);
  lcd.print(CurrentVol);
  lcd.setCursor(0,1);
  lcd.print("VolToGoal:");
  lcd.setCursor(11,1);
  lcd.print(voltogoal);
  delay(1000);
}
//Get Volume + Get voldiff
void measurement()
{
 
  //get three average value
  n = scale.get_units(10);
  n = abs(n);
  m = scale.get_units(10); 
  m = abs(m);
  k = scale.get_units(10);
  k = abs(k);
  
 
  //if the difference btwn the three values less than 20, 
  //then get the average value to be current value
  if (abs(n-m)<=30 and abs(m-k)<=30 and abs(n-k)<=30){
    OldVol = CurrentVol;
    CurrentVol=((n+m+k)/3)*0.596;
    CurrentVol=abs(CurrentVol);
    if (CurrentVol<50){
      CurrentVol = 0;
    }
  }
 
  //Analyse data(all the data you want is in the form of "Serial.println")
  VolDiff = CurrentVol - OldVol;
  if (abs(VolDiff)>=100){
    //Serial.print("Current Volume:");
    //Serial.println(CurrentVol);
    VolDiff = CurrentVol - OldVol;
    if (VolDiff<0){
      VolDiff = abs(VolDiff);
      drank = drank + VolDiff;
      voltogoal = goal - drank;
      sendstatus = 1;
      //Serial.print("Volume Difference:");
      //Serial.println(VolDiff);
    }
    OldVol=CurrentVol;
  }  
}
//Progressbar >>
void progressbar()
{
lcd.setCursor(0,1);
  lcd.print(">");
  int i=0;
  while (i<17)
  {
    lcd.setCursor(i,1);
    lcd.print(">");
    i++;
    delay(1000);
  }
}


//Buzzing
void buzz()
{
  int a = 0;
  while (a < 3)
  {
    digitalWrite(buzzer, LOW);
    delay (500);
    digitalWrite(buzzer,HIGH);
    a++;
  }
  digitalWrite(buzzer,HIGH);
}
//Receive goal
void getgoal()
{
  while (goal == 0)
{
  while(Serial.available())
  {
    char c = Serial.read();
    if (c == '1')
    {
      //Serial.println("Judged");
      goal = 1600;  
    }
    if (c == '2')
    {
      //Serial.println("Judged");
      goal = 1800;  
    }
    if (c == '3')
    {
      //Serial.println("Judged");
      goal = 2000;  
    }
    if (c == '4')
    {
      //Serial.println("Judged");
      goal = 2200;  
    }
    if (c == '5')
    {
      //Serial.println("Judged");
      goal = 2400;  
    }
    if (c == '6')
    {
      //Serial.println("Judged");
      goal = 2600;  
    }
    if (c == '7')
    {
      //Serial.println("Judged");
      goal = 2800;  
    }
    if (c == '8')
    {
      //Serial.println("Judged");
      goal = 3000;  
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("     Goal:");
    lcd.setCursor(12,0);
    lcd.println(goal);
    //Serial.println(c);
    delay(5000);
  }
}
}

///////////////////////////////////////////Body///////////////////////////////////////////////////////////
void setup()
{
//Bluetooth
 Serial.begin(9600);
 //Serial.println("Bluetooth Start Sucessesfully");
//Screen
 lcd.begin(16,2);
 lcd.print("System Booting...");
 //Serial.println("System Booting...");
 delay(2000);
 lcd.clear();
 lcd.print("Waiting for Goal");
//HX711 initiation
 scale.begin(DT_PIN, SCK_PIN);
 scale.set_scale();
 scale.tare();

//Buzzer
pinMode(buzzer, OUTPUT);
digitalWrite(buzzer,HIGH);
}


void loop()
{
//Set the Goal(Only enter once when goal == 1)
getgoal();
//HX711 detect and figure out currentvol and voltogoal
measurement();
//General Display:
general();

//Daily waiting for signal(should put all of the daily data receive job here)
while (Serial.available())
{
  ble = Serial.read();
//Reminders
while (ble =='B')
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PLS drink 50ml");
  progressbar();
  buzz();
  break;
}
while (ble =='C')
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PLS drink 100ml");
  progressbar();
  buzz();
  break;
}
while (ble =='D')
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PLS drink 150ml");
  progressbar();
  buzz();
  break;
}
while (ble =='E')
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PLS drink 200ml");
  progressbar();
  buzz();
  break;
}
while (ble =='F')
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PLS drink 250ml");
  progressbar();
  buzz();
  break;
}
while (ble =='G')
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PLS drink 300ml");
  progressbar();
  buzz();
  break;
}
while (ble =='H')
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PLS drink 350ml");
  progressbar();
  buzz();
  break;
}
while (ble =='I')
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PLS drink 400ml");
  progressbar();
  buzz();
  break;
}

//Other signals
while (ble =='A')
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Good Morning!");
  lcd.setCursor(0,1);
  lcd.print("&Start Drinking!");
  delay(7000);
  break;
}
while (ble =='Z')
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Good Job! Done");
  lcd.setCursor(0,1);
  lcd.print("Drank");
  lcd.setCursor(7,1);
  lcd.print(drank);
  delay(7000);
  break;
}
while (ble =='U')
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Updating goal");
  delay(7000);
  break;
}


}


//Send the water inject data to Android
if (sendstatus == 1)
  {
  Serial.println(VolDiff);
  sendstatus = 0;
  }
}
