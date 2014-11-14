/*

      (*) Autor: Jesus Rodriguez
      Seudonimo: ireiser - thelf
          Fecha: 06/11/2014
    Descripcion: Control de dos (2) motores DC utilizando tres (3) sensores ultrasonicos y modulo de bluetooth JY-MCU en placa arduino (Romeo V2 all in one // para este caso) 
      Generales: Utilice comandos AT para configurar su modulo bluetooth; el pin RX del modulo bluetooth JY-MCU opera a un voltaje de 3.3 v (tomar las precauciones necesarias) 
       Licencia: Este Sketch es software libre ; usted puede redistribuirlo y / o modificar --- respetando la mencion del autor original (*) ; se espera su contribucion publicando las mejoras que realice.   

    URL biicode: http://www.biicode.com/ireiser/ireiser/explorador_arduino

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

English:

     (*) Author: Jesus Rodriguez
      Pseudonym: ireiser - thelf
           Date: 06/11/2014
    Description: Control two (2) DC motors using three (3) ultrasonic sensors and bluetooth module JY-MCU Arduino board (Romeo V2 // all in one for this case)
        General: Use AT commands to configure the Bluetooth module; the RX pin of the Bluetooth module JY-MCU operates at a voltage of 3.3 v (take precautions)
        License: This Sketch is free software; you can redistribute it and / or modify --- respecting the mention of the original author (*); their contribution is expected to publish the improvements you make.

    Biicode URL: http://www.biicode.com/ireiser/ireiser/explorador_arduino
*/

//#include <SoftwareSerial.h>
//#include <Ultrasonic.h>

/////////////////////////////////////////////

#include "Arduino.h"
#include "arduino/libraries/softwareserial/softwareserial.h"
#include "Ultrasonic.h"

/////////////////////////////////////////////

// Element for bluetooth settings
// Elemento para la configuracion bluetooth

SoftwareSerial BT(10,11);   // Bluetooth PIN //10 RX, 11 TX

char cadena[255];
int i=0;      
int orden;

/////////////////////////////////////////////

int sensorPin = A0;
int sensorValue = 0;

/////////////////////////////////////////////

// Variables for handling ultrasonic pulses sensor
// Variables para el manejo de los pulsos del sensor ultrasonico
Ultrasonic SUizquierda (13,12);   // (Trig PIN,Echo PIN)
Ultrasonic SUderecha     (3,2);   // (Trig PIN,Echo PIN)
Ultrasonic SUfrente      (9,8);   // (Trig PIN,Echo PIN)

// Ultrasonic Sensor. stores the distance between the browser and the objects (left, front, right)
// Sensor Ultrasonico, almacena la distancia entre el explorador y los objetos (izquierda, frente, derecha)
long distanciaI=0;
long distanciaF=0;
long distanciaD=0;

// Ultrasonic Sensor. bouncing time (left, front, right)
// Sensor Ultrasonico. tiempo de rebote (izquierda, frente, derecha)
long tiempoI=0;
long tiempoF=0;
long tiempoD=0;

/////////////////////////////////////////////

void clean()  // clean Array
{
  for (int cl=0; cl<=i; cl++)
  {cadena[cl]=0;}
  i=0;
}

/////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////

// Motor Settings
// Configuración de motores

// Standard PWM DC control for Romeo V2

// Declaration of DC motors, two pins for investment motor voltages
// Declaracion de motores DC, dos pines por motor para inversion de voltajes

// Motor (1 - 2) speed control
int E1 = 5;
int E2 = 6;

// Motor (1 - 2) direction control
int M1 = 4;
int M2 = 7;

// Functions for managing engines
// Funciones para la administración de los motores

void stop(void)                // Stop DC motors // Detener motores DC
{
  digitalWrite(E1,LOW);
  digitalWrite(E2,LOW);
}

void advance(char a,char b)    // Forward // Avanzar
{
  analogWrite (E1,a);
  digitalWrite(M1,HIGH);
  analogWrite (E2,b);
  digitalWrite(M2,HIGH);
}

void back_off (char a,char b)  // fall back // Retroceder
{
  analogWrite (E1,a);
  digitalWrite(M1,LOW);
  analogWrite (E2,b);
  digitalWrite(M2,LOW);
}

