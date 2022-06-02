int a0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int a0 = analogRead(A0);
  float battery = map(a0, 302, 420, 0, 100);
  if (battery >= 100){
    battery = 100;
  } else if (battery <= 0){
    battery = 1;
  }
  Serial.println(battery);
  delay(1000);
}
