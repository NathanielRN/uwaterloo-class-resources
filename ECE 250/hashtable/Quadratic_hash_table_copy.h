/*****************************************
 * Instructions
 *  - Replace 'uwuserid' with your uWaterloo User ID
 *  - Select the current calendar term and enter the year
 *  - List students with whom you had discussions and who helped you
 *
 * uWaterloo User ID:  enruizno @uwaterloo.ca
 * Submitted for ECE 250
 * Department of Electrical and Computer Engineering
 * University of Waterloo
 * Calender Term of Submission:  (Winter) 2018
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *
 * The following is a list of uWaterloo User IDs of those students
 * I had discussions with in preparing this project:
 *    gnardyee
 *    zsnafzig
 *    cxlei
 *    y327lin
 *
 * The following is a list of uWaterloo User IDs of those students
 * who helped me with this project (describe their help; e.g., debugging):
 *    -
 *****************************************/

#ifndef DOUBLE_HASH_TABLE_H
#define DOUBLE_HASH_TABLE_H

#ifndef nullptr
#define nullptr 0
#endif

#include "Exception.h"
#include "ece250.h"

enum bin_state_t { UNOCCUPIED, OCCUPIED, ERASED };

/****************************************************
 * Quadratic_hash_table Class
 *
 * Implementation of a hash table with a modulo hash function
 *
 *  Public classes, fields and methods
 *
 *      Constructors
 *
 *          new             constructor     Create an new hash table with default size 2 ^ 5
 *
 *          copy(obj)       constructor     Takes in a hash value and returns a copy of it
 *
 *      Deconstructors
 *
 *          delete          deconstructor   Delete the hash table's contents including info about the bins
 *
 *      Accessors
 *
 *          capacity()                      Returns the how many elements the has table is able to store
 *
 *          load_factor()                   Returns the ratio of currently occupied and erased bins to the capacity
 *                                          of the hash table
 *
 *          size()                          Returns the variable tracking how many elements are currently
 *                                          stored in the hash table
 *
 *          empty()                         Returns true if the hash table has no elements stored, false otherwise
 *
 *          member(obj)                     Returns true if the passed in object is stored by the hash table, false
 *                                          otherwise
 *
 *          bin(int)                        Returns the contents at the bin with index matching the passed in value
 *
 *      Mutators
 *
 *          clear()                         Sets all the elements of the hash_table to 0 and sets all the information
 *                                          of the individual bins to be "unoccupied"
 *
 *          insert(Type const &obj)         Attempts to insert the passed in object into the hash table, ignores
 *                                          duplicates and throws an error if the hash table is full
 *
 *          erase(Type const &obj)          Attempts to erase the specified object from the hash_table, returns true
 *                                          if it is found in the hash_table and false otherwise
 *
 *  Private classes, fields and methods
 *
 *      Fields
 *
 *          count                           Field that stores how many elements are stored in the hash table
 *
 *          power                           Two raised to this power is how many elements the hash table can store
 *
 *          array_size                      Stores the capacity of the hash table
 *
 *          mask
 *
 *          array                           An array which is the hash table itself where the indices of this array
 *                                          corresponds to the indices of the bins
 *
 *          occupied                        An array which stores information on the state of every bin which can be one
 *                                          of the enum cases of bin_state_t (UNOCCUPIED, ERASED, OCCUPIED)
 *
 *          used_bins                       Variable which stores the number of used bins in the past including erased
 *                                          and currently occupied bins, used to determine the load factor
 *
 *      Methods
 *
 *          hash(obj)                       Returns the hash value used to store the passed in object
 *
 ****************************************************/
template <typename Type>
class Quadratic_hash_table {
	private:
		int count;
		int power;
		int array_size;
		int mask;
		Type *array;
		bin_state_t *occupied;

		int hash( Type const & ) const;

		int used_bins;

	public:
		Quadratic_hash_table( int = 5 );
		~Quadratic_hash_table();
		int size() const;
		int capacity() const;
		double load_factor() const;
		bool empty() const;
		bool member( Type const & ) const;
		Type bin( int ) const;

		void print() const;

		void insert( Type const & );
		bool erase( Type const & );
		void clear();

	// Friends

	template <typename T>
	friend std::ostream &operator<<( std::ostream &, Quadratic_hash_table<T> const & );
};

