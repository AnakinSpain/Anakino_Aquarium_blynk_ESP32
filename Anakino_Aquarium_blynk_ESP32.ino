

///////////////////////////////////////////////////////////////
//
//
//
//
//     CONTROLADOR DE ACUARIO CON LA APP BLYNK 
//     Y USANDO UNA MODULO ESP32 PARA CONTROLARLO
//     V1.0 
//     
//     TO DO: 1) Añadir opcion de luces auto, man on o man off y controladas por el arduino
//            2) Añadir comedero
//            3) Añadir los pines para los reles. segun ya tengo conectado
//            4) Añadir led verde o rojo segun si está online o no
//
//
//
///////////////////////////////////////////////////////////////
//Listado de PINs utilizados
///////////////////////////////////////////////////////////////


//        Sondas temp.     4   (D4)
//        Boya nivel agua  16  (RX2)
//                             
//        Ventilador       17 rele 12v  (PWM) (TX2)
//        Bomba  relleno   5  rele 12v  (PWM) (D5)
//        Calentador       18 rele 220v (1)   (D18)
//        Aireador         19 rele 220v (2)   (D19)
//        Lampara UV       21 rele 220v (3)   (D21)
//        CO2              3  rele 220v (4)   (RX0)
//        rele x1          22 rele x1 (sin definir) (D22)
//        rele x2          23 rele x2 (sin definir) (D23)

//        Leds blancos     12 led blancos (PWM) (D12)
//        Leds Azules      14 led azules  (PWM) (D14)

//        Sensor Ph        13 (D13)
//        Peristaltica 1   27 (D27)
//        Servo comedero   26 (D26)
//        Pin Libre        33 (D33)
//        Pin Libre        32 (D32)

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
#include <Servo.h>
#include "ThingSpeak.h"
#include <TimeLib.h>
#define BLYNK_PRINT Serial
#include <WidgetRTC.h>

////////////////////////////////////////////////////////////////////////
//VALORES QUE SE PUEDEN CAMBIAR AL GUSTO
////////////////////////////////////////////////////////////////////////

#define temperatura_margen  0.5    // Margen de actuacion del calentador

////////////////////////////////////////////////////////////////////////
//VARIABLES DE PWM
////////////////////////////////////////////////////////////////////////
#define vent_channel_0 0     // defino canal 0 para el pwm del ventilador
#define pwm_8_bit 8        // defino la precision de 8 bits para que sea de 0 a 255
#define pwm_base_freq 25000 // 25000hz de frecuencia

int pwm_vent;  // variable que llevará el valor del pwm de 0 a 255

////////////////////////////////////////////////////////////////////////
//CONFIGURACION WIFI
////////////////////////////////////////////////////////////////////////

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "atorcha";
char pass[] = "X55-mCx-DVL-kF4";
WiFiClient  client;
////////////////////////////////////////////////////////////////////////
//DECLARACION DE VARIABLES de THINGSPEAK
////////////////////////////////////////////////////////////////////////
 unsigned long myChannelNumber = 358672;
 const char * myWriteAPIKey = "K6GT1NEIV57XGXIL";
 long previous_Millis_datos = 0;

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
#define LED_BUILTIN 2   // pin led azul
#define ventilador 17   // Ventilador pin 17

static const int servoPin = 26;

////////////////////////////////////////////////////////////////
//*********************** Variables de control de temperatura del agua ********************
// Preparamos otras variable
float temp_agua;    //Temperatura del agua
float temp_agua_des; //temperatura agua deseada
int contador_temp = 0;
int contador_med_temp = 0;
float temperatura_agua_temp;       // Temperatura temporal del agua
int contador_global = 0;

float temp_agua_inst;
float temp_habit_inst;
float temp_disp_inst;
 
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

BlynkTimer timer;

WidgetRTC rtc;

bool tweet_enviado = false;

bool isConnected;


  BLYNK_CONNECTED()     // This function will run every time Blynk connection is established
  {  
   Blynk.syncAll();  // Request Blynk server to re-send latest values for all pins
   rtc.begin();  // Synchronize time on connection
   }

    
  BLYNK_WRITE(V1) // Variable del modo AIREADOR (auto, manual on, off )
{
  int modo_ai_blynk = param.asInt(); 
  Serial.println("V1 modo_ai value is: "+ String(modo_ai_blynk));
  modo_ai=modo_ai_blynk;
}

  BLYNK_WRITE(V2) //  Variable del modo LAMPARA UV (auto, manual on, off )
{
  int modo_uv_blynk = param.asInt(); 
  Serial.println("V2 modo_uv value is: " + String(modo_uv_blynk));
  modo_uv=modo_uv_blynk;
}
 WidgetLED led1(V10);  // estado de la bomba de relleno
 WidgetLED led2(V11); //  estado del calentador
 WidgetLED led3(V12); //  estado del ventilador
 

  BLYNK_WRITE(V23)  // variable de la temperatura del agua deseada
{
  float temp_agua_des_blynk = param.asFloat(); // Asigna un valor a la variable temp_agua_des desde el slider de la app
  Serial.println("Temp Deseada blynk: " + String(temp_agua_des_blynk));
  temp_agua_des = (temp_agua_des_blynk);
}

  BLYNK_WRITE(V30) // Modo del rellenador On u off 
{
  int modo_relleno_blynk = param.asInt(); 
  Serial.println("V30 modo_relleno value is: "+ String(modo_relleno_blynk));
  modo_relleno=modo_relleno_blynk;
}

  BLYNK_WRITE(V40) //Obtenemos el estado del  aireador en HIGH si está dentro del temporizador encendido
{
  int temp_ai_blynk = param.asInt(); 
  Serial.println("V40 modo_ai value is: "+String(temp_ai_blynk));
  temp_ai=temp_ai_blynk;
  
}

  BLYNK_WRITE(V41) //Obtenemos el estado de la lamp uv en HIGH si está dentro del temporizador encendido
{
  int temp_uv_blynk = param.asInt(); 
  Serial.println("V41 modo_temp uv value is: "+String(temp_uv_blynk));
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


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//  servo
///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
Servo servo1;



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

