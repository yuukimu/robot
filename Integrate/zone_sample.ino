void zone()
{
  static int count = 0; // 回転の数
  static int spin = 1;  // 回転か停止かの判断

  switch (mode_G) {
    case 0:
       zone_start_time_G = timeNow_G;
       mode_G = 1;
       break;
     case 1:
       motors.setSpeeds(0, 0);
       if(steadyState(500))
         mode_G = 2;
       break;
     case 2:
       motors.setSpeeds(SPEED, SPEED);
       if(steadyState(500))
         mode_G = 3;
        break;
      case 3:
        motors.setSpeeds(0, 0);
        if(steadyState(500))
          mode_G = 4;
        break;
      case 4:
        if(count < zoneNumber_G && spin == 1){  // 200ms回転
          motors.setSpeeds(-SPEED, SPEED);
        if(steadyState(200))
          spin = 0;
        }
        else if(count < zoneNumber_G && spin == 0){ // 200ms停止
          motors.setSpeeds(0, 0);
          if(steadyState(200)){
            spin = 1;
            count++;
          }
        }
        else if(count >= zoneNumber_G){
          mode_G = 5;
        }
        break;
      case 5:
        zoneNumber_G = 8;  
        mode_G = 0;
        spin = 1;
        count = 0;
        break;
      default:
        break;
  }
  //button.waitForButton();
}

void zone4(){
  static int speed = SPEED;
  static long count = 0;
  static int setupFlag = 0;
  static bool skip = true;
  unsigned long interval, distance;
  if(setupFlag == 0){
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    pinMode(power, OUTPUT);
    digitalWrite(power, HIGH);
    mode_G = 0;
    setupFlag = 1;
  }
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  interval = pulseIn(echo, HIGH, 18000);
  distance = 340 * interval / 10000 / 2;
Serial.print("distance = ");
Serial.println(distance);
  if(skip == true){
    skip = false;
  } else {
    if(distance == 0){
      digitalWrite(power, LOW);
      digitalWrite(power, HIGH);
      skip = true;
    }
  }
  switch (mode_G) {
      case 0:
        if(steadyState(500)){
          setupFlag = 2;
          mode_G = 1;
        }
        break;
      case 1:
        motorR_G = 150;
        motorL_G = 150;
        if(steadyState(1000)) mode_G = 2;
        if(distance <= 35) mode_G = 3;
        if(identifyColor(0)){
          mode_G = 4;
        }
        Serial.println(mode_G);
        break;
      case 2:
        motorR_G = 140;
        motorL_G = -140;
        if(distance <= 35) mode_G = 3;
        if(steadyState(1500)) mode_G = 1;
        if(identifyColor(0)){
          mode_G = 4;
        }
        break;
      case 3:
        motorR_G = 140;
        motorL_G = 140;
        if(identifyColor(0)){
          mode_G = 4;
        }
        if(distance > 35){
          count = 0;
          mode_G = 1;
        }
        Serial.println(mode_G);
        break;
      case 4:
        motorR_G = -SPEED;
        motorL_G = -SPEED;
        if(steadyState(800)){
          mode_G = 1;
        }
        break;
      default:
      break;
  }
}

void zone6(){
  int done;
   switch ( mode_G ) {
    case 0: // setupが必要ならここ（必要が無くても形式的に）
    // zone_start_time_G = timeNow_G;
      start_angle = averageHeading();
      mode_G = 3;
      break;
    case 1: // ライントレース（図形を検知するまで）
      goStraight6();
      done = identifyColor( 3 );
      if ( done == 1 ) {
        mode_G = 3;
        motors.setSpeeds(0,0);
      }
      break;
    case 2: // 白を検知するまで
      identifyColor(1);
      done = identifyColor( 1 );
      if ( done == 1 ){
        mode_G = 3;
      }
      break;
     case 3: // ライントレース（黄と黒の混合色を検知するまで）
      if(identifyColor(0)){
        motors.setSpeeds(0,0);
      }
       // linetracePID6();
       motorR_G = 0;
       motorL_G = 0;
       break;

    default:
      break;
  }
}

int steadyState( unsigned long period )
{
  static int flagStart = 0;
  static  unsigned long startTime = 0;

  if ( flagStart == 0 ) {
    startTime = timeNow_G;
    flagStart = 1;
  }

  if ( timeNow_G - startTime > period ) {
    flagStart = 0;
    startTime = 0; // 必要ないがstatic変数は形式的に初期化
    return 1;
  }
  else
    return 0;
}

