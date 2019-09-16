int state = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  pinMode(OUTPUT, 7);
}

void loop() {
  // put your main code here, to run repeatedly:
  btTestRoutine();
}
void btTestRoutine(){
   if(Serial.available() > 0){
      state = Serial.read();
   }
   if(state == '0'){
      Serial.println("State 0");
   }else{
      Serial.println("State 1");
      state = 0;
   }
}
