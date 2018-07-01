void check_ventilador(){
  if (temp_agua>=temp_agua_des+4)
  { 
    SetRele(ventilador,HIGH);
    led3.on();
  }
  else
  {
    SetRele(ventilador, LOW);
    led3.off();
  }
}

