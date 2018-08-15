
/////////////////////////////////////////////////////////////////
// Controlamos Sensor de agua 
// activar o desactivar bomba de relleno
////////////////////////////////////////////////////////////////

void check_nivel(){
  
    if (modo_relleno == 1)
  {
     // Serial.print("modo rellenador activado");
     //  Serial.println("");
      
      nivel_estado = digitalRead(nivel_acu);
      
      if (nivel_estado != 1) // Si la lectura del sensor del nivel acuario indica que esta bajo
      {
      SetRele(bomba, LOW);     // desactiva la bomba
      led1.off();
  //   Serial.print("desactiva bomba");
  //   Serial.println("");
      }
   else  
   {
   SetRele(bomba, HIGH);
   led1.on();
 //  Serial.print("activa bomba");
 //  Serial.println("");
   }
  } 

} 
/////////////////////////////////////////////////////////////////
//Fin controlNivel
/////////////////////////////////////////////////////////////////

