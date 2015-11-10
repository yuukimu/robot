#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <Wire.h>
#include <LSM303.h>

#define SPEED           200 // Maximum motor speed when going straight; variable speed when turning

#define CALIBRATION_SAMPLES 100  // Number of compass readings to take when calibrating

ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
LSM303 compass;
float azimuth = 0;
int inByte = 0;

void setup() {
  LSM303::vector<int16_t> running_min = {32767, 32767, 32767}, running_max = {-32767, -32767, -32767};
  unsigned char index;
  Serial.begin(57600); // シリアル通信の初期化
  Wire.begin(); // 無線通信の初期化
  compass.init();
  compass.enableDefault();

  /* ヒント:各軸の最大・最小値を検出・格納する(課題 8.2.2) */
  compass.m_max.x = 0;
  compass.m_max.y = 0;
  compass.m_min.x = 0;
  compass.m_min.y = 0;
  azimuth = 0;
  button.waitForButton(); // ボタンが押されるまで停止

  motors.setLeftSpeed(SPEED); // 左タイヤの速度を設定
  motors.setRightSpeed(-SPEED);  // 右タイヤの速度を設定

  // キャリブレーションを行う
  for(index = 0; index < CALIBRATION_SAMPLES; index ++)
  {
    // センサーの値を読み取る
    compass.read();

    running_min.x = min(running_min.x, compass.m.x);
    running_min.y = min(running_min.y, compass.m.y);

    running_max.x = max(running_max.x, compass.m.x);
    running_max.y = max(running_max.y, compass.m.y);

    Serial.println(index);

    delay(50);
  }

  motors.setLeftSpeed(0);
  motors.setRightSpeed(0);

  // XとYの最大値、最小値を取得
  compass.m_max.x = running_max.x;
  compass.m_max.y = running_max.y;
  compass.m_min.x = running_min.x;
  compass.m_min.y = running_min.y;

  button.waitForButton();
}

void loop() {
  compass.read(); // センサーの値を読み取る  
  float heading, relative_heading;
  int speed;

  azimuth = averageHeading(); /* 課題 8.2.2 で使用,Compass 参照 */
  send_print(); // センサーの値をProcessingに送信
  // motors.setLeftSpeed(200); // 左タイヤの速度を設定
  // motors.setRightSpeed(-200); // 右タイヤの速度を設定
  delay(10);
}

void send_print(void) {
  if(Serial.available() > 0 || inByte == 0){
    inByte = Serial.read();
    Serial.write('S');
    Serial.write((int)(azimuth) >> 8);
    Serial.write((int)(azimuth) & 255);
  }
}

template <typename T> float heading(LSM303::vector<T> v)
{
  float x_scaled =  2.0*(float)(v.x - compass.m_min.x) / ( compass.m_max.x - compass.m_min.x) - 1.0;
  float y_scaled =  2.0*(float)(v.y - compass.m_min.y) / (compass.m_max.y - compass.m_min.y) - 1.0;

  float angle = atan2(y_scaled, x_scaled)*180 / M_PI;
  if (angle < 0)
    angle += 360;
  return angle;
}

float averageHeading()
{
  LSM303::vector<int32_t> avg = {0, 0, 0};

  for(int i = 0; i < 10; i ++)
  {
    compass.read();
    avg.x += compass.m.x;
    avg.y += compass.m.y;
  }
  avg.x /= 10.0;
  avg.y /= 10.0;

  return heading(avg);
}

