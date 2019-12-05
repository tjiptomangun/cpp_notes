//#include <glib.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <assert.h> 
#include <stdbool.h> 
#include <signal.h> 
#include <string.h> 
#include "lutils.h" 

//static void printCar(Car*);
union_decl (Car, Volvo, Fiat, Ferrari)
union_type (Volvo, int x; double y;)
union_type (Fiat, char *brand, *model;)
union_type (Ferrari, char *brand, *model;)
union_end (Car)


static void testCar (Car *c, char const *value){
  if (c->kind == Volvo) g_assert_cmpstr(value, ==, "3");
  else if (c->kind == Fiat) g_assert_cmpstr(value, ==, "234");
  else if (c->kind == Ferrari) g_assert_cmpstr(value, ==, "Ferrari");
  else g_assert_not_reached(); 
}

static void printCar(Car *c) {
  char temp[40] = {0};
  char *value = c->kind == Volvo  ? (snprintf(temp, 40, "%d", c->Volvo.x) > 0? temp : "")
              : c->kind == Ferrari ? (void) c->Ferrari.brand, c->Ferrari.model
              : c->kind == Fiat ? c->Fiat.model
              : union_fail("Not a valid car type");

  fprintf(stdout, "%s \n", value);
}

static void testUnion() {
  Car c;
  union_set(&c, Volvo, .x = 3, .y = 4);
  printCar(&c);
  union_set(&c, Ferrari, .brand = "Ferrari", .model = "GT");
  printCar(&c);
  union_set(&c, Fiat, .brand = "Fiat", .model = "234");
  printCar(&c);
}
int main(int argc, char **argv) {
  testUnion();
}
