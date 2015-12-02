#include <Wire.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <ZumoBuzzer.h>
#include <LSM303.h>

ZumoMotors motors;
ZumoBuzzer buzzer;
Pushbutton button(ZUMO_BUTTON);
LSM303 compass;

#define SPEED          150 // default motor speed 
#define LIMIT      20000

float red_G, green_G, blue_G; //  RGB values
int zoneNumber_G; // zone number
int mode_G; // mode in each zone
unsigned long timeInit_G, timeNow_G; // start time, current time, 
int motorR_G, motorL_G;  // input values to the motors
unsigned long zone_start_time_G;
float azimuth = 0;  // 角度

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  button.waitForButton();
  
  setupColorSensor(); // カラーセンサーのsetup
  CalibrationColorSensor(); // カラーセンサーのキャリブレーション
  setupCompass(); // 地磁気センサーのsetup
  CalibrationCompass(); // 地磁気センサーのキャリブレーション
  buzzer.play(">g32>>c32");
  zoneNumber_G = 0;
  mode_G = 0;
  timeInit_G = millis();
  button.waitForButton();
}

void loop()
{
  compass.read();
  azimuth = averageHeading();
  readRGB();
  clearInterrupt();
  timeNow_G = millis() - timeInit_G;
  motors.setSpeeds(motorL_G, motorR_G);
  sendData();
  
  switch ( zoneNumber_G ) {
    case 0:
      startToZone(); // start to zone
      break;
    case 1:
      zone(); // zone 1
      break;
    case 2:
      zone(); // zone 2
      break;
    case 3:
      zone(); // zone 3
      break;
    case 4:
      zone(); // zone 4
      break;
    case 5:
      zone(); // zone 5
      break;
    case 6:
      zone6(); // zone 6
      break;
    case 7:
      zone(); // finish zone
      break;
    case 8:
      zoneToZone(); // zone to zone
      break;
    default:
      break;
  }
  

  // 20s経過していたらゾーンを移動
  /*if(timeNow_G - zone_start_time_G >= 20000){
    mode_G = 5;
  }*/
}

void sendData()
{
  static unsigned long timePrev = 0;
 
  if ( timeNow_G - timePrev > 50 ) { // 50msごとにデータ送信
    /*
    Serial.write('H');
    Serial.write(zoneNumber_G);
    Serial.write(mode_G);
    Serial.write((int)red_G);
    Serial.write((int)green_G);
    Serial.write((int)blue_G);
    */
    //Serial.println(zoneNumber_G);
    /*
    Serial.print(red_G);
    Serial.print(green_G);
    Serial.println(blue_G);
    Serial.println((red_G + green_G + blue_G) /3 ); 
    */
    if(Serial.read() == 'A') {
      Serial.write('H');
      Serial.write((int)(azimuth) >> 8);
      Serial.write((int)(azimuth) & 255);
      timePrev = timeNow_G;
    }
  }
}

