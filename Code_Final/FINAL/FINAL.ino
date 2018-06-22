#include <SPI.h>
#include <math.h>
#include <LiquidCrystal.h>
#include "Adafruit_BLE_UART.h"
#define ADAFRUITBLE_REQ 10 // Pin MOSI
#define ADAFRUITBLE_RDY 2 // Pin RDY
#define ADAFRUITBLE_RST 9 //PIN RST
#define pi 3.141592 // Utile plus bas lors du calcul des angles

//Déclaration de l'écran LCD
const int rs = 8, en = 7 ,d4 = 6, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


// Déclaration des pins de l'accéléromètre
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
Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST); // Declaration des pin Bluetooth
int value = 0;

void setup(void)
{
    Serial.begin(9600); //Serial 9600 baud
    while(!Serial);
    BTLEserial.setDeviceName("Accel_S"); /*Nom du module Bluetooth */
    BTLEserial.begin();
    lcd.begin(16,2);
    lcd.clear();
}

aci_evt_opcode_t laststatus = ACI_EVT_DISCONNECTED; // On met le dernier status en déconnecté

void loop()
{

  int xv,yv,zv; //Variable en volt
  double xg,yg,zg; // Variable en g
  double A,G,Axy; // A - Accélération _ G - Résultante
 int Angle;
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

//Affiche accelération en g en haut a gauche
lcd.setCursor(0,0);
lcd.print("Ag=");
lcd.setCursor(3,0);
lcd.print(G);

//Affiche accélération en m/s²
lcd.setCursor(0,1);
lcd.print("Ams=");
lcd.setCursor(4,1);
lcd.print(A);

//Affiche angle de l'accélérometre
lcd.setCursor(8,0);
lcd.print("A=");
lcd.setCursor(10,0);
lcd.print(Angle );
delay(300);

// On clear pour les valeurs d'angles a 3 chiffres
lcd.clear();

// Test de status de la puce
//*************************************************************************************************************//
    BTLEserial.pollACI();
    aci_evt_opcode_t status = BTLEserial.getState();
    if (status != laststatus) {
        if (status == ACI_EVT_DEVICE_STARTED) {
            Serial.println(F("* Visible par les appareils"));
        }
        if (status == ACI_EVT_CONNECTED) {
            Serial.println(F("* Connecté !"));
        }
        if (status == ACI_EVT_DISCONNECTED) {
            Serial.println(F("* Vous avez été déconnecté :/ "));
        }
        laststatus = status; // Actualisation du status
    }
//*************************************************************************************************************//

// Je suis connecté !
//*************************************************************************************************************//
    if (status == ACI_EVT_CONNECTED) {

//Je recois des données !
//*************************************************************************************************************//
    if (BTLEserial.available()) {
      Serial.print("* "); Serial.print(BTLEserial.available()); Serial.println(F("Donnée recu de votre téléphone"));
    }

    while (BTLEserial.available()) {
      char c = BTLEserial.read();
      Serial.print(c);
    }

//Envoie de l'accélération m/s² puis Accélération en g puis L'angle de l'accélérometre
//*************************************************************************************************************//
      Serial.setTimeout(100);
      String sa = doubleToString(A,2);

      // Conversion des données
      uint8_t sendbufferA[5];
      sa.getBytes(sendbufferA, 5);
      char sendbuffersizeA = min(5, sa.length());
delay (100);
      Serial.print(F("\n* Envoie -> \"")); Serial.print((char *)sendbufferA); Serial.println("\"");

      // Envoie des données
      BTLEserial.write(sendbufferA, sendbuffersizeA);


      Serial.setTimeout(100);
      String sg = doubleToString(G,2);

      // Conversion des données
      uint8_t sendbufferG[5];
      sg.getBytes(sendbufferG, 5);
      char sendbuffersizeG = min(5, sg.length());
delay (100);
      Serial.print(F("\n* Envoie -> \"")); Serial.print((char *)sendbufferG); Serial.println("\"");

      // Envoie des données
      BTLEserial.write(sendbufferG, sendbuffersizeG);

      String san = doubleToString(Angle,2);

      // Conversion des données
      uint8_t sendbufferAN[5];
      san.getBytes(sendbufferAN, 5);
      char sendbuffersizeAN = min(5, san.length());
delay (100);
      Serial.print(F("\n* Envoie -> \"")); Serial.print((char *)sendbufferAN); Serial.println("\"");

      // Envoie des données
      BTLEserial.write(sendbufferAN,sendbuffersizeAN);

  }
}
