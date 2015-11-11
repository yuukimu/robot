
import processing.serial.*;
Serial port; 
int zoneNumber, mode;
int green, red, blue, green_p, red_p, blue_p;
int[] sensors = new int[5];
int count;
int[] sensors_p = new int[5];

void setup() {
  size(1200, 400);
  background(255);
  count = 0;
  println(Serial.list());
  // String arduinoPort = Serial.list()[1];
  // port = new Serial(this, arduinoPort, 9600 );
  port = new Serial(this, "/dev/tty.usbserial-A400FRLO", 9600 ); // シリアルポート名は各自の環境に合わせて適宜指定
  zoneNumber = 0;
  red_p = 0; 
  green_p = 0; 
  blue_p = 0;
}

void draw() {


  float y_p, y;

  y_p = map(red_p, 0, 100, height*0.9, height*0.1);
  y = map(red, 0, 100, height*0.9, height*0.1);
  stroke(255, 0, 0);
  line((count-1)*10, y_p, (count)*10, y );

  y_p = map(green_p, 0, 100, height*0.9, height*0.1);
  y = map(green, 0, 100, height*0.9, height*0.1);
  stroke(0, 255, 0);
  line((count-1)*10, y_p, (count)*10, y );


  y_p = map(blue_p, 0, 100, height*0.9, height*0.1);
  y = map(blue, 0, 100, height*0.9, height*0.1);
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

void serialEvent(Serial p) { // p is anonymous

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
      p.clear(); // 念のためクリア

      print("zone = ");
      println((int)zoneNumber, (int)mode);
      ++count;
    }
  }

  //  port.write("A");
}