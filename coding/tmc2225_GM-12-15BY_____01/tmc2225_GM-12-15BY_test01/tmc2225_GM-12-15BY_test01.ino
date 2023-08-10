int en_1 = 2;//LOW_ON HIGH_OFF
int step_1 = 3;//HIGH > LOW:1 step
int dir_1 = 4;//change direction
int step_num_1;//value 400 equals 1 loop


int en_2 = 5;//LOW_ON HIGH_OFF
int step_2 = 6;//HIGH > LOW:1 step
int dir_2 = 7;//change direction
int step_num_2;//value 400 equals 1 loop


int en_3 = 8;//LOW_ON HIGH_OFF
int step_3 = 9;//HIGH > LOW:1 step
int dir_3 = 10;//change direction
int step_num_3;//value 400 equals 1 loop


void setup() {
//  pinMode(en_1,OUTPUT);
//  pinMode(step_1,OUTPUT);
//  pinMode(dir_1,OUTPUT);
//  digitalWrite(en_1,LOW);
//  digitalWrite(step_1,LOW);
//  digitalWrite(dir_1,LOW);

  pinMode(en_2,OUTPUT);
  pinMode(step_2,OUTPUT);
  pinMode(dir_2,OUTPUT);
  digitalWrite(en_2,LOW);
  digitalWrite(step_2,LOW);
  digitalWrite(dir_2,LOW);

//  pinMode(en_3,OUTPUT);
//  pinMode(step_3,OUTPUT);
//  pinMode(dir_3,OUTPUT);
//  digitalWrite(en_3,LOW);
//  digitalWrite(step_3,LOW);
//  digitalWrite(dir_3,HIGH);

  delay(1000);
   
//  tone(step_1,700);
//  tone(step_2,700);
//  tone(step_3,700);
}

void loop() {
//  for(step_num_1 = 400;step_num_1>0;step_num_1--){
//    digitalWrite(step_1,HIGH);
//    delay(20);    
//    digitalWrite(step_1,LOW);
//    delay(20);  
//  }   
//  
//  digitalWrite(dir_1,HIGH);
//
//  for(step_num_1 = 400;step_num_1>0;step_num_1--){
//    digitalWrite(step_1,HIGH);
//    delay(20);    
//    digitalWrite(step_1,LOW);
//    delay(20);
//  }
//
//  digitalWrite(dir_1,LOW);
    
  for(step_num_2 = 400;step_num_2>0;step_num_2--){
    digitalWrite(step_2,HIGH);
    delay(20);    
    digitalWrite(step_2,LOW);
    delay(20);  
  }   
  
  digitalWrite(dir_2,HIGH);

  for(step_num_2 = 400;step_num_2>0;step_num_2--){
    digitalWrite(step_2,HIGH);
    delay(20);    
    digitalWrite(step_2,LOW);
    delay(20);
  }

  digitalWrite(dir_2,LOW);

//  for(step_num_3 = 400;step_num_3>0;step_num_3--){
//    digitalWrite(step_3,HIGH);
//    delay(20);    
//    digitalWrite(step_3,LOW);
//    delay(20);  
//  }   
//  delay(1000); 
//  
//  digitalWrite(dir_3,HIGH);
//
//  for(step_num_3 = 400;step_num_3>0;step_num_3--){
//    digitalWrite(step_3,HIGH);
//    delay(20);    
//    digitalWrite(step_3,LOW);
//    delay(20);
//  }
//  delay(1000);
//
//  digitalWrite(dir_3,LOW);
  
//  tone(step_1,600);
//  tone(step_2,600);
//  tone(step_3,600);

}
