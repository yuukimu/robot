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
    // motorR_G = SPEED;
    // motorL_G = -SPEED;
    setupFlag = 1;
  }
  // if(setupFlag==1){
  //   if(steadyState(500)){
  //     motorR_G = 100;
  //     motorL_G = -100;
  //     setupFlag++;
  //   }
  // }
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  interval = pulseIn(echo, HIGH, 18000);
  distance = 340 * interval / 10000 / 2;
  //distance = 30;
Serial.print("distance = ");
Serial.println(distance);
  if(skip == true){
    skip = false;
  } else {
    //Serial.println(distance);
    if(distance == 0){
      digitalWrite(power, LOW);
      //if(steadyState(10)){
        digitalWrite(power, HIGH);
        skip = true;
      //}
    }
  }
  if(steadyState(400)) setupFlag = 2;
  if(setupFlag == 2){
  switch (mode_G) {
      case 0:
        mode_G = 1;
        break;
      case 1:
        motorR_G = 200;
        motorL_G = -SPEED;
        //motors.setSpeeds(motorL_G, motorR_G);
        if(distance <= 35) mode_G = 2;
        Serial.println(mode_G);
        break;
      case 2:
        motorR_G = SPEED;
        motorL_G = SPEED;
        //motors.setSpeeds(motorL_G, motorR_G);
        if(distance > 25){
          count = 0;
          mode_G = 0;
        }
        Serial.println(mode_G);
        break;
      default:
      break;
  }
  }
  //motors.setSpeeds(motorL_G, motorR_G);
}

void zone6(){
  int done;
   switch ( mode_G ) {
    case 0: // setupが必要ならここ（必要が無くても形式的に）
    zone_start_time_G = timeNow_G;
      mode_G = 1;
      break;
    case 1: // ライントレース（図形を検知するまで）
      goStraight();
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
       linetracePID6();
    //   done = identifyColor( 2 );
    //   if ( done == 1 ) {
    //     mode_G = 4;
    //   }
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

