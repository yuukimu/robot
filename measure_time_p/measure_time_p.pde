import processing.serial.*;
Serial port;
int time = 0;
float a=0.0, b=0.0;
float t = 0.0;
String str, str1, str2;
int startFlag = 0;

void setup() {
  size(500, 500);
  port = new Serial(this, "/dev/tty.usbmodem1411", 9600);
  port.clear();
  textSize(40);
  fill(0);
}
    
void draw() {
//  a = time / 1000;Serial.println(l); 
  t = (float)time / 1000.0;
  str = nf(t, 2, 2);
  String[] ary  = split(str, ".");
  background(255);
  if(startFlag == 1){
    text("START", 100, 100, 140, 100);
  }
  text(ary[0]+":"+ary[1], 200, 200, 200, 200);
//  text(a+":"+b, 200, 200);
}

void serialEvent(Serial p){
  if(p.available() >= 2){
    if(p.read() == 'H'){
      //int x = p.read(), y = p.read();
      //time = (int)(x<<8|y);
      time = (int)p.read();
    }
    port.write('A');
    startFlag = 1;
    //port.clear();
  }
}
