
/////////////////////////////////////////////////////////////////
// Controlamos las temperaturas para 
// activar o desactivar calentador y ventiladores
/////////////////////////////////////////////////////////////////
void check_temp(){

  contador_temp ++;        // LEE LOS VALORES DE LAS TEMPERATURAS
  contador_med_temp ++;    // para leer solo la temperatura cada 10 loops

  if (contador_med_temp == 5)
  {
  sensors.requestTemperatures();   // call sensors.requestTemperatures() to issue a global 
  // temperature request to all devices on the bus
  temperatura_agua_temp += (sensors.getTempC(sensor_agua));  
  temperatura_habitacion_temp += (sensors. getTempC(sensor_habitacion));       // lee temperatura habitacion.
  temperatura_disipador_temp += (sensors.getTempC(sensor_disipador));   //read heatsink temperature

  Serial.print("temperatura instantánea: "); Serial.print(sensors.getTempC(sensor_agua)); Serial.print(" temp. acumulada: "); Serial.println(temperatura_agua_temp);
  contador_med_temp = 0;
  } 
  if(contador_temp == 50)
  {
    temp_agua = temperatura_agua_temp / 10; Serial.print("temperatura media: "); Serial.println(temp_agua);
    tempHB = temperatura_habitacion_temp / 10;
    tempD = temperatura_disipador_temp / 10; 
    
    contador_temp = 0;  
    temperatura_agua_temp = 0;    
    temperatura_habitacion_temp = 0;
    temperatura_disipador_temp = 0; 
  }
                                         // ACTUA EN FUNCION DE LOS VALORES DE LA TEMPERATURA
  
    if (temp_agua != -127 && temp_agua != 85){
     
      if (temp_agua <= temp_agua_des-temperatura_margen){
        SetRele(calentador,HIGH); // Encendemos Calentador
        led2.on();
   //     Serial.print("Enciende calentador: "); Serial.print(temp_agua); Serial.print(" "); Serial.println(temp_agua_des-temperatura_margen); 
       }
       
      if (temp_agua >= temp_agua_des+temperatura_margen){
        SetRele(calentador,LOW); // Apagamos Calentador
        led2.off();
    //    Serial.print("Apaga calentador");Serial.print(temp_agua); Serial.print(" "); Serial.println(temp_agua_des+temperatura_margen);
      }
            
    }
    else
    {
      SetRele(calentador,LOW); // Apagamos Calentador
      led2.off();
    }
    
} 

/////////////////////////////////////////////////////////////////
//Fin controlTemperatura
/////////////////////////////////////////////////////////////////

