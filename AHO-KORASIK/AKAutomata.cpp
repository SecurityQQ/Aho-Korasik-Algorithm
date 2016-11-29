//
//  AKAutomata.cpp
//  AHO-KORASIK
//
//  Created by Alexander Malyshev on 16.11.15.
//  Copyright © 2015 Alexander Malyshev. All rights reserved.
//

#include "AKAutomata.h"
#include <iostream>
AKNode * AKAutomata:: root() {
    if (!_root) {
        _root = new AKNode();
    }
    return _root;
}

AKAutomata:: AKAutomata() {
    _root = new AKNode();
}


void AKAutomata:: addString(const std::string string) {
    patterns.push_back(string);
    AKNode *currentNode = _root;
    for (StringSizeType i = 0; i < string.size(); ++i) {
        if (currentNode->getChild(string[i]) == nullptr) {
            AKNode *child = new AKNode(string[i], currentNode);
            currentNode->setChild(child);
        }
        currentNode = currentNode->getChild(string[i]);
    }
    currentNode->addTerminateString(patterns.size() - 1);
}