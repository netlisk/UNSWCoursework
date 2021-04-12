#define MAX_MODULES 3
#define IR_MODULE 0
#define RFID_MODULE 1
#define BAD_USB 2

#define IR_OPTIONS 3
#define USB_OPTIONS 2
#define MAX_MODULE_NAME_LENGTH 12

#define MAIN_MENU 0
#define IR_MENU 1
#define USB_MENU 2

#define GO_BACK 0
#define IR_RECORD 1
#define IR_SEND 2
#define IR_MAX_TIME 150

#define USB_SEND 1

// Global Variables
// ==============================================================================
int input = 0;
char mainMenu[MAX_MODULES][MAX_MODULE_NAME_LENGTH] = {"IR Mode", "RFID Mode", "BadUSB Mode"};
char IRMenu[IR_OPTIONS][MAX_MODULE_NAME_LENGTH] = {"Go Back", "Record", "Send"};
char USBMenu[USB_OPTIONS][MAX_MODULE_NAME_LENGTH] = {"Go Back", "Play"};

// IR
struct IRCodeStruct {
  IRData receivedIRData;
  uint8_t rawCode[RAW_BUFFER_LENGTH];
  uint8_t rawCodeLength;
} IRCode;
// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// BadUSB Setup
// Helper Functions
// ==============================================================================
// Arduino doesn't seem to care for function prototypes, but I've put them here
// anyways.
bool menu(int options, int menunumber);
void IRModule();
void sendCode(IRCodeStruct *sending);
void storeCode(IRData *received);
void BadUSB();