//#include "Search_tree.h"
//#include <cassert>
//
//int devmain() {
//    Search_tree<int> my_tree;
//
//    int const N = 29;
//
//    int data[N] = { //Breadth-first traversal of tree in notes AVL Trees ppt, slide 77
//            51, 30, 69, 18, 42, 63, 87, 12, 24, 36, 45, 57, 66, 81, 93, 15, 21, 27, 33, 39, 48, 54, 60, 75, 84, 90, 96, 72, 78
//    };
//
//    int const M = 8;
//
//    // All of these must cause an AVL rotation...
//    int new_data[M] = {73, 77, 76, 80, 74, 64, 55, 70 };
//
//    for ( int i = 0; i < N; ++i ) {
//        std::cout << my_tree << std::endl;
//        my_tree.insert( data[i] );
//    }
//
//
//
//    for ( int j = 0; j < M; ++j ) {
////        my_tree.clear();
//
////        for ( int i = 0; i < N; ++i ) {
////            std::cout << my_tree << std::endl;
////            my_tree.insert( data[i] );
////        }
//
//        int height = my_tree.height();
//
//        my_tree.insert( new_data[j] );
//
//        std::cout << "Tried to insert: " << new_data[j] << std::endl;
//        std::cout << "Old height: " << height << std::endl;
//        std::cout << "New height: " << my_tree.height() << std::endl;
//        assert( height - my_tree.height() < 2 || height - my_tree.height() > -2 );
//    }
//
//    return 0;
//}