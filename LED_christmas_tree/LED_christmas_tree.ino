//15 LED's in a triangle (half of a 5x5 matrix)
//     14
//    12 13
//   9 10 11
//  5  6  7 8
// 1  2  3 4 5

#define MATRIXFREQ 1000L
byte scanrow=0;
const byte apin[15]={2,3,4,5,6,3,4,5,6,4,5,6,5,6,6};
const byte kpin[15]={A5,A4,A3,A2,A1,A5,A4,A3,A2,A5,A4,A3,A5,A4,A5};
unsigned int treeimg=0;    //all off
unsigned int tree[]={
  16383,30719,32751,30719,16383,32255,32766,32255,16383,327676,
  32753,32287,31743,20479,32767,
  32753,32287,31743,20479,32767,
  32753,32287,31743,20479,32767,
};

void setup() {
  treeSetup();
}

void loop() {
  static int i=0;             //index counter for cycling through patterns
  i=(i+1)%(sizeof(tree)/2);   //work with however many elements in tree
  treeimg=tree[i];            //change output image => actually displaying is done by the ISR
  delay(300);                 //wait a bit
}

void treeSetup(){
  for(int i=0;i<15;i++){
    pinMode(apin[i],OUTPUT);
    pinMode(kpin[i],OUTPUT);
    digitalWrite(apin[i],LOW);    //anodes off
    digitalWrite(kpin[i],HIGH);   //cathodes off
  }
  // Timer 1 set up as a FREQ Hz sample interrupt, only common thing this affects is servo library
  TCCR1A = 0;
  TCCR1B = _BV(WGM12) | _BV(CS10);
  TCNT1 = 0;
  OCR1A = F_CPU / MATRIXFREQ;
  TIMSK1 = _BV(OCIE1A);
}

ISR(TIMER1_COMPA_vect) {    //gets triggered FREQ times/second (works on Leo and Uno)
  scanrow=(scanrow+1)%5;    //next row
  for(int i=0;i<15;i++){
    digitalWrite(apin[i],LOW);    //anodes off
    digitalWrite(kpin[i],HIGH);   //cathodes off
  }
  digitalWrite(apin[scanrow],HIGH);    //active anode on
  for(int i=0;i<15;i++){
    if(apin[i]==apin[scanrow]){         //if it's on the same anode
      if((1<<i)&treeimg){              //and LED is supposed to be lit
        digitalWrite(kpin[i],LOW);     //cathode on        
      }
    }
  }
}
