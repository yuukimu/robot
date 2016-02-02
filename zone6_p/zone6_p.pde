import processing.serial.*;

Serial port;
float position_x = 400;
float position_y = 400;
int azimuth = -1;
int motorR, motorL;
int red_G = 0, green_G = 0, blue_G = 0;
int lange = 2;
int inByte = 0;
long timeNow, timePrev;

void setup() {
  size(800, 700);
  background(255);
  strokeWeight(10);
  textSize(25);
  port = new Serial(this,"/dev/tty.usbserial-AL00FMHT",57600); 
  port.clear();

}

void draw() {
  
  noStroke();
  fill(255);
  rect(360, 450, 100, 200);
  if(red_G > 50 && blue_G < 50) {
    stroke(255, 0, 0);
  }
  else {
    stroke(0, 0, 255);
  }
  fill(63, 127, 255);
  text(azimuth, 380 , 500);
  //println(azimuth);
  if(azimuth != -1){
    float oldx = position_x;
    float oldy = position_y;
    strokeWeight(2);
    float  k = map(abs(motorR - motorL), 0, 100, 1.5, 0);
    position_x += k*sin(radians(azimuth));
    position_y -= k*cos(radians(azimuth));
    line(oldx, oldy, position_x, position_y);
    println(azimuth);
  }
  if(inByte == 0){
    port.write('A');
    inByte = 1;
  }
  timeNow = millis();
  if(timeNow - timePrev >= 500){
    port.write('A');
  }
  // if(position_x > 800 || position_x < 0 || position_y < 0 || position_y > 900){
  //   position_x = 400;
  //   position_y = 850;
  //   background(255);
  // }
}

void serialEvent(Serial p) { 
  if (p.available() >= 8) {
    if(p.read() == 'H'){
      int h = p.read(), l = p.read();
      azimuth = (int)(h<<8|l);
      if(azimuth > 32767) azimuth -= 65536;
      motorR = p.read();
      motorL = p.read();
      red_G = (int)p.read();
      green_G = (int)p.read();
      blue_G = (int)p.read();
      port.write('A');
      timePrev = millis();
    } 
  }
}
