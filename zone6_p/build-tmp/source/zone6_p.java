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

public class zone6_p extends PApplet {



Serial port;
float position_x = 400;
float position_y = 400;
int azimuth = -1;
int motorR, motorL;
int red_G = 0, green_G = 0, blue_G = 0;
int lange = 2;
int inByte = 0;
long timeNow, timePrev;

public void setup() {
  
  background(255);
  strokeWeight(10);
  textSize(25);
  port = new Serial(this,"/dev/tty.usbserial-AL00FMHT",57600); 
  port.clear();

}

public void draw() {
  
  noStroke();
  fill(255);
  rect(360, 450, 100, 200);
  if(red_G > 50 && blue_G < 50) {
    stroke(255, 0, 0);
  }
  else {
    stroke(0, 0, 255);
  }
  //stroke(red_G, green_G, blue_G);
   //stroke(30, 170, 250);
  fill(63, 127, 255);
  text(azimuth, 380 , 500);
  //println(azimuth);
  if(azimuth != -1){
    float oldx = position_x;
    float oldy = position_y;
    strokeWeight(2);
    float  k = map(abs(motorR - motorL), 0, 100, 1.5f, 0);
    position_x += k*sin(radians(azimuth));
    position_y -= k*cos(radians(azimuth));
    line(oldx, oldy, position_x, position_y);
    /*pushMatrix();
    rotate(azimuth);
    triangle(100, 20, 180, 180, 20, 180);
    popMatrix();
    */
    println(azimuth);
    
    //point(position_x, position_y);
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

public void serialEvent(Serial p) { 
  if (p.available() >= 8) {
    if(p.read() == 'H'){
      int h = p.read(), l = p.read();
      azimuth = (int)(h<<8|l);
      if(azimuth > 32767) azimuth -= 65536;
      h = p.read();
      //l = p.read();
      motorR = h;

      h = p.read();
      //l = p.read();
      motorL = h;

      red_G = (int)p.read();
      green_G = (int)p.read();
      blue_G = (int)p.read();
      port.write('A');
      timePrev = millis();
    } 
  }
}
  public void settings() {  size(800, 700); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "zone6_p" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
