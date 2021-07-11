/*
 * Bailey Kyle Thompson
 * github.com/bkthomps
 *
 * Rename the main() in Resizable_deque_driver to something else, then add this
 * file to your compilation list and run it. If no assertion errors occur and
 * the program exits with exit code 0, the tests have passed. To make sure
 * there are no memory leaks, run valgrind.
 */

#include <cassert>
#include <iostream>
#include "Resizable_deque.h"

static void test_exceptions() {
    Resizable_deque<int> a{};
    try {
        a.front();
        assert(false);
    } catch (underflow) {}
    try {
        a.back();
        assert(false);
    } catch (underflow) {}
    try {
        a.pop_front();
        assert(false);
    } catch (underflow) {}
    try {
        a.pop_back();
        assert(false);
    } catch (underflow) {}
}

static void test_default_size() {
    Resizable_deque<int> a{};
    assert(a.capacity() == 16);
    Resizable_deque<int> b{15};
    assert(b.capacity() == 16);
    Resizable_deque<int> c{8};
    assert(c.capacity() == 16);
}

static void test_copy_constructor_and_pop(Resizable_deque<int> a, Resizable_deque<int> b) {
    // Checking pop front
    for (int i = 0; i < 16; i++) {
        a.pop_front();
    }
    assert(a.size() == 17);
    assert(a.capacity() == 64);
    a.pop_front();
    assert(a.size() == 16);
    assert(a.capacity() == 32);
    for (int i = 0; i < 16; i++) {
        a.pop_front();
    }
    assert(a.empty());
    assert(a.size() == 0);
    assert(a.capacity() == 16);
    // Checking pop back
    for (int i = 0; i < 16; i++) {
        b.pop_back();
    }
    assert(b.size() == 17);
    assert(b.capacity() == 64);
    b.pop_back();
    assert(b.size() == 16);
    assert(b.capacity() == 32);
    for (int i = 0; i < 16; i++) {
        b.pop_back();
    }
    assert(b.empty());
    assert(b.size() == 0);
    assert(b.capacity() == 16);
}

static void test_clear(Resizable_deque<int>& a) {
    a.clear();
    assert(a.empty());
    assert(a.capacity() == 16);
    Resizable_deque<int> b{24};
    for (int i = 0; i < 256; i++) {
        b.push_back(i);
    }
    b.clear();
    assert(b.empty());
    assert(b.capacity() == 24);
}

static void test_main() {
    // Pushing back
    Resizable_deque<int> a{};
    assert(a.capacity() == 16);
    for (int i = 0; i < 16; i++) {
        a.push_back(i);
    }
    assert(a.capacity() == 16);
    assert(a.size() == 16);
    a.push_back(-1);
    assert(a.capacity() == 32);
    // Pushing front
    Resizable_deque<int> b{};
    assert(b.capacity() == 16);
    for (int i = 0; i < 16; i++) {
        b.push_front(i);
    }
    assert(b.capacity() == 16);
    assert(b.size() == 16);
    b.push_front(-1);
    assert(b.capacity() == 32);
    // Pushing the other way
    for (int i = 0; i < 15; i++) {
        a.push_front(i);
    }
    assert(a.capacity() == 32);
    assert(a.size() == 32);
    a.push_front(-1);
    assert(a.capacity() == 64);
    assert(a.size() == 33);
    // Pushing the other way
    for (int i = 0; i < 15; i++) {
        b.push_back(i);
    }
    assert(b.capacity() == 32);
    assert(b.size() == 32);
    b.push_back(-1);
    assert(b.capacity() == 64);
    assert(b.size() == 33);
    test_copy_constructor_and_pop(a, b);
    // Verify that the copy constructor was called
    assert(a.capacity() == 64);
    assert(a.size() == 33);
    assert(b.capacity() == 64);
    assert(b.size() == 33);
    test_clear(a);
}

static void test_value_store() {
    // Check pushing to front works
    Resizable_deque<int> a{44};
    for (int i = -150; i < 150; i++) {
        a.push_front(i);
        assert(a.front() == i);
    }
    // Check pushing to back works
    Resizable_deque<int> b{44};
    for (int i = -150; i < 150; i++) {
        b.push_back(i);
        assert(b.back() == i);
    }
    // Verify move constructor
    auto c{std::move(a)};
    assert(c.size() == 300);
    auto d{std::move(b)};
    assert(d.size() == 300);
    // Verify values are there
    for (int i = 149; i >= -150; i--) {
        assert(c.front() == i);
        assert(d.back() == i);
        c.pop_front();
        d.pop_back();
    }
    assert(c.empty());
    assert(d.empty());
}

static void test_stress() {
    Resizable_deque<int> a{23};
    for (int i = 1; i < 12451234; i++) {
        if (i % 7 == 0 && !a.empty()) {
            a.pop_front();
        } else if (i % 7 == 1 && !a.empty()) {
            a.pop_back();
        }
        a.push_back(i);
        a.push_front(i);
        assert(a.back() == i);
        assert(a.front() == i);
        assert(a.capacity() >= 23);
        assert(a.size() < 23 || a.capacity() < 4 * a.size());
        assert(2 * a.capacity() > a.size());
    }
    while (a.size() > 10000) {
        assert(a.back() > 0);
        a.pop_back();
        assert(a.capacity() >= 23);
        assert(a.size() < 23 || a.capacity() < 4 * a.size());
        assert(2 * a.capacity() > a.size());
    }
    while (!a.empty()) {
        assert(a.front() > 0);
        a.pop_front();
        assert(a.capacity() >= 23);
        assert(a.size() < 23 || a.capacity() < 4 * a.size());
        assert(2 * a.capacity() > a.size());
    }
    while (a.size() < 1000) {
        a.push_front(5);
        assert(a.front() == 5);
        assert(a.capacity() >= 23);
        assert(a.size() < 23 || a.capacity() < 4 * a.size());
        assert(2 * a.capacity() > a.size());
    }
}

int mainno() {
    test_exceptions();
    test_default_size();
    test_main();
    test_value_store();
    test_stress();
    std::cout << "Hello" << std::endl;
}
