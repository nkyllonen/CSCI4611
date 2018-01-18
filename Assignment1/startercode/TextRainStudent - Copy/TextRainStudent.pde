/*///////////////////////////////////////////////////////////
    CSCI 4611 Spring '16 Assignment #1: Text Rain
    Nikki Kyllonen - kyllo089
///////////////////////////////////////////////////////////*/
import processing.video.*;

// Global variables for handling video data and the input selection screen
String[] cameras;
Capture cam;      //Capture and Movie both inherit from PImage
Movie mov;
PImage inputImage;
boolean inputMethodSelected = false;

PFont font;
ArrayList<SBlock> origBlocks = new ArrayList<SBlock>();
ArrayList<SBlock> dispBlocks = new ArrayList<SBlock>();
ArrayList<SBlock> notDispBlocks = new ArrayList<SBlock>();
float minV = 1.0;
float maxV = 3.0;
float threshold = 150;
boolean debugSelected = false;
int numCheckPixels = 7;
int numDrawnPer = 20; //manipulatable number of origBlocks drawn/sec call

String dataDir = "../data/"; // a relative path to the data directory
String lyricsFile = dataDir + "spring_day_hangul_spaced.txt";
////////////////////////////////////////////////////////////
void setup() {
  size(1280, 720);  
  inputImage = createImage(width, height, RGB);
  font = createFont("NanumGothic", 20);
  textFont(font, 30);
  println("time interval between draws = " + (1.0/numDrawnPer)*1000);
  loadLyrics();
}
////////////////////////////////////////////////////////////
void draw() {
  // When the program first starts, draw a menu of different options for which camera to use for input
  // The input method is selected by pressing a key 0-9 on the keyboard
  if (!inputMethodSelected) {
    cameras = Capture.list();
    int y=40;
    text("0: Offline mode, test with TextRainInput.mov movie file instead of live camera feed.", 20, y);
    y += 40; 
    for (int i = 0; i < min(9,cameras.length); i++) {
      text(i+1 + ": " + cameras[i], 20, y);
      y += 40;
    }
    return;
  }


  // This part of the draw loop gets called after the input selection screen, during normal execution of the program.

  
  // STEP 1.  Load an image, either from a movie file or from a live camera feed. Store the result in the inputImage variable
  color camPix;
  float pixB;
  
  if ((cam != null) && (cam.available())) {
    cam.read();    
    mirrorImage(cam);
  }
  else if ((mov != null) && (mov.available())) {
    mov.read();
    inputImage.copy(mov, 0,0,mov.width,mov.height, 0,0,inputImage.width,inputImage.height);
    
    //debug thresholding
    if (debugSelected) {
      inputImage.loadPixels();
      
      for(int x = 0; x < inputImage.width; x++){
        for(int y = 0; y < inputImage.height; y++){
          camPix = grayscale(inputImage.pixels[x+y*inputImage.width]);
          
          //debug thresholding
          if (debugSelected) {
            camPix = thresholdPixel(camPix);
          }
          
          inputImage.pixels[x+y*inputImage.width] = camPix;
        }
      }
      
      inputImage.updatePixels();
    }
    
    set(0, 0, inputImage);
  }
  else {
    // Tip: This code draws the current input image to the screen
    set(0, 0, inputImage);
  }
  
  //keep track of time : (1/d)*(1000ms/1s) = ms time interval
  if (millis() % (int)((1.0/numDrawnPer)*1000) == 0) {
    //make sure there's enough to grab from
    if (notDispBlocks.size() < numDrawnPer) { //not enough - fill with origBlocks
      println("Not enough! Refill notDispBlocks.");
      notDispBlocks.clear(); //first empty it
      notDispBlocks.addAll(origBlocks);
    }
    grabBlocksToDisplay();
  }
  
  //make sure there's stuff to draw
  if (dispBlocks.size() == 0) {
    grabBlocksToDisplay();
  }
  
  //move the already displayed blocks
  for (int i = 0; i < dispBlocks.size(); i++) {
    dispBlocks.get(i).move();
  }
  
  //show numDrawnPer value
  fill(50,50,50);
  rectMode(CENTER);
  rect(150, height-50, 250, 40);
  
  fill(255);
  textSize(25);
  textAlign(CENTER, CENTER);
  text("Drawing " + numDrawnPer + " per sec", 150, height-50);
  
  //show threshold value
  fill(50,50,50);
  rect(width-150, height-50, 250, 40);
  
  fill(255);
  textSize(25);
  textAlign(CENTER, CENTER);
  text("Threshold: " + threshold, width-150, height-50);
  
}
////////////////////////////////////////////////////////////
void keyPressed() {
  if (!inputMethodSelected) {
    // If we haven't yet selected the input method, then check for 0 to 9 keypresses to select from the input menu
    if ((key >= '0') && (key <= '9')) { 
      int input = key - '0';
      if (input == 0) {
        println("Offline mode selected.");
        mov = new Movie(this, "TextRainInput.mpg");
        mov.loop();
        inputMethodSelected = true;
      }
      else if ((input >= 1) && (input <= 9)) {
        println("Camera " + input + " selected.");           
        // The camera can be initialized directly using an element from the array returned by list():
        cam = new Capture(this, cameras[input-1]);
        cam.start();
        inputMethodSelected = true;
      }
    }
    return;
  }


  // This part of the keyPressed routine gets called after the input selection screen during normal execution of the program
  // Fill in your code to handle keypresses here..
  
  if (key == CODED) {
    if (keyCode == UP) {
      // up arrow key pressed = increase threshold value
      threshold += 1;
    }
    else if (keyCode == DOWN) {
      // down arrow key pressed = decrease threshold value
      threshold -= 1;
    }
    else if (keyCode == LEFT) {
      numDrawnPer--;
    }
    else if (keyCode == RIGHT) {
      numDrawnPer++;
    }
  }
  else if (key == ' ') {
    // space bar pressed = set/unset debug mode
    debugSelected = !(debugSelected);
    
    if (debugSelected) {
      println("Debug mode enabled.");
    }
    else {
      println("Debug mode disabled.");
    }
  } 
  
}
////////////////////////////////////////////////////////////
// ADDITIONAL METHODS AND CLASSES
////////////////////////////////////////////////////////////
void loadLyrics() {
  //read in txt file
  String lines[] = loadStrings(lyricsFile);
  
  //parse each line and save characters into big ArrayList
  String[] temp;
  float prevX = 5;
  float newX;
  float v, y;
  SBlock newB;
  
  for (int i = 0; i < lines.length; i++) {
    temp = split(lines[i], " ");
    for (int j = 0; j < temp.length; j++) {    
      v = random(minV, maxV);
      y = random(-20, 20);
      newB = new SBlock(temp[j], 0, y, v, origBlocks.size());
      origBlocks.add(newB);
    }
  }
  
  //determine x-spacing based on size of origBlocks and canvas
  int numAcross = ceil(origBlocks.size()/10);
  float between = width/numAcross;
  int ith = 1;
  float xx;
  
  for (int i = 0; i < origBlocks.size(); i++) {
    if (ith > numAcross) {
      ith = 1;
    }
    xx = (ith*between) + random(-(between/4), (between/4));
    
    if (xx >= width) { //to be safe
      xx = width-10;
      println("Overshot right side: new xx = " + xx);
    }
    
    origBlocks.get(i).setX(xx);
    notDispBlocks.add(origBlocks.get(i));
    ith++;
  }
  
  println("width = " + width);
}
////////////////////////////////////////////////////////////
void mirrorImage(PImage img) {
  color camPix;
  
  ///////////////////////////////////////
  //following code adapted from:
  //  https://forum.processing.org/one/topic/mirroring-a-webcam-in-a-sketch.html
  img.loadPixels();
  
  //Mirroring the image
  for(int x = 0; x < img.width; x++){
    for(int y = 0; y < img.height; y++){
      camPix = grayscale(img.pixels[(img.width-(x+1))+y*img.width]);
      
      //debug thresholding
      if (debugSelected) {
        camPix = thresholdPixel(camPix);
      }
      
      inputImage.pixels[x+y*img.width] = camPix;
    }
  }
  
  inputImage.updatePixels();
  image(inputImage,0,0);
  ///////////////////////////////////////
}
////////////////////////////////////////////////////////////
color thresholdPixel(color camPix) {
  float pixB = brightness(camPix);
        
  if (pixB > threshold) { //higher brightness -> white
    return color(255, 255, 255);
  }
  else { //lower brightness -> black
    return color(0,0,0);
  }
}
////////////////////////////////////////////////////////////
color grayscale(color c) {
   return color((0.2989*red(c)) + (0.5870*green(c)) + (0.1140*blue(c)));
}
////////////////////////////////////////////////////////////
boolean isDark(int xx, int yy) {
  //check pixel value at (xx,yy)
  inputImage.loadPixels();
  //if thresholdPixel() --> black: TRUE
  if (thresholdPixel(inputImage.get(xx, yy)) == color(0,0,0)) {
     return true;
  }
  //if thresholdPixel() --> white: FALSE
  else if (thresholdPixel(inputImage.get(xx, yy)) == color(255,255,255)) {
     return false;
  }
  
  return true;
}
////////////////////////////////////////////////////////////
void grabBlocksToDisplay() {
  int randIndex = 0;
  int ndp_size = notDispBlocks.size();
  int picked = 0;
  
  //loop through and grab enough blocks
  while (picked < numDrawnPer) {
    randIndex = (int)random(0, ndp_size);
    //println("randIndex = " + randIndex);
    dispBlocks.add(notDispBlocks.get(randIndex));
    notDispBlocks.remove(randIndex);
    
    picked ++;
    ndp_size --; //decrement b/c ArrayLists are resized dynamically
  }
}
////////////////////////////////////////////////////////////
class SBlock {
  String str; //syllable block
  float x; //current x position
  float y; //current y position
  float v; //current velocity
  float t; //current time
  float a; //block acceleration
  color c; //color
  int index;//loc within origBlocks array
  /*-------------------------------------*/
  //CONSTRUCTOR//
  /*-------------------------------------*/
  SBlock(String s, float xPos, float yPos, float vel, int i) {
    str = s;
    x = xPos;
    y = yPos;
    v = vel;
    t = 0;
    a = 0.2;
    c = color((int)random(255), (int)random(255), (int)random(255));
    index = i;
    
    println("Added index #" + index + " to origBlocks.");
  }
  /*-------------------------------------*/
  //METHODS//
  /*-------------------------------------*/
  void move() {
    //y = v*t + 0.5*a*pow(t,2); //update y position
    //t = millis()/1000.0;
    //println("t = " + t);
    
    //check for negative y
    if (y <= 0) {
      y += v + 0.5*a;
      //y = v*t + 0.5*a*pow(t,2);
      return;
    }
    
    //check for if current (x,y) is dark BEFORE updating y
    if (isDark((int)x, (int)y)) {
      float newY = edgeFollow();
      //check to see if the edge moved up
      if (newY != -1) {
        y = newY;
      }
      else { //did not detect an edge to follow --> drop
        y += v + 0.5*a;
      }
    }
    else { //if the current (x,y) is NOT dark
      y += v + 0.5*a; //assuming dt = 1
      //y = v*t + 0.5*a*pow(t,2);
    }
    
    //remove from drawn blocks if at bottom
    if (y > height) {
      t = 0;
      x += random(-10, 10); //slightly shift x pos
      y = random(-20, 20);
      v = random(minV, maxV);
      
      dispBlocks.remove(origBlocks.get(index));
      notDispBlocks.add(origBlocks.get(index));
    }
    
    //only draw if y is positive
    if ( y >= 0 ) {
      fill(c);
      textAlign(CENTER, BOTTOM); //centers text on (x,y) & draws text above (x,y)
      text(str, x, y);
    }
  }
  /*-------------------------------------*/
  // return -1 --> no edge to follow
  // else return new y-coord --> yes edge to follow
  float edgeFollow() {
    float tempY = y - 1;
    int count = 1;
    
    //check vertically; above current y val
    if (!isDark((int)x, (int)tempY)) { //if on top of an edge
      if (!isDark((int)x+2, (int)y+2)) { //bottom right
        //println("slide to bottom right");
        this.setX(x+2);
        return y+2;
      }
      if (!isDark((int)x-2, (int)y+2)) { //bottom left
        //println("slide to bottom left");
        this.setX(x-2);
        return y+2;
      }
    }
    
    //keep checking vertical
    while (count < numCheckPixels) {
      if (!isDark((int)x, (int)tempY)) { //if (x,tempY) is light
        return tempY;
      }
      tempY -= numCheckPixels;
      
      count ++;
    }
    
    //don't follow anything
    return -1;
  }
  /*-------------------------------------*/
  void setX(float xx) {
    x = xx;
  }
  /*-------------------------------------*/
}