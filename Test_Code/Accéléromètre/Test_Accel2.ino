#include <math.h>
const int x_out = A1; /* axe_x Analog */
const int y_out = A2; /* axe_y Analog */
const int z_out = A3; /* axe_z Analog */


void setup () {
  Serial.begin(9600);
}

void loop() {

int xv,yv,zv; //Variable en volt
double xg,yg,zg; // Variable en g
double A,G; // A - Accélération _ G - Résultante

//Référence a 3.3 volt (Pin 18=Ref voir datasheet)
analogReference(18);

//*****Lecture de la valeur de chaques axes en volt *****
    xv= analogRead(x_out);
    yv= analogRead(y_out);
    zv= analogRead(z_out);

    //*****Conversion de volt à g *****
        xg=  ( ((double)(xv - 505)*5)/1024);
        yg=  ( ((double)(yv - 506)*5)/1024);
        zg=  ( ((double)(zv - 630)*5)/1024);

        //Calcul de la résultante
            G = sqrt(xg*xg + yg*yg + zg*zg);

          //Calcul de l'accéleration en m/s²
              A = G * 9,81;


  Serial.print("Accélération G :");
  Serial.print(G);
  Serial.print("\t\t\t\t");
  Serial.print("Accélération m/s² :");
  Serial.print(A);
  Serial.print("\n");
  delay(100);

}
