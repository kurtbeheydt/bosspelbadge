//
// debug code
// infrarood leezen
//
// Wouter V
//
//niet af

#include <Fri3dBadge.h>

// create an instance of the badge
Fri3dBadge badge;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  badge.on_button_change(aan);
  badge.on_ir_activity(lol);
  badge.ir_rx_start();
}
int a = 0;
bool rest = false;
bool attack = false;

void lol(){
  Serial.print(a);
  Serial.println("\n");
  a = a + 1;
}

void reset(){
  a = 0;
  Serial.println("reset");
  Serial.println("\n");
  Serial.print(a);
  Serial.println("\n");
  rest = false;
}

void aan(){
  rest = true;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (badge.button_is_pressed()){
    if (rest){
      reset();
    }
  }
}
