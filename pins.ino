void pin_mode(int pin, int mode) {
  pinMode(pin, mode);
}

void digital_write(int pin, int value) {
  digitalWrite(pin, value);
}

int digital_read(int pin) {
  return digitalRead(pin);
}

