/*****************************************
 * UW User ID:  enruizno
 * Submitted for ECE 250
 * Semester of Submission:  (Winter) 2018
 *
 * By  submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *****************************************/

#ifndef RPOVDGBQN9TIEO3P
#define RPOVDGBQN9TIEO3P

#include "Exception.h"
#include "ece250.h"
#include <cassert>

/****************************************************
 * Search_tree Class
 *
 * Implementation of an AVL tree with rotations that allows iteration of the stored items
 *
 *  Public classes, fields and methods
 *
 *      Constructors
 *
 *          new             constructor     create an avl tree with an initially empty set of nodes
 *
 *      Deconstructors
 *
 *          delete          deconstructor   delete the avl search tree instance by clearing items and
 *                                          deleting the sentinels
 *      Accessors
 *
 *          front()                         Returns the element at the front, the leftmost item, of
 *                                          the search tree as seen by the iterator helper class
 *
 *          back()                          Returns the element at the back, the rightmost item, of
 *                                          the search tree as seen by the iterator helper class
 *
 *          size()                          Returns the variable tracking how many elements are currently
 *                                          stored in this search tree
 *
 *          empty()                         Evaluates whether the root_node is currently the nullptr
 *                                          meaning there are no nodes in this search tree
 *
 *          height()                        Returns the root_node's variable that tracks the current height
 *                                          of the search tree as seen from this root_node
 *
 *      Mutators
 *
 *          clear()                         Calls clear on the root_node, and re-initializes all variables
 *                                          including setting the root_node to nullptr, tree_size to 0 and
 *                                          reassigning the front and back sentinels
 *
 *          insert(Type const &obj, Node *&to_this)
 *
 *                                          Attempts to insert a new node object into the tree, returns a
 *                                          1 to indicate success, a 0 to indicate failure, duplicates are
 *                                          rejected
 *
 *          erase(Type const &obj, Node *&to_this)
 *
 *                                          Attempts to erase the specified object from the tree, returns
 *                                          a 1 to indicate success, a 0 to indicate failure
 *
 *  Private classes, fields and methods
 *
 *      Classes
 *
 *          Node            class           Node class used by search tree to store objects
 *
 *      Fields
 *
 *          root_node                       Field that holds an instance of the search tree's root_node
 *                                          - a Node type instance
 *
 *          tree_size                       Field that tracks the size of the tree by keeping tracking
 *                                          of the number of node instances stored by the search tree
 *
 *          front_sentinel                  Node class pointer that points the front-most element in the
 *                                          tree (the leftmost item)
 *
 *          back_sentinel                   Node class pointer that points the back-most element in the
 *                                          tree (the rightmost item)
 ****************************************************/
