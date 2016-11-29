  //
//  AKMastAutomata.cpp
//  AHO-KORASIK
//
//  Created by Alexander Malyshev on 16.11.15.
//  Copyright © 2015 Alexander Malyshev. All rights reserved.
//

#include "AKMaskAutomata.h"
#include <iostream>

AKMaskAutomata:: AKMaskAutomata(const std::string& regExp) {
    std::string subPattern;
    for (unsigned long long i = 0; i < regExp.size(); ++i) {
        if ((regExp[i] >= 'a' && regExp[i] <= 'z') || regExp[i] == '?') {
            if (regExp[i] == '?') {
                if (subPattern != "") {
                    addString(subPattern);
                    subPattern = "";
                }
            } else {
                if (subPattern == "") {
                    startPatternPositions.push_back(i);
                }
                subPattern += regExp[i];
            }
        }
    }
    if (subPattern != "") {
        addString(subPattern);
    }

    maskRegExpSize = regExp.size();
}

void AKMaskAutomata:: processTextAndPrintResult(const std::string &text) {
    std::vector<StringSizeType> subPatternCounter(text.length() + 1, 0);
    auto currentState = root();
    for (unsigned long long i = 0; i < text.size(); ++i) {
        if ('a'<= text[i] && text[i] <= 'z') {
            currentState = currentState->getTransition(text[i]);
            auto movingState = currentState;
            while (movingState != root()) {
                if (movingState->isTerminal()) {
                    auto stringIDs = movingState->getStringIDs();
                        for (auto id = stringIDs.begin(); id != stringIDs.end(); ++id) {
                            long long index = i + 1 - (long long)patterns[*id].length() - (long long)startPatternPositions[*id];
                            if (index >= 0) {
                                ++subPatternCounter[index];
                            }
                        }
                }
                movingState = movingState->getCompressedSuffixLink();
            }
        }
    }
    
    for (StringSizeType i = 0; i < subPatternCounter.size() + 1 - maskRegExpSize; ++i) {
        if (startPatternPositions.size() <= subPatternCounter[i]) {
            std::cout<< i << " ";
        }
    }
}

