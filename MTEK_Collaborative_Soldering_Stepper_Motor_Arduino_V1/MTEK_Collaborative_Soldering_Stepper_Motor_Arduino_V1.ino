


/*
   BYJ48 Stepper motor code
   Connect :
   IN1 >> D8
   IN2 >> D9
   IN3 >> D10
   IN4 >> D11
   VCC ... 5V Prefer to use external 5V Source
   Gnd
   written By :Mohannad Rawashdeh
     28/9/2013

     Rev Mikael Glans
     28/1/2019
  */

#define IN1  8
#define IN2  9
#define IN3  10
#define IN4  11
int Steps = 0;
boolean Direction = true;
unsigned long last_time;
unsigned long currentMillis ;
int steps_left=800;
long time;

//Interrupt
int sensePin = 2;
int ledPin= 13;
volatile int value = 0;
boolean start = false;



// Install the interrupt routine.
ISR(INT0_vect) {
  
  // check the value again - since it takes some time to
  // activate the interrupt routine, we get a clear signal.
  
  value = digitalRead(sensePin);
delay(100);
  
 
  if (value) {
   EIMSK = (0<<INT0);
  Serial.println("Value high!");
    digitalWrite(ledPin, HIGH);
start=true;
Direction = true;
steps_left=800;

 EIMSK = (1<<INT0);
 
 } else if(!value) {
  EIMSK = (0<<INT0);
 Serial.println("Value low!");
  digitalWrite(ledPin, LOW);
   start=true;
    Direction = false;
 steps_left=800;
    
    EIMSK = (1<<INT0);
  }

  



  
  
}


void setup()
{
Serial.begin(115200);
pinMode(IN1, OUTPUT); 
pinMode(IN2, OUTPUT); 
pinMode(IN3, OUTPUT); 
pinMode(IN4, OUTPUT); 

// sets the digital pin as output
  pinMode(ledPin, OUTPUT);
  // read from the sense pin
  pinMode(sensePin, INPUT);

SREG = 0x80; //bit 7 - I: Global Interrupt Enable set
EICRA = (0<<ISC01)|(1<<ISC00);//The rising edge of INT0 generates an interrupt request.
EIMSK = (1<<INT0);
}
void loop()
{
 
  if(start&&steps_left>0){
     while(steps_left>0){
      currentMillis = micros();
      if(currentMillis-last_time>=1000){ //800ms minimum
      stepper(1); 
      time=time+micros()-last_time;
      last_time=micros();
      steps_left--;
      }
      }
       Serial.println(time);
      Serial.println("Wait...!");
      EIMSK = (0<<INT0);
    delay(1000);
    EIMSK = (1<<INT0);
      
  }


  
  
}

void stepper(int xw){
  for (int x=0;x<xw;x++){
switch(Steps){
   case 0:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, HIGH);
   break; 
   case 1:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, HIGH);
   break; 
   case 2:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, LOW);
   break; 
   case 3:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, LOW);
   break; 
   case 4:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
   case 5:
     digitalWrite(IN1, HIGH); 
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
     case 6:
     digitalWrite(IN1, HIGH); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
   case 7:
     digitalWrite(IN1, HIGH); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, HIGH);
   break; 
   default:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
}
SetDirection();
}
} 
void SetDirection(){
if(Direction==true){ Steps++;}
if(Direction==false){ Steps--; }
if(Steps>7){Steps=0;}
if(Steps<0){Steps=7; }
}

void changeDirection(){
  Direction=!Direction;
}
