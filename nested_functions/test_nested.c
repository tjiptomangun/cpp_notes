#include <stdio.h>

double quardos(double a, double b) {
  double square(double z) {
    return z * z;
  }

  return square(a) * square(b);
}
int main (int argc, char **argv) {
  fprintf(stdout, "quardos (kardus) 3 5 = %f\n", quardos(3, 5));
  
}
