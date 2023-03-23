volatile int pos = 0;
volatile int encoder_a[2] = {0, 0};
volatile int encoder_b[2] = {0, 0};
//volatile bool encoder_a_error = false;
//volatile bool encoder_b_error = false;
int encoder_a_pin = 2;
int encoder_b_pin = 3;
int p_val = 0;
float angular_velocity = 0;//counts per ms
float angular_velocity_dps = 0;
int time[2] = {0, 0};

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
  Serial.print("                   ");
  Serial.println(angular_velocity);
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

  p_val = analogRead(A0);

  //***************************angular velocity calc***********************************

  // if the runtime of a interrupt function is larger than 1024 microseconds, use delayMicroseconds() to calc w
  //pos_his[1] = pos;
  //delayMicroseconds(10000);//10ms delay
  //pos_his[0] = pos;
  //angular_velocity = (pos_his[0]-pos_his[1])/10;

  pos_his[1] = pos_his[0];
  time[1] = time[0];
  pos_his[0] = pos;
  time[0] = millis();
  angular_velocity = (pos_his[0]-pos_his[1])/(time[0]-time[1]);


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
  if (encoder_b[1] == 0 && encoder_b[0] == 0) {  //can delete encoder_b[1]==0 after verification: pos will off by 1 count if deleting encoder_b[1]
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
  else if  (encoder_b[1] == 1 && encoder_b[0] == 1) {//Same here
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
  if (encoder_a[1] == 0 && encoder_a[0] == 0) {  //can delete encoder_a[1]==0 after verification: pos will off by 1 count if deleting encoder_b[1]
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
  else if  (encoder_a[1] == 1 && encoder_a[0] == 1) {//Same here
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