////////////////////////////////////////////////////////////////
// VOID SETUP
///////////////////////////////////////////////////////////////
void setup() {
 Serial.begin(9600);    //Iniciamos el puerto serie
  
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8442);


  Serial.println("WiFi connected ");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

//Asignamos los distintos PIN
  pinMode(calentador, OUTPUT);  // Calentador     
  pinMode(aireador, OUTPUT);  // Aireador
  pinMode(lamp_uv, OUTPUT);  // Lampara UV
  pinMode(luz, OUTPUT);  // 
  pinMode(bomba, OUTPUT);   // Bomba rellenador 
  pinMode(nivel_acu, INPUT);// Indicamos el puerto de control de la boya del acuario 
  
////////////////////////////////////////
////////Con esto apagamos todos los reles. 
/////////////////////////////////////////

  digitalWrite(calentador,LOW);
  digitalWrite(aireador,LOW);
  digitalWrite(lamp_uv,LOW);
  digitalWrite(luz,LOW);
 
  
////////////////////////////////////////
// LLAMAMOS A LOS SENSORES DE LA TEMPERATURA
////////////////////////////////////////

  sensors.begin();     //Inicia las lecturas de las sondas de temperatura.
  if (!sensors.getAddress(sensor_agua, 0)) Serial.println("Falta el sensores de temperatura del agua");
  if (!sensors.getAddress(sensor_disipador, 1)) Serial.println("Falta el sensores de temperatura de la tapa");
  if (!sensors.getAddress(sensor_habitacion, 2)) Serial.println("Falta el sensores de temperatura de la habitacion");
  
  
  sensors.setResolution(sensor_agua, 12); // Define la resolucion en 10 bits.
  sensors.setResolution(sensor_habitacion, 12); // Define la resolucion en 10 bits.
  sensors.setResolution(sensor_disipador, 12); // Define la resolucion en 10 bits.

  //////////////////////////////
  
  Serial.println("Test the serial monitor!");    //Test the serial monitor
}

/////////////////////////////////////////////////
// FUNCION PARA RELES CHINO QUE VAN AL CONTRARIO
/////////////////////////////////////////////////
void SetRele( int Pin, boolean Estado )  //Funcion que hace que el rele chino funcione bien y se puede poner LOW o HIGH segun corresponda.
{
 
 digitalWrite( Pin, !Estado );

}

////////////////////////////////////////////////////////////////
// FIN DEL SETUP
///////////////////////////////////////////////////////////////

