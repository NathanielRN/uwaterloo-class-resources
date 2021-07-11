#!/usr/bin/env python3

from collections import defaultdict

class AVLTreeIterator:
    """ 2 way iterator with sentinel checking."""
    def __init__(self, tree, value=None, front_sentinel=False, back_sentinel=False):
        self.tree = tree
        # weird bug where root node is randomly out of order!
        self.container = sorted([node.value for node in tree])
        self.idx = self.container.index(value) if value in self.container else None
        self.front_sentinel = front_sentinel
        self.back_sentinel = back_sentinel
        valid_value = self.idx is None and self.is_sentinel()
        valid_value = valid_value or self.idx is not None and not self.is_sentinel()
        if not valid_value:
            raise ValueError("Value is not in list or is None but front/back sentinel is not set")
        
        if front_sentinel and back_sentinel:
            raise ValueError("Front sentinel and back sentinel should not both be set")

    
    def value(self):
        """Get the current value of the iterator """
        if self.is_sentinel():
            return None
        else:
            return self.container[self.idx]
    
    def is_sentinel(self):
        """ Determine if the iterator is currently at a 'sentinel' node"""
        return self.front_sentinel or self.back_sentinel
    
    def next(self):
        """Get next node in the tree"""
        return next(self)
    
    def previous(self):
        """Get previous node in the tree if not at front sentinel"""
        if self.front_sentinel:
            raise StopIteration
        if self.back_sentinel:
            self.back_sentinel = False
            self.idx = len(self.container)
        
        self.idx -= 1
        if self.idx < 0:
            self.front_sentinel = True
            self.idx = None


        
    def __next__(self):
        """Get next node in the tree if not at back sentinel"""
        if self.back_sentinel:
            raise StopIteration
        if self.front_sentinel:
            self.front_sentinel = False
            self.idx = -1
        
        self.idx += 1
        if self.idx >= len(self.container):
            self.back_sentinel = True
            self.idx = None

    def __iter__(self):
        """Allow an iterator to be a py iterator (though it shouldn't really be used this way)"""
        return self

class AVLTree:
    """Binary Tree that ensures AVL Balance"""
    PREFER_RIGHT = "right"
    PREFER_LEFT = "left"

    def __init__(self, root=None, preferred_erase=PREFER_LEFT, preferred_dir=PREFER_LEFT):
        self.preferred_dir = preferred_dir
        self.preferred_erase = preferred_erase

        self.root = root if root is not None else AVLNode(preferred_erase=preferred_erase, preferred_dir=preferred_dir)
        self.tree_size = 0

    def is_root(self):
        """Determine whether a tree is not a subtree"""
        return self.root.parent is None
    
    def empty(self):
        """Determine whether a tree contains nodes"""
        return self.root.empty()
    
    def height(self):
        """Determine height of tree"""
        return self.root.height
    
    def size(self):
        """Determine number of elements in tree"""
        return self.tree_size
    
    def __len__(self):
        """Pythonic way to get size"""
        return self.size()
    
    def insert(self, value):
        """
        Insert into tree at correct location 
        if not already inserted and ensure AVL Balance
        """
        if self.root.insert(value):
            self.tree_size += 1
            return True
        return False
    def insert_multiple(self, values):
        """Insert multiple values in the order given"""
        for val in values:
            self.insert(val)

    def erase(self, value):
        """Erase value from tree if exists and ensure AVL Balance"""
        if not self.empty() and self.root.erase(value):
            self.tree_size -= 1
            return True
        return False
    
    def front(self):
        """Get lowest value in tree"""
        return self.root.front()
    
    def back(self):
        """Get largest value of tree"""
        return self.root.back()
    
    def begin(self):
        """
        Get an iterator starting to the first (minimum) node. Goes to the end if empty
        """
        if self:
            return AVLTreeIterator(self, self.root.front())
        else:
            return self.end()
    
    def end(self):
        """ Get an iterator starting at the 'back sentinel' """
        return AVLTreeIterator(self, back_sentinel=True)
    
    def rbegin(self):
        """
        Get an iterator starting to the last (maximum) node. Goes to the rend if empty
        """
        if self:
            return AVLTreeIterator(self, self.root.back())
        else:
            return self.rend()

    def rend(self):
        """ Get an iterator starting at the 'front sentinel' """
        return AVLTreeIterator(self, front_sentinel=True)

    
    def find(self, value):
        """Get iterator starting at the value's node in the tree"""
        if self.root.find(value):
            return AVLTreeIterator(self, value)
        else:
            return self.end()
    
    def clear(self):
        """ Clear all elements from tree"""
        self.root.clear()
        self.tree_size = 0

    def get_max(self):
        """ Gets maximum element from tree"""
        if self.empty():
            raise ValueError("Should not be getting max of null tree")
        return self.root.get_max()
    
    def get_min(self):
        """ Gets minimum element from tree"""
        if self.empty():
            raise ValueError("Should not be getting min of null tree")
        return self.root.get_min()
    
    def pretty_print(self, tabs = 0):
        """
        Helper method to print tree structure. Also see bintree_printer for use on small trees
        """
        if self.is_root():
            print("Size: %d" % self.size())

        self.root.pretty_print(tabs)
    
    def __bool__(self):
        """ Pythonic way to check whether a tree is empty"""
        return not self.empty()

    def __iter__(self):
        """Forward In-order traversal of the tree to generate nodes"""
        if self:
            for smaller in self.root.left_tree:
                yield smaller
            yield self.root
            for bigger in self.root.right_tree:
                yield bigger
    
    def __reversed__(self):
        """Backwards In-order traversal of the tree to generate nodes"""
        if self:
            for bigger in reversed(self.root.right_tree):
                yield bigger
            yield self.root
            for smaller in reversed(self.root.left_tree):
                yield smaller

