#define DISTANCE 20
const int trig = 7; //Trig ピンをデジタル 7 番に接続
const int echo = 8;  //Echo ピンをデジタル 8 番に接続
unsigned long interval; //Echoのパルス幅
unsigned long timeInit, timeStart, timeNow, timeEnd;
int mode = 0;
int count = 0;
int inByte;   // 送信要求を確認

void send_data(unsigned long data);

void setup(){
  pinMode(trig, OUTPUT); // 7番ポートを出力に設定
  pinMode(echo, INPUT); // 8番ポートを入力に設定
  Serial.begin(9600);
  timeInit = millis();
}

void loop(){
  int sensorValue = analogRead(A0);
  float vo = sensorValue * (5.0/1024.0);
  float temp = (vo*1000-600)/10;
  digitalWrite(trig, HIGH); //10μs のパルスをセンサーに入力
  delayMicroseconds(16);
  digitalWrite(trig, LOW); // 7番ポートにLOWを出力
  
  interval = pulseIn(echo, HIGH, 500000);  //Echo信号がHIGHである時間(μs)を計測
  double c = 331.5 + 0.61 * temp; // 音速を25℃の場合に設定
  float l = c * interval / 10000 / 2; // 距離を計測(cm)

  if(l <= DISTANCE && l != 0 && mode == 0){
    Serial.println("aaa");
    count++;
    if(count >= 10){
      mode = 1;
      count = 0;
    }
  }
  else if(l > DISTANCE && mode == 1){
    //Serial.println("START!!");
    timeStart = millis() - timeInit;
    mode = 2;
    //delay(200);
  }
  else if(l <= DISTANCE && l != 0 && mode == 2){
    count++;
    if(count >= 4){
      timeEnd = timeNow - timeStart; 
      //Serial.print("GOAL: ");
      send_data(timeEnd); // Processingにデータを送信
      delay(700);
      mode = 0;
      count = 0;
    }
  } 
  else if(mode == 2){
    timeNow = millis() - timeInit;
    send_data(timeNow);
  }
  else{
    count = 0;
  }
}

void send_data(unsigned long data){
  if(Serial.available() > 0){
    inByte = Serial.read();
    Serial.write('H');
    Serial.write(data >> 8);
    Serial.write(data & 8);
  }
}
