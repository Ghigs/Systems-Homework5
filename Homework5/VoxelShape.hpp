//
//  VoxelShape.hpp
//  Homework5
//
//  Created by Jason Ghiglieri on 2/22/18.
//  Copyright © 2018 Jason Ghiglieri. All rights reserved.
//

#ifndef VoxelShape_h
#define VoxelShape_h

#include "BitVector.hpp"
#include <cmath>

enum FaceType {
    NX,
    NY,
    NZ,
    PX,
    PY,
    PZ
};


struct Triangle{
    float normal[3];
    float v1[3];
    float v2[3];
    float v3[3];
};


class VoxelShape
{
public:
    VoxelShape(int x, int y, int z);
    ~VoxelShape();
    
    void fillShape();
    void clearShape();
    bool getBit(int x, int y, int z);
    void setBit(int x, int y, int z);
    void clearBit(int x, int y, int z);
    void toggleBit(int x, int y, int z);
    
    void addSphere(float cx, float cy, float cz, float radius);
    void subtractSphere(float cx, float cy, float cz, float radius);
    void toggleSphere(float cx, float cy, float cz, float radius);
    
    void writeSTL(VoxelShape& shape);
private:
    int initX;
    int initY;
    int initZ;
    BitVector* vector;
    
    int getIndex(int x, int y, int z);
    
    
    uint32_t findTriangles(int x, int y, int z, VoxelShape& shape, FILE* file);
    void writeTriangles(Triangle& t1, Triangle& t2, FILE* file);
    void extractFace(int x, int y, int z, FaceType face, Triangle& t1, Triangle& t2);
    inline void fillPlane(int a1, int a2, int b1, int b2, int c, int cInd, Triangle& t1, Triangle& t2);
};



int VoxelShape::getIndex(int x, int y, int z)
{
    int index = ((x * (initY * initZ)) + (y * initZ) + z);
    return index;
}


VoxelShape::VoxelShape(int x, int y, int z)
{
    initX = x;
    initY = y;
    initZ = z;
    vector = new BitVector(x*y*z);
}

VoxelShape::~VoxelShape()
{
    vector->~BitVector();
    vector = NULL;
    initX = initY = initZ = 0;
}

void VoxelShape::fillShape()
{
    vector->fillVector();
}

void VoxelShape::clearShape()
{
    vector->clearVector();
}

bool VoxelShape::getBit(int x, int y, int z)
{
    return vector->getBit(getIndex(x,y,z));
}

void VoxelShape::setBit(int x, int y, int z)
{
    vector->setBit(getIndex(x,y,z), true);
}

void VoxelShape::clearBit(int x, int y, int z)
{
    vector->setBit(getIndex(x,y,z), false);
}

void VoxelShape::toggleBit(int x, int y, int z)
{
    int index = getIndex(x,y,z);
    if (vector->getBit(index))
    {
        vector->setBit(index, true);
    }
    else
    {
        vector->setBit(index, false);
    }
}

void VoxelShape::addSphere( float cx, float cy, float cz, float radius)
{
    for (int x = 0; x < initX; x++)
    {
        for (int y = 0; y < initY; y++)
        {
            for (int z = 0; z < initZ; z++)
            {
                if (pow(x-cx, 2) + pow(y-cy, 2) + pow(z-cz, 2) < pow(radius, 2))
                {
                    setBit(x, y, z);
                }
            }
        }
    }
}


void VoxelShape::subtractSphere(float cx, float cy, float cz, float radius)
{
    for (int x = 0; x < initX; x++)
    {
        for (int y = 0; y < initY; y++)
        {
            for (int z = 0; z < initZ; z++)
            {
                if (pow(x-cx, 2) + pow(y-cy, 2) + pow(z-cz, 2) < pow(radius, 2))
                {
                    clearBit(x, y, z);
                }
            }
        }
    }
}


void VoxelShape::toggleSphere(float cx, float cy, float cz, float radius)
{
    for (int x = 0; x < initX; x++)
    {
        for (int y = 0; y < initY; y++)
        {
            for (int z = 0; z < initZ; z++)
            {
                if (pow(x-cx, 2) + pow(y-cy, 2) + pow(z-cz, 2) < pow(radius, 2))
                {
                    toggleBit(x, y, z);
                }
            }
        }
    }
}






void VoxelShape::writeSTL(VoxelShape& shape)
{
    FILE* file = fopen("shapeFile.stl", "wb+");
    
    uint32_t numTris = 0;
    
    fseek(file, 84, SEEK_SET);
    
    
    for (int x = 0; x < shape.initX; x++)
    {
        for (int y = 0; y < shape.initY; y++)
        {
            for (int z = 0; z < shape.initZ; z++)
            {
                if (shape.getBit(x, y, z))
                {
                    numTris += findTriangles(x, y, z, shape, file);
                }
            }
        }
    }
    
    
    fseek(file, 80, SEEK_SET);
    fwrite(&numTris, sizeof(uint32_t), 1, file);
    fclose(file);
}


