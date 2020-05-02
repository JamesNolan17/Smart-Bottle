////////////////////////////////////////Libraries & Pre Set Up///////////////////////////////////////////////////
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
int led = 9;           // the PWM pin the LED is attached to
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

int button = 8;
int onoff = 1;

int CurrentVol = 0;
int OldVol = 0;
int VolDifff = 0;
int VolDiff = 0;
int drank = 0;
int goal = 0; //Changed when set
int voltogoal;
int reminder = 0;
int buzzer = 10;
int sendstatus = 0; // 0 for False and 1 for True
//weight sensor
long m = 0;
long n = 0;
long k = 0;
long x = 0;
long zero_factor = 0;
const int DT_PIN = 7;
const int SCK_PIN = 6;
char c;
char ble;
HX711 scale;

/////////////////////////////////////////Functions///////////////////////////////////////////////////
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
 
  m = scale.get_units(5);
  m = abs(m);
  n = scale.get_units(5);
  n = abs(n);
  k = n-m;
 
  
  
  //if the difference btwn the three values less than 20, then get the average value to be current value
  if (abs(k)<=500){
    x=((n+m)/2 - zero_factor)*0.000485;
    x=abs(x) - 184;
    //x=abs(x);
  }
 
  //Analyse data(all the data you want is in the form of "Serial.println"
  VolDifff = x - CurrentVol;
  if (abs(VolDifff)>=100){
    OldVol = CurrentVol;
    CurrentVol = x;
    VolDiff = CurrentVol - OldVol; 
    if (VolDiff<0){
      VolDiff = abs(VolDiff);
      drank = drank + VolDiff;
      voltogoal = goal - drank;
      sendstatus = 1;
    }
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
    delay(312);
  }
}

//

//Buzzing
void buzz()
{
  int a = 0;
  while (a < 2)
  {
    digitalWrite(buzzer, LOW);
    delay (200);
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

void Button()
{
 int n = digitalRead(button);
 if (onoff == 0)
 {
 if (n == HIGH)
 {
 delay(500);
 onoff++; 
 }
 }
 else 
 {
 if (n == HIGH)
 {
 delay(500);
 onoff--;
 }
 }
}

void flash()
{
  int count = 0;
  while (count < 3)
  {
  analogWrite(led,0);
  delay(100);
  analogWrite(led,50);
  delay(100);
  analogWrite(led,100);
  delay(100);
  analogWrite(led,150);
  delay(100);
  analogWrite(led,200);
  delay(100);
  analogWrite(led,250);
  delay(100);
  analogWrite(led,200);
  delay(100);
  analogWrite(led,150);
  delay(100);
  analogWrite(led,100);
  delay(100);
  analogWrite(led,50);
  delay(100);
  analogWrite(led,0);
  delay(100);
  count ++;
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
 lcd.print("I'm Hydragooals!");
 //Serial.println("System Booting...");
 delay(3000);
 lcd.clear();
 lcd.print("Waiting for Goal");
//HX711 initiation
 scale.begin(DT_PIN, SCK_PIN);
 scale.set_scale();
 scale.tare();
 zero_factor = scale.read_average(); //Get a baseline reading
  

//Buzzer
pinMode(buzzer, OUTPUT);
digitalWrite(buzzer,HIGH);

//LED
pinMode(led, OUTPUT);
//Button
pinMode(button,INPUT);
}


void loop()
{
//Set the Goal(Only enter once when goal == 1)
getgoal();
//Button
Button();
//HX711 detect and figure out currentvol and voltogoal
if (onoff == 0)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Paused");
  lcd.setCursor(0,1);
  lcd.print("Press to start");
  delay(1000);
}
else
{
measurement();
//General Display:
general();
}
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
  flash();
  buzz();
  break;
}
while (ble =='C')
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PLS drink 100ml");
  progressbar();
  flash();
  buzz();
  break;
}
while (ble =='D')
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PLS drink 150ml");
  progressbar();
  flash();
  buzz();
  break;
}
while (ble =='E')
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PLS drink 200ml");
  progressbar();
  flash();
  buzz();
  break;
}
while (ble =='F')
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PLS drink 250ml");
  progressbar();
  flash();
  buzz();
  break;
}
while (ble =='G')
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PLS drink 300ml");
  progressbar();
  flash();
  buzz();
  break;
}
while (ble =='H')
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PLS drink 350ml");
  progressbar();
  flash();
  buzz();
  break;
}
while (ble =='I')
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PLS drink 400ml");
  progressbar();
  flash();
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
  lcd.print("Start Drinking!");
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
  delay(5000);
  break;
}


while (ble =='W')
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Master");
  lcd.setCursor(0,1);
  lcd.print("I am here!");
  flash();
  buzz();
  delay(500);
  buzz();
  delay(500);
  buzz();
  delay(500);
  buzz();
  delay(500);
  buzz();
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
