//
//  AKNode.cpp
//  AHO-KORASIK
//
//  Created by Alexander Malyshev on 16.11.15.
//  Copyright © 2015 Alexander Malyshev. All rights reserved.
//

#include "AKNode.h"
#include <assert.h>

bool AKNode:: isRoot() {
    return parent == nullptr;
}

void AKNode:: addTerminateString(IDType stringID) {
    patternsIDs.push_back(stringID);
}

AKNode:: AKNode(LiteralType symbol, AKNode *parent) {
    childs.assign(Const::DefaultAlphabetSize(), nullptr);
    transitions.assign(Const::DefaultAlphabetSize(), nullptr);
    charToParent = (parent != nullptr) ? symbol : '\0';
    this->parent = parent;
    suffixLink = nullptr;
    compressedSuffixLink = nullptr;
}

void AKNode:: setTerminatingString(const IDType stringID) {
    patternsIDs.push_back(stringID);
}

AKNode * AKNode:: getSuffixLink() {
    if (suffixLink == nullptr) {
        if (isRoot()) {
            suffixLink = this;
        } else if (parent->isRoot()) {
            suffixLink = parent;
        } else {
            suffixLink = _nextTransition(parent->getSuffixLink(), charToParent);
        }
    }
    return suffixLink;
}

AKNode * AKNode:: _nextTransition(AKNode *node, const LiteralType symbol) {
    if (node->transitions.at(symbol - 'a') == nullptr) {
        if (node->getChild(symbol)) {
            node->setTransition(symbol, node->getChild(symbol));
        } else if (node->isRoot()) {
            node->setTransition(symbol, node);
        } else {
            node->setTransition(symbol, _nextTransition(node->getSuffixLink(), symbol));
        }
    }
    return node->transitions.at(symbol - 'a');
}

AKNode * AKNode:: getCompressedSuffixLink() {
    if (compressedSuffixLink == nullptr) {
        auto link = getSuffixLink();
        if (link->isTerminal()) {
            compressedSuffixLink = link;
        } else if (link->isRoot()) {
            compressedSuffixLink = link;
        } else {
            compressedSuffixLink = link->getCompressedSuffixLink();
        }
    }
    return compressedSuffixLink;
}

bool AKNode:: isTerminal() {
    return patternsIDs.size() > 0;
}

AKNode * AKNode::getTransition(const LiteralType symbol) {
    assert('a' <= symbol && symbol >= 'A');
    return _nextTransition(this, symbol);
}
void AKNode:: setTransition(const LiteralType symbol, AKNode *newValue) {
    assert('a' <= symbol && symbol >= 'A');
    transitions.at(symbol - 'a') = newValue;
}

AKNode * AKNode::getChild(const LiteralType symbol) {
    assert('a' <= symbol && symbol >= 'A');
    return childs.at(symbol - 'a');
}

std::vector<IDType> & AKNode:: getStringIDs() {
    return patternsIDs;
}

void AKNode:: setChild(AKNode *newValue) {
    assert('a' <= newValue->charToParent && newValue->charToParent >= 'A');
    childs.at(newValue->charToParent - 'a') = newValue;
}