template <typename Type>
class Search_tree {
public:
    class Iterator;

private:
    /****************************************************
     * Node Class
     *
     * Node class which is the method of storage for the search tree it exists in. Every node class instance
     * can store references to its immediate subtrees whose root nodes are its children (if they exist).
     *
     *  Public classes, fields and methods
     *
     *      Constructors
     *
     *          new             constructor     Creates a node instance with node value equal to the passed in
     *                                          argument
     *
     *      Accessors
     *
     *          update_height()                 Updates the node's tracking height variable based on the max of
     *                                          the heights as seen by the right subtree's root node and the left
     *                                          subtree's root node
     *
     *          front()                         Returns the de-referenced node value of the leftmost node in the
     *                                          subtree where this node is the root node of that subtree
     *
     *          back()                          Returns the de-referenced node value of the rightmost node in the
     *                                          subtree where this node is the root node of that subtree
     *
     *          is_leaf()                       Evaluates whether the Node's class tracking left_tree AND right_tree
     *                                          variables are nullptr and returns the result of that evaluation
     *
     *          find(obj)                       Attempts to locate the passed in object in the search tree by
     *                                          creating an iterator class that iterates through all the nodes of
     *                                          the sub tree where this is the root node. If found it returns the
     *                                          iterator class with the search_results, if not it returns the
     *                                          iterator class with the back_sentinel
     *
     *          height()                        Returns the value of the node's tree_height variable, returns -1
     *                                          in the case where this node is a nullptr
     *
     *          print()                         Pretty prints the structure of the avl with its stored nodes
     *
     *      Mutators
     *
     *          update_height()
     *
     *          clear()                         Calls clear on the root_node, and re-initializes all variables
     *                                          including setting the root_node to nullptr, tree_size to 0 and
     *                                          reassigning the front and back sentinels
     *
     *          insert(Type const &obj, Node *&to_this)
     *
     *                                          Attempts to insert a new node object into the tree, returns a
     *                                          1 to indicate success, a 0 to indicate failure, duplicates are
     *                                          rejected
     *
     *          erase(Type const &obj, Node *&to_this)
     *
     *                                          Attempts to erase the specified object from the tree, returns
     *                                          a 1 to indicate success, a 0 to indicate failure
     *
     *          fix_potential_imbalance()       Checks to see if an imbalance occured and rotates the nodes to
     *                                          fix it if it did occur
     *
     *       Fields
     *
     *          node_value                      The value stored by this instance of the node class
     *
     *          tree_height                     Variable that tracks the height as seen by the subtree this
     *                                          node exists in where this node is root node of that subtree
     *
     *          left_tree                       The left subtree of the current node instance referenced by
     *                                          a Node class pointer to the root node of that left subtree
     *
     *          right_tree                      The right subtree of the current node instance referenced by
     *                                          a Node class pointer to the root node of that right subtree
     *
     *          previous_node                   The node that directly follows this node, used by the helper
     *                                          iterator class to iterate through the elements of the tree
     *
     *          next_node                       The node that this node follows, used by the helper
     *                                          iterator class to iterate through the elements of the tree
     ****************************************************/
    class Node {
    public:
        Type node_value;
        int tree_height;

        // The left and right sub-trees
        Node *left_tree;
        Node *right_tree;

        // Hint as to how you can create your iterator
        // Point to the previous and next nodes in linear order
        Node *previous_node;
        Node *next_node;

        // Member functions
        Node( Type const & = Type() );

        void update_height();

        int height() const;
        bool is_leaf() const;
        Node *front();
        Node *back();
        Node *find( Type const &obj );

        void clear();
        bool insert( Type const &obj, Node *&to_this );
        bool erase( Type const &obj, Node *&to_this );

        // Introduced Methods
        void fix_potential_imbalance(Search_tree<Type>::Node* &to_this);
        void print( std::ostream &out, int tabs = 0);
    };

    Node *root_node;
    int tree_size;

    // Hint as to how to start your linked list of the nodes in order
    Node *front_sentinel;
    Node *back_sentinel;

public:
    /****************************************************
     * Iterator Class
     *
     * Creates an iterator class used as a helper class by the search tree class
     *
     *  Public classes, fields and methods
     *
     *      Operators
     *
     *          *                               Dereference the current node and return the value it stores
     *
     *          ++                              Update the current_node to point to the node that it follows,
     *                                          if we are at the end of the tree continue pointing to the
     *                                          right-most (back) element
     *
     *          --                              Update the current_node to point to the node that follows it,
     *                                          if we are at the start of the tree continue pointing to the
     *                                          left-most (front) element
     *
     *          ==                              Evaluate whether the passed in argument is equal to the current
     *                                          node pointed to by the iterator, return the result of the
     *                                          evaluation
     *
     *          !=                              Evaluate whether the passed in argument is not equal to the
     *                                          current node pointed to by the iterator, return the result of the
     *                                          evaluation
     *
     *  Private classes, fields and methods
     *
     *      Fields
     *
     *          containing_tree                 The tree it is to iterate through
     *
     *          current_node                    The node the iterator is currently referencing, operators will
     *                                          allow this field to be update so that the iterator can access
     *                                          all nodes of its containing_tree
     *      Constructor
     *
     *          new                             Creates a new iterator by taking in as parameters the tree it is
     *                                          to iterate through and the node it will begin referencing from
     ****************************************************/
    class Iterator {
    private:
        Search_tree *containing_tree;
        Node *current_node;
        bool is_end;

