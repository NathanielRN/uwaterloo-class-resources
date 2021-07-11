/*****************************************
 * UW User ID:  enruizno
 * Submitted for ECE 250
 * Semester of Submission:  (Winter) 2018
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided  code.
 *****************************************/

#ifndef DYNAMIC_DEQUE_H
#define DYNAMIC_DEQUE_H

#include "Exception.h"
/****************************************************
 * Resizable_deque Class
 *
 * Class to create and manipulate a resizable deque data structure.
 *
 *  Constructors
 *
 *   new             constructor    create a deque with the default array of 16
 *   new: n          constructor    create a deque with an array size of n minimum of 16
 *   copy         constructor   create a copy of the elements of the passed in deque
 *                          and assign them to this newly created deque, return this deque
 *
 *  Accessors
 *
 *   front()                  Returns the element at the front of the deque using the ifront
 *                          index.
 *   back()                      Returns the element at the back of the deque using the iback
 *                          index.
 *   size()                     Returns the variable tracking the current size of the deque.
 *   empty()                    Evaluates whether the current size tracking variable is 0 and
 *                          returns the result of the boolean expression
 *   capacity()                   Returns the variable that tracks the current capacity of the
 *                          deque.
 *
 *  Mutators
 *
 *   swap(deque)                the passed in deque has all its values swapped with the
 *                          current deque where the old deques values are this deques
 *                          values now
 *   push_front(object)           push an object to the front, one before the ifront index
 *   push_back(object)            push an object to the back, one after the iback index
 *   pop_front()                pop the element at the front of the deque and update the front
 *                          index
 *   pop_back()                 pop the element at the back of the deque and update the back
 *                          index
 *   clear()                       Resets all the member variables to a default and reinitializes
 *                          the array with empty data.
 *
 *  Private variables
 *
 *   array                   The array that contains a reference to the deque we have.
 *                          Through indexing we can access the elements of the deque at the
 *                          front or back.
 *   ifront                      The index that tracks where the front of the deque is within the
 *                          elements of the array.
 *   iback                          The index that tracks where the back of the deque is within the
 *                          elements of the array.
 *   deque_size                     A variable that tracks how many elements are in the deque.
 *   initial_array_capacity          The variable that tracks the size that the deque starts at. This
 *                          must be more than 16.
 *   array_capacity                The variable that tracks the changing size of the deque. It
 *                          cannot go below the inital array capacity.
 *
 ****************************************************/
template <typename Type>
class Resizable_deque {
public:
   Resizable_deque( int = 16 );
   Resizable_deque( Resizable_deque const & );
   Resizable_deque( Resizable_deque && );
   ~Resizable_deque();

   Type front() const;
   Type back() const;
   int size() const;
   bool empty() const;
   int capacity() const;

   void swap( Resizable_deque & );
   Resizable_deque &operator=( Resizable_deque const& );
   Resizable_deque &operator=( Resizable_deque && );
   void push_front( Type const & );
   void push_back( Type const & );
   void pop_front();
   void pop_back();
   void clear();

private:
   // Your member variables
   Type *array;
   int ifront;
   int iback;
   int deque_size;
   int inital_array_capacity;
   int array_capacity;

   // Any private member functions
   //   - helper functions for resizing your array?

   // Friends

   template <typename T>
   friend std::ostream &operator<<( std::ostream &, Resizable_deque<T> const & );
};

/////////////////////////////////////////////////////////////////////////
//                   Constructors and Destructors                      //
/////////////////////////////////////////////////////////////////////////

/* This constructor initializes all the values to some default value and then creates a new empty
 * array for the deque. This is the most basic constructor and it will start the back index one
 * before the front index. This indexing occurs only during the empty case and for all other cases
 * it is correctly set. The capacity related variables will follow the passed in capacity, if the
 * passed in value is less than 16 it will default it to a value of 16. And if the capacity is not
 * specified this function will use default initialization to start the capacity at 16. Finally it
 * returns the created deque.*/
template <typename Type>
Resizable_deque<Type>::Resizable_deque( int n ):
      array(new Type[n]),
      ifront(0),
      iback(-1),
      deque_size(0),
      inital_array_capacity(n),
      array_capacity(n)
// Your initalization list
{
   if (n < 16) {
      inital_array_capacity = 16;
      array_capacity = 16;
      delete[] array;
      array = new Type[16];
   }
}

