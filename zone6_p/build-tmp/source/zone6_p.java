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
int position_x = 400;
int position_y = 850;
int azimuth = -1;
int lange = 2;
int start_angle;

public void setup() {
  
  background(255);
  strokeWeight(10);
  port = new Serial(this,"/dev/tty.usbserial-AL00FMHT",57600); 
  port.clear();
}

public void draw() {
 //background(255);
  stroke(0); lights();
  fill(63, 127, 255);
  text(azimuth, 380 , 500);
  if(azimuth != -1){
    text(start_angle, 380, 450);
    line(position_x , position_y, (position_x)+4*sin(radians(start_angle-azimuth)), (position_y)+10*cos(radians(start_angle-azimuth)));
    position_x--;
    position_y--;
  }
  delay(50);
}

public void serialEvent(Serial p) { 
  if (p.available() >= 3) {
    if(p.read() == 'S'){
      int h = p.read(), l = p.read();
      if(azimuth == -1){
        azimuth = (int)(h<<8|l); if(azimuth > 32767) azimuth -= 65536;
        start_angle = azimuth;
      }
      else {
        azimuth = (int)(h<<8|l); if(azimuth > 32767) azimuth -= 65536;
      }
    } 
  }
}
  public void settings() {  size(800, 900, OPENGL); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "zone6_p" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
