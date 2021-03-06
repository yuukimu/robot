import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import processing.serial.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class measure_time_p extends PApplet {


Serial port;
int time = 0;
float a=0.0f, b=0.0f;
float t = 0.0f;
String str, str1, str2;
int startFlag = 0;

public void setup() {
  
  port = new Serial(this, "/dev/tty.usbmodem1411", 9600);
  port.clear();
  textSize(40);
  fill(0);
}
    
public void draw() {
//  a = time / 1000;Serial.println(l); 
  t = (float)time / 1000.0f;
  str = nf(t, 2, 2);
  String[] ary  = split(str, ".");
  background(255);
  if(startFlag == 1){
    text("START", 100, 100, 140, 100);
  }
  text(ary[0]+":"+ary[1], 200, 200, 200, 200);
//  text(a+":"+b, 200, 200);
}

public void serialEvent(Serial p){
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
  public void settings() {  size(500, 500); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "measure_time_p" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
