

int scaleFactor = 4;

void settings() {
  //size(size*scaleFactor, (size/2)*scaleFactor);
  size(64*scaleFactor, 64*scaleFactor);
}

void draw() {

  background(0);
  colorMode(RGB); 
  noStroke();
  color c;
  int h = 8;
  int w = 8;
  for (int j = 0; j < h; j++) { 
    for (int i = 0; i < w; i++) {
      c = color(r,g,b);
      fill(c);
      rect(i*scaleFactor, j*scaleFactor, scaleFactor, scaleFactor);
    }
  }
}
