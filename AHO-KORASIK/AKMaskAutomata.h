//
//  AKMaskAutomata.h
//  AHO-KORASIK
//
//  Created by Alexander Malyshev on 16.11.15.
//  Copyright © 2015 Alexander Malyshev. All rights reserved.
//

#ifndef AKMaskAutomata_h
#define AKMaskAutomata_h
#include "AKAutomata.h"

class AKMaskAutomata: public AKAutomata {
public:
    AKMaskAutomata(const std::string &regExp);
    void processTextAndPrintResult(const std::string &text);
private:
    
    StringSizeType maskRegExpSize;
    std::vector<StringSizeType> patternsCount;
    std::vector<StringSizeType> startPatternPositions;
};

#endif /* AKMaskAutomata_h */
