void envio_datos(){

  if (millis() - previous_Millis_datos > 60000){  //Envia datos  cada 60 segundos
    
  previous_Millis_datos = millis();

  //// Envia datos a ThingSpeak
    ThingSpeak.setField(1,temp_agua);
    ThingSpeak.setField(2,tempD);
    ThingSpeak.setField(3,tempHB);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    Serial.println("Envia datos a thinkspeak");

  }
}