class AVLNode():
    """Binary Tree node that ensures AVL Balance"""
    INSERT = 1
    ERASE = 0

    def __init__(self, parent=None, value=None, preferred_erase=AVLTree.PREFER_LEFT, preferred_dir=AVLTree.PREFER_LEFT):
        self.parent = parent
        self.value = value
        self.left_tree = None
        self.right_tree = None
        self.height = -1

        self.preferred_dir = preferred_dir
        self.preferred_erase = preferred_erase

        # if this isn't a null node then create null children
        if not self.empty():
            self.left_tree = AVLTree(AVLNode(self, preferred_erase=preferred_erase, preferred_dir=preferred_dir))
            self.right_tree = AVLTree(AVLNode(self, preferred_erase=preferred_erase,preferred_dir=preferred_dir))
            # update height
            self.height = 0
    

    # overide boolean and node comparison to make life easy ...
    
    def __bool__(self):
        """ Pythonic way to check whether it's a null node """
        return (self.value is not None)

    def __lt__(self, other):
        """ Easy comparison node with another node/value """
        try:
            return self.value < other.value
        except AttributeError:
            return self.value < other
        return NotImplemented
    
    def __gt__(self, other):
        """ Easy comparison node with another node/value """
        try:
            return self.value > other.value
        except AttributeError:
            return self.value > other
        return NotImplemented
    
    def __int__(self):
        """ Easy formatting and value access using int function """
        return self.value

    def empty(self):
        """ Check if this is a null node"""
        return not self
        
    def _to_full(self, value):
        """ Convert a null node (child of a leaf node) to a regular node with given value """
        assert(self.empty())

        self.value = value
        # create null node
        self.height = 0
        self.left_tree = AVLTree(AVLNode(self, preferred_erase=self.preferred_erase, preferred_dir=self.preferred_dir))
        self.right_tree = AVLTree(AVLNode(self, preferred_erase=self.preferred_erase, preferred_dir=self.preferred_dir))
    
    def _update_height(self):
        """ Helper method to update the height internally """
        if not self:
            self.height = -1
            return
        self.height = max(
            self.left_tree.height() + 1, 
            self.right_tree.height() + 1
        )

    def back(self):
        """ Harder-nym for get maximum element in tree"""
        return self.get_max()

    def get_max(self):
        """ Get maximum element in tree"""
        assert (self), "should not be trying to get max of null nodes ..."
        
        if self.right_tree:
            return self.right_tree.get_max()
        else:
            return self.value

    def front(self):
        """ Harder-nym for get minimum element in tree"""
        return self.get_min()

    def get_min(self):
        """ Get minimum element in tree"""
        assert (self), "should not be trying to get min of null nodes ..."

        if self.left_tree:
            return self.left_tree.get_min()
        else:
            return self.value
    
    def find(self, value):
        """ Determine whether a value is in the tree"""
        if not self:
            return False
        if value < self:
            if not self.left_tree:
                return False
            return self.left_tree.root.find(value)
        elif value > self:
            if not self.right_tree:
                return False
            return self.right_tree.root.find(value)
        else:
            return True

    def insert(self, value):
        """ Insert a value into the AVL Tree and ensure  AVL Balance """
        inserted = False
        if not self:
            self._to_full(value)
            inserted = True
        
        elif value < self:
            inserted = self.left_tree.insert(value)
        elif value > self:
            inserted = self.right_tree.insert(value)
        else:
            # ignore duplicates ...
            return False
        
        self._update_height()
        self._fix_balance(value, AVLNode.INSERT)
        return inserted

    def is_leaf(self):
        """Determine whether a node is a leaf node """
        return not( self.left_tree or self.right_tree )
    
    def clear(self):
        """ Convert a node to a null node and abandon all it's children """
        self.value = None
        self.height = -1
        self.left_tree = None
        self.right_tree = None
    
    def erase(self, value):
        """ Remove a value into the AVL Tree and ensure AVL Balance"""
        if not self:
            return False
        erased = False
        if value < self:
            erased = self.left_tree.erase(value)
        elif value > self:
            erased = self.right_tree.erase(value)
        else:
            if self.right_tree and self.left_tree:
                # 2 children
                if self.preferred_erase == AVLTree.PREFER_LEFT:
                    self.value = self.left_tree.get_max()
                    self.left_tree.erase(self.value)
                else:
                    self.value = self.right_tree.get_min()
                    self.right_tree.erase(self.value)
                value = self.value

            elif self.right_tree:
                # right child only
                tree = self.right_tree
                self.value = tree.root.value
                self.left_tree = tree.root.left_tree
                self.right_tree = tree.root.right_tree

            elif self.left_tree:
                # left child only
                tree = self.left_tree
                self.value = tree.root.value
                self.left_tree = tree.root.right_tree
                self.right_tree = tree.root.left_tree
            else:
                # no children
                self.clear()
            erased = True
        
        self._update_height()
        # figure out how to fix balance
        self._fix_balance(value, AVLNode.ERASE)
        return erased      

    def balanced(self):
        """Check whether an AVL Tree is AVL Balanced"""
        return abs(self.left_tree.height() - self.right_tree.height()) < 2

    def _fix_balance(self, changed, change_type):
        """Determine whether to fix a node and do so based on operation performed """
        a = None
        v = None
        l = None
        r"""
        (a)
          \
          (v)
            \ 
             (l)
        a is the top node
        v is the second node
        l is the third node
        """
        if self and not self.balanced():
            a = self
            if change_type == AVLNode.INSERT:
                # find path towards insert
                v = a.left_tree.root if changed < a else a.right_tree.root
                assert (v), "Unexpectedly, V node doesn't exist"
                l = v.left_tree.root if changed < v else v.right_tree.root
                assert (l), "Unexpectedly, L node doesn't exist"
            
            elif change_type == AVLNode.ERASE:
                # fix the opposite and "taller" tree.  There are multiple options here.

                if self.preferred_erase == AVLTree.PREFER_LEFT:
                    v = a.right_tree.root if changed <= a.value else a.left_tree.root
                else:
                    v = a.right_tree.root if changed < a.value else a.left_tree.root
                assert (v), "Unexpectedly, V node doesn't exist"
                
                if not v.balanced():
                    return v._fix_balance(changed, change_type)

                # doesn't matter which node you pick next but it should exist ...
                if v == a.left_tree.root:
                    l = v.right_tree.root if v.right_tree.height() > v.left_tree.height() else v.left_tree.root
                else:
                    l = v.left_tree.root if v.left_tree.height() > v.right_tree.height() else v.right_tree.root
                assert (l), "Unexpectedly, L node doesn't exist"
                

            self._rotate(a, v, l) # Should/Could be static ... **shrug**
            

    def _rotate(self, a,v,l):
        """Generic AVL rotate given 3 nodes"""
        ### In python do to lack of pointers this gets really confusing
        ### I cannot support my coding methods here (1 letter variable names, 
        ### changing meaning of varaibles etc.) but it works, I think.

        a_val = a.value
        v_val = v.value
        l_val = l.value

        if l < v < a:
            # Basic Idea of PY Rotate:
            # Shift value of top node down and move next highest up
            a.value = v_val
            v.value = a_val
            
            # move the subtrees to match movement of values
            v.left_tree = v.right_tree
            v.right_tree = a.right_tree
            # change parenthood
            v.right_tree.root.parent = v

            # attach lower nodes to the top node as prescribed
            a.left_tree = AVLTree(l)
            a.right_tree = AVLTree(v)

        elif v < a and l > v:
            
            l.value = a_val
            a.value = l_val

            v.right_tree = l.left_tree
            l.left_tree = l.right_tree
            
            l.right_tree = a.right_tree
            l.right_tree.root.parent = l

            # Wacky situation on erase where rotation causes inbalance
            v._update_height()
            l._update_height()
            if not (v.balanced() and l.balanced()):
                v.right_tree, l.left_tree = l.left_tree, v.right_tree
            
            v.right_tree.root.parent = v

            a.right_tree = AVLTree(l)

        elif v > a and l < v:
            
            l.value = a_val
            a.value = l_val
            
            v.left_tree = l.right_tree
            l.right_tree = l.left_tree

            l.left_tree = a.left_tree
            l.left_tree.root.parent = l
            # Wacky situation on erase where rotation causes inbalance
            v._update_height()
            l._update_height()
            if not (v.balanced() and l.balanced()):
                v.left_tree, l.right_tree = l.right_tree, v.left_tree
            
            v.left_tree.root.parent = v


            a.left_tree = AVLTree(l)            

        elif l > v > a:
            # opposite of first test
            a.value = v_val
            v.value = a_val
             
            v.right_tree = v.left_tree
            v.left_tree = a.left_tree
            v.left_tree.root.parent = v

            a.right_tree = AVLTree(l)
            a.left_tree = AVLTree(v)

        else:
            raise ValueError(
                "relationship between a & v & l is broken! "
                "For a=%s, v=%s, l=%s" % (a.value, v.value, l.value)
                )
        # update heights
        l._update_height()
        v._update_height()
        a._update_height()


    def pretty_print(self, tabs):
        if not self:
            return
        if tabs:
            subtree = "L" if self < self.parent else "R"
            print(" "*(tabs - 1) + subtree + "---" + str(self.value), end="")
        else:
            print(self.value, end="")
        print(" h=%d" % self.height)
        self.left_tree.pretty_print(tabs + 1)
        self.right_tree.pretty_print(tabs + 1)

