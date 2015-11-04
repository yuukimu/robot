import processing.serial.*;
Serial port;
long time;

void setup() {
  size(500, 500);
  port = new Serial(this, "/dev/tty.usbmodem1421", 9600);
  port.clear();
  textSize(20);
  fill(0);
}

void draw() {
  background(255);
  text(time, 200, 200);
}
void serialEvent(Serial p){
  if(p.available() >= 3){
    if(p.read() == 'H'){
      long x = p.read(), y = p.read();
      time = (long)(x<<8|y);
    }
    //port.clear();
  }
}