        // The constructor is private so that only the search tree can create an iterator
        Iterator( Search_tree *tree, Node *starting_node );

    public:
        // DO NOT CHANGE THE SIGNATURES FOR ANY OF THESE
        Type operator*() const;
        Iterator &operator++();
        Iterator &operator--();
        bool operator==( Iterator const &rhs ) const;
        bool operator!=( Iterator const &rhs ) const;

        // Make the search tree a friend so that it can call the constructor
        friend class Search_tree<Type>;
    };

    // DO NOT CHANGE THE SIGNATURES FOR ANY OF THESE
    Search_tree();
    ~Search_tree();

    bool empty() const;
    int size() const;
    int height() const;

    Type front() const;
    Type back() const;

    Iterator begin();
    Iterator end();
    Iterator rbegin();
    Iterator rend();
    Iterator find( Type const & );

    void clear();
    bool insert( Type const & );
    bool erase( Type const & );

    // Friends

    template <typename T>
    friend std::ostream &operator<<( std::ostream &, Search_tree<T> const & );
};

//////////////////////////////////////////////////////////////////////
//                Search Tree Public Member Functions               //
//////////////////////////////////////////////////////////////////////

// The initialization of the front and back sentinels is a hint
template <typename Type>
Search_tree<Type>::Search_tree():
        root_node( nullptr ),
        tree_size( 0 ),
        front_sentinel( new Search_tree::Node( Type() ) ),
        back_sentinel( new Search_tree::Node( Type() ) ) {
    front_sentinel->next_node = back_sentinel;
    back_sentinel->previous_node = front_sentinel;
}

template <typename Type>
Search_tree<Type>::~Search_tree() {
    clear();  // might as well use it...
    delete front_sentinel;
    delete back_sentinel;
}

template <typename Type>
bool Search_tree<Type>::empty() const {
    return ( root_node == nullptr );
}

template <typename Type>
int Search_tree<Type>::size() const {
    return tree_size;
}

template <typename Type>
int Search_tree<Type>::height() const {
    return root_node->height();
}

template <typename Type>
Type Search_tree<Type>::front() const {
    if ( empty() ) {
        throw underflow();
    }

    return root_node->front()->node_value;
}

