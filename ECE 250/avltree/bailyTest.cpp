/*
 * Bailey Thompson
 * github.com/bkthomps
 *
 * Automated test for AVL tree and iterator. Make sure to make your inner Node
 * class is public for this test.
 */

#include <iostream>
#include <cassert>
#include "Search_tree.h"

static void forward_iterate(Search_tree<int>& tree) {
    int count = 0;
    int last = -13371337;
    for (auto itr = tree.begin(); itr != tree.end(); ++itr) {
        int num = *itr;
        assert(last < num);
        last = num;
        count++;
    }
    assert(count == tree.size());
}

static void reverse_iterate(Search_tree<int>& tree) {
    int count = 0;
    int last = 13371337;
    for (auto itr = tree.rbegin(); itr != tree.rend(); --itr) {
        int num = *itr;
        assert(last > num);
        last = num;
        count++;
    }
    assert(count == tree.size());
}

static void verify_avl_tree(Search_tree<int>::Node* node) {
    if (!node) {
        return;
    }
    if (node->left_tree) {
        assert(node->left_tree->node_value < node->node_value);
    }
    if (node->right_tree) {
        assert(node->right_tree->node_value > node->node_value);
    }
    verify_avl_tree(node->left_tree);
    verify_avl_tree(node->right_tree);
}

static bool stub_insert(Search_tree<int>& me, int obj) {
    bool ret = me.insert(obj);
    verify_avl_tree(me.root_node);
    forward_iterate(me);
    reverse_iterate(me);
    return ret;
}

static bool contains_recurse(Search_tree<int>::Node* node, int obj) {
    if (!node) {
        return false;
    }
    if (node->node_value == obj) {
        return true;
    }
    return contains_recurse(node->left_tree, obj) || contains_recurse(node->right_tree, obj);
}

static bool stub_contains(Search_tree<int>& me, int obj) {
    bool ret = contains_recurse(me.root_node, obj);
    verify_avl_tree(me.root_node);
    forward_iterate(me);
    reverse_iterate(me);
    return ret;
}

