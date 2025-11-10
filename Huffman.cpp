#include "Huffman.h"
#include <fstream>
#include <sstream>
#include <bitset>
#include <queue>

#define INTERNAL_NODE_CHARACTER char(128)
#define PSEUDO_EOF char(129)
#define CHARACTER_CODE_SEPERATOR char(130)
#define HEADER_ENTRY_SEPERATOR char(131)
#define HEADER_TEXT_SEPERATOR char(132)


// Build Huffman tree
void Huffman::huffer(std::unordered_map<char, int> frequencyMap) {
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, myCompartor> HufferQueue;
    std::string tempString;
    std::shared_ptr<Node> leftNode, rightNode, newNode;

    for (const auto &item : frequencyMap)
        HufferQueue.push(std::make_shared<Node>(item.first, item.second));

    HufferQueue.push(std::make_shared<Node>(PSEUDO_EOF, 1));

    while (HufferQueue.size() != 1) {
        leftNode = HufferQueue.top();
        HufferQueue.pop();
        rightNode = HufferQueue.top();
        HufferQueue.pop();

        newNode = std::make_shared<Node>(INTERNAL_NODE_CHARACTER, leftNode->getFrequency() + rightNode->getFrequency());
        newNode->setLeft(leftNode);
        newNode->setRight(rightNode);
        HufferQueue.push(newNode);
    }

    encodeCharacters(HufferQueue.top(), tempString);
}

// Encode characters recursively
void Huffman::encodeCharacters(std::shared_ptr<Node> rootNode, std::string codeString) {
    if (!rootNode) return;

    if (rootNode->getCharacter() != INTERNAL_NODE_CHARACTER)
        codeMap[rootNode->getCharacter()] = codeString;

    encodeCharacters(rootNode->getLeft(), codeString + "0");
    encodeCharacters(rootNode->getRight(), codeString + "1");
}

// Compress file
void Huffman::compressTofile(std::string InputfileName, std::string OutputfileName) {
    char character;
    std::string file;
    std::ifstream inputStream(InputfileName, std::ios::in);
    std::ofstream outputStream(OutputfileName, std::ios::out);

    writeHeader(outputStream);

    while (inputStream.get(character))
        file += codeMap[character];

    inputStream.close();

    file += codeMap[PSEUDO_EOF];

    unsigned long remainder = (file.size() - 1) % 8;
    for (int i = 0; i < 8 - remainder; ++i)
        file += '0';

    std::stringstream stringStream(file);

    while (stringStream.good()) {
        std::bitset<8> bits;
        stringStream >> bits;
        char c = char(bits.to_ulong());
        outputStream << c;
    }

    outputStream.flush();
    outputStream.close();
}

// Write Huffman header
void Huffman::writeHeader(std::ofstream &outputStream) {
    for (const auto &item : codeMap)
        outputStream << item.first << CHARACTER_CODE_SEPERATOR << item.second << HEADER_ENTRY_SEPERATOR;
    outputStream << HEADER_TEXT_SEPERATOR;
}

// Decompress file
void Huffman::deHuffer(std::string compressedFileName, std::string decompressedFileName) {
    char character;
    std::string codeString;
    std::ifstream inputStream(compressedFileName, std::ios::in);

    readHeader(inputStream);

    while (inputStream.get(character)) {
        std::bitset<8> bits(character);
        codeString += bits.to_string();
    }

    auto rootNode = buildDecodingTree(codeMap);
    decompressToFile(codeString, rootNode, decompressedFileName);
}

// Read Huffman header
void Huffman::readHeader(std::ifstream &inputStream) {
    codeMap.clear();
    char character;
    inputStream.get(character);
    char key = character;

    while (character != HEADER_TEXT_SEPERATOR) {
        if (character == CHARACTER_CODE_SEPERATOR) {
            inputStream.get(character);
            while (character != HEADER_ENTRY_SEPERATOR) {
                codeMap[key] += character;
                inputStream.get(character);
            }
        } else {
            key = character;
        }
        inputStream.get(character);
    }
}

// Build decoding tree from code map
std::shared_ptr<Node> Huffman::buildDecodingTree(std::unordered_map<char, std::string> encodingMap) {
    auto rootNode = std::make_shared<Node>(INTERNAL_NODE_CHARACTER);
    std::shared_ptr<Node> previousNode;

    for (const auto &item : encodingMap) {
        previousNode = rootNode;
        auto newNode = std::make_shared<Node>(item.first);
        std::string characterCode = item.second;

        for (int i = 0; i < characterCode.size(); ++i) {
            if (characterCode[i] == '0') {
                if (i == characterCode.size() - 1)
                    previousNode->setLeft(newNode);
                else {
                    if (!previousNode->getLeft()) {
                        previousNode->setLeft(std::make_shared<Node>(INTERNAL_NODE_CHARACTER));
                        previousNode = previousNode->getLeft();
                    } else previousNode = previousNode->getLeft();
                }
            } else {
                if (i == characterCode.size() - 1)
                    previousNode->setRight(newNode);
                else {
                    if (!previousNode->getRight()) {
                        previousNode->setRight(std::make_shared<Node>(INTERNAL_NODE_CHARACTER));
                        previousNode = previousNode->getRight();
                    } else previousNode = previousNode->getRight();
                }
            }
        }
    }

    return rootNode;
}

// Decompress code string to file
void Huffman::decompressToFile(std::string codeString, std::shared_ptr<Node> rootNode, std::string decompressedFileName) {
    std::ofstream outputStream(decompressedFileName, std::ios::out);
    auto traversingPointer = rootNode;

    for (size_t i = 0; i < codeString.size(); ++i) {
        if (codeString[i] == '0')
            traversingPointer = traversingPointer->getLeft();
        else
            traversingPointer = traversingPointer->getRight();

        if (traversingPointer->getCharacter() != INTERNAL_NODE_CHARACTER) {
            if (traversingPointer->getCharacter() == PSEUDO_EOF)
                break;
            outputStream << traversingPointer->getCharacter();
            traversingPointer = rootNode;
        }
    }

    outputStream.flush();
    outputStream.close();
}
