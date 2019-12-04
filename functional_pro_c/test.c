#include <glib.h> 
#include <stdio.h> 
#include <assert.h> 
#include <stdbool.h> 
#include <signal.h> 
#include <string.h> 
#include "lutils.h" 

//static void printCar(Car*);

static void testUnion() {
  Car c;
  union_set (&c, Volvo, .x =3, .y = 4);
  printCar(&c);
  union_set (&c, Ferrari, .brand = "Ferrari");
  printCar(&c);
  union_set (&c, Fiat, .brand = "Fiat", .model="2.3.4");
  printCar(&c);
}

int main(int argc, char **argv) {
}
