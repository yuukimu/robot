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

public class Integrate_P extends PApplet {



Serial port; 
int zoneNumber, mode;
int green, red, blue, green_p, red_p, blue_p;
int[] sensors = new int[5];
int count;
int[] sensors_p = new int[5];

public void setup() {
  
  background(255);
  count = 0;
  println(Serial.list());
  // String arduinoPort = Serial.list()[1];
  // port = new Serial(this, arduinoPort, 9600 );
  port = new Serial(this, "/dev/tty.usbserial-A400FRLO", 9600 ); // \u30b7\u30ea\u30a2\u30eb\u30dd\u30fc\u30c8\u540d\u306f\u5404\u81ea\u306e\u74b0\u5883\u306b\u5408\u308f\u305b\u3066\u9069\u5b9c\u6307\u5b9a
  zoneNumber = 0;
  red_p = 0; 
  green_p = 0; 
  blue_p = 0;
}

public void draw() {


  float y_p, y;

  y_p = map(red_p, 0, 100, height*0.9f, height*0.1f);
  y = map(red, 0, 100, height*0.9f, height*0.1f);
  stroke(255, 0, 0);
  line((count-1)*10, y_p, (count)*10, y );

  y_p = map(green_p, 0, 100, height*0.9f, height*0.1f);
  y = map(green, 0, 100, height*0.9f, height*0.1f);
  stroke(0, 255, 0);
  line((count-1)*10, y_p, (count)*10, y );


  y_p = map(blue_p, 0, 100, height*0.9f, height*0.1f);
  y = map(blue, 0, 100, height*0.9f, height*0.1f);
  stroke(0, 0, 255);
  line((count-1)*10, y_p, (count)*10, y );

  /*
  red = map(sensors[2],0, 100, 0, 255);
   green = map(sensors[3],0, 100, 0, 255);
   blue = map(sensors[4],0, 100, 0, 255);
   fill(red, green, blue);
   ellipse(count*10, 20, 10, 10 );
   */

  stroke(255);
  fill(255);
  rect(0, height-50, 300, 50);
  textSize(50);
  fill(0);
  text("zone = ", 10, height-10);
  text((int)zoneNumber, 200, height-10);
  text(", ", 225, height-10);
  text((int)mode, 250, height-10);
  noFill();


  if ( count == 120 ) {
    count = 0;
    background(255);
  }
}

public void serialEvent(Serial p) { // p is anonymous

  if ( p.available() >= 6 ) { 
    if ( p.read() == 'H' ) {

      red_p = red;
      green_p = green;
      blue_p = blue;


      zoneNumber = p.read();
      mode =  p.read();
      red =  p.read();
      green = p.read();
      blue =  p.read();
      p.clear(); // \u5ff5\u306e\u305f\u3081\u30af\u30ea\u30a2

      print("zone = ");
      println((int)zoneNumber, (int)mode);
      ++count;
    }
  }

  //  port.write("A");
}
  public void settings() {  size(1200, 400); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "Integrate_P" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
