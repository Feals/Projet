#include <math.h>
const int x_out = A1; /* axe_x Analog */
const int y_out = A2; /* axe_y Analog */
const int z_out = A3; /* axe_z Analog */


void setup () {
  Serial.begin(9600);
  //Serial.begin(115200); // Ã€ Tester
}

void loop() {

  int x_val_volt, y_val_volt, z_val_volt;
 // double x_g_value, y_g_value, z_g_value;
  analogReference(18);
  x_val_volt = analogRead(x_out); /* Digital x_out pin */
  y_val_volt = analogRead(y_out); /* Digital y_out pin */
  z_val_volt = analogRead(z_out); /* Digital z_out pin */


 // x_g_value = ( ( ( (double)((x_val_volt)* 5)/1024) - 1.65 ) / 0.330 ); /* Acceleration in x-direction in g units */ 
 // y_g_value = ( ( ( (double)((y_val_volt)* 5)/1024) - 1.65 ) / 0.330 ); /* Acceleration in y-direction in g units */ 
 // z_g_value = ( ( ( (double)((z_val_volt) * 5)/1024) - 1.80 ) / 0.330 ); /*  */
  
  Serial.print("Digital x = ");
  Serial.print(x_val_volt);
  Serial.print("\t\t\t\t");
  Serial.print("Digital y = ");
  Serial.print(y_val_volt);
  Serial.print("\t\t\t\t");
  Serial.print("Digital z = ");
  Serial.print(z_val_volt);
  Serial.print("\n");
  delay(100);



}
