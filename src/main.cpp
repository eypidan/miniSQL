//
// Created by px on 6/14/2019.
//

#include "../include/BufferManager.h"

void CreateStruct_Test();

void GetStruct_Test();

int main() {
//    CreateStruct_Test();
    GetStruct_Test();
}

void CreateStruct_Test() {
    BufferManager test;
    int i;
    BlockNode bn;
    bn.FileName = "book";
    char *testdata = new char[BlOCKSIZE];
    for (i = 0; i < BlOCKSIZE; i++) {
        testdata[i] = 50;
    }
    bn.Data = testdata;

    test.CreateStruct(&bn);
}

void GetStruct_Test() {
    BufferManager test;
    BlockNode *x;
    int count = 0;
    x = test.GetStruct("book");
    for (int i = 0; i < 4096; i++) {
        printf("The information in this struct block is:/n");
        printf("%02x ", x->Data[i] & 0xff);
        count = (count + 1) % 11;
        if (count == 0) printf("\n");
    }
}