#include <SPI.h>
#include <math.h>
#include "Adafruit_BLE_UART.h"
#define ADAFRUITBLE_REQ 10
#define ADAFRUITBLE_RDY 2
#define ADAFRUITBLE_RST 9
#define pi 3.141592

const int x_out = A1; /* axe_x Analog */
const int y_out = A2; /* axe_y Analog */
const int z_out = A3; /* axe_z Analog */

//  Fonction Convertisseur double vers string
//*************************************************************************************************************//

String doubleToString(double input,int decimalPlaces){
  if(decimalPlaces!=0){
    String string = String((int)(input*pow(10,decimalPlaces)));
      if(abs(input)<1){
        if(input>0)
        string = "0"+string;
        else if(input<0)
        string = string.substring(0,1)+"0"+string.substring(1);
        }
        return string.substring(0,string.length()-decimalPlaces)+"."+string.substring(string.length()-decimalPlaces);
      }
    else {
    return String((int)input);
  }
}
//*************************************************************************************************************//




//Définition des pins digitales
Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);
int value = 0;

void setup(void)
{
    Serial.begin(9600);
    while(!Serial);
    BTLEserial.setDeviceName("Accel_S"); /*Nom du module */
    BTLEserial.begin();
}

aci_evt_opcode_t laststatus = ACI_EVT_DISCONNECTED; // On met le dernier status en déconnecté

void loop()
/////////////////////////////////////////Connection/////////////////////////////////////////
{

  int xv,yv,zv; //Variable en volt
  double xg,yg,zg; // Variable en g
  double A,G,Axy,Angle; // A - Accélération _ G - Résultante

  //Référence a 3.3 volt (Pin 18=Ref voir datasheet)
  analogReference(18);

  //*****Lecture de la valeur de chaques axes en volt *****
      xv= analogRead(x_out);
      yv= analogRead(y_out);
      zv= analogRead(z_out);
      //*****Conversion de volt à g *****
          xg=  ( ((double)(xv - 505)*5)/1024);
          yg=  ( ((double)(yv - 505)*5)/1024);
          zg=  ( ((double)(zv - 425.3333)*5)/1024);

          //Calcul de la résultante
              G = xg*xg + yg*yg + zg*zg;
              G=sqrt(G);
            //Calcul de l'accéleration en m/s²
              A = (G * 9.81);
              //Calcul de l'angle de l'accéléromètre en degrés.
                Axy=xg*xg+yg*yg;
                Axy=sqrt(Axy);
                Angle = 180.0/pi * atan2(Axy,zg);


    BTLEserial.pollACI();

    aci_evt_opcode_t status = BTLEserial.getState();
    if (status != laststatus) {
        if (status == ACI_EVT_DEVICE_STARTED) {
            Serial.println(F("* Advertising started"));
        }
        if (status == ACI_EVT_CONNECTED) {
            Serial.println(F("* Connected!"));
        }
        if (status == ACI_EVT_DISCONNECTED) {
            Serial.println(F("* Disconnected or advertising timed out"));
        }
        laststatus = status;
    }
//////////////////////////////////////////////////////////////////////////////////////////
    if (status == ACI_EVT_CONNECTED) {

    if (BTLEserial.available()) {
      Serial.print("* "); Serial.print(BTLEserial.available()); Serial.println(F(" bytes available from BTLE"));
    }

    while (BTLEserial.available()) {
      char c = BTLEserial.read();
      Serial.print(c);
    }

    // Next up, see if we have any data to get from the Serial console

      // Read a line from Serial
      Serial.setTimeout(100); // 100 millisecond timeout
      String sa = doubleToString(A,2);

      // We need to convert the line to bytes, no more than 20 at this time
      uint8_t sendbuffer[5];
      sa.getBytes(sendbuffer, 5);
      char sendbuffersize = min(5, sa.length());
delay (1000);
      Serial.print(F("\n* Sending -> \"")); Serial.print((char *)sendbuffer); Serial.println("\"");

      // write the data
      BTLEserial.write(sendbuffer, sendbuffersize);

  }
}