// Copy Constructor
/* This array will copy the passed in deque without modifying it. It first creates a basic deque
 * in its initialization list. This basic deque follows the values that belong to the passed in
 * deque. First it creates its array with the same capacity as the passed in deque, and capacity
 * the same as the passed in deque. Next, the body will iterate through all the elements in the
 * deque and copy them over into this deque's new array. It uses an offset from the passed in
 * deque's ifront variable to reorder the passed in deque's elements so that the start of the new
 * array has the first element and the next elements follow after that. Even through ifront is
 * correct at 0, iback will depend on whether the passed in array is empty. It will correctly be at
 * -1 if the array is empty, or the size - 1 if it is not empty because of the reorder we did with
 * the elements of the deque. Finally the constructor returns the new deque.*/
template <typename Type>
Resizable_deque<Type>::Resizable_deque( Resizable_deque const &deque ):
      array(new Type[deque.capacity()]),
      ifront(0),
      iback(deque.size() - 1),
      deque_size(deque.size()),
      inital_array_capacity(deque.inital_array_capacity),
      array_capacity(deque.capacity())
{
   for(int i = 0; i < deque.size(); i++) {
      array[i] = deque.array[(deque.ifront + i) % deque.array_capacity];
   }
}

// Move Constructor
/* This constructor also begins by creating a basic deque. When a deque gets passed in through this
 * move constructor it is understood that the passed in deque will be modified and this new deque
 * will now hold all the new variables instead. To accomplish this we use this functions swap
 * function. That swap function changes ownership of the passed in deque's variables to be passed
 * on to this deque and in all respects it becomes the passed in deque. Finally it returns this new
 * deque and has replaced the passed in deque with in initial basic deque that will be destroyed by
 * the OS. */
template <typename Type>
Resizable_deque<Type>::Resizable_deque( Resizable_deque &&deque ):
      array(new Type[deque.capacity()]),
      ifront(0),
      iback(-1),
      deque_size(0),
      inital_array_capacity(1),
      array_capacity(1)
{
   this->swap(deque);
}

// Destructor
/* This destructor is responsible for release any memory that has been held by the class back to
 * the operating system. The only variable it needs to get rid of is the array pointer which holds
 * a reference to the deque. So when delete is called on this deque, this function will delete all
 * the individual members of the array and ensure no access to memory is lost and no memory leaks
 * occur. It finally releases the memory it was occupying itself. */
template <typename Type>
Resizable_deque<Type>::~Resizable_deque() {
   delete[] array;
}

/////////////////////////////////////////////////////////////////////////
//                     Public Member Functions                         //
/////////////////////////////////////////////////////////////////////////

/* The function returns the value stored by the variable tracking the size of the deque. Because
 * the const keyword makes it so that member variables can't be changed through the access provided
 * by this function. This is a safe way to return the deque size without accidentally modifying it
 * because this function would prevent that. */
template <typename Type>
int Resizable_deque<Type>::size() const {
   return deque_size;
}

/* The function returns the variable keeping track of the deque capacity. Using the const keyword
 * it provides a safe way of accessing the private member variable without accidently modifying it
 * because this function prevents that. */
template <typename Type>
int Resizable_deque<Type>::capacity() const {
   return array_capacity;
}

/* This function determines whether the deque is currently empty or not. It does so by using its
 * access to the private variable tracking the size and seeing if it is equal to 0. The return
 * value of this function is then the boolean result of this operation and because of its const
 * keyword it provides a safe way of seeing whether it is empty since it does not allow modification
 * of this class's member variables through the provided access. */
template <typename Type>
bool Resizable_deque<Type>::empty() const {
   return deque_size == 0;
}

/* The front function will return the element at the front of the deque using the ifront index.
 * By using the ifront index to access a certain element in the array that stores the elements of
 * the deque, it can successfully return the most front element relative to the other elements in
 * the deque. This function can return an exception if the deque is empty, because you can't access
 * elements that aren't there. It does so using the empty function. */
template <typename  Type>
Type Resizable_deque<Type>::front() const {
   if (empty()) {
      throw underflow();
   }
   return array[ifront];
}

/* The back element returns the element that is at the back of the deque relative to the other
 * elements in the deque. It uses the iback tracking variable to find this by accessing the array
 * at this index and since the array is the deque it will return the proper value. If the deque is
 * empty it will throw an underflow error because you can't access an element that's not there. */
template <typename  Type>
Type Resizable_deque<Type>::back() const {
   if (empty()) {
      throw underflow();
   }
   return array[iback];
}

/* This swap function uses the standard library's swap function to swap all the relevant values of
 * passed in the deque with the values of this deque. At the end of the function the ownership
 * of all the specified variables has been changed.*/
