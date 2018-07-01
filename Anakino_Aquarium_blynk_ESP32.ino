


///////////////////////////////////////////////////////////////
//
//
//
//
//     CONTROLADOR DE ACUARIO CON LA APP BLYNK 
//     Y USANDO UNA MODULO ESP32 PARA CONTROLARLO
//     V1.0 
//     
//     TO DO: Añadir opcion de luces auto, man on o man off y controladas por el arduino
//
//
///////////////////////////////////////////////////////////////
//Listado de PINs utilizados
///////////////////////////////////////////////////////////////

///////// PINES INPUT///////////
//        Sondas temp.     4
//        Boya nivel agua  16

/////// PINES  OUTPUT/////
//                             
//        Ventilador       17 rele 12v
//        Bomba  relleno   5  rele 12v
//        Calentador       18 rele 220v (1)
//        Aireador         19 rele 220v (2)
//        Lampara UV       21 rele 220v (3)
//        CO2              3  rele 220v (4)
//        rele x1          22 rele x1 (sin definir)
//        rele x2          23 rele x2 (sin definir)

//        Leds blancos     12 led blancos
//        Leds Azules      14 led azules 


/////// PUERTOS VIRTUALES BLYNK /////
//                            
//        V1       Modo Airador
//        V2       Modo lamp UV
//        V10      Aviso rellenador funcionando
//        V11      Aviso calentador funcionando
//        V12      Aviso ventilador funcionando
//        V20      temp de agua
//        V21      temp habitacion
//        V22      temp pantalla
//        V23      Asigna valor a la temp. del acuario desde blynk
//        V30      boton de Rellenador ON / OFF
//        V40      Disparador del temporizador del aireador (temporizadores)
//        V41      Disparador del temporizador de la Lamp UV (temporizadores)
//        
//       
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
//Librerias necesarias
///////////////////////////////////////////////////////////////
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define BLYNK_PRINT Serial

////////////////////////////////////////////////////////////////////////
//VALORES QUE SE PUEDEN CAMBIAR AL GUSTO
////////////////////////////////////////////////////////////////////////

#define temperatura_margen  0.5    // Margen de actuacion del calentador
#define VentMax 34     // Temperatura de encendido Ventilador
#define VentMin 30     // Temperatura de apagado Ventilador

////////////////////////////////////////////////////////////////////////
//CONFIGURACION WIFI
////////////////////////////////////////////////////////////////////////

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "atorcha";
char pass[] = "X55-mCx-DVL-kF4";

////////////////////////////////////////////////////////////////////////
//DECLARACION DE VARIABLES Y PINES
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
// Definimos los pines 
////////////////////////////////////////////////////////////////
#define calentador 18    // Calentador             
#define aireador  19     // Aireador    
#define lamp_uv  21      // Lampara UV
#define luz  3          // Luz

#define ventilador 17    // Ventilador pin 17

////////////////////////////////////////////////////////////////
//*********************** Variables de control de temperatura del agua ********************
// Preparamos otras variable
float temp_agua;    //Temperatura del agua
float temp_agua_des; //temperatura agua deseada
int contador_temp = 0;
int contador_med_temp = 0;
float temperatura_agua_temp;       // Temperatura temporal del agua

 
//float tempHB;    //Temperatura de la habitacion
float temp2_temp; //temperatura temporal sistema

float temperatura_disipador_temp; // Temperatura temporal del disipador
float temperatura_habitacion_temp; // temperatura temporal de la habitacion
    
 float tempD = 0;     // Temperatura del disipador
 float tempHB = 0;    // Temperatura habitacion
 
//////////////////////////////////////////////////////////////
//************ Variables rellenador


 int nivel_estado = 1;
 #define nivel_acu  16  // Boya nivel agua acuario
 #define bomba 5      // Pin accionara la bomba de llenado 

/*
float PinSensor = 30; //Sensor de nivel de agua en el pin 30
int VarSensor = 0;  //Control del nivel de agua
int floatPin = 31;  //Boya de nivel del depósito de relleno
*/
/////////////////////////////////////////////////
// Preparamos  modos de funcionamiento