def bintree_printer(avl):
    """ Visually print of a binary tree with branches et al. """
    height = avl.height() - 1
    max_height = height
    queue = [avl.root]
    strlen = lambda x: len(str(x.value))
    max_width =strlen(max(avl, key=strlen))
    def subtree_width(h): 
        return int(max_width*(2**(h + 1) -1))

    while any(queue):
        ### Print nodes and spacing
        for node in range(len(queue)):
            node = queue.pop()
            if node is None:
                print(" "*max_width, end="")
                queue.insert(0, None)
                continue
            
            if node.left_tree:
                queue.insert(0, node.left_tree.root)
            else:
                queue.insert(0, AVLNode())
            print(" "*subtree_width(height), end="")

            if node:
                print("{:^{width}d}".format(node.value, width=max_width), end="")
            else:
                print("{:^{width}.{width}s}".format("xx", width=max_width), end="")
            queue.insert(0, None)

            if node.right_tree:
                queue.insert(0, node.right_tree.root)
            else:
                queue.insert(0, AVLNode())
            print(" "*subtree_width(height), end="")
        print()
        ### Print branches
        next_width = subtree_width(height-1)
        next_nodes = 2**(max_height - height)
        for i in reversed(range(next_width)):
            for n in range(next_nodes):
                padding_space = ((next_width + i) + max_width - 1)
                internal_space = ((next_width - i)*2  + max_width - 1)
                print(" "*padding_space, end="")
                print("/", end="")
                print(" "*internal_space, end="")
                print("\\", end="")
                print(" "*padding_space, end="")
                if n != (next_nodes - 1):
                    print(" "*(max_width +1), end="")
            print()
        
        height = height - 1

