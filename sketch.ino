#define IN1 9
#define IN2 10
#define IN3 11
#define IN4 12

const int LDR_PIN = A0;          
const int min_val = 330;       
const int trials = 120;      

const int open = 0;     
const int close = -100;

int step_number = 0;             
int current_position = 0;        
int trial_count = 0;

unsigned long start_time = 0;
unsigned long stop_time = 0;

void moveToPosition(int target);
void waitForBall(unsigned long timeout_us);

// Half-step sequence for stepper motor
const int seq[8][4] = {
  {1,0,0,0},
  {1,1,0,0},
  {0,1,0,0},
  {0,1,1,0},
  {0,0,1,0},
  {0,0,1,1},
  {0,0,0,1},
  {1,0,0,1}
};

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(115200);

  // Move to open position first
  current_position = open;
  moveToPosition(open);

  delay(1000);

  Serial.println("Trial | Time (µs)");
}

void loop() {
  if (trial_count >= trials) {
    Serial.println("DONE");
    while (1) delay(1000);
  }

  // Close gate and wait
  moveToPosition(close);
  delay(1000);

  // Start trial
  trial_count++;
  Serial.print(trial_count);
  Serial.print(" | ");

  start_time = micros();
  stop_time = 0;

  // Open gate
  moveToPosition(open);

  // Wait 10 seconds
  waitForBall(10UL * 1000000UL); // 10 seconds in microseconds

  // Record results
  if (stop_time > 0) {
    unsigned long elapsed_time = stop_time - start_time;
    Serial.println(elapsed_time);
  } else {
    Serial.println("N/A");
  }

  // Close gate and wait before next trial
  moveToPosition(close);
  delay(5000);
}

// ---- Motor control ----
void moveToPosition(int target) {
  int steps_needed = target - current_position;
  int direction = (steps_needed > 0) ? 1 : -1;

  for (int i = 0; i < abs(steps_needed); i++) {
    step_number += direction;
    if (step_number > 7) step_number = 0;
    if (step_number < 0) step_number = 7;

    digitalWrite(IN1, seq[step_number][0]);
    digitalWrite(IN2, seq[step_number][1]);
    digitalWrite(IN3, seq[step_number][2]);
    digitalWrite(IN4, seq[step_number][3]);

    delay(3);
    current_position += direction;
  }
}

void waitForBall(unsigned long timeout_us) {
  unsigned long t0 = micros();
  while ((micros() - t0) < timeout_us) {
    int ldr_value = analogRead(LDR_PIN);
    if (ldr_value < min_val) {
      stop_time = micros();
      return; // exit early if ball detected
    }
  }
  // If we reach here, timeout occurred
  stop_time = 0;
}
