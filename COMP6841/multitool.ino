#include <HID.h>
#include <Keyboard.h>
#include <IRremote.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "multitool.h"
// #include "test.ino"
/*
Interface:
selection no. = array of modules
< = decrease selection no.
> = increase selection no.
manually add folders
*/

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial to initialise.
  }

  // Establish LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("LCD Established.");
  // Establish IR module
  IrReceiver.begin(7, 0);
  IrSender.begin(4, 0, 0);
}

void loop() {
  // for (int i = 0; i < MAX_MODULES; i++){
  //   Serial.print(i); // Yes, this is ugly. Unfortunately, it is also
  //   efficient, as print to serial does not accept format strings
  //   Serial.print(": ");
  //   Serial.println(mainMenu[i]);
  // }
  while (menu(MAX_MODULES, MAIN_MENU));
  Serial.print("User selected: ");
  Serial.println(mainMenu[input]);
  switch (input) {
  case IR_MODULE:
    IRModule();
    break;
  case RFID_MODULE:
    // setup_rfid();
    // start_rfid();
    break;
  case BAD_USB:
    BadUSB();
    break;
  default:
    Serial.println("An issue has occurred. Please go cry in a hole.");
  }
}
// BADUSB
void BadUSB() {
  while (1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(USBMenu[input]);
    while (menu(USB_OPTIONS, USB_MENU));
    switch (input) {
    case USB_SEND: // Malicious keyboard inputs go here
      Keyboard.begin();
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('r');
      delay(4000);
      Keyboard.releaseAll();
      delay(500);
      Keyboard.println("cmd");
      delay(500);
      Keyboard.println("echo I'm in your mainframe :)");
      Keyboard.end();
      break;
    case GO_BACK:
    default:
      return;
    }
  }
}

// IR
void IRModule() {
  // Serial.print("IR Module:\n");
  // for (int i = 0; i < IR_OPTIONS; i++){
  //   Serial.print(i); // Yes, this is ugly. Unfortunately, it is also
  //   efficient, as print to serial does not accept format strings
  //   Serial.print(": ");
  //   Serial.println(IRMenu[i]);
  // }
  while (1) { // Loop until return.
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(IRMenu[input]);
    while (menu(IR_OPTIONS, IR_MENU))
      ;
    switch (input) {
    case IR_RECORD:
      Serial.println("Recording..");
      IrReceiver.start();
      if (IrReceiver.available())
        storeCode(IrReceiver.read());
      IrReceiver.stop();
      IrReceiver.printIRResultRawFormatted(&Serial, true);
      break;
    case IR_SEND:
      Serial.println("Sending recorded value..");
      sendCode(&IRCode);
    case GO_BACK:
      return;
    default:
      break;
    }
  }
}

void storeCode(IRData *received) {
  Serial.print("IR Signal received.");
  IRCode.receivedIRData = *received;
  IRCode.rawCodeLength = IrReceiver.decodedIRData.rawDataPtr->rawlen - 1;
  IrReceiver.compensateAndStoreIRResultInArray(IRCode.rawCode);
  return;
}

void sendCode(IRCodeStruct *sending) {
  Serial.print("Sending stored IR signal.");
  IrSender.sendRaw(sending->rawCode, sending->rawCodeLength, 38);
  return;
}

// Menu function: Change input value until user is happy
// while (menu()); should return with the desired option in input,
bool menu(int options, int menunumber) {
  bool inMenu = true;
  // Serial.print("Currently selecting: ");
  // Serial.println(input);
  // if (Serial.available()>0){
  lcd.clear();
  lcd.setCursor(0, 0);
  switch (menunumber) { // Edit UI here.
  case MAIN_MENU:
    lcd.print(mainMenu[input]);
    break;
  case IR_MENU:
    lcd.print(IRMenu[input]);
    break;
  case USB_MENU:
    lcd.print(USBMenu[input]);
    break;
  }
  lcd.setCursor(0, 1);
  lcd.print("Sel:. Next:>");
  while (Serial.available() <= 0) {
  }
  int char_input = Serial.read();
  switch (char_input) {
  case '>':
    input++;
    break;
  case '.':
    inMenu = false;
  default:
    break;
  }
  input = input % options;
  input =
      abs(input); // Note: keep math outside of abs due to implementation bugs
  //}
  return inMenu;
}
