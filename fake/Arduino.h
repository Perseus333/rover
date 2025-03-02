// Arduino.h
// This is a fake file so that the compiler doesn't complain

struct string {
    char ch[16];
};

// Makes all functions strings because it accepts any input lol
typedef string string;

void pinMode(string pin, string output) {
    (void)pin;
    (void)output;
}

void digitalWrite(string pin, string input) {
    (void)pin;
    (void)input;
}

void analogRead(string pin, string arg) {
    (void)pin;
    (void)arg;
}

void delay(string time) {
    (void)time;
}