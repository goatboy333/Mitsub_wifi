
const int buttonPin = D1; // temp push button
int irPin = D2;  // Pin esp3288 D1 mini.
// unsigned long signal = 0b000011010101111100101010; // "ON" code for NEC protocol
//long signal [5]= {0b00111011001011001001101101111111,0b11111111111111111110111111001111,
//0b11111100110011011111111111111111,0b11111111111111111111111111111111,0b1111111101010011};

long signal [5]= {0b00111011001011001001101101111111,0b11111111111110111110111101001111,
0b11111100110011011111111111111111,0b11111111111111111111111111111111,0b1111111110010101};

int bitTiming = 435; // length of bit 562 microsecond for NEC Protocol

int buttonState = 0;       // current state of the button
int lastButtonState = 0;

void setup() {
  pinMode(irPin, OUTPUT); 
  digitalWrite(irPin, LOW);
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
  digitalWrite(irPin,HIGH);
  delayMicroseconds(9);
  digitalWrite(irPin,LOW);
  delayMicroseconds(9);
  }

 
}

void sendBits() {

  for (int i = 0; i < 4; i++){
    for (int j = 31; j > -1; j--){
    
      Serial.print(getBit(signal[1,i],j));
    
      if (getBit(signal[1,i],j) == 1){
       sendPulses(bitTiming); // just used as a reference in serial to see if bits match the signal.
        delayMicroseconds(bitTiming); 
      }
   
      else {
        sendPulses(bitTiming);
        delayMicroseconds(bitTiming * 3);
      }
     
    }
  } 

  for (int j = 15; j > -1; j--){
    
      Serial.print(getBit(signal[1,4],j));
    
      if (getBit(signal[1,4],j) == 1){
       sendPulses(bitTiming); // just used as a reference in serial to see if bits match the signal.
        delayMicroseconds(bitTiming); 
      }
   
      else {
        sendPulses(bitTiming);
        delayMicroseconds(bitTiming * 3);
      }
     
    }



  
}




/*
void sendBits() {

  for (int i = 31; i > -1; i--){
    
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
   
} */



void buttonCheck(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  if (buttonState != lastButtonState) {
  if (buttonState == HIGH) {
    sendPulses(3450);  // send header
    delayMicroseconds(1700); // send header space
    sendBits();   // send bits
    sendPulses(bitTiming); // send one pulse to signify end of tranmission
    delay(15.5); // extra debounce for button
    sendPulses(3500);  // send header
    delayMicroseconds(1700); // send header space
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
