#include <cstdlib>
#include <iostream>
#include "Weighted_graph.h"

/*******************************************************************
 *
 * Currently, the instructor's output for the following executions
 * on the machine ecelinux9 is as follows:
 *
 *    $ g++ -O2 windows.cpp
 *
 *    $ time ./a.out 100
 *    14094.5, 2533418
 *    0.090u ...
 *
 *    $ time ./a.out 200
 *    59379.7, 23205244
 *    0.810u ...
 *
 *    $ time ./a.out 300
 *    137440, 85092893
 *    3.247u ...
 *
 *    $ time ./a.out 400
 *    250691, 222524342
 *    9.527u ...
 *
 * The argument to ./a.out is the number of nodes in the graph.
 *
 * The next two numbers are the sum of the minimum spanning trees
 * and the sum of the number of edges checked.
 *
 * Finally, the time in question is the first number followed by
 * a 'u'.  This will indicate how fast your algorithm is.
 *******************************************************************/

int main( int argc, char *argv[] ) {
    if ( argc != 2 ) {
        return 0;
    }

    int N = std::atoi( argv[1] );

    // The random number seed will change
    srand( 10 );

    double total = 0.0;
    int count = 0;

    Weighted_graph g( N );

    for ( int i = 0; i < N*N; ++i ) {
        for ( int j = 0; j < 10; ++j ) {
            int x = rand() % N;
            int y = rand() % N;
            double d = static_cast<double>( rand() ) / static_cast<double>( RAND_MAX );
            g.insert_edge( x, y, d );
        }

        std::pair<double, int> result = g.minimum_spanning_tree();
        total += result.first;
        count += result.second;
    }

    std::cout << total << ", " << count << std::endl;

    return 0;
}