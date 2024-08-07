/**
 * This is te code for the arduino.
 * 
 * The arduino is tasked ith controlling all the peripherals, and obey requests that 
 *  are sent over serial by the WiFi module. This is where things actually happen when
 *  you press "Light On" in the android app.
 * 
 * Read the Readme.md file to get a better grasp of hardware connections.
*/


//Stepper Libraries

#include <Servo.h>

//LCD Libraries
#include <LiquidCrystal.h>

//RTC libraries
#include <Wire.h>
#include <ds3231.h>

//Temp sensor libraries
#include <OneWire.h> 
#include <DallasTemperature.h>


//LCD
LiquidCrystal lcd(8,7,6,5,4,3);
bool lcdShowIP  = false;

//RTC
struct ts raw_time;

//Stepper
const int stepsPerRevolution = 200;
//Stepper myStepper(stepsPerRevolution, 9, 11, 10, 12);
Servo myStepper;

int alarm_H = 13;
int alarm_M = 00;
bool already_fed = false; 





//Temp sensor
const int tempSensorReadPin = A3;
OneWire oneWire(A3);
DallasTemperature tempSensor(&oneWire);
double raw_temperature = -1;


//Ligth relay
const int lightRelayPin = A1;
bool lightState = false;


//IP Button
const int buttonPin0 =  2;


//Book keeping String Variables

String temperatureString = "N/A";
String alarmString = "13:30";
String timeString = "N/A";
String lightString = "N/A";
String ipString = "N/A";




/**
 * setup()
 * 
 * Runs once when the arduino is first boot up.
*/
void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  myStepper.attach(11); 
  myStepper.write(0);
  //LCD Setup
  lcd.begin(16,2);
  lcd.clear();

  //RTC setup
  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
  
  tempSensor.begin();


  //Light relay setup
  pinMode(lightRelayPin, OUTPUT);
  digitalWrite(lightRelayPin, HIGH); //OPPOSITE FOR THIS RELAY
;
  

  //Button Setup
  pinMode(buttonPin0, INPUT);
  //attachInterrupt(digitalPinToInterrupt(buttonPin0), toggleLCD, CHANGE);

  initialize();
  
}

/**
 * loop()
 * 
 * This function is called by the arduino continusly as it is on.
 * The statements in this function are called like they are in a while loop.
 * 
*/
void loop() {
  displayNormal();
  checkWiFiRequest(); //check if the wifi-module is asking for something
  updateTemperature(); //update temp values
  updateTime(); //update the time stirng
  updateLight();
  checkAlarm(); //check the alarm
  
  delay(500); //delay
  
}



/**
 * Takes care of initializing all the I/O devices to their initial states
 * 
*/
void initialize()
{
  //Display a message on the lcd, saying that we are initializing
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Initializing...");

  
  //Light
  lightState = false;
  updateLight();

  

  //Stepper
  already_fed = false;

  //Temperature
  updateTemperature();

  
  updateTime();

  //LCD
  displayNormal(); //This would remove the initializing message from the lcd and display the temp

}
  
/**
 * updateLight()
 * 
 * Toggles the light state.
 * Depending on the value of the lightState global variable
*/
void updateLight()
{
  if(lightState == true)
  {
    digitalWrite(lightRelayPin, HIGH);
    lightString = "ON";
    }
  else
  {
    digitalWrite(lightRelayPin, LOW);
    lightString = "OFF";
  }
  
  }

/**
 * updateTemperature()
 * 
 * Gets the temperature sensor reading
 * and updates the global variable with that reading
*/
void updateTemperature()
{
  tempSensor.requestTemperatures();
  raw_temperature = tempSensor.getTempCByIndex(0);
  temperatureString = ((String)raw_temperature)   + " C";
  }



void updateTime()
{
  DS3231_get(&raw_time);
  timeString = ((String)raw_time.hour) + ":" +  ( (raw_time.min < 10) ? ("0"):("") )  +  ((String)raw_time.min); 
}

/**
 * updateAlarm()
 * 
 * Updates the alarm string based on the global variables, alarm_H (for hour),
 * alarm_M (for minute).
*/
void updateAlarm()
{
  alarmString = ((String)alarm_H) + ":" +  ( (alarm_M < 10) ? ("0"):("") )  +  ((String)alarm_M); 
}

/**
 * setRTC()
 * 
 * Changes the time on the RTC to the specified function arguments.
*/
void setRTC(int nhr, int nmin, int nsec)
{
  raw_time.hour=nhr; 
  raw_time.min=nmin;
  raw_time.sec=nsec;


  DS3231_set(raw_time);
 }


/**
 * updateIP()
 * 
 * updates the IP address global string. 
*/


/**
 * startStepperOnAlarm()
 * 
 * starts the stepper motor when the alarm time has been reached
*/
void startStepperOnAlarm()
{
  //Remeber the alarm is only being checked by checking the hour and min?
  //We need to make sure that the motor is not triggered for every second in that minute
  // Hence we use the already_fed flag to indicate this
  already_fed  = true; 
  startStepper();
  }

void startStepper()
{
    myStepper.write(180);
    // Dừng chờ 3 giây (3000 ms)
    delay(3000);
    myStepper.write(0);
    delay(3000);
    
}


/**
 * toggleLCD()
 * 
 * toggles the flag that controls what is shown on the LCD
 * 
*/

/**
 * displayLCD()
 * 
 * Decides what to display based on the state of the lcdShowip Flag.
 * 
*/


