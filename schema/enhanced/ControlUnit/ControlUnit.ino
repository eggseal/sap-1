//  PINS
#define LDA 30 //  LOAD ACCUMULATOR
#define ADD 31 //  LOAD REGISTER, ADD
#define SUB 32 //  LOAD REGISTER, SUBTRACT
#define MUL 33 //  LOAD REGISTER, MULTIPLY
#define GOT 35 //  EXECUTE INSTRUCTION FROM A POSITION
#define OUT 36 //  LOAD OUTPUT, DISPLAY
#define HLT 37 //  HALT EXECUTION

#define CP 22 //  COUNT
#define EP 23 //  ENABLE PC
#define LM 24 //  LOAD MAR
#define ER 25 //  ENABLE PROM
#define LI 26 //  LOAD INST REGISTER
#define EI 27 //  ENABLE INST REGISTER
#define LA 28 //  LOAD ACCUMULATOR A
#define EA 29 //  ENABLE ACCUMULATOR A
#define AD 49 //  TOGGLE ALU ADDITION
#define SU 48 //  TOGGLE ALU SUBTRACTION
#define MU 47 //  TOGGLE ALU MULTIPLICATION
#define EU 46 //  ENABLE ALU
#define LB 45 //  LOAD B REGISTER
#define LO 44 //  LOAD OUTPUT REGISTER 

#define CLK 34

//  SETUP STATES
#define NEXT_INST_0 0 //  LOAD MAR FROM PC
#define NEXT_INST_C 102
#define NEXT_INST_1 1 //  LOAD IR FROM PROM
#define NEXT_INST_2 2 //  DISABLE IR AND PROM, NO CLOCK WAIT
#define EXECUTE_INS 3 //  READ INSTRUCTION AND CHANGE STATE ACCORDINGLY

#define CLOCK_AS_P0 100 //  WAIT UNTIL CLK IS HIGH TO CHANGE TO NEXT STATE
#define CLOCK_AS_P1 101 //  WAIT UNTIL CLK IS HIGH TO CHANGE TO NEXT STATE
unsigned int currState = NEXT_INST_0;
unsigned int nextState = NEXT_INST_0;

//  INSTRUCTION STATES
#define LDA_P0 5 //  LOAD MAR FROM IR
#define LDA_P1 6 //  LOAD ACC FROM PROM
#define LDA_P2 7 //  TURN OFF USED PINS
#define ADD_ST 8 //  LOAD MAR FROM IR
#define SUB_ST 9 //  LOAD B REGISTER FROM PROM
#define MUL_ST 10 //  LOAD ACCUMULATOR A FROM ALU
#define ALU_P0 11 //  LOAD MAR FROM IR
#define ALU_P1 12 //  LOAD B REGISTER FROM PROM
#define ALU_P2 13 //  LOAD ACCUMULATOR A FROM ALU
#define ALU_P3 14 //  TURN OFF USED PINS
#define GOT_P0 95 //  LOAD MAR FROM IR
#define GOT_P1 94 //  TURN OFF PINS, RUN NEXT_INST_P1
#define OUT_P0 97 //  LOAD OUT REGISTER FROM ACCUMULATOR A
#define OUT_P1 98 //  TURN OFF USED PINS
#define HLT_P0 99 //  STOP EVERYTHING

void setup() {
  pinMode(CP, OUTPUT);
  digitalWrite(CP, LOW);
  pinMode(EP, OUTPUT);
  digitalWrite(EP, LOW);
  pinMode(LM, OUTPUT);
  digitalWrite(LM, LOW);
  pinMode(ER, OUTPUT);
  digitalWrite(ER, LOW);
  pinMode(LI, OUTPUT);
  digitalWrite(LI, LOW);
  pinMode(EI, OUTPUT);
  digitalWrite(EI, LOW);
  pinMode(LA, OUTPUT);
  digitalWrite(LA, LOW);
  pinMode(EA, OUTPUT);
  digitalWrite(EA, LOW);
  pinMode(AD, OUTPUT);
  digitalWrite(AD, LOW);
  pinMode(SU, OUTPUT);
  digitalWrite(SU, LOW);
  pinMode(MU, OUTPUT);
  digitalWrite(MU, LOW);
  pinMode(EU, OUTPUT);
  digitalWrite(EU, LOW);
  pinMode(LB, OUTPUT);
  digitalWrite(LB, LOW);
  pinMode(LO, OUTPUT);
  digitalWrite(LO, LOW);

  pinMode(LDA, INPUT);
  pinMode(ADD, INPUT);
  pinMode(SUB, INPUT);
  pinMode(MUL, INPUT);
  pinMode(GOT, INPUT);
  pinMode(OUT, INPUT);
  pinMode(HLT, INPUT);
}

bool clockNew = false;
bool clockOld = false;

