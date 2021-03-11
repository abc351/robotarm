#include <Servo.h>
#define hpi 1.5707963267948966192313216916398
#define tpi 6.283185307179586476925286766559
#define pi 3.1415926535897932384626433832795
#define cov 57.295779513082320876798154814105
Servo a[6];
double theta[5];
int b2;
double x, y, z, th;
double _x, _y, _z, _th;
double nx, ny, nz, nth;
double bias[5] = {90, 88, 98, 95, 95};
int b3;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  a[0].attach(2);
  a[1].attach(3);
  a[2].attach(4);
  a[3].attach(5);
  a[4].attach(6);
  a[5].attach(7);
  pinMode(8, INPUT);
  pinMode(13, OUTPUT);
  a[0].write(bias[0]);
  delay(500);
  a[1].write(bias[1]);
  delay(500);
  a[2].write(bias[2]);
  delay(500);
  a[3].write(bias[3]);
  delay(500);
  a[4].write(bias[4]);
  delay(500);
  a[5].write(90);

  delay(500);
  digitalWrite(13, 1);
  for (int i = 90; i > 0; i -= 15) {
    Serial.print("!");
    if (digitalRead(8)) {
      i += 20;
      a[5].write(i);
      delay(300);
      for (; i > 0; i -= 4) {
        if (digitalRead(8)) {
          digitalWrite(13, 0);
          i += 4;
          if (i < 0) i = 0;
          a[5].write(i);
          break;
        }
        a[5].write(i);
        delay(150);
      }

    }
    a[5].write(i);
    delay(150);
  }
  b3 = 1;
  nx = 143; ny = 43; nz = 103; nth = 0;
}
double tr(double a, double b, double c) { //asinx+bcosx=c,x=?
  double t = atan2(b, a);
  double u = asin(c / sqrt(a * a + b * b)) - t;
  return u;
}

void loop() {
  if (Serial.available()) {
    delay(50);
    _x = Serial.parseFloat();
    _y = Serial.parseFloat();
    _z = Serial.parseFloat();
    _th = Serial.parseFloat();
    _th/=cov;
    b2 = Serial.parseInt();
    delay(100);
    while (Serial.available()) Serial.read();
    Serial.print("in:");
    Serial.print(_x);
    Serial.print(" ");
    Serial.print(_y);
    Serial.print(" ");
    Serial.print(_z);
    Serial.print(" ");
    Serial.print(_th);
    Serial.print(" ");
    Serial.println(b2);
    if (b2 == 2) {
      bias[(int)(x)] += y;
      a[0].write(bias[0] - cov * theta[0]);
      delay(500);
      a[1].write(bias[1] - cov * theta[1]);
      delay(500);
      a[2].write(bias[2] - cov * theta[2]);
      delay(500);
      a[3].write(bias[3] - cov * theta[3]);
      delay(500);
      a[4].write(bias[4] - cov * theta[4]);
      Serial.print("bias:");
      Serial.print(bias[0]);
      Serial.print(" ");
      Serial.print(bias[1]);
      Serial.print(" ");
      Serial.print(bias[2]);
      Serial.print(" ");
      Serial.print(bias[3]);
      Serial.print(" ");
      Serial.print(bias[4]);
      Serial.print(" ");
      Serial.println("");
    }
    else {
      while (1) {
        x = _x;
        if (x > nx + 3) x = nx + 3;
        else if (x < nx - 3) x = nx - 3;
        y = _y;
        if (y > ny + 3) y = ny + 3;
        else if (y < ny - 3) y = ny - 3;
        z = _z;
        if (z > nz + 3) z = nz + 3;
        else if (z < nz - 3) z = nz - 3;
        th = _th;
        if (th > nth + 0.2) th = nth + 0.2;
        else if (th < nth - 0.2) th = nth - 0.2;
        nx = x;
        ny = y;
        nz = z;
        nth = th;
        theta[0] = tr(-21.*cos(th) + x, -21.*sin(th) + y, 43.);
        double p = (-21.*cos(th) + x - 43.*sin(theta[0])) / (-122.*cos(theta[0]));
        double q = (19. + z) / 122.;
        double s = tr(2 * q, -2 * p, (p * p + q * q));
        theta[1] = atan2(p + cos(s), q - sin(s));
        theta[2] = theta[1] - s;
        if (theta[2] < -hpi || theta[2] > hpi) theta[2] = theta[1] + s;
        theta[3] = theta[2] - theta[1];
        theta[4] = th + theta[0];
        for (int i = 0; i < 5; i++) {
          if (theta[i] < -pi) theta[i] += tpi;
          else if (theta[i] > pi) theta[i] -= tpi;
          if (theta[i] > hpi) theta[i] = hpi;
          else if (theta[i] < -hpi) theta[i] = -hpi;
          else if (theta[i] > -10000.);
          else goto X;
        }
        a[0].write(90 - cov * theta[0]);
        a[1].write(90 - cov * theta[1]);
        a[2].write(98 - cov * theta[2]);
        a[3].write(90 - cov * theta[3]);
        a[4].write(90 - cov * theta[4]);
        delay(300);
X:;
        if (_x == nx && _y == ny && _z == nz && _th == nth) break;
      }
      delay(300);
      if (b2 == 0) {
        a[5].write(90);
        b3 = 0;
      }
      else if (b3 == 0) {
        b3 = 1;
        digitalWrite(13, 1);
        for (int i = 90; i > 0; i -= 15) {
          Serial.print("!");
          if (digitalRead(8)) {
            i += 20;
            a[5].write(i);
            delay(300);
            for (; i > 0; i -= 4) {
              if (digitalRead(8)) {
                digitalWrite(13, 0);
                i += 4;
                if (i < 0) i = 0;
                a[5].write(i);
                break;
              }
              a[5].write(i);
              delay(150);
            }

          }
          a[5].write(i);
          delay(150);
        }
      }
    }
  }
}
