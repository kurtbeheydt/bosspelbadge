#include <Fri3dBadge.h>

// create an instance of the badge
Fri3dBadge badge;

void setup() {
  // prepare randomness with some random reading from an unconnected analog pin
  randomSeed(analogRead(0));

  // select your team color at boot ...
  select_team(1); // demo to select team 1
  revive_player(3); // demo to give a player rank 3

  // ... and allow to show the rank using the button
  badge.on_button_change(show_rank);
  
/*  
  // start the IR receiver
  badge.on_ir_activity(handle_activity);
  badge.ir_rx_start();
*/
}

uint8_t r = 0, g = 0, b = 0;

bool alive;
uint8_t team;
uint8_t rank;


void select_team(uint8_t newteam) {
  uint8_t team = newteam; // 1 = rood, 2 = blauw
  
  r = g = b = 0;

  if (team == 1) {
    r = 255;
  } else if (team == 2) {
    b = 255;
  }

//  flash_color();
}

void revive_player(uint8_t newrank) {
  alive = true;
  rank = newrank;
  flash_color();
}

void select_color() {

}

bool showing_rank = false;


void show_rank() {
  if( ! badge.button_is_pressed() ) {
    showing_rank = true;
  }
}

bool activity_detected = false;

// don't respond to the very first interrupt, but make sure the received pulse
// was part of a pulse train
#define SKIP_ACTIVITY 2
int8_t skip_activity;

// don't respond to our own IR signals ;-)
volatile bool sending = false;

void handle_activity() {
  if(sending) { return; }
  if(skip_activity-- > 0) { return; }
  // set a flag to handle the activity (don't do too much in interrupt handler)
  activity_detected = true;
}

void flash_color() {
  badge.rgb_set_color(r, g, b);
  delay(500);
	badge.rgb_set_color(0, 0, 0);
}

/*
void fade_color() {
  for(uint8_t i=0; i<25; i++) {
    badge.rgb_set_color(r/25*i, g/25*i, b/25*i);
    delay(10);
  }
  delay(500);
  for(int8_t i=25; i>0; i--) {
    badge.rgb_set_color(r/25*i, g/25*i, b/25*i);
    delay(10);
  }
  badge.rgb_set_color(0, 0, 0);
}
*/

/*
void wheel( byte pos ) {
  if(pos < 85) {
   badge.rgb_set_color( pos, 85 - pos, 0 );
  } else if( pos < 170) {
   pos -= 85;
   badge.rgb_set_color( 85 - pos, 0, pos );
  } else {
   pos -= 170;
   badge.rgb_set_color( 0, pos, 85 - pos );
  }
}
*/

/*
void reply_with_party_parrot() {
  for(uint8_t i = 0; i < 255; i++) {
    wheel(i);
    delay(4);
  }
  badge.rgb_set_color(0, 0, 0);
}
*/

// timestamp of next dialog (= send pulse train)
uint32_t next_dialog = 0;

//timestamp of next alive flash
uint32_t next_alive_flash = 0;

// we take a minimal interval of 5s and add a random part of the same length
#define INTERVAL    2000 + (random(6) * 1000) // start dialog every 5s + [0-5]s

// a pulse train of x pulses of pulse length y
#define PULSES        10
#define PULSE_LENGTH 500   // us

void loop() {
  uint32_t now = millis(); // rollover of ~49.7 days ... not gonna happen ;-)

  // TODO listen to serial
  // 


  if (now >= next_alive_flash) {
    flash_color();
    next_dialog = now + 1000;
  }

  // start a dialog at INTERVAL
  if( now >= next_dialog ) {
    // send an IR pulse train

    // TODO hack to pulse your rank
    
    sending = true;
    for(uint8_t t=0; t<PULSES; t++) {
      badge.ir_send_pulse(PULSE_LENGTH);
      delayMicroseconds(PULSE_LENGTH);
    }
    sending = false;

    // schedule the next dialog after interval
    next_dialog = now + INTERVAL;
  }

  // respond to activity
  if(activity_detected) {
    delay(250);

    // TODO: detect other rank and act accordinly: die or not
    
    activity_detected = false;
   // reply_with_party_parrot();
    skip_activity = SKIP_ACTIVITY;
    next_dialog = now + INTERVAL;
  }

  // showing rank on button pressed
  if (showing_rank) {
    for (int count = 0; count < rank; count++) {
        badge.rgb_set_color(255, 255, 255);
        delay(500);
        badge.rgb_set_color(0, 0, 0);
        delay(250);
    }
    showing_rank = false;
    next_dialog = now + INTERVAL;
  }
  
  delay(5);
}
