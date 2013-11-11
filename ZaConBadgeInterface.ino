#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX

char old_menu = '0';
char current_menu = '0';
char message_type;
String badge1;
String badge2;
boolean message_ready = false;

void setup() {
  Serial.begin(9600);
  while (!Serial) { ; } // wait for serial port to connect. Needed for Leonardo only
  mySerial.begin(9600); // set the data rate for the SoftwareSerial port
}

void loop() {
  listen_for_badge_events();
}

void listen_for_badge_events() {
  String message = "";
  int i=0;

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
    // // DEBUG
    // Serial.print("RECEIVED: ");Serial.print(message);
    message_ready = false;

    if (message.substring(0,5) == "menu:") {
      current_menu = message[5];
      if (current_menu != old_menu) {
        old_menu = current_menu;
        menu_changed(current_menu);
      }
    }

    if (message.substring(0,8) == "message:") {
      message_type = message[8];

      switch (message_type) {
        case 'S':
          badge1 = message.substring(9,13);
          saw_badge(badge1);
          break;

        case 'R':
          badge1 = message.substring(9,13);
          badge2 = message.substring(14,18);
          saw_badges(badge1, badge2);
          break;

        case 'L':
          // LiveSpeaker update
          break;

        case 'U':
          // Nick ("whoami") update
          break;

        case 'A':
          // Reset Badge
          break;

        case 'C':
          // "Cool Bage Mode"
          break;

        // default:
          // some other message?
      }
    }
  }
}

void menu_changed(char menu_num) {
  switch (menu_num) {
    case '1':
      // schedule
      Serial.println("schedule");
      break;

    case '2':
      // live speaker
      Serial.println("live speaker");
      break;

    case '3':
      // about
      Serial.println("about");
      break;

    case '4':
      // stats
      Serial.println("stats");
      break;

    case '5':
      // who am i
      Serial.println("who am i");
      break;

    default:
      // main menu (or error)
      Serial.println("main menu (or error)");
  }
}

void saw_badge(String badge) {
  Serial.print("Saw badge: ");Serial.println(badge);
}

void saw_badges(String badge1, String badge2) {
  Serial.print("Saw badge ");Serial.print(badge1);
  Serial.print(" and ");Serial.println(badge2);
}