template <typename Type>
int Quadratic_hash_table<Type>::hash(Type const &obj) const {

    // We use a static cast to converted any item to an int so we can hash it to its appropriate hash value
	auto convertedObject = static_cast<int>(obj);

    // Our hash is to use the modulus of the objected casted as an int, because 2^ M is a power of two, the mask which
    // is 2^M - 1 is a bit string with a 0 in the place that represent 2^M and all ones after it. Doing a logical and
    // means we take our converted Object's binary representation and only keep those bits that are in bit positions
    // 2 ^ (M - 1) down to 2 ^ 0. This gives the modulus with respect to 2^M (the hash table size) and should be faster
    // than taking the modulus with base ten digits which would require divisions and more multiplication.
	int hashedValue = convertedObject & mask;

    // If the hash is negative, we add the hash table size to make it positive and in the range of 0 to
    // (hash table size) - 1
	if (hashedValue < 0) {
		hashedValue += array_size;
	}

	return hashedValue;
}

template <typename Type>
Quadratic_hash_table<Type>::Quadratic_hash_table( int m ):
count( 0 ), power( m ),
array_size( 1 << power ),
mask( array_size - 1 ),
array( new Type[array_size] ),
occupied( new bin_state_t[array_size] ),
used_bins( 0 ) {

    if (m < 0) {
        power = 5;
        array_size = 1 << power;
        mask = array_size - 1;
        array = new Type[array_size];
        occupied = new bin_state_t[array_size];
    }

	for ( int i = 0; i < array_size; ++i ) {
		occupied[i] = UNOCCUPIED;
	}
}

template <typename Type>
Quadratic_hash_table<Type>::~Quadratic_hash_table() {

    // Deletes the two instances that allocated memory so there are no memory leaks
    delete[] array;
    delete[] occupied;
}

template <typename Type>
int Quadratic_hash_table<Type>::size() const {

    // Returns the value of the field tracking how many elements are stored in the hash table
	return count;
}

template <typename Type>
int Quadratic_hash_table<Type>::capacity() const {

    // Returns the unchanging variable that tracks how many elements can be stored in the hash table
	return array_size;
}

template <typename Type>
double Quadratic_hash_table<Type>::load_factor() const {

    // Returns the ratio of bins marked as occupied and erased to the number elements that can be stored in the hash
    // table
	return static_cast<double>(used_bins)/static_cast<double>(array_size);
}

template <typename Type>
bool Quadratic_hash_table<Type>::empty() const {

    // Compares the variable tracking the number of elements in the hash table to 0 and so returns true if the number
    // of stored elements is 0 by returning the result of the evaluation
	return count == 0;
}

template <typename Type>
bool Quadratic_hash_table<Type>::member(Type const &obj) const {

	// Retrieve the hash index for this object
	int hashIndex = hash(obj);

	// Using the hash index for this object, start searching for it at the bin with corresponding index
	int bin = hashIndex;
	for(int i = 0; i < array_size; i++) {
		// If we haven't found it yet, we use quadratic probing to ensure we check all elements of the hash table
		// because it could be anywhere
		bin += i;

		// Check to see if the item in the bin is the object we are looking for. If it is then we check if that
		// bin is marked as occupied as seen by the hash table. If it is then we return true because we have found
		// it. Checking for the object first slightly reduces the number of comparisons we make since there are no
		// duplicates in the has table but likely more than one occupied bin.
		if (array[bin & mask] == obj && occupied[bin & mask] == OCCUPIED) {
			return true;
		}
	}

	// If we managed to check every element of the array and still not find it, return false to indicate that it
	// is not in the hash table
	return false;
}

template <typename Type>
Type Quadratic_hash_table<Type>::bin(int target_number) const {

    // If the value is allowed, return the contents of the hash table at the bin with this matching index
	return array[target_number];
}

template <typename Type>
void Quadratic_hash_table<Type>::print() const {

}