template <typename Type>
void Resizable_deque<Type>::swap( Resizable_deque<Type> &deque ) {
   // Swap the member variables
   std::swap( ifront, deque.ifront );
   std::swap( iback, deque.iback );
   std::swap( array_capacity, deque.array_capacity );
   std::swap( deque_size, deque.deque_size );
   std::swap( array, deque.array );
    std::swap( inital_array_capacity, deque.inital_array_capacity);
}

template <typename Type>
Resizable_deque<Type> &Resizable_deque<Type>::operator=( Resizable_deque<Type> const &rhs ) {
   // This is done for you...
   Resizable_deque<Type> copy( rhs );
   swap( copy );

   return *this;
}

template <typename Type>
Resizable_deque<Type> &Resizable_deque<Type>::operator=( Resizable_deque<Type> &&rhs ) {
   // This is done for you...
   swap( rhs );

   return *this;
}

/* This function will push a passed in object to the front of the deque. Normally, it will use
 * modulus arithmetic to ensure the ifront doesn't go outside the bounds specified by the tracking
 * capacity variable when it decreases ifront by one to go to the position where this new object
 * will now live (before all the other elements) so that it is properly at the front relative to the
 * elements in the deque. I use the updated ifront index and place the element into this index of
 * the array variable holding the deque elements. I then increase the variable tracking the size of
 * the deque. In the special case where the deque is full of elements, before any of the steps
 * outlined above, I create a bigger array that has a size double the capacity of the current array.
 * I then iterate through the elements of the deque and using modulus arithmetic again to ensure I
 * don't go out of bounds. I assign all the elements in the current array to the newly created bigger
 * array. I redo the ordering so that the first element is at index 0. I then delete the current
 * array's contents so that its memory is released back to the system, and assign the tracking array
 * variable to now be this array with a bigger capacity. As cleanup I update the array capacity, and
 * update the ifront and iback vairables to be 0 and "relative to the deque size" respectively.*/
template <typename Type>
void Resizable_deque<Type>::push_front( Type const &obj ) {
   if (empty()) {
      push_back(obj);
      return;
   } else if (deque_size == array_capacity) {
      auto bigger_array = new Type[array_capacity*2];
      for (int i = 0; i < deque_size; i++) {
         bigger_array[i+1] = array[(ifront + i) % array_capacity];
      }
      delete[] array;
      array = bigger_array;
      array_capacity *= 2;
      ifront = 1;
      iback = deque_size;
   }
   ifront = ((ifront - 1) + array_capacity ) % array_capacity;
   array[ifront] = obj;
   deque_size++;
}

/* This function will push a passed in object to the back of the deque. Normally, it will use
 * modulus arithmetic to ensure the iback doesn't go outside the bounds specified by the tracking
 * capacity variable and will increase back by one to go to the position where this new object
 * will now be placed so that it is properly at the back relative to the elements in the deque. I
 * use the updated iback index and place the element into this index of the array variable holding
 * the deque elements. I then increase the variable tracking the size of the deque by one. In the
 * special case where the deque is full of elements, before any of the steps outlined above, I
 * create a bigger array that has a size double the capacity of the current array. I then iterate
 * through the elements of the deque and using modulus arithmetic again to ensure I don't go out of
 * bounds. I assign all the elements in the current array to the newly created bigger array. I redo
 * the ordering so that the first element is at index 0. I then delete the current array's contents
 * so that its memory is released back to the system, and assign the tracking array variable to now
 * be this array with a bigger capacity. As cleanup I update the array capacity, and update the
 * ifront and iback variables to be 0 and "relative to the deque size" respectively.*/
template <typename Type>
void Resizable_deque<Type>::push_back( Type const &obj ) {
   if (deque_size == array_capacity) {
      auto bigger_array = new Type[array_capacity*2];
      for (int i = 0; i < deque_size; i++) {
         bigger_array[i] = array[(ifront + i) % array_capacity];
      }
      delete[] array;
      array = bigger_array;
      array_capacity *= 2;
      ifront = 0;
      iback = deque_size - 1;
   }
   iback = ((iback + 1) + array_capacity ) % array_capacity;
   array[iback] = obj;
   deque_size++;
}

