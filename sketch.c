// Basic program skeleton for a Sketch File (.sk) Viewer
#include "displayfull.h"
#include "sketch.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Allocate memory for a drawing state and initialise it
state *newState(void) {
  //TO DO
  state *out = malloc(sizeof(state));
  *out = (state) {0,0,0,0,LINE,0,0,0};
  return out; // this is a placeholder only
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
  const byte extractor = 31; // 0001 1111
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

// Execute the next byte of the command sequence.
void obey(display *d, state *s, byte op) {
  //TO DO
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
