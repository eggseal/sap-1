#define LDA 22
#define ADD 23
#define SUB 24
#define OUT 25
#define HLT 26
#define MUL 29

#define CLR 12
#define CLK 13

#define CP 53
#define EP 52
#define LM 51
#define ER 50
#define LI 37
#define EI 36
#define LA 35
#define EA 34
#define SU 33
#define EU 32
#define LB 31
#define LO 30

int BUS_INPUT[8] = { 49, 48, 47, 46, 45, 44, 43, 42 };
int BUS_OUTPUT[4] = { 6, 7, 8, 9 };
int count = 0;

void setup() {
  pinMode(LDA, INPUT);
  pinMode(ADD, INPUT);
  pinMode(SUB, INPUT);
  pinMode(MUL, INPUT);
  pinMode(OUT, INPUT);
  pinMode(HLT, INPUT);

  pinMode(CLK, OUTPUT);
  pinMode(CLR, OUTPUT);
  digitalWrite(CLK, LOW);
  digitalWrite(CLR, LOW);

  pinMode(CP, OUTPUT);
  pinMode(EP, OUTPUT);
  pinMode(LM, OUTPUT);
  pinMode(ER, OUTPUT);
  pinMode(LI, OUTPUT);
  pinMode(EI, OUTPUT);
  pinMode(LA, OUTPUT);
  pinMode(EA, OUTPUT);
  pinMode(SU, OUTPUT);
  pinMode(EU, OUTPUT);
  pinMode(LB, OUTPUT);
  pinMode(LO, OUTPUT);
  turnOffAction();

  for (int i = 0; i < 8; i++) {
    pinMode(BUS_INPUT[i], INPUT);
  }
  
  for (int i  = 0; i < 4; i++) {
    pinMode(BUS_OUTPUT[i], OUTPUT);
    digitalWrite(BUS_OUTPUT[i], LOW);    
  }
}

bool HALTED = false;
void loop() {
  if (HALTED) return;
  
  //  GET NEXT INSTRUCTION
  digitalWrite(EP, HIGH);
  digitalWrite(LM, HIGH);
  clock(2);
  digitalWrite(EP, LOW);
  digitalWrite(LM, LOW);
  digitalWrite(ER, HIGH);
  digitalWrite(LI, HIGH);  
  clock(2);
  digitalWrite(ER, LOW);
  digitalWrite(LI, LOW);

  //  EXECUTE NEXT INSTRUCTION
  if (digitalRead(LDA) == HIGH) instructionLDA();
  else if (digitalRead(ADD) == HIGH) instructionADD();
  else if (digitalRead(SUB) == HIGH) instructionSUB();
  else if (digitalRead(MUL) == HIGH) instructionMUL();
  else if (digitalRead(OUT) == HIGH) instructionOUT();
  else if (digitalRead(HLT) == HIGH) instructionHLT();
  else return;

  if (HALTED) return;

  turnOffAction();
  incrementPC();
}

void incrementPC() {
  count++;
  if (count >= 16) count = 0;

  for (int i = 0; i < 4; i++) {
    int bit = bitRead(count, i);
    digitalWrite(BUS_OUTPUT[i], bit);
  }  
}

void instructionLDA() {
  digitalWrite(EI, HIGH);
  digitalWrite(LM, HIGH);
  clock(2);
  digitalWrite(EI, LOW);
  digitalWrite(LM, LOW);
  digitalWrite(ER, HIGH);
  digitalWrite(LA, HIGH);
  clock(2);
}

void instructionADD() {
  digitalWrite(EI, HIGH);
  digitalWrite(LM, HIGH);
  clock(2);
  digitalWrite(EI, LOW);
  digitalWrite(LM, LOW);
  digitalWrite(ER, HIGH);
  digitalWrite(LB, HIGH);
  clock(2);
  digitalWrite(ER, LOW);
  digitalWrite(LB, LOW);
  digitalWrite(EU, HIGH);
  digitalWrite(LA, HIGH);
  clock(2);
}

void instructionSUB() {
  digitalWrite(SU, HIGH);
  instructionADD();  
}

void instructionMUL() {
  digitalWrite(EI, HIGH);
  digitalWrite(LM, HIGH);
  clock(2);
  digitalWrite(EI, LOW);
  digitalWrite(LM, LOW);
  int multAmount = readModule(ER);

  clock(2);
  digitalWrite(EA, HIGH);
  digitalWrite(LB, HIGH);
  clock(2);
  digitalWrite(EA, LOW);
  digitalWrite(LB, LOW);    
  digitalWrite(EU, HIGH);
  digitalWrite(LA, HIGH);
  digitalWrite(SU, LOW);

  for (int i = 0; i < multAmount; i++) {
    clock(2);
  }

  digitalWrite(EU, LOW);
  digitalWrite(LA, LOW);
}

void instructionOUT() {
  digitalWrite(EA, HIGH);
  digitalWrite(LO, HIGH);
  clock(2);
}

void instructionHLT() {
  HALTED = true;
}

void clock(int length) {
  digitalWrite(CLK, HIGH);
  delay(length);
  digitalWrite(CLK, LOW);
  delay(length);
}

int readModule(int moduleEnablePin) {
  int outputNum = 0;
  turnOffAction();
  digitalWrite(moduleEnablePin, HIGH);
  clock(2);
    for (int i = 0; i < 8; i++) {
    int bit = digitalRead(BUS_INPUT[i]);
    outputNum += bit * pow(2, i);      
  }  

  digitalWrite(moduleEnablePin, LOW);
  clock(2);
  
  return outputNum;
}

void turnOffAction() {
  digitalWrite(CP, LOW);
  digitalWrite(EP, LOW);
  digitalWrite(LM, LOW);
  digitalWrite(ER, LOW);
  digitalWrite(LI, LOW);
  digitalWrite(EI, LOW);
  digitalWrite(LA, LOW);
  digitalWrite(EA, LOW);
  digitalWrite(SU, LOW);
  digitalWrite(EU, LOW);
  digitalWrite(LB, LOW);
  digitalWrite(LO, LOW);
}