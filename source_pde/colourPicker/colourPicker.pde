import processing.serial.*;

//Global Variables
ColorPicker cp;
Serial myPort;
String anStatus = "Analyzing";
int flip = 0;
static String PORT = "COM9";

void setup() 
{
  //Setting up window
  size( 500, 500 );
  frameRate( 100 ); 
  //Initalizing the colour picker
  cp = new ColorPicker( 10, 10, 400, 400, 255 );
  //Setting up port for serial communication
  myPort = new Serial(this, PORT, 9600); 
  //Creating a buffer until a newline character is read
  myPort.bufferUntil('\n');
}
//An event to detect when data is available to be read in the port
void serialEvent (Serial myPort){ 
  //Setting the animation that is being played 
  anStatus = myPort.readStringUntil('\n');
}

void draw ()
{
  background( 80 );
  cp.render();
}

/*
    The Colour Picker GUI
*/
void mouseClicked(){
    //Checking to see what button has been clicked
    //These buttons show the two colours 
    if(cp.overRect(cp.x, cp.y+cp.h+10, 60, 60 )){
      cp.activeButton = 0;
      cp.colours[0] = cp.c;
    }
    if(cp.overRect(cp.x+80, cp.y+cp.h+10, 60, 60 )){
      cp.activeButton = 1;
      cp.colours[1] = cp.c;
    }
    //If the send button
    if(cp.overRect(cp.x+200, cp.y+cp.h+10, 80, 20)){
      cp.sendRequest(); 
    }
}
  
public class ColorPicker 
{
  int x, y, w, h, c, activeButton;
  int[] colours = new int[2];
  PImage cpImage;
  
  public ColorPicker ( int x, int y, int w, int h, int c)
  {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
    this.c = c;
    this.activeButton = 1;
    cpImage = new PImage( w, h );
    
    init();
  }
  
  private void init ()
  {
    // drawing colour to make the ranibow gradient.
    int cw = w - 60;
    for( int i=0; i<cw; i++ ) 
    {
      float nColorPercent = i / (float)cw;
      float rad = (-360 * nColorPercent) * (PI / 180);
      int nR = (int)(cos(rad) * 127 + 128) << 16;
      int nG = (int)(cos(rad + 2 * PI / 3) * 127 + 128) << 8;
      int nB = (int)(Math.cos(rad + 4 * PI / 3) * 127 + 128);
      int nColor = nR | nG | nB;
      
      setGradient( i, 0, 1, h/2, 0xFFFFFF, nColor );
      setGradient( i, (h/2), 1, h/2, nColor, 0x000000 );
    }
    
    // draw black/white.
    drawRect( cw, 0,   30, h/2, 0xFFFFFF );
    drawRect( cw, h/2, 30, h/2, 0 );
    
    // draw grey scale.
    for( int j=0; j<h; j++ )
    {
      int g = 255 - (int)(j/(float)(h-1) * 255 );
      drawRect( w-30, j, 30, 1, color( g, g, g ) );
    }
  }

  private void setGradient(int x, int y, float w, float h, int c1, int c2 )
  {
    float deltaR = red(c2) - red(c1);
    float deltaG = green(c2) - green(c1);
    float deltaB = blue(c2) - blue(c1);

    for (int j = y; j<(y+h); j++)
    {
      int c = color( red(c1)+(j-y)*(deltaR/h), green(c1)+(j-y)*(deltaG/h), blue(c1)+(j-y)*(deltaB/h) );
      cpImage.set( x, j, c );
    }
  }
  
  private void drawRect( int rx, int ry, int rw, int rh, int rc )
  {
    for(int i=rx; i<rx+rw; i++) 
    {
      for(int j=ry; j<ry+rh; j++) 
      {
        cpImage.set( i, j, rc );
      }
    }
  }
  
  public void render ()
  {
    image( cpImage, x, y );
    if( mousePressed &&
  mouseX >= x && 
  mouseX < x + w &&
  mouseY >= y &&
  mouseY < y + h )
    {
      c = get( mouseX, mouseY );
    }
    
    createButtons();
    text(activeButton, 100,100);
  }
  public void createButtons(){
    //Drawing the buttons
    PShape[] buttons = {createShape(RECT, x, y+h+10, 60, 60),
        createShape(RECT, x+80, y+h+10, 60, 60 )};
        
      for(int i = 0; i < buttons.length; i++){
        buttons[i].setFill(colours[i]);
        shape(buttons[i]); 
      }
      buttons[activeButton].setFill(c);
      shape(buttons[activeButton]);
      
      //Making a button to send the bluTooth
      PShape submitButton = createShape(RECT, x+200, y+h+10, 80, 20 );
      submitButton.setFill(0);
      shape(submitButton);
      text("Send", x+225,y+h+25);
      
      //Showing what animation the LED is on
      text("Current Animation: " + anStatus, x+300,y+h+25);
  }
  
  public boolean overRect(int x, int y, int width, int height)  {
    if (mouseX >= x && mouseX <= x+width && 
        mouseY >= y && mouseY <= y+height) {
      return true;
    } else {
      return false;
    }
  }
  public void sendRequest(){
    //sending the preset to arduino
    println("Sending to board...");
    //Letting the arduino know to read
    String hexColourA = hex(colours[0], 6);
    String hexColourB = hex(colours[1], 6);
    println(hexColourA);
    println(hexColourB);
    myPort.write(hexColourA);
    myPort.write(hexColourB);
  }
}
