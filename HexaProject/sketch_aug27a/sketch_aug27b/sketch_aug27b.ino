void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
for (int i=0;i<=2;i++){
  Serial.print(analogRead(i));
  Serial.print(",");
}
Serial.println();
}