template <typename Type>
void Quadratic_hash_table<Type>::insert(Type const &obj) {
	// If the tracking size variable equals our capacity, we are full so we won't even try to insert
	if (size() == capacity()) {
		throw overflow();
	}

	// We store the original hashIndex because we may want to restart our probe from this point
	int hashIndex = hash(obj);

	// We start searching for the existence of our object at the hash index returned by our hash function
	int bin = hashIndex;

	// We first assure ourselves that the object does not exist in the hash table
	for (int i = 0; i < array_size; i++) {
		// Starting with no offset, we check at the bin the object should be at. If we are on a new
		// iteration, that means we're not done yet so each iteration adds the an incrementing offset and so uses
		// quadratic probing to make the jumps that help avoid primary clusters but is consistent with the jumps
		// we would have made for insertion
		bin += i;
		if (occupied[bin & mask] == UNOCCUPIED) {
			// If we encounter an unoccupied bin we are certain the object cannot exist past this point otherwise it
			// would have populated this bin. Further this bin means there is space for the object so we assert that.
			// We break out of the loop and continue on to insert the value
			break;
		} else if (occupied[bin & mask] == OCCUPIED) {
			// If in our quadratic probing we find an occupied bin, we must check if the value we are trying to store is
			// already there and return early if it is
			if (array[bin & mask] == obj) {
				return;
			}
		}
	}

	// We reset the bin value so we start probing at the correct place
	bin = hashIndex;

	// The above loop asserted that there would be space for our object so we will eventually find an erased or
	// unoccupied bin
	for (int i = 0; i < array_size; i++) {
		// Starting with no offset, we check at the bin the object should be at. If we are on a new
		// iteration, that means we're not done yet so we add the correct new offset and use quadratic probing to
		// make the jumps that help avoid primary clusters but is consistent with the jumps we will use to insert
		bin += i;
		if (occupied[bin & mask] != OCCUPIED) {
			// We are at an unoccupied or erased node and our obj can go in this index be stored in this bin

			// If the bin was originally unoccupied, we have to increment our used_bins counter because from now on
			// the bin will either be occupied or erased
			if (occupied[bin & mask] == UNOCCUPIED) {
				used_bins++;
			}

			// We will change the state of this bin to occupied since we can fill its contents with the passed in object
			occupied[bin & mask] = OCCUPIED;

			// Put the passed in object into the bin
			array[bin & mask] = obj;

			// Increment the variable tracking the number of variables in the hash table
			count++;
			return;
		}
	}

}

template <typename Type>
bool Quadratic_hash_table<Type>::erase(Type const &obj) {

	// We start searching for the object we are to erase at the hash index of the object returned by our hash function
	int bin = hash(obj);
	for (int i = 0; i < array_size; i++) {

		// Starting with no offset, we check at the bin the object should be at. Successful loops will add the
		// appropriate offsets so that we use quadratic probing to make the jumps that insert would have made to
		// insert the element in the first place
		bin += i;

		if (occupied[bin & mask] == UNOCCUPIED) {
			// With the same hash index, inserting this object would have made the same jumps and would have populated this
			// bin if it were unoccupied, that fact that it isn't means that our object has never passed by here
			return false;
		} else if (occupied[bin & mask] == OCCUPIED) {

			// If we are at an occupied bin, we check if the item stored at this bin is our object
			if (array[bin & mask] == obj) {

				// Once we find it, we zero-out the bin of the hash table to remove its contents
				array[bin & mask] = 0;

				// We decrement the variable tracking the number of elements stored by the hash table
				count--;

				// We update the array tracking information about the bins of the hash table to reflect the fact that
				// this bin's contents have been erased
				occupied[bin & mask] = ERASED;
				return true;
			}
		}
	}

	// If we checked all the elements and didn't find our object, we return false to indicate the element was not in
	// the hash table
	return false;
}

template <typename Type>
void Quadratic_hash_table<Type>::clear() {

    // We reset the variables tracking the number of elements stored and the number of bins used since all elements will
    // be removed
    used_bins = 0;
    count = 0;

    for ( int i = 0; i < array_size; ++i ) {
        // We must iterate through all elements of the hash table and zero out all the bins to remove their content
        // as well as update the array tracking the values to indicate that all bins are unoccupied and don't need
        // to keep "erased" information since there are no values past it to jump to
        occupied[i] = UNOCCUPIED;
    }
}

template <typename T>
std::ostream &operator<<( std::ostream &out, Quadratic_hash_table<T> const &hash ) {
	for ( int i = 0; i < hash.capacity(); ++i ) {
		if ( hash.occupied[i] == UNOCCUPIED ) {
			out << "- ";
		} else if ( hash.occupied[i] == ERASED ) {
			out << "x ";
		} else {
			out << hash.array[i] << ' ';
		}
	}

	return out;
}

#endif