import processing.serial.*;

Serial port;
int position_x = 400;
int position_y = 850;
int azimuth = -1;
int lange = 2;
int stage_w = 800, stage_h = 900;

void setup() {
  size(800, 900, OPENGL);
  background(255);
  strokeWeight(10);
  textSize(25);
  port = new Serial(this,"/dev/tty.usbserial-AL00FMHT",57600); 
  port.clear();

}

void draw() {
  //background(255);
  stroke(0); lights();
  fill(63, 127, 255);
  text(azimuth, 380 , 500);
  if(azimuth != -1){
    //line(position_x , position_y, (position_x)+sin(radians(360-azimuth)), (position_y)+2*cos(radians(360-azimuth)));
    point(position_x, position_y);
    //if(position_x-20 > 0 && position_x+20 < stage_w)
      position_x += 4*sin(radians(360-azimuth));
    //if(position_y-20 > 0 && position_y+20 < stage_h)
      position_y += 4*cos(radians(360-azimuth));
    port.write('A');
  }
  delay(50);
}

void serialEvent(Serial p) { 
  if (p.available() >= 3) {
    if(p.read() == 'S'){
      int h = p.read(), l = p.read();
      azimuth = (int)(h<<8|l); if(azimuth > 32767) azimuth -= 65536;
    } 
  }
}