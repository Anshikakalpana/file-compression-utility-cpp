#include "CharFrequencyAnalyzer.h"



void CharFrequencyAnalyzer::readFile(string fileName){
     char character;
     ifstream inputFile;

     inputFile.open(fileName , ios::in);
     while(inputFile.get(character)){
        frequencyMap[character]++;

     
     }
        inputFile.close();
}

const unordered_map<char, int> &CharFrequencyAnalyzer::getFrequencyMap() const {
    return frequencyMap;
}