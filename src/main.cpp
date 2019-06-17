//
// Created by px on 6/14/2019.
//

#include "../include/BufferManager.h"

void BufferManager_Test();

void FileNode_Test();
int main() {
    srand((unsigned) time(nullptr));
    BufferManager_Test();
    FileNode_Test();
    printf("\nTest Finish.\n");
    return 0;

}

void BufferManager_Test() {
    BufferManager test;
    int i;
    if (test.JudgeFileExistence("testfile")) {
        printf("this table has been existed.");
        //test.DeleteFile("testfile");
    } else {
        test.CreateFile("testfile");
        FileNode *testfile;
        testfile = test.GetFile("testfile");
    }

}


void FileNode_Test() {
    BufferManager test;
    FileNode *testfile;
    if (test.JudgeFileExistence("testfile")) {
        testfile = test.GetFile("testfile");
    }

    //start to test fileNode

    for (int i = 0; i < 30000; i++) {
        auto BNset = new BlockNode;
        BNset->Data = new char[BLOCKSIZE];
        for (int j = 0; j < BLOCKSIZE; j++)
            BNset->Data[j] = (char) (j % 125);
        testfile->allocNewNode(BNset);
    }

    int rand_num;
    for (int i = 0; i < 1000; i++) {
        rand_num = (int) (30000 * rand()) / (RAND_MAX + 1);
        BlockNode *x;
        x = testfile->getblock(rand_num);
    }

}