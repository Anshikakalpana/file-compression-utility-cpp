#ifndef HUFFMAN_NODE_H
#define HUFFMAN_NODE_H

#include <memory>
using namespace std;

class Node {
public:
    
    char character;              // The actual character (like 'A', 'B', etc.)
    int frequency;               // How many times this character appears
    shared_ptr<Node> left;       // Pointer to left child (for Huffman tree)
    shared_ptr<Node> right;      // Pointer to right child (for Huffman tree)


    // --- Constructors ---

    // Constructor: sets both character and frequency
    Node(char ch, int freq)
        : character(ch), frequency(freq), left(nullptr), right(nullptr) {}

    // Constructor: sets only character, frequency becomes 0
    explicit Node(char ch)
        : character(ch), frequency(0), left(nullptr), right(nullptr) {}


    // --- Getter Functions ---

    // Get frequency value
    int getFrequency() const { 
        return frequency; 
    }

    // Get character value
    char getCharacter() const { 
        return character; 
    }


    // --- Setter Functions (for child connections) ---

    // Connect left child node
    void setLeft(shared_ptr<Node> l) { 
        left = l; 
    }

    // Connect right child node
    void setRight(shared_ptr<Node> r) { 
        right = r; 
    }


    // --- Getter Functions (for child access) ---

    // Get left child node
    shared_ptr<Node> getLeft() const { 
        return left; 
    }

    // Get right child node
    shared_ptr<Node> getRight() const { 
        return right; 
    }
};

#endif
