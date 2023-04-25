volatile int pos = 0;
volatile int encoder_a[2] = {0, 0};
volatile int encoder_b[2] = {0, 0};
//volatile bool encoder_a_error = false;
//volatile bool encoder_b_error = false;
int encoder_a_pin = 2;
int encoder_b_pin = 3;
int pot_val = 0;
float angular_velocity = 0;//counts per ms
int time[2] = {0, 0};

float error_his[2] = {0, 0};
float error = 0;
float error_sum = 0;
float d_error = 0;
float input = 0;
float ref = 0;

//control mode
bool pos_control = false;
bool speed_control = true;

//control parameters
float kp = 50;
float ki = 0;
float kd = 0;

int pos_his[2] = {0,0};

void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);

  attachInterrupt(digitalPinToInterrupt(encoder_a_pin), renew_data_a, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder_b_pin), renew_data_b, CHANGE);

  Serial.begin(9600);
  time[0] = millis();
  if (speed_controller == true) {
    digitalWrite(9,LOW);
  }
}

void loop() {
  // ***********************************DEBUGGING********************************************
  //Serial.print(encoder_a[0]);
  //Serial.print(encoder_a[1]);
  //Serial.print(encoder_a[2]);
  //Serial.print(encoder_a[3]);
  //Serial.print(";                  ");
  //Serial.print(encoder_b[0]);
  //Serial.print(encoder_b[1]);
  //Serial.print(encoder_b[2]);
  //Serial.print(encoder_b[3]);
  //Serial.print("                   ");
  Serial.print(pos);
  Serial.print("    ref");
  Serial.println(ref);
  Serial.print("    e");
  Serial.println(error);
  Serial.print("    p");
  Serial.println(pot_val);  
  //if(encoder_a_error == true) {
  //  Serial.println("Encoder A Counting Error");
  //  encoder_a_error = false;
  //}
  //if(encoder_b_error == true) {
  //  Serial.println("Encoder B Counting Error");
  //  encoder_b_error = false;
  //}

  //***********************************************************************************

  //************************************Control Code Here************************************

  pot_val = analogRead(A0);

  if (pos_control == true) {
    position_controller();
    speed_control = false;
  }

  if (speed_control == true) {
    speed_controller();
  }

  input = kp*error+ki*error_sum+kd*d_error;
  analogWrite(direction(input),constrain(int(round(abs(input)*255)),0,255));

  //***************************angular velocity calc***********************************

  // if the runtime of a interrupt function is larger than 1024/4.81 microseconds, use delayMicroseconds() to calc w
  //pos_his[1] = pos;
  //delayMicroseconds(10000);//10ms delay
  //pos_his[0] = pos;
  //angular_velocity = (pos_his[0]-pos_his[1])/10;

  pos_his[1] = pos_his[0];
  error_his[1] = error_his[0];
  time[1] = time[0];
  pos_his[0] = pos;
  error_his[0] = error;
  time[0] = millis();
  angular_velocity = float(pos_his[0]-pos_his[1])/(time[0]-time[1]);
  d_error = (error_his[0]-error_his[1])/(time[0]-time[1]);
  error_sum = error_sum + error;
  //time sensitive cal go above


}

void renew_data_a() {
  int a = digitalRead(encoder_a_pin);
  encoder_a[1] = encoder_a[0];
  encoder_a[0] = a;
  encoder_b[1] = encoder_b[0];
  calc_pos_a();
}
//run time 4 microseconds


void renew_data_b() {
  int b = digitalRead(encoder_b_pin);
  encoder_a[1] = encoder_a[0];

  encoder_b[1] = encoder_b[0];
  encoder_b[0] = b;
  calc_pos_b();
}
// run time 4 microseconds

void calc_pos_a() {
  if (encoder_b[0] == 0) {   
    if (encoder_a[0] > encoder_a[1]) {
      pos++;
    }
    else if (encoder_a[0] < encoder_a[1]) {
      pos--;
    }
//    else {
//      encoder_a_error = true;
//    }
  }
  else if  (encoder_b[0] == 1) {
    if (encoder_a[0] < encoder_a[1]) {
      pos++;
    }
    else if (encoder_a[0] > encoder_a[1]) {
      pos--;
    }
//    else {
//      encoder_a_error = true;
//    }
  }
//  else {
//    encoder_a_error = true;
//  }
}

void calc_pos_b() {
  if (encoder_a[0] == 0) {  
    if (encoder_b[0] < encoder_b[1]) {
      pos++;
    }
    else if (encoder_b[0] > encoder_b[1]) {
      pos--;
    }
//    else {
//      encoder_b_error = true;
//    }
  }
  else if  (encoder_a[0] == 1) {
    if (encoder_b[0] > encoder_b[1]) {
      pos++;
    }
    else if (encoder_b[0] < encoder_b[1]) {
      pos--;
    }
//    else {
//      encoder_b_error = true;
//    }
  }
//  else {
//    encoder_b_error = true;
//  }
}

float direction(float plant_input) {
  if (plant_input >= 0) {
    digitalWrite(9,LOW);
    return 10; 
  }
  else {
    digitalWrite(10,LOW);
    return 9;
    }
}

void position_controller () {
  ref = float(pot_val)/1023*8245.92;
  error = (ref - float(pos))/8245.92;
  input = kp*error+ki*error_sum+kd*d_error;
  analogWrite(direction(input),constrain(int(round(abs(input)*255)),0,255));
}

void speed_controller () {
  ref = float(pot_val)/1023*3.6;
  error = ref - angular_velocity;
  input = kp*error+ki*error_sum+kd*d_error;
  analogWrite(10,constrain(int(round(input*255)),0,255));
}