int modo_relleno; // Rellenador 1 -> activado // 0-> desactivado
int modo_uv;     // modo de funcionamiento uv
int modo_ai;     // modo de funcionamiento aireador

int temp_uv;          // Variable para indicar si activa o no la lamp uv desde blynk por el temporizador
int temp_ai;          // Variable para indicar si activa o no el aireador desde blynk por el temporizador


/////////////////////////////////////////////////////////////////
//Blynk
////////////////////////////////////////////////////////////////
char auth[] = "4c9d2ca518b04366b3de5496f99c75dd";

  BLYNK_CONNECTED()     // This function will run every time Blynk connection is established
  {  
   Blynk.syncAll();  // Request Blynk server to re-send latest values for all pins
   }

    
  BLYNK_WRITE(V1) // Variable del modo AIREADOR (auto, manual on, off )
{
  int modo_ai_blynk = param.asInt(); 
  Serial.print("V1 modo_ai value is: ");
  Serial.println(modo_ai_blynk);
  modo_ai=modo_ai_blynk;
}

  BLYNK_WRITE(V2) //  Variable del modo LAMPARA UV (auto, manual on, off )
{
  int modo_uv_blynk = param.asInt(); 
  Serial.print("V2 modo_uv value is: ");
  Serial.println(modo_uv_blynk);
  modo_uv=modo_uv_blynk;
}
 WidgetLED led1(V10);  // estado de la bomba de relleno
 WidgetLED led2(V11); //  estado del calentador
 WidgetLED led3(V12); //  estado del ventilador
 

  BLYNK_WRITE(V23)  // variable de la temperatura del agua deseada
{
  float temp_agua_des_blynk = param.asInt(); // Asigna un valor a la variable temp_agua_des desde el slider de la app
  Serial.print("V23 temperatura agua deseada:  ");
  Serial.println(temp_agua_des_blynk);
  temp_agua_des = (temp_agua_des_blynk/10);
}

  BLYNK_WRITE(V30) // Modo del rellenador On u off 
{
  int modo_relleno_blynk = param.asInt(); 
  Serial.print("V30 modo_relleno value is: ");
  Serial.println(modo_relleno_blynk);
  modo_relleno=modo_relleno_blynk;
}

  BLYNK_WRITE(V40) //Obtenemos el estado del  aireador en HIGH si está dentro del temporizador encendido
{
  int temp_ai_blynk = param.asInt(); 
  Serial.print("V40 modo_ai value is: ");
  Serial.println(temp_ai_blynk);
  temp_ai=temp_ai_blynk;
  
}

  BLYNK_WRITE(V41) //Obtenemos el estado de la lamp uv en HIGH si está dentro del temporizador encendido
{
  int temp_uv_blynk = param.asInt(); 
  Serial.print("V41 modo_temp uv value is: ");
  Serial.println(temp_uv_blynk);
  temp_uv=temp_uv_blynk;
  
}
 

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//Establecemos los PIN de los sensores de temperatura
///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


OneWire OneWireBus(4);      //Sensores de temperatura conectados al pin 15.
DallasTemperature sensors(&OneWireBus);
DeviceAddress sensor_agua, sensor_habitacion, sensor_disipador;
//DeviceAddress sensor_agua= {0x28, 0x10, 0x32, 0x2B, 0x04, 0x00, 0x00, 0x38 }; // Es necesario cambiar este valor acorde con nuestro sensor.
//DeviceAddress sensor_disipador = {0x28, 0x99, 0x47, 0x2B, 0x04, 0x00, 0x00, 0xFB }; // Es necesario cambiar este valor acorde con nuestro sensor.
//DeviceAddress sensor_habitacion = {0x28, 0x6D, 0x70, 0x2B, 0x04, 0x00, 0x00, 0x80 }; // Es necesario cambiar este valor acorde con nuestro sensor.

/*
/////////////////////////////////////////////////////////////////
// Variables de la sonda de PH
/////////////////////////////////////////////////////////////////
#define SensorPin A0            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00            //deviation compensate
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;    
static float pHValue;
*/

