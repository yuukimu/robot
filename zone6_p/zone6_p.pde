import processing.serial.*;

Serial port;
int position_x = 400;
int position_y = 850;
int azimuth = -1;
int red_G = 0, green_G = 0, blue_G = 0;
int lange = 2;
int inByte = 0;
long timeNow, timePrev;

void setup() {
  size(800, 900);
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
  // stroke(red_G, green_G, blue_G);
  stroke(30, 170, 250);
  fill(63, 127, 255);
  text(azimuth, 380 , 500);
  println(azimuth);
  if(azimuth != -1){
    int oldx = position_x;
    int oldy = position_y;
    position_x += 1.5*sin(radians(azimuth));
    position_y -= 1.5*cos(radians(azimuth));
    line(oldx, oldy, position_x, position_y);
    // point(position_x, position_y);
  }
  if(inByte == 0){
    port.write('A');
    inByte = 1;
  }
  timeNow = millis();
  if(timeNow - timePrev >= 500){
    port.write('A');
  }
  if(position_x > 800 || position_x < 0 || position_y < 0 || position_y > 900){
    position_x = 400;
    position_y = 850;
    background(255);
  }
  delay(80);
}

void serialEvent(Serial p) { 
  if (p.available() >= 6) {
    if(p.read() == 'H'){
      int h = p.read(), l = p.read();
      azimuth = (int)(h<<8|l);
      if(azimuth > 32767) azimuth -= 65536;
      red_G = (int)p.read();
      green_G = (int)p.read();
      blue_G = (int)p.read();
      port.write('A');
      timePrev = millis();
    } 
  }
}