unsigned int operation = 0; //  [0] NOTHING. [1] ADDITION. [2] SUBTRACTION. [3] MULTIPLICATION.
bool HALTED, COUNT = LOW;

void loop() {
  if (HALTED) return;

  clockOld = clockNew;
  clockNew = digitalRead(CLK);
  
  switch(currState) {
    case NEXT_INST_0: {
      digitalWrite(LM, HIGH);
      digitalWrite(EP, HIGH);
      syncChangeState(NEXT_INST_C);
      break;
    }
    case NEXT_INST_C: {
      COUNT = HIGH;
    }
    case NEXT_INST_1: {
      digitalWrite(LM, LOW);
      digitalWrite(EP, LOW);
      digitalWrite(CP, COUNT);
      digitalWrite(ER, HIGH);
      digitalWrite(LI, HIGH);
      COUNT = LOW;
      syncChangeState(NEXT_INST_2);

      break;
    }
    case NEXT_INST_2: {
      digitalWrite(CP, LOW);
      digitalWrite(ER, LOW);
      digitalWrite(LI, LOW);
      currState = EXECUTE_INS;
      break;
    }
    case EXECUTE_INS: {
      if (digitalRead(LDA) == HIGH) currState = LDA_P0;
      else if (digitalRead(ADD) == HIGH) currState = ADD_ST;
      else if (digitalRead(SUB) == HIGH) currState = SUB_ST;
      else if (digitalRead(MUL) == HIGH) currState = MUL_ST;
      else if (digitalRead(GOT) == HIGH) currState = GOT_P0;
      else if (digitalRead(OUT) == HIGH) currState = OUT_P0;
      else if (digitalRead(HLT) == HIGH) currState = HLT_P0;
      break;
    }
    case LDA_P0: {
      digitalWrite(EI, HIGH);
      digitalWrite(LM, HIGH);
      syncChangeState(LDA_P1);
      break;
    }
    case LDA_P1: {
      digitalWrite(EI, LOW);
      digitalWrite(LM, LOW);
      digitalWrite(ER, HIGH);
      digitalWrite(LA, HIGH);
      syncChangeState(LDA_P2);
      break;
    }
    case LDA_P2: {
      digitalWrite(ER, LOW);
      digitalWrite(LA, LOW);
      currState = NEXT_INST_0;
      break;
    }
    case ADD_ST: {
      if (operation == 0) operation = 1;
    }
    case SUB_ST: {
      if (operation == 0) operation = 2;
    }
    case MUL_ST: {
      if (operation == 0) operation = 3;
    }
    case ALU_P0: {
      digitalWrite(EI, HIGH);
      digitalWrite(LM, HIGH);
      syncChangeState(ALU_P1);
      break;
    }
    case ALU_P1: {
      digitalWrite(EI, LOW);
      digitalWrite(LM, LOW);
      digitalWrite(ER, HIGH);
      digitalWrite(LB, HIGH);
      syncChangeState(ALU_P2);
      break;
    }
    case ALU_P2: {
      digitalWrite(ER, LOW);
      digitalWrite(LB, LOW);

      if (operation == 1) digitalWrite(AD, HIGH);
      else if (operation == 2) digitalWrite(SU, HIGH);
      else if (operation == 3) digitalWrite(MU, HIGH);
      operation = 0;

      digitalWrite(EU, HIGH);
      digitalWrite(LA, HIGH);
      syncChangeState(ALU_P3);
      break;
    }
    case ALU_P3: {
      digitalWrite(EU, LOW);
      digitalWrite(LA, LOW);

      digitalWrite(AD, LOW);
      digitalWrite(SU, LOW);
      digitalWrite(MU, LOW);
      currState = NEXT_INST_0;
      break;
    }
    case GOT_P0: {
      digitalWrite(EI, HIGH);
      digitalWrite(LM, HIGH);
      syncChangeState(GOT_P1);
      break;
    }
    case GOT_P1: {
      digitalWrite(EI, LOW);
      digitalWrite(LM, LOW);
      currState = NEXT_INST_1;
      break;
    }
    case OUT_P0: {
      digitalWrite(EA, HIGH);
      digitalWrite(LO, HIGH);
      syncChangeState(OUT_P1);
      break;
    }
    case OUT_P1: {
      digitalWrite(EA, LOW);
      digitalWrite(LO, LOW);
      currState = NEXT_INST_0;
      break;
    }
    case HLT_P0: {
      HALTED = true;
      syncChangeState(NEXT_INST_0);
      break;
    }
    case CLOCK_AS_P0: {
      if (!(clockOld == LOW && clockNew == HIGH)) break;
      currState = CLOCK_AS_P1;
      break;
    }
    case CLOCK_AS_P1: {
      if (!(clockOld == HIGH && clockNew == LOW)) break;
      currState = nextState;
      break;
    }
  }
}

void syncChangeState(int newState) {
  currState = CLOCK_AS_P0;
  nextState = newState;
}
