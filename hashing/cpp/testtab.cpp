// FILE: testtab.cpp
// An interactive test program for the first Table ADT.

#include <ctype.h>     // Provides toupper
#include <iostream.h>  // Provides cin, cout
#include <stdlib.h>    // Provides EXIT_SUCCESS and size_t
#include <assert.h>    // Provides assert
#include <math.h>      // Provides log
#include "table.h"    // Provides the Table class

// Struct definition for the RecordType, which has a key and a double.
struct RecordType
{
    int key;
    double data;
};

// PROTOTYPES for functions used by this test program:
void print_menu( );
// Postcondition: A menu of choices for this program has been written to cout.

char get_user_command( );
// Postcondition: The user has been prompted to enter a one character command.
// The next character has been read (skipping blanks and newline characters), 
// and this character has been returned.

/*
RecordType get_record( ); 
// Postcondition: The user has been prompted to enter data for a record. The
// key has been read, echoed to the screen, and returned by the function.

int get_key( );
// Postcondition: The user has been prompted to enter a key for a record. The
// items have been read, echoed to the screen, and returned by the function.
*/

// functions for random numbers generation
void random_init(unsigned int seed); 
double random_fraction( );
double random_real(double low, double high);

// get the load factor from user
float get_load_factor();

// print the "theoretical" average-case searching time
void print_average_case(float load_factor);

int main( )
{
    // Table<RecordType> test;    // A Table that we'll perform tests on
    char choice;               // A command character entered by the user
    // bool found;                // Value returned by find function
    // RecordType result;         // Value returned by find function
    
    cout << "I have initialized an empty Table. Each record in the Table\n";
    cout << "has an integer key and a real number as data." << endl;

    do
    {
        print_menu( );
        choice = toupper(get_user_command( ));
        switch (choice)
        {
	case 'L':
        {          
          const unsigned int MAX_TRIALS = 100; // number of trials 
          unsigned int i, j; 
          RecordType rec, res;
          bool found;  
        
          unsigned int num_entries;
          int number_of_accesses, total_number_of_accesses = 0;  

          float load_factor = get_load_factor( );
          print_average_case(load_factor);
          
          // for every trial do 
	  for(i=1; i<=MAX_TRIALS; i++)
	    { Table<RecordType> test;
              random_init(i);
              num_entries = (unsigned int) (test.CAPACITY * load_factor);
              // insert entries to hash table   
              for(j=0; j< num_entries; j++)
		{
                  rec.data = -1.0;
                  rec.key = int (random_real(0, 99999));                  
                  test.insert(rec);
		}
              random_init(i);
              // search for all entries in hash table
	      for(j=0; j< num_entries; j++)
		{
                  rec.key = int (random_real(0, 99999));
                  // increment the total number of accesses to array
                  number_of_accesses = (int) (test.find(rec.key, found, res));
                  if(found)
                    total_number_of_accesses += number_of_accesses;
                }              
	    }
	    // print out results	  
	    cout << "Experimental average-case behavior: \n" << (float) (total_number_of_accesses) / MAX_TRIALS / num_entries << " elements examined during a successful search" << endl;
	}
	       break;
	       
            case 'Q': cout << "Ridicule is the best test of truth." << endl;
                      break;
		      
            default:  cout << choice << " is invalid. Sorry." << endl;
	       
        }
    }
    while ((choice != 'Q'));

    return EXIT_SUCCESS;
}

void print_menu( )
// Library facilities used: iostream.h
{   
    cout << endl; // Print blank line before the menu
    cout << "The following choices are available: " << endl;
    cout << " L   Enter load factor" << endl;    
    cout << " Q   Quit this test program" << endl;
}

char get_user_command( )
// Library facilities used: iostream.h
{
    char command;

    cout << "Enter choice: ";
    cin >> command; // Input of characters skips blanks and newline character

    return command;
}

void random_init(unsigned int seed)
{
  // cout << "The random number generator is seeded with " << seed << " ..." << endl;
  srand(seed);
}

double random_fraction( )
{
  return rand( ) / double(RAND_MAX);
}

double random_real(double low, double high)
{
  assert(low <= high);
  return low + random_fraction( ) * (high - low);
}

float get_load_factor()
{
  float load_factor;
    
  do
  {
    cout << "Please enter a load factor in the range [0.5,1.0) for hash table simulation: ";
    cin  >> load_factor;
  }
  while (load_factor < 0.5 || load_factor >= 1.0);
  cout << load_factor << " has been read." << endl;
  return load_factor;
}

void print_average_case(float load_factor)
{ 
  assert(load_factor > 0.0 && load_factor < 1.0);

  // Fill in - comment next statement and uncomment the one that follows
  cout << "Theoretical average case behavior : \n" << 0.5*(1.0 + 1.0 / (1.0 - load_factor) ) << " elements examined during a successful search" << endl;

  // cout << "Theoretical average case behavior : \n" << -log(1.0-load_factor) / load_factor << " elements examined during a successful search" << endl;

}



