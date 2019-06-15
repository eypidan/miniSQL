//
// Created by px on 6/14/2019.
//

#include "../include/BufferManager.h"

void CreateStruct_Test();
void GetStruct_Test();

void DeleteFile_Test();
int main() {
//    CreateStruct_Test();
//    GetStruct_Test();
    DeleteFile_Test();
    printf("\nTest Finish.\n");
    return 0;
}

void CreateStruct_Test() {
    BufferManager test;
    int i;
    BlockNode bn;
    bn.FileName = "book";
    auto *testdata = new char[BLOCKSIZE];
    for (i = 0; i < BLOCKSIZE; i++) {
        testdata[i] = 50;
    }
    bn.Data = testdata;
    if (test.JudgeStructExistence("book")) {
        printf("this table has been existed.");
    } else {
        test.CreateStruct(&bn);
    }

}

void GetStruct_Test() {
    BufferManager test;
    BlockNode *x;
    int count = 0;
    x = test.GetStruct("book");
    printf("The information in this struct block is:/n");
    for (int i = 0; i < 4096; i++) {
        printf("%02x ", x->Data[i] & 0xff);
        count = (count + 1) % 11;
        if (count == 0) printf("\n");
    }
}

void DeleteFile_Test() {
    BufferManager test;
    int i;
    BlockNode bn;
    bn.FileName = "deltetable";
    auto *testdata = new char[BLOCKSIZE];
    for (i = 0; i < BLOCKSIZE; i++) {
        testdata[i] = 50;
    }
    bn.Data = testdata;

    if (test.JudgeStructExistence("deltetable")) {
        printf("this table has been existed.");
    } else {
        test.CreateStruct(&bn);
    }
    test.DeleteFile("deltetable");
}