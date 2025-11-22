// Basic program skeleton for a Sketch File (.sk) Viewer
#include "displayfull.h"
#include "sketch.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static int n_commands = 0;

// Allocate memory for a drawing state and initialise it
state *newState(void) {
  //TO DO
  state *out = malloc(sizeof(state));
  *out = (state) {0,0,0,0,LINE,0,0,0};
  return out; // this is a placeholder only
}

void resetState(state* s) {
  s->tool = LINE;
  s->data = 0;
  s->end = 0;
  s->x = 0;
  s->y = 0;
  s->tx = 0;
  s->ty = 0;
}

// Release all memory associated with the drawing state
void freeState(state *s) {
  free(s);
  //TO DO
}

// Extract an opcode from a byte (two most significant bits).
int getOpcode(byte b) {
  //TO DO
  const byte extractor = 192; // 1100 0000
  const byte data = 192;
  const byte tool = 128;
  const byte dx = 0;
  const byte dy = 64;
  byte onlyCode = b & extractor;
  int opCode = 0;
  switch (onlyCode) {
    case tool:
      opCode = TOOL; break;
    
    case dx:
      opCode = DX; break;
    
    case dy:
      opCode = DY; break;
    
    case data:
      opCode = DATA; break; 
  }
    
  
  return opCode; // this is a placeholder only
}

int convertNegInt(byte b) {
  /*
  1. subtract 1
  2. Negate to flip bits
  3. & 0011 1111
  4. Convert to int
  5. If 0 then -32 else *-1 
  */
  //Check if it is a negative
  
  const byte removeOpCode = 63; // 0011 1111
  int out;
  b &= removeOpCode;
  b -= 1;
  b = ~b;
  b &= removeOpCode;
  out = (int) b;
  if (b == 0) {
    out = -32;
  } else {
    out *= -1;
  }
  return out;
}

// Extract an operand (-32..31) from the rightmost 6 bits of a byte.
int getOperand(byte b) {
  //TO DO
  const byte extractor = 31; // 0001 1111
  const byte negSign = 32; // 0010 0000
  byte operand = b & extractor;
  int out = (int) operand;
  if ((b & negSign) == 32) { // Negative int
    out = convertNegInt(b);
  } 
  
  return out; 
}

void performTool(display *d, state *s, int operand) {
  if (operand == 0) { // 1000 0000
    s->tool = NONE;
  } else if (operand == 1) { //1000 0001
    s->tool = LINE;
  }
  switch(operand) {
    case NONE:
      printf("|NONE| \n");
      s->tool = NONE; break;
      
    case LINE:
      printf("|LINE| \n");
      s->tool = LINE; break;
    case BLOCK:
      printf("|BLOCK| \n");
      s->tool = BLOCK; break;
    case COLOUR:
      printf("|COLOUR| %02x\n",s->data);
      colour(d,s->data); break;
      
    case TARGETX:
      printf("|TARGETX|\n");
      s->tx = s->data; break;
    case TARGETY:
      printf("|TARGETY|\n");
      s->ty = s->data; break;
    case SHOW:
      printf("|SHOW|\n");
      show(d); break;
      
    case PAUSE:
      printf("|PAUSE|\n");
      pause(d,s->data); break;
      
    case NEXTFRAME:
      printf("|NEXTFRAME|\n");
      s->start = n_commands;
      s->end = true;
    default:
      break;
    
  }
}

void performDx(state *s, int operand) {
  s->tx += operand;
  // printf("X: %d TX: %d Y:%d TY: %d\n",s->x,s->tx,s->y,s->ty);
}

void performDy(display *d, state *s, int operand) {
  s->ty += operand;
  
  // printf("X: %d TX: %d Y:%d TY: %d\n",s->x,s->tx,s->y,s->ty);
  
  if (s->tool == LINE) {
    line(d,s->x,s->tx,s->y,s->ty);  
  } else if (s->tool == BLOCK) {
    /*
    Draw block at x,y with w=tx-x and h = ty-y
    */
    int w = s->tx - s->x;
    int h = s->ty - s->y;
    // printf("Block X: %d TX: %d Y:%d TY: %d\n",s->x,s->tx,s->y,s->ty);
    block(d,s->x,s->y,w,h);
  }
  
  s->x = s->tx;
  s->y = s->ty;
}

