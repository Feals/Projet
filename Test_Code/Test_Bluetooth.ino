    int a = 10;
    int b = 20;
    int c = 45;

    void setup() {
      // put your setup code here, to run once:
      Serial1.begin(9600);
    }

    void loop() {
      Serial1.print(a);
      Serial1.print(",");
      Serial1.print(b);
      Serial1.print(",");
      Serial1.print(c);
      delay(1000);

  }
