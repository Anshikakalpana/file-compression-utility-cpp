#ifndef HUFFMANCODING_HUFFER_H
#define HUFFMANCODING_HUFFER_H
#include <sstream>
#include <bitset>
#include <string>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <memory>
#include <queue>
#include "Node.h"

using namespace std;

class Huffman {
    unordered_map<char, string> codeMap;

    // Internal helper functions using shared_ptr<Node>
    void encodeCharacters(shared_ptr<Node> rootNode, string codeString);
    void writeHeader(ofstream &outputStream);
    void readHeader(ifstream &inputStream);
    shared_ptr<Node> buildDecodingTree(unordered_map<char, string> encodingMap);
    void decompressToFile(string codeString, shared_ptr<Node> rootNode, string decompressedFileName);

    // Comparator for priority_queue
    class myCompartor {
    public:
        bool operator()(shared_ptr<Node> node1, shared_ptr<Node> node2) {
            return node1->getFrequency() > node2->getFrequency();
        }
    };

public:
    // Public interface
    void huffer(unordered_map<char, int> frequencyMap);
    void deHuffer(string compressedFileName, string decompressedFileName);
    void compressTofile(string InputfileName, string OutputfileName);
};

#endif

