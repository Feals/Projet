#include <SPI.h>
#include <math.h>
#include "Adafruit_BLE_UART.h"
#define ADAFRUITBLE_REQ 10
#define ADAFRUITBLE_RDY 2
#define ADAFRUITBLE_RST 9

Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);
int value = 0;


const int x_out = A1; /* axe_x Analog */
const int y_out = A2; /* axe_y Analog */
const int z_out = A3; /* axe_z Analog */
void setup(void)
{ 
    Serial.begin(9600);
    while(!Serial);
    BTLEserial.setDeviceName("Accel_S"); /* 7 characters max! */
    BTLEserial.begin();
}

aci_evt_opcode_t laststatus = ACI_EVT_DISCONNECTED;

void loop()
/////////////////////////////////////////Connection///////////////////////////////////////// 
{
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
 
  /*  if (BTLEserial.available()) {
      Serial.print("* "); Serial.print(BTLEserial.available()); Serial.println(F(" PC "));
    }
    // OK while we still have something to read, get a character and print it out
    while (BTLEserial.available()) {
      char c = BTLEserial.read();
      Serial.print(c);
    }
*/
   

    if (Serial.available()) {
      // Read a line from Serial
      Serial.setTimeout(100); // 100 millisecond timeout

        int x_val_volt, y_val_volt, z_val_volt;
                                    // double x_g_value, y_g_value, z_g_value;
  analogReference(18);
  x_val_volt = analogRead(x_out); /* Digital x_out pin */
  y_val_volt = analogRead(y_out); /* Digital y_out pin */
  z_val_volt = analogRead(z_out); /* Digital z_out pin */


                                      // x_g_value = ( ( ( (double)((x_val_volt)* 5)/1024) - 1.65 ) / 0.330 ); /* Acceleration in x-direction in g units */ 
                                       // y_g_value = ( ( ( (double)((y_val_volt)* 5)/1024) - 1.65 ) / 0.330 ); /* Acceleration in y-direction in g units */ 
                                    // z_g_value = ( ( ( (double)((z_val_volt) * 5)/1024) - 1.80 ) / 0.330 ); /*  */
  
      
      int s = x_val_volt;



      // We need to convert the line to bytes, no more than 20 at this time
      uint8_t sendbuffer[20];
      s.getBytes(sendbuffer, 20);
      //char sendbuffersize = min(20, s.length());

      Serial.print(F("\n* Sending -> \"")); Serial.print((char *)sendbuffer); Serial.println("\"");

      // write the data
      BTLEserial.write(sendbuffer, sendbuffersize);
    }
  }
}
