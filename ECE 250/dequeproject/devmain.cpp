#include <iostream>
#include "Resizable_deque.h"

int maindev() {

    std::cout << "Whats up " << -1 % 5 << std::endl;
    std::cout << "Dev test DequeProject >>>>>" << std::endl;

    std::cout << "===== Constructors =====" << std::endl;

    std::cout << "Regular Constructor" << std::endl;
    std::cout << "Expected: " << std::endl;
    auto r1 = new Resizable_deque<int>();
    std::cout << "Actual new r1: " << std::endl << *r1 << std::endl;

    std::cout << "Regular Constructor with size" << std::endl;
    std::cout << "Expected: " << std::endl;
    auto r4 = new Resizable_deque<int>(200);
    std::cout << "Actual: " << r4 << std::endl;

    std::cout << "Copy Constructor" << std::endl;
    std::cout << "Expected: " << std::endl;
    auto r2 = new Resizable_deque<int>(*r1);
    std::cout << "Actual: " << r1 << std::endl;

    std::cout << "Move Constructor" << std::endl;
    r2->push_front(50);
    delete r2;
    std::cout << "Before all stuff: " << *r2 << std::endl;
    auto r3 = new Resizable_deque<int>(std::move(*r2));
    std::cout << "Expected r2 to be empty and r3 to be r2" << std::endl;
    std::cout << "Actual r2: " << *r2 << std::endl <<  "Actual r3:" << *r3 << std::endl;


    std::cout << "===== Mutators =====" << std::endl;

    std::cout << "Pop front throw" << std::endl;
    std::cout << "Expected r1 to throw for pop front: " << 0 << std::endl;
    try {
        r1->pop_front();
    } catch (const underflow &e) {
        std::cout << "r1 throws for empty pop front: " << std::endl;
    }

    std::cout << "Pop back throw" << std::endl;
    std::cout << "Expected r1 to throw for pop back: " << 0 << std::endl;
    try {
        r1->pop_back();
    } catch (const underflow& underflow) {
        std::cout << "r1 throws for empty pop back: " << std::endl;
    }

    std::cout << "Clear throw" << std::endl;
    std::cout << "Expected r1 to clear its elements" << std::endl;
    r1->clear();
    std::cout << "Actual r1 clear result: " << std::endl;

    std::cout << "Push front function" << std::endl;
    for (int j = 0; j < 100; j++) {
        r1->push_front(j);
    }
    std::cout << "Actual new r1: " << *r1 << std::endl;

    std::cout << "Will clear the deque: " << std::endl;
    r1->clear();

    std::cout << "Push back function" << std::endl;
    for (int j = 0; j < 100; j++) {
        r1->push_back(j);
    }
    std::cout << "Actual new r1: " << *r1 << std::endl;

    std::cout << "Pop back function" << std::endl;
    for (int k = 0; k < 4; k++) {
        r1->pop_back();
    }
    std::cout << "Actual new r1: " << *r1 << std::endl;

    std::cout << "Pop front function" << std::endl;
    for (int k = 0; k < 4; k++) {
        r1->pop_front();
    }
    std::cout << "Actual new r1: " << *r1 << std::endl;

    std::cout << "===== Accesors =====" << std::endl;

    std::cout << "Size function" << std::endl;
    std::cout << "Expected r1 to have a size of: " << 0 << std::endl;
    std::cout << "Actual r1 size: " << r1->size() << std::endl;

    std::cout << "Capacity function" << std::endl;
    std::cout << "Expected r1 to have a capacity of: " << 16 << std::endl;
    std::cout << "Actual r1 capacity: " << r1->capacity() << std::endl;

    std::cout << "Empty function" << std::endl;
    std::cout << "Expected r1 to be empty and: " << true << std::endl;
    std::cout << "Actual r1 empty result: " << r1->empty() << std::endl;

    std::cout << "Front function" << std::endl;
    std::cout << "Expected r1 can throw for empty front: " << std::endl;
    try {
        r1->front();
    } catch (const underflow&e) {
        std::cout << "r1 underflow for front when empty success: " << std::endl;
    }

    std::cout << "Back function" << std::endl;
    std::cout << "Expected r1 can throw for empty back: " << std::endl;
    try {
        r1->back();
    } catch (const underflow&e) {
        std::cout << "r1 underflow for front when empty success: " << std::endl;
    }


    std::cout << "===== End of testing =====" << std::endl;

    return 0;
}