void displayNormal()
{
  lcd.clear();
  lcd.setCursor(0,0);
  String line1 = temperatureString ;
  lcd.print(line1);
  lcd.setCursor(0,1);
  String line2 = "T:" + timeString +" A:"+ alarmString;
  lcd.print(line2);
  }

/**
 * displayIP()
 * 
 * Displays the IP address on the LCD.
 * If the arduino does not have the IP, then it asks the wifi module.
*/

/**
 * checkAlarm()
 * 
 * checks if the alaram time has reached.
 * If yes it triggers the the stepper motor.
 * 
*/
void checkAlarm()
{
  if(alarm_H == raw_time.hour && alarm_M == raw_time.min && already_fed == false)
  {
    displayNormal();
    startStepperOnAlarm();
    }
}


/**
 * cleanUpRequestString()
 * 
 * This function cleans up any noise from the string.
 * The information we need is enclosed in "!" symbols.
 * 
 * So a message from the the client to turn OFF the lights looks like !L:OFF!
 *  and to turn ON the client will send !L:ON!"
 * 
 * This function cleans up anything recieved before and after the opening and closing "!"
*/
String cleanUpRequestString(String request)
{
  String clean = "";
  boolean append = false;
  
  for(int i = 0; i < request.length(); i ++)
  {
    if(request.charAt(i) == '!' && append == false)
    {
      append = true;
      }

    else if(request.charAt(i) == '!' && append == true)
     {
      append = false;
      clean = clean + request.charAt(i);
      break;
      }

     if(append == true)
     {
      clean = clean + request.charAt(i);
      }
    
    }

    return clean;
  }

/**
 * checkWiFiRequest()
 * 
 * checks if the wifi module has a request in the serial connection.
 * 
 * if yes, we handle the request by generating an appropriate response.
 * 
*/
void checkWiFiRequest()
{ 
    //check if there the wifi module wants to send data
    // if(!Serial.available())
    // {
    // return;
    // }
  
    //get the request
    String request = Serial.readStringUntil('\n');
    //String cleanRequest  = cleanUpRequestString(request); //clean the request
    request.trim(); 
    String response = responseGenerator(request); //generate a response
    Serial.println(response); //send the reponse back to the wifi module
  }



/**
 * responseGenerator()
 * 
 * generates a reponse when the for a request that the arduino recieves
 * and also refelcts on any I/O changes reflected in the request.
*/
String responseGenerator(String request)
{

  //request to turn of the lights
  if(request.indexOf("L:OFF") != -1)
  {
    lightState = false;
    updateLight();

    return getTempratureForClient();
    
    //return "ARD: led off";
  }

  //request to turn off the light
  else if(request.indexOf("L:ON") != -1)
  {
    lightState = true;
    updateLight();

     return getTempratureForClient();
    //return "ARD: led on";
  }

  //rquest to start the stepper motor
  else if(request.indexOf("S:START") != -1)
  {
     startStepper();
     return getTempratureForClient();
    //return "ARD: stepper start";
  }
  //Request to change the alarm
  else if(request.indexOf("CA:") != -1)
  {
    int hr_u = -1;
    int min_u = -1;
    getTimeFromRequest(request, hr_u, min_u);
    alarm_H = hr_u;
    alarm_M = min_u;
    updateAlarm();
    already_fed = false;
    
    return getTempratureForClient();
    
  }

  //request to change the rtc time
  else if(request.indexOf("CRTC:") != -1)
  {
    int hr_u = -1;
    int min_u = -1;
    getTimeFromRequest(request, hr_u, min_u);
    setRTC(hr_u, min_u,0);
    updateTime();

    return getTempratureForClient(); 
    }


  //if no match, invalid request
  return getTempratureForClient();
  
}

/**
 * getStatusResponseForClient()
 * 
 * generates a string with proper format for sending to the client
 * in this case all the status of the I/O devices.
*/
// String getStatusResponseForClient()
// {

//   return "!" + getLightStatusForClient()  + "," 
//              + getTempratureForClient()  + "," 
//              + getAlarmForClient()  + "," 
//              + getRtcTimeForClient() + ","
//              + "!";
  
// }

/**
 * getTimeForRequest()
 * 
 * generates a string with proper format for sending to the client.
*/
void getTimeFromRequest(String r, int &refhr, int &refmin )
{
  String sh = "";
  bool sh_w = false;
  String sm = "";
  bool sm_w = false;

  for(int i = 0; i < r.length(); i++ )
  {


    if(r.charAt(i) == ':')
    {
      sh_w = true;
      continue;
      }
     if(r.charAt(i) == ',')
     {
      sm_w = true;
      sh_w= false;
      continue;
      }
      
    if(sm_w == true && r.charAt(i) == '!')  break;

    if(sh_w) sh = sh + r[i];
    if(sm_w) sm = sm + r[i]; 

  }

  refhr = sh.toInt();

  refmin = sm.toInt();

}

/**
 * Below methods deal with generating proper string formats to 
 * send a reponse to the client. To ensure the client program
 * can read the response.
 * 
*/
String getLightStatusForClient()
{
  if(lightState) return "L:ON";
  else return "L:OFF";
  }

String getTempratureForClient()
{
  return "T:"+ String(raw_temperature);
  }

String getAlarmForClient()
{
  return "CA:" + alarmString;
  }


String getRtcTimeForClient()
{
  return "CRTC:" + timeString;
  }


