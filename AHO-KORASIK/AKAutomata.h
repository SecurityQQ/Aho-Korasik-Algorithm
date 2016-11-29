//
//  AKAutomata.h
//  AHO-KORASIK
//
//  Created by Alexander Malyshev on 13.11.15.
//  Copyright © 2015 Alexander Malyshev. All rights reserved.
//

#ifndef AKAutomata_h
#define AKAutomata_h
#include "Constants.h"
#include "AKNode.h"

class AKNode;

class AKAutomata {
public:
    AKAutomata();
    
    AKNode *root();
    
    void addString(const std::string string);
    void findPattern(IDType patternID);
protected:
    AKNode *_root;
    std::vector<std::string> patterns;
};

#endif /* AKAutomata_h */
