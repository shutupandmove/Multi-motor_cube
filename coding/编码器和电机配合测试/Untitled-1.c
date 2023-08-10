
#define outputA 6
#define outputB 7

int counter = 0;
int aState;
int aLastState;

void setup(){
    pinMode(outputA,INPUT);
    pinMode(outputB,INPUT);

    aLastState = digitalRead(outputA);
}

void loop(){
    aState = digitalRead(outputA);
    if (aState != aLastState){
        if (digitalRead(outputB != aState))
            counter++;
        else
            counter--;
    }
    aLastState = aState;
}