int main() {
    {
        Search_tree<int> me{};
        int key;
        // left-left
        key = 5;
        stub_insert(me, key);
        key = 3;
        stub_insert(me, key);
        key = 1;
        stub_insert(me, key);
        key = 0xdeadbeef;
        me.clear();
        // right-right
        key = 1;
        stub_insert(me, key);
        key = 3;
        stub_insert(me, key);
        key = 5;
        stub_insert(me, key);
        key = 0xdeadbeef;
        me.clear();
        // left-right
        key = 5;
        stub_insert(me, key);
        key = 1;
        stub_insert(me, key);
        key = 3;
        stub_insert(me, key);
        key = 0xdeadbeef;
        me.clear();
        // right-left
        key = 1;
        stub_insert(me, key);
        key = 5;
        stub_insert(me, key);
        key = 3;
        stub_insert(me, key);
        key = 0xdeadbeef;
        me.clear();
        // Two children edge case.
        key = 8;
        stub_insert(me, key);
        key = 5;
        stub_insert(me, key);
        key = 11;
        stub_insert(me, key);
        key = 2;
        stub_insert(me, key);
        key = 6;
        stub_insert(me, key);
        key = 10;
        stub_insert(me, key);
        key = 15;
        stub_insert(me, key);
        key = 1;
        stub_insert(me, key);
        key = 3;
        stub_insert(me, key);
        key = 4;
        stub_insert(me, key);
        key = 7;
        stub_insert(me, key);
        key = 9;
        stub_insert(me, key);
        key = 12;
        stub_insert(me, key);
        key = 13;
        stub_insert(me, key);
        key = 16;
        stub_insert(me, key);
        key = 14;
        me.clear();
        // Two children edge case.
        key = 8;
        stub_insert(me, key);
        key = 4;
        stub_insert(me, key);
        key = 12;
        stub_insert(me, key);
        key = 2;
        stub_insert(me, key);
        key = 6;
        stub_insert(me, key);
        key = 10;
        stub_insert(me, key);
        key = 15;
        stub_insert(me, key);
        key = 1;
        stub_insert(me, key);
        key = 3;
        stub_insert(me, key);
        key = 5;
        stub_insert(me, key);
        key = 7;
        stub_insert(me, key);
        key = 9;
        stub_insert(me, key);
        key = 11;
        stub_insert(me, key);
        key = 13;
        stub_insert(me, key);
        key = 16;
        stub_insert(me, key);
        key = 14;
        stub_insert(me, key);
        me.clear();
        // Add a lot of items.
        int count = 0;
        bool flip = false;
        for (int i = 1234; i < 82400; i++) {
            int num = i % 765;
            const bool is_already_present = !stub_insert(me, num);
            const bool is_now_present = !stub_insert(me, num);
            if (!is_already_present && is_now_present) {
                count++;
            }
            if (i == 1857 && !flip) {
                i *= -1;
                flip = true;
            }
        }
        assert(count == me.size());
    } // Destructor call
    {
        Search_tree<int> me{};
        assert(me.size() == 0);
        assert(me.empty());
        int key = 4;
        stub_insert(me, key);
        assert(me.size() == 1);
        stub_insert(me, key);
        assert(me.size() == 1);
        assert(!me.empty());
        assert(stub_contains(me, key));
        key = 7;
        assert(!stub_contains(me, key));
        stub_insert(me, key);
        assert(me.size() == 2);
        assert(stub_contains(me, key));
        int c[10] = {5, 9, 4, -5, 0, 6, 1, 5, 7, 2};
        for (int i = 0; i < 10; i++) {
            stub_insert(me, c[i]);
            assert(stub_contains(me, c[i]));
        }
        assert(me.size() == 9);
        for (int i = 0; i < 10; i++) {
            assert(stub_contains(me, c[i]));
        }
        for (int i = -100; i < 100; i++) {
            bool contains = false;
            for (int j = 0; j < 10; j++) {
                if (c[j] == i) {
                    contains = true;
                }
            }
            assert(stub_contains(me, i) == contains);
        }
        int num = -3;
        assert(!me.erase(num));
        assert(me.size() == 9);
        assert(!stub_contains(me, num));
        num = 6;
        assert(me.erase(num));
        assert(me.size() == 8);
        assert(!stub_contains(me, num));
        num = 4;
        assert(me.erase(num));
        assert(me.size() == 7);
        assert(!stub_contains(me, num));
        num = 7;
        assert(me.erase(num));
        assert(me.size() == 6);
        assert(!stub_contains(me, num));
        num = 9;
        assert(me.erase(num));
        assert(me.size() == 5);
        assert(!stub_contains(me, num));
        num = -5;
        assert(me.erase(num));
        assert(me.size() == 4);
        assert(!stub_contains(me, num));
        num = 0;
        assert(me.erase(num));
        assert(me.size() == 3);
        assert(!stub_contains(me, num));
        num = 1;
        assert(me.erase(num));
        assert(me.size() == 2);
        assert(!stub_contains(me, num));
        num = 5;
        assert(me.erase(num));
        assert(me.size() == 1);
        assert(!stub_contains(me, num));
        num = 2;
        assert(me.erase(num));
        assert(me.size() == 0);
        assert(!stub_contains(me, num));
        // Add a lot of items and remove individually.
        for (int i = 5000; i < 6000; i++) {
            stub_insert(me, i);
            assert(stub_contains(me, i));
        }
        assert(me.size() == 1000);
        for (int i = 5000; i < 5500; i++) {
            me.erase(i);
            assert(!stub_contains(me, i));
        }
        assert(me.size() == 500);
        assert(!me.empty());
        me.clear();
        assert(me.size() == 0);
        assert(me.empty());
        // Add a lot of items and clear.
        for (int i = 5000; i < 6000; i++) {
            stub_insert(me, i);
            assert(stub_contains(me, i));
        }
        assert(me.size() == 1000);
        me.clear();
        int p = 0xdeadbeef;
        assert(!me.erase(p));
        assert(me.size() == 0);
        assert(me.empty());
    } // Call destructor
    {
        // Create odd shape graph.
        Search_tree<int> me{};
        int key = 10;
        stub_insert(me, key);
        key = 5;
        stub_insert(me, key);
        key = 15;
        stub_insert(me, key);
        key = 3;
        stub_insert(me, key);
        key = 8;
        stub_insert(me, key);
        key = 12;
        stub_insert(me, key);
        key = 18;
        stub_insert(me, key);
        key = 12;
        me.erase(key);
        key = 5;
        me.erase(key);
        key = 3;
        me.erase(key);
        key = 8;
        me.erase(key);
        me.clear();
        // Allocate many nodes.
        for (int i = 8123; i < 12314; i += 3) {
            stub_insert(me, i);
            assert(stub_contains(me, i));
        }
        for (int i = 13000; i > 8000; i--) {
            stub_insert(me, i);
            assert(stub_contains(me, i));
        }
        me.clear();
        // Create another odd shape graph.
        key = 20;
        stub_insert(me, key);
        key = 10;
        stub_insert(me, key);
        key = 40;
        stub_insert(me, key);
        key = 5;
        stub_insert(me, key);
        key = 15;
        stub_insert(me, key);
        key = 30;
        stub_insert(me, key);
        key = 50;
        stub_insert(me, key);
        key = 25;
        stub_insert(me, key);
        key = 35;
        stub_insert(me, key);
        key = 36;
        stub_insert(me, key);
        key = 34;
        stub_insert(me, key);
        key = 33;
        stub_insert(me, key);
        key = 32;
        stub_insert(me, key);
        key = 30;
        me.erase(key);
        key = 32;
        assert(stub_contains(me, key));
        me.clear();
        // One sided tree.
        key = 10;
        stub_insert(me, key);
        key = 9;
        stub_insert(me, key);
        key = 8;
        stub_insert(me, key);
        key = 7;
        stub_insert(me, key);
        key = 8;
        me.erase(key);
        key = 7;
        assert(stub_contains(me, key));
    } // Call constructor
    // Replace two sided two children.
    Search_tree<int> me{};
    int key = 5;
    stub_insert(me, key);
    key = 1;
    stub_insert(me, key);
    key = 6;
    stub_insert(me, key);
    key = -1;
    stub_insert(me, key);
    key = 3;
    stub_insert(me, key);
    key = 7;
    stub_insert(me, key);
    key = -2;
    stub_insert(me, key);
    key = 0;
    stub_insert(me, key);
    key = 2;
    stub_insert(me, key);
    key = 4;
    stub_insert(me, key);
    key = 1;
    me.erase(key);
    assert(!stub_contains(me, key));
    me.clear();
    key = 5;
    stub_insert(me, key);
    key = 1;
    stub_insert(me, key);
    key = 6;
    stub_insert(me, key);
    key = -1;
    stub_insert(me, key);
    key = 3;
    stub_insert(me, key);
    key = 7;
    stub_insert(me, key);
    key = -2;
    stub_insert(me, key);
    key = 0;
    stub_insert(me, key);
    key = 4;
    stub_insert(me, key);
    key = 1;
    me.erase(key);
    assert(!stub_contains(me, key));
}
