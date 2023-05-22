#include <DynamixelWorkbench.h>
DynamixelWorkbench dxl_wb;

void setup() 
{
  Serial.begin(9600);
  dxl_wb.begin("",1000000);
  dxl_wb.ping(1);
  dxl_wb.ping(2);
  dxl_wb.wheelMode(1);
  dxl_wb.wheelMode(2);
}

void loop() {
    char c;
    int x,y;
    if(Serial.available()){
      delay(300);
      c=Serial.read();
      x=Serial.parseInt();
      y=Serial.parseInt();
    }
    Serial.flush();
    if(c=='A'){
      Serial.print(x);
      Serial.print(y);
    dxl_wb.goalSpeed(1,x);
    dxl_wb.goalSpeed(2,y);
    }
}
