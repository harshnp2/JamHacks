#include <Servo.h>          //add servo, lcd display and DHT module library
#include <LiquidCrystal.h>   
#include <DHT.h>
#define Type DHT11          //define the type of the DHT module


#define ENABLE1 9          // stating the PWM pins used for the dc motor
#define ENABLE2 11




//remember to include the pin for th humidity and temperature module
int IN1 = 50, IN2 = 52;  //writing the direction pins for both the dc motors
int IN4 = 44, IN3 = 46;
int buttonPin1 = 12;     //pins connected to the 4 buttons
int buttonPin2 = 10;
int buttonPin3 = 8;
int buttonPin4 = 5;
int buttonRead1;        //used to read the button state 
int buttonRead2;
int buttonRead3;
int buttonRead4;
int rs = 36, e = 37, d4 = 38, d5 = 39, d6 = 40, d7 = 41;   //pins connected to the lcd display
int hourPlacement = 1, minPlacement = 5, secPlacement = 9; //used to help define the placement of the times 
int Time[] = {0, 0, 0}; //{hours, minutes, seconds} 
int finished = 1;  // allows the timed mode to keep watering the plants based on the time chosen
int OGHours, OGMin, OGSec;     //original time
int heatPin = 3;      //pin for the Humidity and Temperature module
DHT HT(heatPin, Type);
float humidity;       //floats used ti record the humidity and temperature
float tempC;
float tempF;
//remember to include the pin for the photoresistor
int LservoPin = 6, RservoPin = 2;   //servo pins
int lPos;                           //used to make the servo tilt based on the for loop
int rPos = 180;
Servo LtiltServo;
Servo RtiltServo;
LiquidCrystal lcd(rs,e,d4,d5,d6,d7); 
// remember to do DHT HT(humidityPin,Type);
int lightPin = A0;             //pin for photoresistor
int lightVal;                  //variable to will  be used to record the light
float lux = 0, ADC_value = 0.0048828125;   //numbers that will allow to change the units from LDR (what is measured by the photoresistor) to lux
int daysPassed = 0;

