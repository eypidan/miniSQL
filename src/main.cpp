//
// Created by px on 6/14/2019.
//

#include "../include/BufferManager.h"

void CreateStruct_Test();
int main() {
    CreateStruct_Test();
}

void CreateStruct_Test() {
    BufferManager test;
    int i;
    BlockNode bn;
    bn.FileName = "book";
    char *testdata = new char[4096];
    for (i = 0; i < 4096; i++) {
        testdata[i] = i;
    }
    bn.Data = testdata;

    test.CreateStruct(bn);
}