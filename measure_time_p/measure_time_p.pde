import processing.serial.*;
Serial port;
int time;

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
  port.write('A');
}

void serialEvent(Serial p){
  if(p.available() >= 3){
    if(p.read() == 'H'){
      int x = p.read(), y = p.read();
      time = (int)(x<<8|y);
    }
    //port.clear();
  }
}
