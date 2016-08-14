//
// mij debug code
// infrarood spreken
//
// Wouter V
//
//niet af

#include <Fri3dBadge.h>

Fri3dBadge badge;

void setup() {
  //start een verbinding via usb kabel
  Serial.begin(9600);
  badge.on_button_change(aan);
  badge.on_button_change(gevecht_start);
  badge.ir_rx_start();
}
bool rest = false;

void aan(){
  rest = true;
}

void gevecht_start(){
  Serial.print("start functie gevecht_start");
  Serial.println("\n");
  if (1 == 1){ //rood team  //vervang eerste 1 door variable team 
    badge.ir_send_pulse(500);
  }
  else if (1 == 2){//blauw team  //vervang eerste 1 door variable team 
    badge.ir_send_pulse(500);
    delay(20);
    badge.ir_send_pulse(500);
  }
  Serial.print("wacht 1 seconde");
  Serial.println("\n");
  delay(1000);
  Serial.println("\n");
  int nu = millis();
  Serial.print(nu);
  Serial.println("\n");
  int no = nu + 5;
  Serial.print(no);
  Serial.println("\n");
  delay(2000);
  //while (nu < no){
    Serial.println("\n");
    Serial.print("puls");
    for (int x = 0; x <= 3; x++){ //verander 3 door variable van rank
      badge.ir_send_pulse(500);
      delay(50);
      Serial.print("puls");
      Serial.println("\n");
    }
    rest = false;
  //}
}

void loop() {
  // put your main code here, to run repeatedly:
  if (badge.button_is_pressed()){
    if (rest){
      gevecht_start();
    }
  }
}