# Personal Test
# avl = AVLTree()
# avl.insert(5)
# avl.insert(3)
# avl.insert(7)
# avl.insert(2)
# avl.insert(4)
# avl.insert(6)
# avl.insert(1)
# bintree_printer(avl)
# avl.insert(5)
# avl.insert(3)
# avl.insert(7)
# avl.insert(2)
# avl.insert(4)
# avl.insert(6)
# avl.insert(1)
# avl.pretty_print()
# avl.erase(6)
# avl.pretty_print()
# avl.pretty_print()
# avl.insert(6)

# avl.insert(8)

# avl.insert(9)

# avl.insert(10)

# avl.insert(11)

# avl.insert(12)

# avl.insert(20)
# avl.insert(30)

# avl.insert(40)
# avl.insert(-1040)

# print([n.value for n in avl])
# print([n.value for n in reversed(avl)])
# bintree_printer(avl)

### Harder Test
# tree = AVLTree(AVLNode())
# for i in range(1,31):
#     print(int(tree.insert( (83*i) % 30 )), end="")
# print()
# for i in range(1,31):
#      print(int(tree.insert( (97*i) % 30 )), end="")
# print()
# print("Front:   %d" % tree.front())
# print("Back:    %d" % tree.back())
# print("Height:  %d <- this will change for an AVL tree" % tree.height())
# print("--> ", end="")
# for n in tree:
#     print("%d " % n, end="")
# print()
# print("--> ", end="")
# for n in reversed(tree):
#     print("%d " % n, end="")
# print()
# tree.clear()
# print("--> ")
# for n in tree:
#     print("%d " % n, end="")


   
