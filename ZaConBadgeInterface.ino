#include <SoftwareSerial.h>       // used for talking to ZaCon Badge
SoftwareSerial mySerial(10, 11);  // pin 10 (RX) connects to badge TX, ping 11 (TX) connects to badge RX

String badge1;
String badge2;
char message_type;
char old_menu = '0';
char current_menu = '0';
boolean message_ready = false;

void setup() {
  Serial.begin(9600);             // set the data rate for the Serial port
  while (!Serial) { ; }           // wait for serial port to connect - needed for Leonardo only
  mySerial.begin(9600);           // set the data rate for the SoftwareSerial port
}

void loop() {
  listen_for_badge_events();
}

void listen_for_badge_events() {
  String message = "";
  int i=0;

  // read a line of text, then process it
  if(mySerial.available()) {
     delay(100);
     while(mySerial.available() && i<99) {
       i++;
       char c = mySerial.read();
       message = message + c;
       message_ready = true;
     }
  }

  if(message_ready) {
    message_ready = false;

    // Serial.print("RECEIVED: ");Serial.print(message);  // uncomment for debugging

    // process menu-changes
    if (message.substring(0,5) == "menu:") {
      current_menu = message[5];
      if (current_menu != old_menu) {
        old_menu = current_menu;
        menu_changed(current_menu);
      }
    }

    // process "messages"
    if (message.substring(0,8) == "message:") {
      message_type = message[8];

      switch (message_type) {
        case 'S':   // badge seen
          badge1 = message.substring(9,13);
          saw_badge(badge1);
          break;

        case 'R':   // relationship seen
          badge1 = message.substring(9,13);
          badge2 = message.substring(14,18);
          saw_badges(badge1, badge2);
          break;

        case 'L':   // update to "LiveSpeaker"
          break;

        case 'U':   // update to "Who Am I"
          break;

        case 'A':   // Reset Badge
          break;

        case 'C':   // "CoolBageMode" - not sure :P
          break;
      }
    }
  }
}

// user navigates to a new menu item
void menu_changed(char menu_num) {
  switch (menu_num) {
    case '1':   // schedule
      Serial.println("schedule");
      break;

    case '2':   // live speaker
      Serial.println("live speaker");
      break;

    case '3':   // about
      Serial.println("about");
      break;

    case '4':   // stats
      Serial.println("stats");
      break;

    case '5':   // who am i
      Serial.println("who am i");
      break;

    default:  // main menu
      Serial.println("main menu (or error)");
  }
}

// badge ID is seen (may already have been seen)
void saw_badge(String badge) {
  Serial.print("Saw badge ");Serial.println(badge);
}

// relationship seen between two other badges (may already have been seen)
void saw_badges(String badge1, String badge2) {
  Serial.print("Saw badge ");Serial.print(badge1);
  Serial.print(" and ");Serial.println(badge2);
}
