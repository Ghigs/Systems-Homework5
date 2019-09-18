//
//  BitVector.hpp
//  Homework5
//
//  Created by Jason Ghiglieri on 2/19/18.
//  Copyright © 2018 Jason Ghiglieri. All rights reserved.
//

#ifndef BitVector_hpp
#define BitVector_hpp

#include <stdio.h>
#include <iostream>


class BitVector
{
public:
    BitVector(int bits);
    ~BitVector();
    void fillVector();
    void clearVector();
    bool getBit(int index);
    void setBit(int index, bool value);
    int size();
    void resize(int bits);
private:
    uint8_t* array;
    int sizeArray;
};


BitVector::BitVector(int bits)
{
    sizeArray = ((bits/8) + 1);
    
    array = new uint8_t[sizeArray];
}

BitVector::~BitVector()
{
    delete [] array;
    array = NULL;
}

void BitVector::fillVector()
{
    for (int i = 0; i < sizeArray; i++)
    {
        array[i] = ~0;
    }
}

void BitVector::clearVector()
{
    for (int i = 0; i < sizeArray; i++)
    {
        array[i] = 0;
    }
}

bool BitVector::getBit(int index)
{
    int byte = index/8;
    uint8_t bit = index%8;
    
    uint8_t comparator = 1 << bit;
    
    if ((array[byte] | comparator) == array[byte])
    {
        return true;
    }
    else
    {
        return false;
    }
}

void BitVector::setBit(int index, bool value)
{
    int byte = index/8;
    uint8_t bit = index%8;
    
    uint8_t comparator = 1 << bit;
    
    if (value)
    {
        array[byte] = array[byte] | comparator;
    }
    else
    {
        comparator = ~comparator;
        array[byte] = array[byte] & comparator;
    }
}

int BitVector::size()
{
    return sizeArray;
}

void BitVector::resize(int bits)
{
    int loopSize = sizeArray;
    sizeArray = (bits/8) + 1;
    uint8_t* newArray = new uint8_t[sizeArray];
    
    
    if (loopSize > sizeArray)
    {
        loopSize = sizeArray;
    }
    
    for (int i = 0; i < loopSize; i++)
    {
        newArray[i] = array[i];
    }
    
    array = newArray;
}


#endif /* BitVector_hpp */
