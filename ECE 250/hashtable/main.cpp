#include <iostream>
#include "Quadratic_hash_table.h"

int main() {
    Quadratic_hash_table<int> ht(10);

    for ( int i = 0; i < 93251534; i += 938200 ) {
        ht.insert( i );
    }

    std::cout << ht << std::endl;

    return 0;
}