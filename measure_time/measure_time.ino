const int trig = 2;
const int echo = 4;
const int power = 13;
int inByte = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(trig, OUTPUT);  
  pinMode(echo, INPUT);
  pinMode(power, OUTPUT);
  digitalWrite(power, HIGH);
}

void loop() {
unsigned long interval, distance; 
static bool skip = true;
digitalWrite(trig, HIGH); 
delayMicroseconds(10); 
digitalWrite(trig, LOW);

interval = pulseIn(echo, HIGH, 18000);
distance = 340 * interval / 10000 / 2;

if (skip == true) {
    skip = false;
  } else {
      Serial.println(distance);
      if (distance == 0) {
        digitalWrite(power, LOW);
        delay(10);
        digitalWrite(power, HIGH);
        skip = true;
      } else if(distance > 20 && distance < 30) {
        send_data(1);
        delay(3000);
      } else if(distance > 0 && distance < 10) {
        send_data(2);
        delay(3000);
      }
  }
  delay(60);
}

void send_data(unsigned long data){
  if(Serial.available() > 0 || inByte == 0){
    inByte = Serial.read();
    Serial.write('H');
    Serial.write(data);
    inByte = 1;
  }
}