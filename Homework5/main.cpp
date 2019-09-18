//
//  main.cpp
//  Homework5
//
//  Created by Jason Ghiglieri on 2/19/18.
//  Copyright © 2018 Jason Ghiglieri. All rights reserved.
//

#include <iostream>
#include "BitVector.hpp"
#include "VoxelShape.hpp"

int main(int argc, const char * argv[])
{
    int nx = 50;
    int ny = 30;
    int nz = 50;
    
    VoxelShape shape = VoxelShape(nx, ny, nz);
    
    //shape.fillShape();
    // Creates a model resembling a ship from Star Wars.
    // Look up "Trade Federation Battleship" on google images to see the inspiration.
    shape.addSphere(25, 15, 25, 25);
    shape.subtractSphere(25, 15, 25, 18);
    for (int x = 0; x < nx; x++)
    {
        for (int y = 0; y < ny; y++)
        {
            for (int z = 0; z < nz; z++)
            {
                
                if (y < 12 || y > 18 || (z < 25 && (x > 18 && x < 32)))
                {
                    shape.clearBit(x, y, z);
                }
                
                if (z > 33 && (x > 22 && x < 28) && (y > 12 && y < 18))
                {
                    shape.setBit(x, y, z);
                }
            }
        }
    }
    shape.addSphere(25, 15, 27, 7);
    
    
    shape.writeSTL(shape);
}
