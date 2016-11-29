//
//  main.cpp
//  AHO-KORASIK
//
//  Created by Alexander Malyshev on 13.11.15.
//  Copyright © 2015 Alexander Malyshev. All rights reserved.
//

#include <iostream>
#include "IGOR.h"
//#include "AKNode.h"
#include "AKMaskAutomata.h"

int main(int argc, const char * argv[]) {
    std::string pattern;
    std::string textToProcess;
    
    std::cin>> pattern;
    std::cin>> textToProcess;
    
    AKMaskAutomata blackNiggerAutomata(pattern);
    
    blackNiggerAutomata.processTextAndPrintResult(textToProcess);

    return 0;
}
