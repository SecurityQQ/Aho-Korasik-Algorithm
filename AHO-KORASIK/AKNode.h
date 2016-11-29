//
//  AKNode.h
//  AHO-KORASIK
//
//  Created by Alexander Malyshev on 13.11.15.
//  Copyright © 2015 Alexander Malyshev. All rights reserved.
//

#ifndef AKNode_h
#define AKNode_h
#include "Constants.h"
#include <vector>

class AKNode {
public:
    AKNode(LiteralType symbol = Const::EmptySymbol(),  AKNode *parent = nullptr);
    void addTerminateString(IDType stringID);
    AKNode *getTransition(const LiteralType symbol);
    AKNode *getChild(const LiteralType symbol);
    AKNode *getSuffixLink();
    AKNode *getCompressedSuffixLink();
    std::vector<IDType> & getStringIDs();
    
    bool isRoot();
    bool isTerminal();
private:

    LiteralType charToParent;
    std::vector<AKNode *> childs;
    std::vector<AKNode *> transitions;
    AKNode *parent;
    AKNode *suffixLink;
    AKNode *compressedSuffixLink;
    std::vector<IDType> patternsIDs;
    
    void    setTransition(const LiteralType symbol, AKNode *newValue);
    void    setChild(AKNode *newValue);
    void setTerminatingString(const IDType stringID);
    AKNode *_nextTransition(AKNode *node, const LiteralType symbol);
    friend class AKAutomata;
};


#endif /* AKNode_h */
