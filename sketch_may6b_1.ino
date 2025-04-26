#include <Keypad.h>
#include <Servo.h>
#include <LiquidCrystal.h>

//declaring the pin of arduino 
const int  rs = 6, en = 5, d4 = 4, d5 = 3, d6 = 2, d7 = 1; 
const int  IR_Pin=A0;
const int  Servo_Pin=0;
const int  LDR_pin=A1;
const int  LM_pin=A2;
const int  fan_pin=A3;
const int  Led_pin=A4;
const int  greenLed_pin=A5;

//declaring the number of COLs and ROWs of keypad
const byte ROWS = 4; 
const byte COLS = 3;

// the lenght of the password
const int MAX_PASSWORD_LENGTH=4;  
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//declaring the servo
Servo garden; 

//configuring the switches of the keypad
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
}; 

// configuring the pins of keypad
byte rowPins[ROWS] = {13, 12, 11, 10}; 
byte colPins[COLS] = {9, 8, 7}; 

//initialization of the keypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup() 
{
// put your setup code here, to run once:
garden.attach(Servo_Pin); 
lcd.begin(16, 2);  // initializing the type of lcd

//setting the mode of pins 
pinMode(IR_Pin,INPUT);
pinMode(fan_pin,OUTPUT);
pinMode(Led_pin,OUTPUT);
pinMode(greenLed_pin,OUTPUT);
//setting the initial values of led and fan
digitalWrite(Led_pin,LOW);
digitalWrite(fan_pin,LOW);
digitalWrite(greenLed_pin,LOW);
}

void loop() 
{
  int y=0;
  while (y==0)
  {
    if (H_CtrlGardenDoor() == 1) 
    {
      y=H_Hello();
    }
  }  
  while (1) 
    {
      int x=0;
      float temp = H_LM_value(); 
      int ledValue =  map(H_LDR_value(), 0, 1023 , 255, 0);
      // Control fan and light based on sensors
      H_FanMode();
      H_LedMode();

      // Display temperature, LED value, and fan status on LCD
      lcd.clear();
      lcd.print("Temp: ");
      lcd.print(temp);
      lcd.print(" C");
      lcd.setCursor(0, 1);
      lcd.print("LED: ");
      lcd.print(ledValue);
      lcd.print(" Fan: ");
      //if the fan is on display FAN ON on LCD , if OFF display FAN OFF
      lcd.println(digitalRead(fan_pin) == HIGH ? "ON" : "OFF");
      
      delay(1000); 
      if (H_CtrlGardenDoor() == 1) 
      {
        x=H_Hello();
        // Waiting user to enter correct password
        while (x==0)
        {
          x=H_Hello();
        }
      }
    }
}


//
int H_Hello()
{
  lcd.println("Hi in garden");
  delay(3000);
  char* enteredPassword = H_getPassword(); // Get the entered password
  int result = H_checkPassword(enteredPassword); // Check the entered password
  if (result==1) // if the correct password
  {
    digitalWrite(greenLed_pin,HIGH);
    lcd.clear();
    lcd.println("Hello in home");
    delay(3000);
    digitalWrite(greenLed_pin,LOW);      
    return 1 ;
  }
  else if (result==0) // if the password is wrong
  {
    int i=0;
    lcd.clear();
    lcd.println("wrong password");
    // giving an alarm by blinking the green led 5 times
    while (i<5)
    {
      digitalWrite(Led_pin,HIGH);
      delay(500);
      digitalWrite(Led_pin,LOW);
      delay(500);
      i++;
    }
    lcd.clear();
    return 0;
  }
}

//function to get the password
char* H_getPassword()
{
  static char password[MAX_PASSWORD_LENGTH + 1]; 
  char key;
  int index = 0;

  lcd.clear();
  lcd.print("Enter Password:");
  lcd.setCursor(0, 1);
  // Read characters until maximum password length is reached or pressing #
  while (index < MAX_PASSWORD_LENGTH)
   {
    key = customKeypad.getKey();
    // using # as enter 
    if (key == '#') 
    {
      break;
    }

    // Check if a key is pressed
    if (key) 
    {
      password[index++] = key;
      lcd.print("*"); // Print '*' to mask the entered password
      delay(200); 
    }
  }

  password[index] = '\0'; //adding null at the end to mark the end of password's array

  return password;
}

//function to check the password it receives an entered password and if it true it returns 1 .. if false returns 0
int H_checkPassword(char* enteredPassword) 
{
  char password[] = "1234";
  //checking if the password is correct or not
  if (strcmp(enteredPassword, password) == 0)
  {
    return 1; // Password is correct
  }
   else
  {
    return 0; // Password is wrong
  }
}

//function to return the IR sensor if move detected it returns 1
int H_IRSensorVal()
{
  int IR_val =0;
  IR_val= digitalRead(IR_Pin);
  if (IR_val==0)
  {
    //motion detected
    return 1;
  }
  else 
  {
    //no motion
    return 0;     
  }

}

//function to control the garden door
int H_CtrlGardenDoor()
{
  int IR_val =H_IRSensorVal();
  if (IR_val==1)
  {
    H_openGarden();
    lcd.clear();
    lcd.println("GARDEN DOOR");
    lcd.setCursor(0, 1);
    lcd.println("IS OPENED");
    delay(2000);
    lcd.clear();
    return 1;
  }
  else 
  {
    H_closeGarden();
    return 0;     
  }

}

// function to open the garden door
void H_openGarden()
{
  garden.write(90);
  delay(500);
}

//function to close the garden door
void H_closeGarden()
{
  garden.write(0);
  delay(500);
}

// function returns the value of LDR
int H_LDR_value()
{
  int LDR_VAL=0;
  LDR_VAL=analogRead(LDR_pin);
  return LDR_VAL;
}
 
//function returns value of lm35 
float H_LM_value()
{
  int temp_adc_val;
  float temp_val;
  temp_adc_val = analogRead(LM_pin);	
  temp_val = (temp_adc_val * 4.88);	// Convert adc value to equivalent(mV) (5/1023)*1000 
  temp_val = (temp_val/10);	// LM35 gives output of 10mV/C 
  return temp_val;
} 

//function to control the fan by temp
void H_FanMode()
{
  int temp=0;
  temp= H_LM_value();
  if (temp>=25 && temp<35)
  {
    digitalWrite(fan_pin,HIGH);
    digitalWrite(greenLed_pin,LOW);
  }
  else if(temp>=35)
  {
    digitalWrite(fan_pin,HIGH);
    digitalWrite(greenLed_pin,HIGH);
    delay(500);
    digitalWrite(greenLed_pin,LOW);
    delay(500);
  }
  else 
  {
    digitalWrite(fan_pin,LOW);
    digitalWrite(greenLed_pin,LOW);  
  }
}

//function to control led using LDR value
void H_LedMode()
{
  int LDR=0 , LED_VAL=0;
  LDR=H_LDR_value();
  LED_VAL = map(LDR, 0, 1023 , 255, 0);
  analogWrite(Led_pin,LED_VAL);
}