void performData(state *s, int operand) {
  /*
  Shifts current data value in s by 6 to the left, insert 6 bit operand 
  1. shift data by 6
  2. use or to merge operand and data
  */
 
  int old = s->data;
  operand &= 63; //0011 1111
  s->data <<= 6;
  s->data |= operand;
  // printf("data: %d newdata: %d\n", old, s->data);
}

// Execute the next byte of the command sequence.
void obey(display *d, state *s, byte op) {
  //TO DO
  /*
  1. Extract opcode and operand
  2. Update tool var in state
  3. if op == TOOL, operand == 0 set NONE else operand == 1 set LINE
  if op == dx, operand = x', set tx = x'
  if op == dy, operand = y', set ty = y', if LINE, call line(x,y,tx,ty), update current location
  */
 int opCode = getOpcode(op);
 int operand = getOperand(op);
  switch(opCode) {
    case TOOL:
      if (operand == 1) {
        
      } else {
        
      }
      performTool(d,s,operand);
      s->data = 0;
      break;
    case DX:
      printf("|DX| OP: %d\n",operand);
      performDx(s,operand); break;
    case DY:
      printf("|DY| OP: %d\n",operand);
      performDy(d,s,operand); break;
    case DATA:
      
      performData(s,operand); 
      printf("|DATA| OP: %02x DATA: %02x\n",operand,s->data);
      break;
    default:
      break;
      
  }
}



// Draw a frame of the sketch file. For basic and intermediate sketch files
// this means drawing the full sketch whenever this function is called.
// For advanced sketch files this means drawing the current frame whenever
// this function is called.
bool processSketch(const char pressedKey, void *data, display *d) {
    
    //TO DO: OPEN, PROCESS/DRAW A SKETCH FILE BYTE BY BYTE, THEN CLOSE IT
    //NOTE: CHECK DATA HAS BEEN INITIALISED... if (data == NULL) return (pressedKey == 27);
    //NOTE: TO GET ACCESS TO THE DRAWING STATE USE... state *s = (state*) data;
    //NOTE: TO GET THE FILENAME... char *filename = getName(d);
    //NOTE: DO NOT FORGET TO CALL show(d); AND TO RESET THE DRAWING STATE APART FROM
    //      THE 'START' FIELD AFTER CLOSING THE FILE
    /*
    1. Get sketch file name
    2. Open file
    LOOP
    3. Get binary char
    4. Call obey
    ENDLOOP
    5. call show
    6. Reset state
    7. Close file
    */
  if (data == NULL) {
    return (pressedKey == 27);
  }
  state* s = (state*) data;
  n_commands = s->start;
  char* name = getName(d);
  FILE* in = fopen(name,"rb");
  if (in == NULL) {
    fprintf(stderr,"Can't open %s: ", name);
    fflush(stderr);
    perror("");
    exit(1);
  }
  int count_command = 0;
  byte b = fgetc(in);
  while (!feof(in) && !s->end) {
    if (count_command >= s->start) {
      n_commands += 1;
      printf("%d %d ",count_command,b);

      obey(d,s,b);
      
    }
    b = fgetc(in);
    count_command++;
  }
  resetState(s);
  if (s->start != n_commands) {
    
    s->start = 0;
  }
  show(d);
  fclose(in);
  
  return (pressedKey == 27);
}

// View a sketch file in a 200x200 pixel window given the filename
void view(char *filename) {
  display *d = newDisplay(filename, 200, 200);
  state *s = newState();
  run(d, s, processSketch);
  freeState(s);
  freeDisplay(d);
}

// Include a main function only if we are not testing (make sketch),
// otherwise use the main function of the test.c file (make test).
#ifndef TESTING
int main(int n, char *args[n]) {
  if (n != 2) { // return usage hint if not exactly one argument
    printf("Use ./sketch file\n");
    exit(1);
  } else view(args[1]); // otherwise view sketch file in argument
  return 0;
}
#endif
