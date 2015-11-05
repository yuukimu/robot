import processing.serial.*;

Serial port;
int position_x = 400;
int position_y = 850;
int azimuth = 0;

void setup() {
  size(800, 900, OPENGL);
  background(255);
  strokeWeight(10);
  port = new Serial(this,"/dev/tty.usbserial-AL00FMHT",57600); 
  port.clear();
}

void draw() {
  background(255);
  stroke(0); lights();
  fill(63, 127, 255);
  text(azimuth, 180 , 100);
  line(position_x , position_y, position_x+30*sin(radians(azimuth)), (position_y-30));

  delay(100);
}

void serialEvent(Serial p) { 
  if (p.available() >= 3) {
    if(p.read() == 'S'){
      int h = p.read(), l = p.read();
      azimuth=(int)(h<<8|l); if(azimuth > 32767) azimuth -= 65536;
    } 
  }
}