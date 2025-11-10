#include <bits/stdc++.h> // correct include
using namespace std;

#ifndef FREQUENCY_ANALYZER_H
#define FREQUENCY_ANALYZER_H

class CharFrequencyAnalyzer{
    unordered_map<char , int>frequencyMap ;

    
public:
    const unordered_map<char, int> &getFrequencyMap() const;
    void readFile(string fileName);
};

#endif 