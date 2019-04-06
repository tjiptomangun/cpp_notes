// FILE: table.h
// TEMPLATE CLASS PROVIDED: Table<RecordType>
//   This class is a container template class for a Table of records.
//   The template parameter, RecordType, is the data type of the records in the
//   Table. It may any type with a default constructor, a copy constructor,
//   an assignment operator, and an integer member variable called key.
//
// MEMBER CONSTANT for the Table<RecordType> template class:
//   static const size_t CAPACITY = _____
//     Table::CAPACITY is the maximum number of records that a Table can hold.
//
// CONSTRUCTOR for the Table<RecordType> template class:
//   Table( )
//     Postcondition: The Table has been initialized as an empty Table.
//
// MODIFICATION MEMBER FUNCTIONS for the Table<RecordType> class:
//   void insert(const RecordType& entry)
//     Precondition: entry.key >= 0. Also if entry.key is not already a key in
//     the table, then the Table has space for another record
//     (i.e., size( ) < CAPACITY).
//     Postcondition: If the table already had a record with a key equal to
//     entry.key, then that record is replaced by entry. Otherwise, entry has
//     been added as a new record of the Table.
//
//   void remove(int key)
//     Postcondition: If a record was in the Table with the specified key, then
//     that record has been removed; otherwise the table is unchanged.
//
// CONSTANT MEMBER FUNCTIONS for the Table<RecordType> class:
//   bool is_present(const Item& target) const
//     Postcondition: The return value is true if there is a record in the
//     Table with the specified key. Otherwise, the return value is false.
//
//   void find(int key, bool& found, RecordType& result) const
//     Postcondition: If a record is in the Table with the specified key, then
//     found is true and result is set to a copy of the record with that key.
//     Otherwise found is false and the result contains garbage.
//
//    size_t size( ) const
//      Postcondition: Return value is the total number of records in the
//      Table.
//
//  VALUE SEMANTICS for the Table<RecordType> template class:
//    Assignments and the copy constructor may be used with Table objects.

#ifndef TABLE1_H
#define TABLE1_H

#include <stdlib.h>    // Provides size_t

    template <class RecordType>
    class Table
    {
    public:
        // MEMBER CONSTANT
        enum { CAPACITY = 811 }; // Or: static const size_t CAPACITY = 811;
        // CONSTRUCTOR
        Table( );
        // MODIFICATION FUNCTIONS
        void insert(const RecordType& entry);
        void remove(int key);
        // CONSTANT FUNCTIONS
        bool is_present(int key) const;
        size_t find(int key, bool& found, RecordType& result) const;
        size_t size( ) const { return used; }
    private:
	// MEMBER CONSTANTS
	// These are used in the key field of special records
	enum { NEVER_USED = -1 }; // Or: static const int NEVER_USED = -1;
	enum { PREVIOUSLY_USED = -1 }; // Or: static const int PREVIOUSLY_USED = -2;
        RecordType data[CAPACITY];
        size_t used;
        // HELPER FUNCTIONS
        size_t hash(int key) const;
        size_t hash2(int key) const;
        size_t next_index(size_t index) const;
        size_t next_index2(size_t index, int key) const;
        size_t find_index(int key, bool& found, size_t& i) const;
    };

#include "table.tem" // Include the implementation

#endif
