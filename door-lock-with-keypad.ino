#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// Definisi Pin Komponen
#define buzzer 12

// Inisialisasi Objek
Servo servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variabel Sistem
String password = "123456";      // Password default (6 digit) [cite: 133]
String enteredPassword = "";     // Menyimpan input user [cite: 134]
byte cursorPosition = 5;         // Posisi kursor LCD untuk input [cite: 135]
bool doorState = false;          // false = Terkunci, true = Terbuka [cite: 136]

// Konfigurasi Keypad [cite: 137, 166]
const byte ROWS = 4; 
const byte COLS = 4; 
char keymap[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};
Keypad customKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  servo.attach(11); [cite: 171]
  servo.write(50);   // Posisi awal servo [cite: 172]
  
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("WELCOME TO");
  lcd.setCursor(0, 1);
  lcd.print("DOOR LOCK SYSTEM");
  delay(3000);
  lcd.clear(); [cite: 180]
}

void loop() {
  lcd.setCursor(1, 0);
  lcd.print("ENTER PASSWORD"); [cite: 184]

  char key = customKeypad.getKey(); [cite: 187]

  if (key != NO_KEY) {
    delay(60);
    if (key == '*') {               // Reset Input [cite: 190]
      resetPassword();
    } else if (key == 'D') {        // Eksekusi Buka/Tutup [cite: 192]
      doorOperation();
    } else if (key == 'B') {        // Hapus Karakter Terakhir [cite: 194]
      deleteLastCharacter();
    } else if (key == 'A') {        // Ganti Password [cite: 196]
      changePassword();
    } else {                        // Input Angka [cite: 198]
      processNumberKey(key);
    }
  }
}

// Efek Suara Buzzer Pendek [cite: 204, 208]
void sfx() {
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
}

void processNumberKey(char key) {
  if (cursorPosition <= 11) {       // Batas input 6 digit [cite: 215]
    lcd.setCursor(cursorPosition, 1);
    lcd.print("*");                 // Masking input [cite: 211]
    sfx();
    enteredPassword += key;
    cursorPosition++;
  }
}

void deleteLastCharacter() {
  if (cursorPosition > 5) {
    cursorPosition--;
    lcd.setCursor(cursorPosition, 1);
    lcd.print(" ");
    sfx();
    enteredPassword.remove(enteredPassword.length() - 1); [cite: 226]
  }
}

void doorOperation() {
  if (doorState == false) {
    doorOpen(); // Fungsi verifikasi untuk membuka [cite: 231, 234]
  } else {
    doorLocked(); // Fungsi verifikasi untuk mengunci kembali [cite: 232, 245]
  }
}

void doorOpen() {
  if (enteredPassword == password) {
    operateDoor(140, "OPEN THE DOOR..."); // Sudut buka [cite: 239]
    doorState = true;
  } else {
    invalidPassword(); [cite: 242]
  }
}

void doorLocked() {
  if (enteredPassword == password) {
    operateDoor(50, "DOOR LOCKED");        // Sudut kunci [cite: 247]
    doorState = false;
  } else {
    invalidPassword(); [cite: 250]
  }
}

void operateDoor(int servoPosition, const char *message) {
  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
  servo.write(servoPosition); [cite: 257]
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CORRECT PASSWORD");
  lcd.setCursor(2, 1);
  lcd.print(message);
  delay(2000);
  lcd.clear();
  resetPassword(); [cite: 265]
}

void invalidPassword() {
  for (int i = 0; i < 3; i++) {    // Bunyi buzzer 3x jika salah [cite: 271, 280]
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
  }
  lcd.setCursor(0, 0);
  lcd.print("WRONG PASSWORD!");
  lcd.setCursor(0, 1);
  lcd.print("PLEASE TRY AGAIN");
  delay(2000);
  lcd.clear();
  resetPassword(); [cite: 287]
}

void resetPassword() {
  enteredPassword = "";
  cursorPosition = 5;
  lcd.clear(); [cite: 291]
}

void verify() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Old PW: ");
  String temp = "";
  while (temp.length() < password.length()) {
    char key = customKeypad.getKey();
    if (key != NO_KEY && key != '#') {
      lcd.print('*');
      sfx();
      temp += key;
    }
  }
  lcd.clear();
  lcd.print("Verifying...");
  delay(1000);
  if (temp == password) {
    lcd.clear();
    lcd.print("Verif Success");
    delay(1000);
  } else {
    lcd.clear();
    lcd.print("Verif Failed");
    delay(1000);
    resetPassword();
  }
}

void changePassword() {
  verify(); // Verifikasi password lama dulu [cite: 328]
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("New PW: ");
  String newPassword = "";
  while (newPassword.length() < 6) {
    char key = customKeypad.getKey();
    if (key != NO_KEY && key != '#') {
      lcd.print('*');
      sfx();
      newPassword += key;
    }
  }
  password = newPassword; [cite: 345]
  lcd.clear();
  lcd.print("Password Changed");
  delay(1000);
  lcd.clear();
}
