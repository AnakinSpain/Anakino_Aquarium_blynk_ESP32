
/////////////////////////////////////////////////////////////////
// Controlamos las temperaturas para 
// activar o desactivar calentador y ventiladores
/////////////////////////////////////////////////////////////////
void check_temp(){
    
  sensors.requestTemperatures();   // call sensors.requestTemperatures() to issue a global 
  // temperature request to all devices on the bus

 temp_agua_inst = (sensors.getTempC(sensor_agua));        // lee temp del agua
 temp_habit_inst = (sensors. getTempC(sensor_habitacion));// lee temp de la habitacion
 temp_disp_inst = (sensors.getTempC(sensor_disipador));   // lee temp del disipador

 if (temp_agua_inst != -127 && temp_habit_inst != -127 && temp_disp_inst != -127) 

 {
  contador_temp ++;                       // LEE LOS VALORES DE LAS TEMPERATURAS
  temperatura_agua_temp += temp_agua_inst;  
  temperatura_habitacion_temp += temp_habit_inst;      
  temperatura_disipador_temp += temp_disp_inst;   
  Serial.println("temperatura instantánea= " + String(temp_agua_inst) + " Temp. acumulada= "+ String(temperatura_agua_temp));
 }

 
  if(contador_temp == 10)
  {
    temp_agua = temperatura_agua_temp / 10; Serial.println("Temperatura media: "+ String(temp_agua));
    tempHB = temperatura_habitacion_temp / 10;
    tempD = temperatura_disipador_temp / 10; 
   
    contador_temp = 0;  
    temperatura_agua_temp = 0;    
    temperatura_habitacion_temp = 0;
    temperatura_disipador_temp = 0; 
    temp_agua_inst = 0;
    temp_habit_inst = 0;
    temp_disp_inst = 0;
    
  }
                                        
}                                      // ACTUA EN FUNCION DE LOS VALORES DE LA TEMPERATURA

 void check_calentador(){
   
    if (temp_agua != -127 && temp_agua != 85 && temp_agua != 0){
     
      if (temp_agua <= temp_agua_des-temperatura_margen){
        SetRele(calentador,HIGH); // Encendemos Calentador
        led2.on();
   //     Serial.print("Enciende calentador: "); Serial.print(temp_agua); Serial.print(" "); Serial.println(temp_agua_des-temperatura_margen); 
       }
       
      if (temp_agua >= temp_agua_des+temperatura_margen){
        SetRele(calentador,LOW); // Apagamos Calentador
        led2.off();
   //     Serial.print("Apaga calentador"+ String(temp_agua) +"Temp agua + margen= " + String(temp_agua_des+temperatura_margen));
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

