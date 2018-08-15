void tweeter(){
  if (tweet_enviado == false){
  Serial.println("Envía tweet"); 
  String currentTime = String(hour()) + ":" + minute() + ":" + second()+ " ";
  String currentDate = String(day()) + "/" + month() + "/" + year();
  Serial.print("Current time: ");
  Serial.print(currentTime);
  Serial.print(" ");
  Serial.print(currentDate);
  Serial.println();
  Blynk.tweet("Reconectado a las: " + String(currentTime) + String(currentDate) + " #arduino #IoT #blynk");
  tweet_enviado = true;
  }
}

