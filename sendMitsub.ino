
const int buttonPin = 7;
int ledPin = 10;
unsigned long signal = 0b000011010101111100101010; // "ON" code for NEC protocol
int bitTiming = 500; // length of bit 562 microsecond for NEC Protocol

int buttonState = 0;       // current state of the button
int lastButtonState = 0;

void setup() {
  pinMode(ledPin, OUTPUT); 
  digitalWrite(ledPin, LOW);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

bool getBit(unsigned long byte, int position){ // return the bit in that position
  return (byte >> position) & 0x1;
}

void sendPulses(int msPulses) {
  
/* This code should replace the send carrier as i thought it needed to start low,
 * but only high burst is necessary. to work out carrier frequecy i took the reciprical and divided
 * it in 2. 1 / 38000 = 26 uS / 2 = 13 uS per cycle. The digital write function delays the signal, so its
 * reduced to 9 uS.  
 */

  for (int p = 0; p < (msPulses / 26); p++) {
  digitalWrite(ledPin,HIGH);
  delayMicroseconds(9);
  digitalWrite(ledPin,LOW);
  delayMicroseconds(9);
  }

 
}

void sendBits() {

  for (int i = 23; i > -1; i--){
    
    Serial.print(getBit(signal,i));
    
    if (getBit(signal,i) == 1){
      sendPulses(bitTiming); // just used as a reference in serial to see if bits match the signal.
      delayMicroseconds(bitTiming * 2); 
      }
   
    else {
      sendPulses(bitTiming);
      delayMicroseconds(bitTiming * 4);
      }
     
    }
   
}

void buttonCheck(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  if (buttonState != lastButtonState) {
  if (buttonState == HIGH) {
    sendPulses(4000);  // send header
    delayMicroseconds(4000); // send header space
    sendBits();   // send bits
    sendPulses(bitTiming); // send one pulse to signify end of tranmission
    delay(8); // extra debounce for button
    sendPulses(4000);  // send header
    delayMicroseconds(4000); // send header space
    sendBits();   // send bits
    sendPulses(bitTiming); // send one pulse to signify end of tranmission
    
  }
    
  else {
    
  }
  }
 
  lastButtonState = buttonState;
}

void loop() {
  buttonCheck();
}