void turn_L (char a,char b)  // Girar a la izquierda, inicia los motores en direcciones opuestas
{
  analogWrite (E1,a);
  digitalWrite(M1,LOW);
  analogWrite (E2,b);
  digitalWrite(M2,HIGH);
}

void turn_R (char a,char b)  // Rotate Right // Girar a la derecha
{
  analogWrite (E1,a);
  digitalWrite(M1,HIGH);
  analogWrite (E2,b);
  digitalWrite(M2,LOW);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void setup(void)
{

  int i;             // array bluetooth
  for(i=4;i<=7;i++)
  pinMode(i, OUTPUT);
  
  BT.begin(9600);
  Serial.begin(9600);

  // Left Ultrasonic Sensor // Sensor ultrasonico izquierda
  pinMode(13, OUTPUT);
  pinMode(12, INPUT);

  // right Ultrasonic Sensor // Sensor ultrasonico derecha
  pinMode(11, OUTPUT);
  pinMode(10, INPUT);

  // front Ultrasonic Sensor // Sensor ultrasonico frente
  pinMode(9, OUTPUT);
  pinMode(8, INPUT);

} 

/////////////////////////////////////////////////////////////////////////////////////////////////////

void loop(void)
{

//sensorValue = analogRead(sensorPin);

/*
  Serial.print(" Lectura Pin Analogico: ");
  Serial.println(sensorValue);
*/

 if(analogRead(sensorPin)==1023) //create a bridge between vcc and A0 to enable autonomous mode //cree un puente entre vcc y A0 para activar modo autonomo
{
 
  // Calculate the distances between objects and sensors
  // calcular las distancias entre objetos y sensores

  distanciaI= SUizquierda.Ranging(CM);
  delay(50);

  distanciaF= SUfrente.Ranging(CM);
  delay(50);

  distanciaD= SUderecha.Ranging(CM);
  delay(50);

  /*
    
    // Monitoring distance. in centimeters by the serial monitor
    // Monitoreo de distancia en centímetros por el monitor serial
    
    Serial.println("Distancia Izquierda ");
    Serial.println(distanciaI);
    Serial.println(" cm");
    
    Serial.println("Distancia Frente ");
    Serial.println(distanciaF);
    Serial.println(" cm");
    
    Serial.println("Distancia Derecha ");
    Serial.println(distanciaD);
    Serial.println(" cm");

*/

/////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////

// conditions to regulate the autonomy
// condiciones para regular la autonomia

  if( distanciaF >= 30)
  {
    advance(113,113);
    Serial.println(" Avanzar de frente PWM 113,113 \n");
  }

    
    if(distanciaI < 50)
    {
      advance (100,75);
      Serial.println(" Avanzar curva derecha PWM 100,75 \n");
    }

 
      if(distanciaI < 20)
      {
        advance (20,20);
        delay(25);
        stop();
        Serial.println(" Parar - obstaculo a la izquierda \n");
        back_off (80,80);
        Serial.println(" Retroceder PWM 80,80 \n");
        delay(50);
        turn_R (90,90);
        Serial.println(" Girar a la derecha PWM 90,90 \n");
        delay(50);
      }

 
        if(distanciaD < 50)
        {
          advance (75,100);
          Serial.println(" Avanzar curva izquierda PWM 75,100 \n");
        }

 
          if(distanciaD < 20)
          {
            advance (20,20);
            delay(25);
            stop();
            Serial.println(" Parar - obstaculo a la derecha \n");
            back_off (80,80);
            Serial.println(" Retroceder PWM 80,80 \n");
            delay(50);
            turn_L (90,90);
            Serial.println(" Girar a la izquierda PWM 90,90 \n");
            delay(50);
          }


            if (distanciaF < 30)
            {
              advance(20,20);
              delay(20);
              stop();
              Serial.println(" Parar - obstaculo al frente \n");
              delay(200);
              back_off (100,100);
              Serial.println(" Retroceder PWM 100,100 \n");
              delay(200);

  
              if(distanciaI > distanciaD < 30)
              {
                turn_L (110,80);
                Serial.println(" Girar a la izquierda PWM 110,80 \n");
                delay(200);
              }

  
                if(distanciaD > distanciaI < 30)
                {
                  turn_R (80,110);
                  Serial.println(" Girar a la derecha PWM 80,110 \n");
                  delay(200);
                }

  
                  if(((distanciaF < 20) && (distanciaI < 20)) && (distanciaD < 20))
                  {
                    int randNumber = random(1,3);
                    Serial.println(randNumber);
                    Serial.println(" condicion  aleatoria de giro \n");
  

                      if ( randNumber == 1)
                      {
                        Serial.println(" giro izquierda  PWM 100,100 \n");
                        turn_L (100,100);
                        delay(200);
                      }

  
                        if ( randNumber == 2)
                        {
                          Serial.println(" giro derecha PWM 100,100 \n");
                          turn_R (100,100);
                          delay(200);
                        }
                  }
             }
 }

/////////////////////////////////////////////////////////////////////////////////////////////////////

if(analogRead(sensorValue) == 0)  // create a bridge between gnd and A0 to activate bluetooth and serial mode  //cree un puente entre gnd y A0 para activar modo bluetooth y serial
{
  
  distanciaF= SUfrente.Ranging(CM);
  delay(50);
  
    if(distanciaF <= 30 && orden == 1)
    {

     stop();
     Serial.print(" Debo detenerme parece que hay un obstaculo al frente \n");
     BT.write("Debo detenerme parece que hay un obstaculo al frente \n \r");
     delay(500);
     orden=0;

    } 
  
   if( (BT.available()) || (Serial.available()))
   {
    
    char val  = BT.read();
    char val2 = Serial.read();
    //cadena[i++]=val;           // Put each character received in the array "cadena" // Coloca cada carácter recibido en el array "cadena"

    
    if( val != '-1')  // Start Bluetooth Communication // Inicia Comunicación Bluetooth
    {
          
      switch(val)
      {
        
      case 'h':
        BT.write("Hola estoy listo para iniciar comunicacion Bluetooth (explorador)\r");
        BT.write("w = Avanzar \n\r");
        BT.write("S = Retroceder \n\r");
        BT.write("a = Giro Izquierda \n\r");
        BT.write("d = Giro Derecha \n\r");
        break;
        
        
      case 'w':                      // Forward // Avanzar
        BT.write("Avanzar\r");
        advance (100,100);
        delay(500);
        advance (255,255);
        orden=1;
        break;

        
      case 's':                      // fall back // Retroceder
        BT.write("Retroceder\r");
        back_off (100,100);
        delay(500);
        back_off (255,255);
        break;

        
      case 'a':                      // Counterclockwise // Giro izquierda
        BT.write("Giro Izquierda\r");
        turn_L (100,100);
        delay(250);
        stop();
        break;

        
      case 'd':                      // Clockwise // Giro derecha
        BT.write("Giro Derecha\r");
        turn_R (100,100);
        delay(250);
        stop();
        break;

      
      case 'x':                      // Stop // Detener
        BT.write("Detener\r");
        stop();
        break;
      }

/////////////////////////////////////////////

      if( val2 != '-1')  // Start Serial Communication // Inicia Comunicacion Serial
    {

      switch(val2)
      {
        
      case 'h':
        Serial.println("\n Hola estoy listo para iniciar comunicacion Serial (explorador) \n");
        Serial.println("\n w = Avanzar \n");
        Serial.println("\n S = Retroceder \n");
        Serial.println("\n a = Giro Izquierda \n");
        Serial.println("\n d = Giro Derecha \n");
        break;

        
      case 'w':                                // Forward // Avanzar
        Serial.print(" Avanzar \n");
        advance (100,100);
        delay(500);
        advance (255,255);
        orden=1;
        break;

        
      case 's':                                // fall back // Retroceder
        Serial.print(" Retroceder \n");
        back_off (100,100);
        delay(500);
        back_off (255,255);
        break;

        
      case 'a':                                // Counterclockwise // Giro izquierda
        Serial.print(" Giro izquierda \n");
        turn_L (100,100);
        delay(250);
        stop();
        break;

        
      case 'd':                                // Clockwise // Giro derecha
        Serial.print(" Giro derecha \n");
        turn_R (100,100);
        delay(250);
        stop();
        break;

      
      case 'x':                               // Stop // Detener
        Serial.print(" Detener \n");
        stop();
        break;
        
      }
    }

    BT.write("\n");
    //clean();
      
    }

    else stop();

  }
}

}