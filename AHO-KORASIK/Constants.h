//
//  Constants.h
//  AHO-KORASIK
//
//  Created by Alexander Malyshev on 13.11.15.
//  Copyright © 2015 Alexander Malyshev. All rights reserved.
//

#ifndef Constants_h
#define Constants_h
typedef long long IDType;
typedef unsigned long long StringSizeType;
typedef char LiteralType;
class Const {
public:
    static char EmptySymbol() { return '\0'; };
    static IDType EmptyNodeID() { return -1; };
    static short DefaultAlphabetSize() { return 26; };
};

#endif /* Constants_h */
