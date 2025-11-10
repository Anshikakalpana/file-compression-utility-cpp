#include <iostream>
#include <fstream>
#include <unordered_map>
#include <ctime>
#include "Huffman.h"

using namespace std;

// Simple function to calculate file size
long long filesize(const string& filename) {
    ifstream in(filename, ios::ate | ios::binary);
    if(!in) return -1;
    return in.tellg();
}

// Simple frequency map builder
unordered_map<char,int> buildFrequencyMap(const string& filename) {
    unordered_map<char,int> freq;
    ifstream file(filename, ios::binary);
    if(!file.is_open()) {
        cerr << "Error opening input file: " << filename << endl;
        return freq;
    }
    char ch;
    while(file.get(ch)) freq[ch]++;
    file.close();
    return freq;
}

int main() {
    string inputFile = "input.txt";
    string compressedFile = "compressed.huff";
    string decompressedFile = "decompressed.txt";

    cout << "Enter mode ('compress' or 'decompress'): ";
    string mode;
    cin >> mode;

    Huffman huff;

    if(mode == "compress") {
        if(filesize(inputFile) <= 0) {
            cerr << "Input file missing or empty!" << endl;
            return 1;
        }

        cout << "Building frequency map..." << endl;
        auto freqMap = buildFrequencyMap(inputFile);

        cout << "Building Huffman tree..." << endl;
        huff.huffer(freqMap);

        cout << "Compressing file..." << endl;
        clock_t start = clock();
        huff.compressTofile(inputFile, compressedFile);
        cout << "Compression done in " << (1.0*(clock()-start)/CLOCKS_PER_SEC) << " sec" << endl;

        cout << "Input Size: " << filesize(inputFile) << " bytes" << endl;
        cout << "Compressed Size: " << filesize(compressedFile) << " bytes" << endl;
        cout << "Compression Ratio: " 
             << (1.0 * filesize(compressedFile) / filesize(inputFile)) << endl;

    } else if(mode == "decompress") {
        if(filesize(compressedFile) <= 0) {
            cerr << "Compressed file missing or empty!" << endl;
            return 1;
        }

        cout << "Decompressing file..." << endl;
        clock_t start = clock();
        huff.deHuffer(compressedFile, decompressedFile);
        cout << "Decompression done in " << (1.0*(clock()-start)/CLOCKS_PER_SEC) << " sec" << endl;

        cout << "Compressed Size: " << filesize(compressedFile) << " bytes" << endl;
        cout << "Decompressed Size: " << filesize(decompressedFile) << " bytes" << endl;

    } else {
        cerr << "Invalid mode! Use 'compress' or 'decompress'." << endl;
        return 1;
    }

    return 0;
}
