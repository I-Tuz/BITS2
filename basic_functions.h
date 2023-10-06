#ifndef BASICFUNCTIONS_H
#define BASICFUNCTIONS_H

void buzzer_signal(int pitch, int count, int delay_time){
    for (int i = 0; i < count; i++){
        tone(BUZZER_PIN, pitch);
        delay(delay_time);    
        noTone(BUZZER_PIN);
        delay(delay_time);
    }
}

void all_led_blink(int count, int delay_time){
    for (int k=0; k<count; k++){
        front_strip.set_pixel_color(thk_LedStripController::Color(red, green, blue));
        front_strip.show();
        back_strip.set_pixel_color(thk_LedStripController::Color(red, green, blue));
        back_strip.show();
        left_strip.set_pixel_color(thk_LedStripController::Color(red, green, blue));
        left_strip.show();
        right_strip.set_pixel_color(thk_LedStripController::Color(red, green, blue));
        right_strip.show();
        delay(delay_time);
        front_strip.set_pixel_color(thk_LedStripController::Color(0, 0, 0));
        front_strip.show();
        back_strip.set_pixel_color(thk_LedStripController::Color(0, 0, 0));
        back_strip.show();
        left_strip.set_pixel_color(thk_LedStripController::Color(0, 0, 0));
        left_strip.show();
        right_strip.set_pixel_color(thk_LedStripController::Color(0, 0, 0));
        right_strip.show();
        delay(delay_time);
    }
}

int us_get_distance(int TRIG, int ECHO)
{
  digitalWrite(TRIG, LOW);
  delay(5);
  digitalWrite(TRIG, HIGH);
  delay(10);
  digitalWrite(TRIG, LOW);
  dauer = pulseIn(ECHO, HIGH);
  entfernung = (dauer / 2) / 29.1;

  return entfernung;
}

int get_front_tof_distance(){
  int distance;
  #ifdef TOF_VL53L0X
  distance = tof_controller.get_distance_cm(0);
  #elif defined(TOF_TFLUNA)
  if( tflI2C.getData( tfDist, tfAddr)) // If read okay...
    {
        distance = tfDist;
    }
    else tflI2C.printStatus(); 
  #endif
  return distance;
}

void make_room(){
  ServoSteer.turn(STEER_START_POS);
  ServoSearch.turn(SEARCH_START_POS);

  US_distance_b = us_get_distance(TRIGB, ECHOB);
  US_distance_f = us_get_distance(TRIGF, ECHOF);
  tof_distance_front = get_front_tof_distance();
  weighted_distance = (US_distance_f+tof_distance_front)/2;

  while ((weighted_distance <= MIN_DIST_F) && (US_distance_b > MIN_DIST_B)){
    US_distance_b = us_get_distance(TRIGB, ECHOB);
    US_distance_f = us_get_distance(TRIGF, ECHOF);
    tof_distance_front = get_front_tof_distance();
    weighted_distance = (US_distance_f+tof_distance_front)/2;
    Motor.drive_backward(velocity_backward);
  }

  Motor.stop();
}

void drive_forward_controlled_velocity(){ 
    US_distance_f = us_get_distance(TRIGF, ECHOF);
    tof_distance_front = get_front_tof_distance();
    weighted_distance = (US_distance_f+tof_distance_front)/2;
    if ((millis()-start)>500){
        if (int(weighted_distance) > MIN_DIST_F){
            velocity = map(weighted_distance, 10, 100, 80, 255);
            Motor.drive_forward(velocity);
        }
        else{
            Motor.stop();
        }
        start = millis();
    }
}

void set_led_color(byte r, byte g, byte b){
  red = r;
  green = g;
  blue = b;
}

void end_of_programm(){
  Motor.stop();
  ServoSteer.end();
  ServoSearch.end();
  buzzer_signal(400,1,100);
  buzzer_signal(700,1,100);
  while (1){
      // End of Programm
  }
}

#endif