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
int stage_w = 800, stage_h = 900;

public void setup() {
  
  background(255);
  strokeWeight(10);
  textSize(25);
  port = new Serial(this,"/dev/tty.usbserial-A400FRLO",9600); 
  port.clear();

}

public void draw() {
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

public void serialEvent(Serial p) { 
  if (p.available() >= 3) {
    if(p.read() == 'H'){
      int h = p.read(), l = p.read();
      azimuth = (int)(h<<8|l); if(azimuth > 32767) azimuth -= 65536;
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