template <typename Type>
Type Search_tree<Type>::back() const {
    if ( empty() ) {
        throw underflow();
    }

    return root_node->back()->node_value;
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::begin() {
    return empty() ? Iterator( this, back_sentinel ) : Iterator( this, root_node->front() );
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::end() {
    return Iterator( this, back_sentinel );
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::rbegin() {
    return empty() ? Iterator( this, front_sentinel ) : Iterator( this, root_node->back() );
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::rend() {
    return Iterator( this, front_sentinel );
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::find( Type const &obj ) {
    if ( empty() ) {
        return Iterator( this, back_sentinel );
    }

    typename Search_tree<Type>::Node *search_result = root_node->find( obj );

    if ( search_result == nullptr ) {
        return Iterator( this, back_sentinel );
    } else {
        return Iterator( this, search_result );
    }
}

template <typename Type>
void Search_tree<Type>::clear() {
    if ( !empty() ) {
        root_node->clear();
        root_node = nullptr;
        tree_size = 0;
    }

    // Reinitialize the sentinels
    front_sentinel->next_node = back_sentinel;
    back_sentinel->previous_node = front_sentinel;
}


template <typename Type>
bool Search_tree<Type>::insert( Type const &obj ) {

    // In the case where the Search_tree is currently empty, we must update the sentinel
    // pointers. Otherwise, the parent node of the newly inserted node handles sentinel pointer
    // assignment.
    if ( empty() ) {
        auto incoming_node = new Search_tree::Node( obj );
        root_node = incoming_node;
        tree_size = 1;

        // This newly created node will point to the front and back sentinels since it is the
        // only element in the list
        incoming_node->previous_node = front_sentinel;
        incoming_node->next_node = back_sentinel;

        // Updating the sentinel nodes to point to the first node allows the search tree's
        // node classes to continue updating them from now on whenever the tree is not empty.
        front_sentinel->next_node = incoming_node;
        back_sentinel->previous_node = incoming_node;

        // Notify the caller that the insertion was successful
        return true;
    } else if ( root_node->insert( obj, root_node ) ) {
        // If the node classes of the search_tree succeeded in inserting the node, we update
        // the tree size
        ++tree_size;

        // We return true to notify the caller the insertion was successful
        return true;
    } else {
        //If the node classes were not successful, notify the caller by returning false
        return false;
    }
}

template <typename Type>
bool Search_tree<Type>::erase( Type const &obj ) {

    if ( !empty() && root_node->erase( obj, root_node ) ) {
        // If the search tree's node classes are successful in deleting the target object,
        // we decrement the tree size
        --tree_size;

        // Notify the caller that the erase was successful by returning true
        return true;
    } else {

        // Notify the caller that the erase was not successful by returning false
        return false;
    }
}

//////////////////////////////////////////////////////////////////////
//                   Node Public Member Functions                   //
//////////////////////////////////////////////////////////////////////

template <typename Type>
Search_tree<Type>::Node::Node( Type const &obj ):
        node_value( obj ),
        left_tree( nullptr ),
        right_tree( nullptr ),
        next_node( nullptr ),
        previous_node( nullptr ),
        tree_height( 0 ) {
    // does nothing
}

template <typename Type>
void Search_tree<Type>::Node::update_height() {
    tree_height = std::max( left_tree->height(), right_tree->height() ) + 1;
}

template <typename Type>
int Search_tree<Type>::Node::height() const {
    return ( this == nullptr ) ? -1 : tree_height;
}

// Return true if the current node is a leaf node, false otherwise
template <typename Type>
bool Search_tree<Type>::Node::is_leaf() const {
    return ( (left_tree == nullptr) && (right_tree == nullptr) );
}

// Return a pointer to the front node
template <typename Type>
typename Search_tree<Type>::Node *Search_tree<Type>::Node::front() {
    return ( left_tree == nullptr ) ? this : left_tree->front();
}

// Return a pointer to the back node
template <typename Type>
typename Search_tree<Type>::Node *Search_tree<Type>::Node::back() {
    return ( right_tree == nullptr ) ? this : right_tree->back();
}

template <typename Type>
typename Search_tree<Type>::Node *Search_tree<Type>::Node::find( Type const &obj ) {
    if ( obj == node_value ) {
        return this;
    } else if ( obj < node_value ) {
        return (left_tree == nullptr) ? nullptr : left_tree->find( obj );
    } else {
        return ( right_tree == nullptr ) ? nullptr : right_tree->find( obj );
    }
}

// Recursively clear the tree
template <typename Type>
void Search_tree<Type>::Node::clear() {
    if ( left_tree != nullptr ) {
        left_tree->clear();
    }

    if ( right_tree != nullptr ) {
        right_tree->clear();
    }

    delete this;
}

template <typename Type>
void Search_tree<Type>::Node::fix_potential_imbalance(Search_tree<Type>::Node* &to_this) {
    // Where the absolute value of the difference between the left and right subtrees is less than 2, there
    // is no imbalance from this root node's perspective and so we exit early from this function
    if (left_tree->height() - right_tree->height() < 2 && left_tree->height() - right_tree->height() > -2) {
        return;
    }

    // At this point there is definitely an imbalance at this node
    // When the imbalance to fix is ambiguous as in from an erase, a
    // right-right or left-left imbalance is favoured

    if (left_tree->height() > right_tree->height()) {
        if (left_tree->right_tree->height() > left_tree->left_tree->height()) {
            // We have a left, right imbalance

            // The right subtree of this node’s left subtree will promoted to
            // the root node, replacing this node
            auto new_root = left_tree->right_tree;

            // the new root will leave behind its left tree where it will
            // become the right tree of this node’s left subtree, since
            // all values in that right tree are bigger than this node’s
            // left subtree
            left_tree->right_tree = new_root->left_tree;

            // the new root will continue to replace this node by taking
            // on this node’s left tree as its own left tree
            new_root->left_tree = left_tree;

            // the new root will leave behind its right tree which will
            // become this node’s left subtree
            left_tree = new_root->right_tree;

            // the new root’s right tree will become this node as the
            // rotation is complete
            new_root->right_tree = this;

            // this node has been pushed down a depth and has gained
            // a left subtree to make way for the new root and so must
            // have its heights updated
            update_height();

            // the new root’s left subtree has also gained a new subtree
            // and so must also have its heights updated
            new_root->left_tree->update_height();

            // the new root has replaced this node and has gained new
            // subtrees and so must have its heights updated
            new_root->update_height();

            // the pointer that was referencing this node must now point
            // to the new root which has taken its position
            to_this = new_root;
        } else {
            // We have a left, left imbalance

            // This node's left tree will become the new root of the sub tree as
            // seen from this node acting as the root
            auto new_root = left_tree;

            // This node's left tree will now take on the new root's right tree,
            // where before its left tree was the new root
            left_tree = new_root->right_tree;

            // The new root's right tree has been taken care of and now it's new
            // right tree will be this node
            new_root->right_tree = this;

            // This rotation will require an update to the height of this node
            // which gained a subtree and was pushed down a depth to make way
            // for the new root taking its place
            update_height();

            // An erase may cause the new root to change its height since it gains
            // the height of this node combined with the new root's old right sub
            // tree that it took with it, we update it as a final precaution
            new_root->update_height();

            // The pointer to this node will now point to the new root which
            // took this node's place
            to_this = new_root;
        }
    } else {
        if (right_tree->left_tree->height() > right_tree->right_tree->height()) {

            // We have a right, left imbalance

            // The left subtree of this node’s right subtree will promoted to
            // the root node, replacing this node
            auto new_root = right_tree->left_tree;

            // the new root will leave behind its right tree where it will
            // become the left tree of this node’s right subtree, since all
            // values in that left tree are smaller than this node’s right
            // subtree
            right_tree->left_tree = new_root->right_tree;

            // the new root will continue to replace this node by taking on
            // this node’s right tree as its own right tree
            new_root->right_tree = right_tree;

            // the new root will leave behind its left tree which will become
            // this node’s right subtree
            right_tree = new_root->left_tree;

            // the new root’s left tree will become this node as the rotation
            // is complete
            new_root->left_tree = this;

            // this node has been pushed down a depth and has gained a new
            // right subtree to make way for the new root and so must have its
            // heights updated
            update_height();

            // the new root’s right subtree has also gained a new subtree
            // and so must also have its heights updated
            new_root->right_tree->update_height();

            // the new root has replaced this node and has gained new subtrees
            // and so must have its heights updated
            new_root->update_height();

            // the pointer that was referencing this node must now point to
            // the new root which has taken its position
            to_this = new_root;
        } else {
            // We have a right, right imbalance

            // This node's right tree will become the new root of the subtree
            // as seen from this node acting as the root
            auto new_root = right_tree;

            // This node's right tree will now take on the new root's left tree,
            // where before its right tree was the new root
            right_tree = new_root->left_tree;

            // The new root's left tree has been taken care of and now it's new
            // left tree will be this node
            new_root->left_tree = this;

            // This rotation will require an update to the height of this node
            // which gained a subtree and was pushed down a depth to make way
            // for the new root taking its place
            update_height();

            // An erase may cause the new root to change its height since it gains
            // the height of this node combined with the new root's old left sub
            // tree that it took with it, we update it as a final precaution
            new_root->update_height();

            // The pointer to this node will now point to the new root which
            // took this node's place
            to_this = new_root;
        }
    }
}

template <typename Type>
bool Search_tree<Type>::Node::insert( Type const &obj, Search_tree<Type>::Node *&to_this ) {
    if ( obj < node_value ) {
        if ( left_tree == nullptr ) {
            auto incoming_node = new Search_tree<Type>::Node( obj );
            left_tree = incoming_node;

            // This node is able to insert the node so we update the height
            update_height();

            // The node is being inserted to the left relative to this node because it is less than this node.
            // We will tell the node before this node to stop pointing at this node and to now point to the new
            // node, thus placing the new node in between this node's previous node and this node.
            previous_node->next_node = incoming_node;

            // The incoming node's previous node is the same as this node's previous node because it must be
            // a number less than this node but greater than whatever was before this node. Having no number
            // less than this node means the new node's previous node will be the front sentinel.
            incoming_node->previous_node = previous_node;

            // The incoming node's next node is this node since this node is the smallest node that is
            // bigger the the incoming node.
            incoming_node->next_node = this;

            // This incoming node is the biggest node that is smaller than this node and so it must become
            // this node's previous node.
            previous_node = incoming_node;

            // Inserting a node may cause an imbalance starting at this node so we
            // check for imbalance and fix if necessary
            fix_potential_imbalance(to_this);

            // Notify the caller that the insertion was successful by returning true
            return true;
        } else {

            // This node cannot insert the incoming node as its own child so the left_tree must decide if
            // it is able to insert it
            if ( left_tree->insert( obj, left_tree ) ) {
                // If the left tree is successful in inserting the node, we update the height
                update_height();

                // The insertion in the children node may have caused an imbalance at this node so we
                // check for imbalance and fix if necessary
                fix_potential_imbalance(to_this);

                // Notify the caller that the insertion was successful by returning true
                return true;
            } else {

                // Notify the caller that the the node class failed to insert the new node by returning false
                return false;
            }
        }
    } else if ( obj > node_value ) {
        if ( right_tree == nullptr ) {
            auto incoming_node = new Search_tree<Type>::Node( obj );
            right_tree = incoming_node;

            // This node is able to insert the node so we update the height
            update_height();

            // The node is being inserted to the right relative to this node because it is greater than this
            // node. We will tell the node after this node to stop pointing at this node and to now point to
            // the new node, thus placing the new node in between this node's next node and this node.
            next_node->previous_node = incoming_node;

            // The incoming node's next node is the same as this node's next node because it must be
            // a number greater than this node but less than whatever was after this node. Having no
            // number greater means the new node's next node will be the back sentinel.
            incoming_node->next_node = next_node;

            // The incoming node's previous node is this node since this node is the biggest node that is
            // smaller the the incoming node.
            incoming_node->previous_node = this;

            // This incoming node is the smallest node that is bigger than this node and so it must become
            // this node's next node.
            next_node = incoming_node;

            // Inserting a node may cause an imbalance starting at this node so we
            // check for imbalance and fix if necessary
            fix_potential_imbalance(to_this);

            // Notify the caller that the insertion was successful by returning true
            return true;
        } else {

            // This node cannot insert the incoming node as its own child so the right_tree must decide if
            // it is able to insert it
            if ( right_tree->insert( obj, right_tree ) ) {

                // If the right tree is successful in inserting the node, we update the height
                update_height();

                // The insertion in the children node may have caused an imbalance at this node so we
                // check for imbalance and fix if necessary
                fix_potential_imbalance(to_this);

                // Notify the caller that the insertion was successful by returning true
                return true;
            } else {

                // Notify the caller that the the node class failed to insert the new node by returning false
                return false;
            }
        }
    } else {
        return false;
    }
}

template <typename Type>
bool Search_tree<Type>::Node::erase( Type const &obj, Search_tree<Type>::Node *&to_this ) {
    if ( obj < node_value ) {
        if ( left_tree == nullptr ) {
            return false;
        } else {
            if ( left_tree->erase( obj, left_tree ) ) {
                update_height();

                // The erase in the left_tree's children nodes may have caused an imbalance at
                // this node so we check for imbalance and fix if necessary
                fix_potential_imbalance(to_this);

                return true;
            }

            return false;
        }
    } else if ( obj > node_value ) {
        if ( right_tree == nullptr ) {
            return false;
        } else {
            if ( right_tree->erase( obj, right_tree ) ) {
                update_height();

                // The erase in the right_tree's children node may have caused an imbalance at
                // this node so we check for imbalance and fix if necessary
                fix_potential_imbalance(to_this);

                return true;
            }

            return false;
        }
    } else {

        // Assert that this node is the node we intended to delete
        assert( obj == node_value );

        if ( is_leaf() ) {
            // As a leaf node, we don't need to worry about this node's sub trees, pointer to this node
            // now points to nullptr
            to_this = nullptr;

            // Update previous and next pointers for iterative purposes

            // This node's previous node should now point to this node's next node as its next node
            previous_node->next_node = next_node;

            // This node's next node should now point to this node's previous node as its previous node
            next_node->previous_node = previous_node;

            // This node is removed so we set its next pointer to nullptr before we delete it and lose
            // access to this reference
            next_node = nullptr;

            // This node is removed so we set its previous pointer to nullptr before we delete it and lose
            // access to this reference
            previous_node = nullptr;

            // Having cleaned up all instances that rely on this node and references this node makes,
            // we delete this node.
            delete this;

            // In this case, since this is the node being deleted, an imbalance cannot occur at a node that
            // no longer exits, the nested erase calls will handle imbalances at their depths
        } else if ( left_tree == nullptr ) {
            // With only a right sub tree, the pointer to this pointer can now point to the root_node of
            // that right sub tree
            to_this = right_tree;

            // Update previous and next pointers for iterative purposes

            // This node's previous node should now point to this node's next node as its next node
            previous_node->next_node = next_node;

            // This node's next node should now point to this node's previous node as its previous node
            next_node->previous_node = previous_node;

            // This node is removed so we set its next pointer to nullptr before we delete it and lose
            // access to this reference
            next_node = nullptr;

            // This node is removed so we set its previous pointer to nullptr before we delete it and lose
            // access to this reference
            previous_node = nullptr;

            // Having cleaned up all instances that rely on this node and references this node makes,
            // we delete this node.
            delete this;

            // In this case, since this is the node being deleted, an imbalance cannot occur at a node that
            // no longer exits, the nested erase calls will handle imbalances at their depths
        } else if ( right_tree == nullptr ) {
            // With only a left sub tree, the pointer to this pointer can now point to the root_node of
            // that left sub tree
            to_this = left_tree;

            // Update previous and next pointers for iterative purposes

            // This node's previous node should now point to this node's next node as its next node
            previous_node->next_node = next_node;

            // This node's next node should now point to this node's previous node as its previous node
            next_node->previous_node = previous_node;

            // This node is removed so we set its next pointer to nullptr before we delete it and lose
            // access to this reference
            next_node = nullptr;

            // This node is removed so we set its previous pointer to nullptr before we delete it and lose
            // access to this reference
            previous_node = nullptr;

            // Having cleaned up all instances that rely on this node and references this node makes,
            // we delete this node.
            delete this;

            // In this case, since this is the node being deleted, an imbalance cannot occur at a node that
            // no longer exits, the nested erase calls will handle imbalances at their depths
        } else {
            // With this node having both a left and a right subtree, we are forced to change this node's value
            // to be the front most value of the right subtree
            node_value = right_tree->front()->node_value;

            // We have erased the target "value" but must now erase the duplicated value by erasing the copied
            // node value from the right tree
            right_tree->erase( node_value, right_tree );

            // The duplicate node was deleted from the node children of this node so it's height must be updated
            update_height();

            // This erase must be checked to see if it caused any imbalances at this node so we check for
            // imbalance and fix it it occurred
            fix_potential_imbalance(to_this);
        }

        // Since we found the object it must be possible to delete it so once it's been deleted we notify the
        // caller the object was successfully deleted by returning true
        return true;
    }
}

// This function starts at the root node, and prints all the nodes below it
// by adding tabs respective to the nodes' depth in the search tree
// Thank you to Gerald NDA for this printer function!
template <typename Type>
void Search_tree<Type>::Node::print( std::ostream &out, int tabs ) {
    for(int i = 0; i < tabs; i++){
        out << " ";
    }
    out << (tabs ? "`==" : "");
    if(this == nullptr){
        out << "xx";
        out << " h=-1" << std::endl;
    } else {
        out << node_value;
        out << " h=" << height() << std::endl;
        left_tree->print(out, tabs+1);
        right_tree->print(out, tabs+1);
    }
}

//////////////////////////////////////////////////////////////////////
//                   Iterator Private Constructor                   //
//////////////////////////////////////////////////////////////////////

template <typename Type>
Search_tree<Type>::Iterator::Iterator( Search_tree<Type> *tree, typename Search_tree<Type>::Node *starting_node ):
        containing_tree( tree ),
        current_node( starting_node ) {
    // This is done for you...
    // Does nothing...
}

//////////////////////////////////////////////////////////////////////
//                 Iterator Public Member Functions                 //
//////////////////////////////////////////////////////////////////////

template <typename Type>
Type Search_tree<Type>::Iterator::operator*() const {
    // This is done for you...
    return current_node->node_value;
}

template <typename Type>
typename Search_tree<Type>::Iterator &Search_tree<Type>::Iterator::operator++() {
    // Update the current node to the node containing the next higher value
    // If we are already at end, do nothing

    // If we want the next node but there is no next node because we are at the end, we
    // return the last node
    if (current_node == containing_tree->back_sentinel) {
        // We update the boolean is_end flag to say we are at the end
        is_end = true;
        return *this;
    }

    // Realizing we are not at the end yet, move the current node forward to the node
    // which comes after it
    current_node = current_node->next_node;


    is_end = false;

    // Return this iterator class with its newly updated incremented current_node
    return *this;
}

template <typename Type>
typename Search_tree<Type>::Iterator &Search_tree<Type>::Iterator::operator--() {
    // Update the current node to the node containing the next smaller value
    // If we are already at either rend, do nothing

    // If we want the previous node but there is no previous node because we are at the start, we
    // return the first node
    if (current_node == containing_tree->front_sentinel) {
        is_end = true;
        return *this;
    }

    // Realizing we are not at the start yet, move the current node back to the node
    // which comes before it
    current_node = current_node->previous_node;

    is_end = false;

    // Return this iterator class with its newly updated decremented current_node
    return *this;
}

template <typename Type>
bool Search_tree<Type>::Iterator::operator==( typename Search_tree<Type>::Iterator const &rhs ) const {
    // This is done for you...
    return ( current_node == rhs.current_node );
}

template <typename Type>
bool Search_tree<Type>::Iterator::operator!=( typename Search_tree<Type>::Iterator const &rhs ) const {
    // This is done for you...
    return ( current_node != rhs.current_node );
}

//////////////////////////////////////////////////////////////////////
//                            Friends                               //
//////////////////////////////////////////////////////////////////////

// You can modify this function however you want:  it will not be tested

template <typename T>
std::ostream &operator<<( std::ostream &out, Search_tree<T> const &list ) {
    list.root_node->print(out);
    return out;
}

#endif
