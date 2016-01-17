const float color[7][3] = { // identifyZone()用の固定値
  {90, 85, 28},
  {52, 20, 35},
  {21, 53, 62},
  {57, 8, 4},
  {13, 43, 15},
  {2, 10, 35},
  {13, 20, 46}
};

float minDistance; // identifyZone()用のグローバル変数

// startからzoneへの移動（zone番号の認識もする）
void startToZone()
{
  int zoneNumber;
  int done;

  switch ( mode_G ) {
    case 0: // setupが必要ならここ（必要が無くても形式的に）
      mode_G = 1;
      break;
    case 1: // ライントレース（黄と黒の混合色を検知するまで）
      linetracePID();
      done = identifyColor( 2 );
      if ( done == 1 ) {
        mode_G = 2;
      }
      break;
    case 2: // 黒を検知するまで直進
      goStraight();
      done = identifyColor( 0 );
      if ( done == 1 )
        mode_G = 3;
      break;
    case 3: // identifyZone()のsetup
      minDistance = 9999999;
      mode_G = 4;
      break;
    case 4: // 白を検知するまで直進（その間ゾーン番号を検知）
      goStraight();
      zoneNumber = identifyZone();
      done = identifyColor( 1 );
      if ( done == 1 ) {
        zoneNumber_G = zoneNumber;
        mode_G = 0;
      }
      break;
    default:
      break;
  }
}

// zoneからzoneへの移動（zone番号の認識もする）
void zoneToZone()
{
  // 各自で作成
  int zoneNumber;
  int done;

  switch ( mode_G ) {
    case 0: // setupが必要ならここ（必要が無くても形式的に）
      mode_G = 1;
      break;
    case 1: // ライントレース（黄と黒の混合色を検知するまで）
      linetracePID();
      done = identifyColor( 2 );
      if ( done == 1 ) {
        mode_G = 2;
      }
      break;
    case 2: // 白を検知するまで
      goStraight();
      done = identifyColor( 1 );
      if ( done == 1 )
        mode_G = 3;
      break;
    case 3: // ライントレース（黄と黒の混合色を検知するまで）
      linetracePID();
      done = identifyColor( 2 );
      if ( done == 1 ) {
        mode_G = 4;
      }
      break;
    case 4: // 黒を検知するまで直進
      goStraight();
      done = identifyColor( 0 );
      if ( done == 1 )
        mode_G = 5;
      break;
    case 5: // identifyZone()のsetup
      minDistance = 9999999;
      mode_G = 6;
      break;
    case 6: // 白を検知するまで直進（その間ゾーン番号を検知）
      goStraight();
      zoneNumber = identifyZone();
      done = identifyColor( 1 );
      if ( done == 1 ) {
        zoneNumber_G = zoneNumber;
        mode_G = 0;
      }
      break;
    default:
      break;
  }
}

// 直進する
void goStraight()
{
  motorL_G = SPEED;
  motorR_G = SPEED;
}
void goStraight6()
{
  motorL_G = 100;
  motorR_G =100;
}

// 黒白の境界に沿ってライントレース
void linetracePID()
{
  static unsigned long timePrev = 0;
  static float lightPrevPD = 0.0;

  float lightNowPD;
  float error, errorSP;
  float diff, diffSP;
  float speedDiff;

  float target = 50;
  float Kp = 1.5;
  float Kd = 1.5;
  
  lightNowPD =  ( red_G + green_G + blue_G ) / 3.0;
  error = lightNowPD - target;
  errorSP = map(error, -target, target, -SPEED, SPEED );
  diff = (lightNowPD - lightPrevPD) / (timeNow_G - timePrev );
  diffSP  = map(diff, -100.0, 100.0, -SPEED, SPEED );

  speedDiff = Kp * errorSP + Kd * diffSP;
/*
  motorL_G = SPEED - speedDiff;
  motorR_G = SPEED + speedDiff;
*/
if (speedDiff > 0) {
    motorL_G = SPEED - speedDiff;
    motorR_G = SPEED;
}
else {
    motorL_G = SPEED;
    motorR_G = SPEED + speedDiff;
  }
  timePrev = timeNow_G;
  lightPrevPD = lightNowPD;
}

// 指定の色を連続４回認識したら1を返す（それ以外0)
int identifyColor( int color )
{
  static int count = 0; // この関数が初めて呼ばれた時にのみ初期化される

  if ( color == 0 && red_G < 25 && green_G < 25 && blue_G < 25 )  // 黒を感知
    ++count;
  else if ( color == 1 && red_G > 75 && green_G > 75 && blue_G > 75 ) // 白を感知
    ++count;
  else if ( color == 2 && ( red_G + green_G ) / 2.0 * 0.7  > blue_G && ( red_G + green_G ) * 0.5 > 30 )  // 黄黒の混合色を感知
    ++count;
  else if (color == 3 && red_G < 15 && green_G < 25 && blue_G < 60 && blue_G > 30){
    count = 4;
  }
  else
    count = 0;

  if ( count > 3 ) { // パラメーター
    count = 0; // 次に呼ばれる時に備えて値を初期値に戻す
    return 1;
  }
  else
    return 0;
}

// KNNで現在最も近い番号を返す
int identifyZone()
{
  // float minDistance; グローバル変数で定義
  static int zoneNumber;
  float distance;

  for ( int i = 0; i < 7; ++i ) {
    distance = (red_G - color[i][0]) * (red_G - color[i][0])
               + (green_G - color[i][1]) * (green_G - color[i][1])
               + (blue_G - color[i][2]) * (blue_G - color[i][2]);
    if ( distance < minDistance ) {
      minDistance = distance;
      zoneNumber = i;
    }
  }

  return zoneNumber + 1; // zone番号は1-7なので+1
}


void linetracePID6()
{
  static unsigned long timePrev = 0;
  static float lightPrevPD = 0.0;

  float lightNowPD;
  float error, errorSP;
  float diff, diffSP;
  float speedDiff;

  float target = 60;
  float Kp = 2.7;
  float Kd = 1.5;
  
  lightNowPD =  (red_G + green_G + blue_G) / 3.0;
  error = lightNowPD - target;
  // errorSP = map(error, -target, target, -SPEED, SPEED );
  int lightNowPD_MIN = 12;
  int lightNowPD_MAX = 85;
  errorSP = map(lightNowPD, lightNowPD_MIN, lightNowPD_MAX, -SPEED, SPEED );
  diff = (lightNowPD - lightPrevPD) / (timeNow_G - timePrev );
  diffSP  = map(diff, -100.0, 100.0, -SPEED, SPEED );

  speedDiff = Kp * errorSP + Kd * diffSP;

  if (speedDiff > 0) {
    motorL_G = SPEED - speedDiff;
    motorR_G = SPEED;
  }
  else {
    motorL_G = SPEED;
    motorR_G = SPEED + speedDiff;
  }
  timePrev = timeNow_G;
  lightPrevPD = lightNowPD;
}
