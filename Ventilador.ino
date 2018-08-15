void check_ventilador(){

  const int temp_min_vent = temp_agua_des + 1; Serial.println("Temp min vent: "+String(temp_min_vent));
  const int temp_max_vent = temp_agua_des + 4; Serial.println("Temp max vent: "+String(temp_max_vent));

  int tempval = int(temp_agua*10);
  pwm_vent = map(tempval, (temp_min_vent*10), (temp_max_vent*10),0,255);
  
  if (pwm_vent < 0) { pwm_vent =0; }
  if (pwm_vent > 255) { pwm_vent =255; }
  
  ledcWrite(vent_channel_0,pwm_vent);
   
  if (temp_agua > temp_min_vent)
  {
    Serial.println("Ventilador a: " + String (pwm_vent));
    led3.on();
  }
  else
  {
    led3.off();
  }
}