uint32_t VoxelShape::findTriangles(int x, int y, int z, VoxelShape& shape, FILE* file)
{
    uint32_t numTris = 0;
    
    if (x-1 < 0 || !shape.getBit(x-1, y, z))
    {
        FaceType f = NX;
        Triangle t1;
        Triangle t2;
        
        extractFace(x, y, z, f, t1, t2);
        
        writeTriangles(t1, t2, file);
        numTris += 2;
    }
    
    if (x+1 > shape.initX-1 || !shape.getBit(x+1, y, z))
    {
        FaceType f = PX;
        Triangle t1;
        Triangle t2;
        
        extractFace(x, y, z, f, t1, t2);
        
        writeTriangles(t1, t2, file);
        numTris += 2;
    }
    
    
    if (y-1 < 0 || !shape.getBit(x, y-1, z))
    {
        FaceType f = NY;
        Triangle t1;
        Triangle t2;
        
        extractFace(x, y, z, f, t1, t2);
        
        writeTriangles(t1, t2, file);
        numTris += 2;
    }
    
    if (y+1 > shape.initY-1 || !shape.getBit(x, y+1, z))
    {
        FaceType f = PY;
        Triangle t1;
        Triangle t2;
        
        extractFace(x, y, z, f, t1, t2);
        
        writeTriangles(t1, t2, file);
        numTris += 2;
    }
    
    
    if (z-1 < 0 || !shape.getBit(x, y, z-1))
    {
        FaceType f = NZ;
        Triangle t1;
        Triangle t2;
        
        extractFace(x, y, z, f, t1, t2);
        
        writeTriangles(t1, t2, file);
        numTris += 2;
    }
    
    if (z+1 > shape.initZ-1 || !shape.getBit(x, y, z+1))
    {
        FaceType f = PZ;
        Triangle t1;
        Triangle t2;
        
        extractFace(x, y, z, f, t1, t2);
        
        writeTriangles(t1, t2, file);
        numTris += 2;
    }
    
    return numTris;
}


void VoxelShape::writeTriangles(Triangle& t1, Triangle& t2, FILE* file)
{
    fwrite(&t1.normal, sizeof(float), 3, file);
    
    fwrite(&t1.v1, sizeof(float), 3, file);
    
    fwrite(&t1.v2, sizeof(float), 3, file);
    
    fwrite(&t1.v3, sizeof(float), 3, file);
    
    fseek(file, 2, SEEK_CUR);
    
    
    fwrite(&t2.normal, sizeof(float), 3, file);
    
    fwrite(&t2.v1, sizeof(float), 3, file);
    
    fwrite(&t2.v2, sizeof(float), 3, file);
    
    fwrite(&t2.v3, sizeof(float), 3, file);
    
    fseek(file, 2, SEEK_CUR);
}


void VoxelShape::extractFace(int x, int y, int z, FaceType face, Triangle& t1, Triangle& t2)
{
    for(int i= 0; i < 3; i++){
        t1.normal[i] = 0;
        t2.normal[i] = 0;
    }
    switch(face){
        case NX:
            t1.normal[0] = -1;
            t2.normal[0] = -1;
            fillPlane(y + 1, y, z, z+1, x, 0, t1, t2);
            break;
        case NY:
            t1.normal[1] = -1;
            t2.normal[1] = -1;
            fillPlane(z + 1, z, x, x+1, y, 1, t1, t2);
            break;
        case NZ:
            t1.normal[2] = -1;
            t2.normal[2] = -1;
            fillPlane(x + 1, x, y, y + 1, z, 2, t1, t2);
            break;
        case PX:
            t1.normal[0] = 1;
            t2.normal[0] = 1;
            fillPlane(y, y + 1, z, z +1, x + 1, 0, t1, t2);
            break;
        case PY:
            t1.normal[1] = 1;
            t2.normal[1] = 1;
            fillPlane(z, z + 1, x, x + 1, y + 1, 1, t1, t2);
            break;
        case PZ:
            t1.normal[2] = 1;
            t2.normal[2] = 1;
            fillPlane(x, x + 1, y, y + 1, z + 1, 2, t1, t2);
            break;
    }
}


inline void VoxelShape::fillPlane(int a1, int a2, int b1, int b2, int c, int cInd, Triangle& t1, Triangle& t2){
    t1.v1[cInd] = c;
    t2.v1[cInd] = c;
    t1.v2[cInd] = c;
    t2.v2[cInd] = c;
    t1.v3[cInd] = c;
    t2.v3[cInd] = c;
    int aInd = (cInd +1) % 3;
    int bInd = (cInd +2) % 3;
    
    t1.v1[aInd] = a1;
    t1.v2[aInd] = a2;
    t1.v3[aInd] = a2;
    
    t2.v1[aInd] = a1;
    t2.v2[aInd] = a2;
    t2.v3[aInd] = a1;
    
    t1.v1[bInd] = b1;
    t1.v2[bInd] = b1;
    t1.v3[bInd] = b2;
    
    t2.v1[bInd] = b1;
    t2.v2[bInd] = b2;
    t2.v3[bInd] = b2;
}

#endif /* VoxelShape_h */