/* This function is responsible for removing the element at the relative front of the deque. If it
 * is empty this function expectantly throws an underflow error. If not, it zeros out the element
 * at the space pointed to by the ifront index and properly increments front using modulus
 * arithmetic to ensure the ifront remains within the bounds set by the array capacity and properly
 * moves forward to get the 2nd place element relative to the positions in the deque. Finally it
 * will decrease the deque's size. If the removal caused the deque size to go smaller than one
 * fourth of the current capacity, and if the current capacity is strictly larger than the initial
 * capacity, then we will decrease the current capacity. Since the capacity only doubles or gets
 * halved, we know it will be larger than or at minimum equal to the initial capacity. To accomplish
 * this I create a smaller array of the same type (on the heap so it exits outside the scope of this
 * function) with a length equal to half the current capacity. I then iterate through the elements
 * of the current deque and assign them into my new small array. I then delete the current array and
 * reassign the array to be this new smaller array. And as cleanup since I know I restructured the
 * ordering so that the deque starts at the 0 index of the new smaller array, I update the ifront
 * to be 0 and iback to be relative to the deque size. Also, I must update the current capacity to
 * be half of what it used to be.*/
template <typename Type>
void Resizable_deque<Type>::pop_front() {
   if (empty()) {
      throw underflow();
   }

   array[ifront] = 0;
   ifront = ((ifront + 1) + array_capacity ) % array_capacity;
   deque_size--;
   if (deque_size <= (array_capacity / 4) && (array_capacity > inital_array_capacity)) {
      auto smaller_array = new Type[array_capacity / 2];
      for (int i = 0; i < deque_size; i++) {
         smaller_array[i] = array[(ifront + i) % array_capacity];
      }
      delete[] array;
      array = smaller_array;
      array_capacity /= 2;
      ifront = 0;
      iback = size() - 1;
   }
}

/* This function will be responsible to removing an element of the deque at the back using the
 * tracking iback variable. If the deque is empty this function will correctly throw an
 * underflow exception. Otherwise, it will zero out whatever is current at the back of the deque
 * by accessing the array variable at the index specified by iback. It will then use modulus
 * arithmetic to ensure that the subtraction of iback back to the second last element of the deque
 * doesn't escape the bounds of the deque array using the tracked array capacity value. Upon
 * successfully popping it will decrement the deque size. Next follows the case where is the size
 * is less than or equal to a quarter of the deque capacity, and if the capacity is not at its
 * minimum length of the initial capacity, where we then halve the size of the capacity. I create a
 * new array of the passed in Type with length that is half the current capacity. I iterate through
 * the elements of the current array and reassign them into this smaller array. I then delete the
 * old array and then reassign our tracking array variable to be this new smaller array. The
 * ordering is redone so that the relatively first element in the deque is at index 0. And the
 * member variables capacity, ifront, and iback are updated appropriately.*/
template <typename Type>
void Resizable_deque<Type>::pop_back() {
   if (empty()) {
      throw underflow();
   }

   array[iback] = 0;
   iback = ((iback - 1) + array_capacity) % array_capacity;
   deque_size--;
   if (deque_size <= (array_capacity / 4) && (array_capacity > inital_array_capacity)) {
      auto smaller_array = new Type[array_capacity / 2];
      for (int i = 0; i < deque_size; i++) {
         smaller_array[i] = array[(ifront + i) % array_capacity];
      }
      delete[] array;
      array = smaller_array;
      array_capacity /= 2;
      ifront = 0;
      iback = size() - 1;
   }
}

/* This function is used to cleanup the deque. It will delete all the elements of the array so that
 * the memory space is release back to the operation system to be used. It then re assigns the array
 * variable that is tracking the deque to be a new empty array of the same type as before but with
 * the original initial array capacity thus resetting any potential multiplications or divisions.
 * Also it resets the relevant member variables such as the variables tracking the changing ifront
 * and iback indices, the deque_size and the array_capacity.*/
template <typename Type>
void Resizable_deque<Type>::clear() {
   delete[] array;
   array = new Type[inital_array_capacity];
   ifront = 0;
   iback = -1;
   deque_size = 0;
   array_capacity = inital_array_capacity;
}
/////////////////////////////////////////////////////////////////////////
//                      Private member functions                       //
/////////////////////////////////////////////////////////////////////////

// Enter any private member functios (helper functions) here


/////////////////////////////////////////////////////////////////////////
//                               Friends                               //
/////////////////////////////////////////////////////////////////////////

// You can modify this function however you want:  it will not be tested

template <typename T>
std::ostream &operator<<( std::ostream &out, Resizable_deque<T> const &list ) {
   if (list.empty()) {
      out << "This list is empty" << std::endl;
   }
   out << "Front at: " << list.ifront << std::endl;
   out << "Back at: " << list.iback << std::endl;
   out << "Size: " << list.size() << std::endl;
   out << "Capacity: " << list.capacity() << std::endl;
   out << "Initially: " << list.inital_array_capacity << std::endl;
   return out;
}

#endif