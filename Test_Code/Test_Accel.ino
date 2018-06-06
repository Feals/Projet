const int x_out = A1; /* axe_x Analog */
const int y_out = A2; /* axe_y Analog */
const int z_out = A3; /* axe_z Analog */

void setup (){
  Serial.begin(9600)
  //Serial.begon(115200) // À Tester
  }

void loop(){
  int x_val_volt, y_val_volt, z_val_volt;

  x_val_volt = analogRead(x_out); /* Digital x_out pin */
  y_val_volt = analogRead(y_out); /* Digital y_out pin */
  z_val_volt = analogRead(z_out); /* Digital z_out pin */

      Serial.print("Digital x = ");
      Serial.print(x_val_volt);
      Serial.print("\t\t\t\t");
      Serial.print("Digital y = ");
      Serial.print(y_adc_value);
      Serial.print("\t\t\t\t");
      Serial.print("Digital z = ");
      Serial.print(z_adc_value);
      Serial.print("\t\t\t\t");
      delay(1000);
  }
