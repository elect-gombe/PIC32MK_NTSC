//needs math library!

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define _MAX_DESAMPLE 3
#define _CLOCK_DIVIDE 16

void calc_wave(uint8_t r,uint8_t g,uint8_t b){
  int output[16];
  int i;
  uint32_t dt;
  double theta;

  double y = (g*0.587+b*0.114+r*0.299)/256;
  double b_y = (-g*0.587+0.886*b-0.299*r)/256;
  double r_y = (-g*0.587-0.114*b+0.701*r)/256;

  double n;
  double err = 0;

  uint8_t reordered[2][3];
  for(i=0;i<_CLOCK_DIVIDE;i++){
    theta = 2*3.14159265358979323*i/_CLOCK_DIVIDE;
    n = (y+0.4921*b_y*sin(theta)+0.8773*r_y*cos(theta)+0.35)*4.13;
    output[i] = n+err;   //prev err correction.
    err += n-output[i];  //calculate err of output.
  }

  /*bit reorder*/
  int j;
  for(j=0;j<3;j++){
    dt = 0;
    for(i=0;i<_CLOCK_DIVIDE;i++){
      if(output[i]&(1<<j)){
	dt |= 1<<(15-i);
      }
    }
    reordered[j][1] = dt&0xFF;
    reordered[j][0] = dt>>8;
  }

  printf("  ");
  for(i=0;i<2;i++){
    printf("0x%06x,",reordered[2][i]<<16|reordered[1][i]<<8|reordered[0][i]);
  }

  printf("  //(r,g,b)=(#%02x%02x%02x) data:",r,g,b);
  for(i=0;i<_CLOCK_DIVIDE;i++){
    printf("%d",output[i]);
  }
  puts("");
}

int main(void){
  int r,g,b;

  printf("uint32_t colordata[]={\n");
  for(r=0;r<256;r+=8){
    calc_wave(r,0,0);
  }
  for(r=0;r<256;r+=8){
    calc_wave(r,r,0);
  }
  for(r=0;r<256;r+=4){
    calc_wave(r,r,r);
  }
  for(r=0;r<256;r+=8){
    calc_wave(r,0,r);
  }
  for(r=0;r<256;r+=8){
    calc_wave(0,r,0);
  }
  for(r=0;r<256;r+=8){
    calc_wave(0,r,r);
  }
  for(r=0;r<256;r+=8){
    calc_wave(0,0,r);
  }

  puts("};\n");  
  return 0;
}
