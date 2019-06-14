//
// Created by eypidan on 6/4/2019.
//
#include "../include/BufferManager.h"

/*
 *  CatalogManager -> .struct file
 *  RecordManager  -> .db file
 */


//================ Struct part funcion===================
//struct Function interface with CatalogManager
bool BufferManager::CreateStruct(BlockNode *Newtable) {
    FILE *fpRead = fopen((Newtable->FileName + ".struct").c_str(), "r");
    if (fpRead == NULL) {
        FILE *fp = fopen((Newtable->FileName + ".struct").c_str(), "w");
        fputs(Newtable->Data, fp);
    } else {
        fclose(fpRead);
        return false; // this file has been existed.
    }
}

BlockNode *BufferManager::GetStruct(string TableName) {
    FILE *fpRead = fopen((TableName + ".struct").c_str(), "r");
    BlockNode *BN = new(BlockNode);
    size_t nread;

    BN->FileName = TableName;
    BN->dirty = false;
    BN->offset = 0;

    if (fpRead) {
        while ((nread = fread(BN->Data, 1, sizeof(BN->Data), fpRead)) > 0)
            if (ferror(fpRead)) { ;/* deal with error */
            }
        fclose(fpRead);
    } else {
        throw logic_error("Can't find this file!");
    }
}

//================ Content part funcion===================
//Content Function interface with RecordManager
FileNode *BufferManager::GetFile(const string TableName) {
    FileNode *FN = new(FileNode);
    FN->FileName = TableName;
    FN->pin = false;
    return FN;
}

//