void setup() {
  Serial.begin(9600);
  HT.begin();
  pinMode(buttonPin1, INPUT);      //tell the microcontroller that these components wil be used
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  pinMode(LservoPin, INPUT);
  pinMode(RservoPin, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(ENABLE1,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(ENABLE2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);

  LtiltServo.attach(LservoPin);  //attach the name created for the servo to its pin
  RtiltServo.attach(RservoPin);
  lcd.begin(16,2);               //declare how many rows and columns my lcd display has
  //remember to do HT.begin()

}

void loop() {
  lcd.setCursor(0,0);       //asks the user which mode they want to choose through the lcd display
  lcd.print("Press the");
  lcd.setCursor(0,1);
  lcd.print("designated");
  delay(1500);
  lcd.clear();
    
  lcd.setCursor(0,0);
  lcd.print("button to pick");
  lcd.setCursor(0,1);
  lcd.print("the mode: ");
  delay(2000);
  lcd.clear();
    
  lcd.setCursor(0,0);
  lcd.print("Plant Growth, ");
  lcd.setCursor(0,1);
  lcd.print("Timed or ");
  delay(2000);
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Manual");
  delay(1500);
  lcd.clear();

  do{
    buttonRead1 = digitalRead(buttonPin1);   //reads button state while none of the buttons are pressed
    buttonRead2 = digitalRead(buttonPin2);
    buttonRead3 = digitalRead(buttonPin3);
    buttonRead4 = digitalRead(buttonPin4);
    Serial.print("Button1: ");
    Serial.print(buttonRead1);
    Serial.print(", ");
    Serial.print("Button2: ");
    Serial.print(buttonRead2);
    Serial.print(", ");
    Serial.print("Button3: ");
    Serial.print(buttonRead3);
    Serial.print(", ");
    Serial.print("Button4: ");
    Serial.println(buttonRead4);
    delay(500);
  }while(buttonRead1 == 1 && buttonRead2 == 1 && buttonRead3 == 1);

  if(buttonRead1 == 0){   //when the button1 is pressed it will go through the loop
    lcd.setCursor(0,0);   
    lcd.print("You chose");
    lcd.setCursor(0,1);
    lcd.print("Plant Growth");
    delay(2000);
    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("How old is");     //asks how old is your plant
    lcd.setCursor(0,1);           
    lcd.print("your plant?");
    delay(2000);
    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("0-30 days,");
    lcd.setCursor(0,1);
    lcd.print("31-60 days,");
    delay(2000);
    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("or 61-90 days");
    delay(1500);
    lcd.clear();

    buttonRead1 = digitalRead(buttonPin1);
    buttonRead2 = digitalRead(buttonPin2);
    buttonRead3 = digitalRead(buttonPin3);
    buttonRead4 = digitalRead(buttonPin4);

   

     
    
    if((20 <= tempC <= 25) && (82 < humidity <= 97) && (buttonRead1 == 1) && (buttonRead2 == 1) && (buttonRead3 == 1)){
    lightVal = analogRead(lightPin);             //reads the temperature, humidity and light
    humidity = HT.readHumidity();
    tempC = HT.readTemperature();
    tempF = HT.readTemperature(true);
    lux = (250.0000000/(ADC_value*lightVal))-50.0000000;
    lcd.setCursor(0,0);
    lcd.print("Temp(C): ");
    lcd.setCursor(10,0);
    lcd.print(tempC);
    lcd.setCursor(0,1);
    lcd.print("Temp(F): ");
    lcd.print(tempF);
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Humidity: ");;
    lcd.setCursor(10,0);
    lcd.print(humidity);
    lcd.setCursor(13,0);
    lcd.print("%");
    lcd.setCursor(0,1);
    lcd.print("Light(lux): ");
    lcd.setCursor(13,1);
    lcd.print(lux);           
    delay(2000);
    lcd.clear();
    }

    if((tempC < 20 && tempC > 25) && (humidity <= 82 && humidity > 97)){  //waters the plant when the conditions are ideal for the plant to grow
      for(int i = 0; i <= 3; i++){
    analogWrite(ENABLE1, 255);      //moves both the motors
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENABLE2, 255);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(2000);                //stops them after two seconds
    digitalWrite(ENABLE1, LOW);
    digitalWrite(ENABLE2, LOW);
    delay(2000);
    
    for(lPos = 0; lPos <= 180; lPos++){     //the bottle tilts and waters the plants
      LtiltServo.write(lPos);
      rPos --; 
      RtiltServo.write(rPos);
      delay(10);
    }
    delay(1000);
    for(lPos = 180; lPos >= 0; lPos--){
      LtiltServo.write(lPos);
      rPos ++;
      RtiltServo.write(rPos);
      delay(10);
    }
    
    
    }
    }

    else if((buttonRead1 == 0) && (daysPassed<=30)){      //waters the plants accordingly based on age
      lcd.setCursor(0,0);
      lcd.print("Your plant is"); 
      lcd.setCursor(0,1);
      lcd.print("0-30 days old");
      delay(2000);
      lcd.clear();
        for(int i = 0; i <= 3; i++){
        analogWrite(ENABLE1, 255);
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENABLE2, 255);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        delay(2000);
        digitalWrite(ENABLE1, LOW);
        digitalWrite(ENABLE2, LOW);
        delay(2000);
        
        for(lPos = 0; lPos <= 180; lPos++){
          LtiltServo.write(lPos);
          rPos --; 
          RtiltServo.write(rPos);
          delay(10);
        }
        delay(1000);
        for(lPos = 180; lPos >= 0; lPos--){
          LtiltServo.write(lPos);
          rPos ++;
          RtiltServo.write(rPos);
          delay(10);
        }
        delay(1000);
        
        
        }
        daysPassed ++;
        delay(86400000);
      }

      else if((buttonRead2 == 0) || (30 < daysPassed <= 60)){
        daysPassed = 31;
        lcd.setCursor(0,0);
        lcd.print("Your plant is");
        lcd.setCursor(0,1);
        lcd.print("0-30 days old");
        delay(2000);
        lcd.clear();
        for(int i = 0; i <= 3; i++){
        analogWrite(ENABLE1, 255);
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENABLE2, 255);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        delay(2000);
        digitalWrite(ENABLE1, LOW);
        digitalWrite(ENABLE2, LOW);
        delay(2000);
        
        for(lPos = 0; lPos <= 180; lPos++){
          LtiltServo.write(lPos);
          rPos --; 
          RtiltServo.write(rPos);
          delay(10);
        }
        delay(1000);
        for(lPos = 180; lPos >= 0; lPos--){
          LtiltServo.write(lPos);
          rPos ++;
          RtiltServo.write(rPos);
          delay(10);
        }
        delay(2000);
        
        
        }
        daysPassed ++;
        delay(86400000);
      }

      else if((buttonRead3 == 0) || (60 < daysPassed < 90)){
        daysPassed = 61;
        lcd.setCursor(0,0);
        lcd.print("Your plant is");
        lcd.setCursor(0,1);
        lcd.print("0-30 days old");
        delay(2000);
        lcd.clear();
        
        for(int i = 0; i <= 3; i++){
        analogWrite(ENABLE1, 255);
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENABLE2, 255);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        delay(2000);
        digitalWrite(ENABLE1, LOW);
        digitalWrite(ENABLE2, LOW);
        delay(2000);
        
        for(lPos = 0; lPos <= 180; lPos++){
          LtiltServo.write(lPos);
          rPos --; 
          RtiltServo.write(rPos);
          delay(10);
        }
        delay(1000);
        for(lPos = 180; lPos >= 0; lPos--){
          LtiltServo.write(lPos);
          rPos ++;
          RtiltServo.write(rPos);
          delay(10);
        }
        
        
        }
        daysPassed ++;
        delay(86400000);
      }
    
  }

  else if(buttonRead2 == 0){    //when the second button is pressed you choose the timed mode
    lcd.setCursor(0,0);
    lcd.print("You chose");
    lcd.setCursor(0,1);
    lcd.print("Timed");
    delay(2000);
    lcd.clear();
    buttonRead1 = digitalRead(buttonPin1);
    buttonRead2 = digitalRead(buttonPin2);
    buttonRead3 = digitalRead(buttonPin3);
    Serial.print("Button1: ");
    Serial.print(buttonRead1);
    Serial.print(", ");
    Serial.print("Button2: ");
    Serial.print(buttonRead2);
    Serial.print(", ");
    Serial.print("Button3: ");
    Serial.print(buttonRead3);
    Serial.print(", ");
    Serial.print("Button4: ");
    Serial.println(buttonRead4);
    delay(500);

    do{
    do{
    buttonRead1 = digitalRead(buttonPin1);
    buttonRead2 = digitalRead(buttonPin2);
    buttonRead3 = digitalRead(buttonPin3);
    buttonRead4 = digitalRead(buttonPin4);
    Serial.print("Button1: ");      
    Serial.print(buttonRead1);
    Serial.print(", ");
    Serial.print("Button2: ");
    Serial.print(buttonRead2);
    Serial.print(", ");
    Serial.print("Button3: ");
    Serial.print(buttonRead3);
    Serial.print(", ");
    Serial.print("Button4: ");
    Serial.println(buttonRead4);
    lcd.setCursor(0,0);             //displays the time and changes when you press the buttons to increase and decrease the time
    lcd.print("Choose your time");
    lcd.setCursor(0,1);
    lcd.print("0"); 
    lcd.setCursor(hourPlacement,1);
    lcd.print(Time[0]);
    lcd.setCursor(2,1);
    lcd.print(": 0");
    lcd.setCursor(minPlacement,1);
    lcd.print(Time[1]);
    lcd.setCursor(6,1);
    lcd.print(": ");
    if(Time[2] < 10){
    lcd.setCursor(8,1);
    lcd.print("0");
    }
    lcd.setCursor(secPlacement,1);
    lcd.print(Time[2]);
    delay(100);
    }while((buttonRead1 == 1) && (buttonRead2 == 1) && (buttonRead3 == 1) && (buttonRead4 == 1));
    
    if((buttonRead1 == 0) && (buttonRead2 == 1) && (buttonRead3 == 1)){   //when the hour button is pressed it adds an hour and displays it on the lcd display
      Time[0] = Time[0] + 1;
      lcd.setCursor(hourPlacement,1);
      lcd.print(Time[0]);
      delay(100);
      if(Time[0]>=10){
        hourPlacement = 0;
      }

      else if(Time[0]<10){
        hourPlacement = 1;
      }
      
  
    }

    else if((buttonRead1 == 1) && (buttonRead2 == 0) && (buttonRead3 == 1)){   //when the minute button is pressed it adds a minute and displays it on the lcd display
      Time[1] = Time[1] + 1;
      lcd.setCursor(minPlacement,1);
      lcd.print(Time[1]);
      delay(100);
      if(Time[1]>=10){
        minPlacement = 4;
      }

      else if(Time[1]<10){
        minPlacement = 5;
      }
      
      if(Time[1] == 60){
        Time[0] = Time[0] + 1;
        Time[1] = 0;
      
      }

      
      
    }

    else if((buttonRead1 == 1) && (buttonRead2 == 1) && (buttonRead3 == 0)){ //when the second button is pressed it adds a second and displays it on the lcd display
      Time[2] = Time[2] + 1;
      lcd.setCursor(secPlacement,1);
      lcd.print(Time[2]);
      delay(100);

      if(Time[2]>=10){
        lcd.clear();
        secPlacement = 8;
     
      }

      else if(Time[2]<10){
        secPlacement = 9;
      }
      
      if(Time[2] == 60){
        Time[1] = Time[1] + 1;
        Time[2] = 0;
      
      }
    }

       if((buttonRead1 == 0) && (buttonRead2 == 0)){ //decreases the time when you press the opposite two minutes
         Time[2] = Time[2] - 1;
       }

       else if((buttonRead1 == 0) && (buttonRead3 == 0)){
         Time[1] = Time[1] - 1;       
       }

       else if((buttonRead2 == 0) && (buttonRead3 == 0)){
         Time[0] = Time[0] - 1;
       }

       if(Time[0] < 0){
        Time[0] = 0;
       }

       else if(Time[1] < 0){
        Time[1] = 0;
       }

       else if(Time[2] < 0){
        Time[2] = 0; 
       }
       lcd.clear();

    }while(buttonRead4 == 1); 
 
    if(buttonRead4 == 0){     //the confirmed button is pressed it starts to count down
      do{
      OGHours = Time[0];
      OGMin = Time[1];
      OGSec = Time[2];
      lcd.clear();
      for(Time[2]; Time[2] >= 0; Time[2] = Time[2] - 1){
        buttonRead1 = digitalRead(buttonPin1);
        buttonRead2 = digitalRead(buttonPin2);
        buttonRead3 = digitalRead(buttonPin3);
        buttonRead4 = digitalRead(buttonPin4);
        Serial.print("Button1: ");
        Serial.print(buttonRead1);
        Serial.print(", ");
        Serial.print("Button2: ");
        Serial.print(buttonRead2);
        Serial.print(", ");
        Serial.print("Button3: ");
        Serial.print(buttonRead3);
        Serial.print(", ");
        Serial.print("Button4: ");
        Serial.println(buttonRead4);
        lcd.setCursor(0,1);
        lcd.print("0"); 
        lcd.setCursor(hourPlacement,1);
        lcd.print(Time[0]);
        lcd.setCursor(2,1);
        lcd.print(": ");
        if(Time[1] < 10){
          lcd.setCursor(4,1);
          lcd.print("0");
        }
        lcd.setCursor(minPlacement,1);
        lcd.print(Time[1]);
        lcd.setCursor(6,1);
        lcd.print(": ");
        if(Time[2] < 10){
          lcd.setCursor(8,1);
          lcd.print("0");
          secPlacement = 9;
        }
        else if(Time[2] >= 10){
          secPlacement = 8;
        }
        lcd.setCursor(secPlacement,1);
        lcd.print(Time[2]);
        delay(1000);

        if((Time[2] == 0) && (Time[1] > 0)){  
          Time[1] = Time[1] - 1;
          Time[2] = 59;
          secPlacement = 8;
          lcd.clear();
        }

        if((Time[1] == 0) && (Time[2] == 0) && (Time[0] > 0)){   //when the time reaches it waters the plant
          Time[0] = Time[0] - 1;
          Time[1] = 59;
          Time[2] = 60;
          secPlacement = 8;
          minPlacement = 4;
          lcd.clear();
        }
      }
        lcd.clear();
        Time[0] = 0;
        Time[1] = 0;
        Time[2] = 0; 

      for(int i = 0; i <= 3; i++){
    analogWrite(ENABLE1, 255);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENABLE2, 255);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(2000);
    digitalWrite(ENABLE1, LOW);
    digitalWrite(ENABLE2, LOW);
    delay(2000);
    
    for(lPos = 0; lPos <= 180; lPos++){
      LtiltServo.write(lPos);
      rPos --; 
      RtiltServo.write(rPos);
      delay(10);
    }
    delay(1000);
    for(lPos = 180; lPos >= 0; lPos--){
      LtiltServo.write(lPos);
      rPos ++;
      RtiltServo.write(rPos);
      delay(10);
    }
    
    
    }
        Time[0] = OGHours;
        Time[1] = OGMin;
        Time[2] = OGSec;
      
    }while(finished == 1); 

  }
  }

  else if(buttonRead3 == 0){  //waters the plants when the third button is pressed
    lcd.setCursor(0,0);
    lcd.print("You chose");
    lcd.setCursor(0,1);
    lcd.print("Manual");
    delay(2000);
    lcd.clear();


    for(int i = 0; i <= 3; i++){
    analogWrite(ENABLE1, 255);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENABLE2, 255);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(2000);
    digitalWrite(ENABLE1, LOW);
    digitalWrite(ENABLE2, LOW);
    delay(2000);
    
    for(lPos = 0; lPos <= 180; lPos++){
      LtiltServo.write(lPos);
      rPos --; 
      RtiltServo.write(rPos);
      delay(10);
    }
    delay(1000);
    for(lPos = 180; lPos >= 0; lPos--){
      LtiltServo.write(lPos);
      rPos ++;
      RtiltServo.write(rPos);
      delay(10);
    }
    }

    
  
  }


}
