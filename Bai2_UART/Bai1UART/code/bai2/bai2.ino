void setup() {
 
  Serial.begin(9600);
  
  
  while (!Serial) {
    ;
  }
  
  
  delay(1000);
}

void loop() {
  
  Serial.println("Hello Bao dz!");
  
  
  delay(2000);
}
