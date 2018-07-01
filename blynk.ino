////////////////////////////////////////////////////////////////
// Funcion variable blynk
////////////////////////////////////////////////////////////////
void blynk(){
  Blynk.virtualWrite(V20, temp_agua); // envia la temp al widget agua
  Blynk.virtualWrite(V21, tempHB); // envia la temp al widget habitacion
  Blynk.virtualWrite(V22, tempD); // envia la temp al widget 
}




