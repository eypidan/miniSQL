
\//
// Created by eypidan on 6/4/2019.
//
#include "../include/BufferManager.h"

/*
 *  CatalogManager -> .struct file
 *  RecordManager  -> .db file
 */



BufferManager::~BufferManager() {
    for (auto &fp : files) {
        fclose(fp);
    }
};
//================ Struct part funcion===================
//struct Function interface with CatalogManager
bool BufferManager::CreateStruct(BlockNode *Newtable) {

    if (access((Newtable->FileName + ".struct").c_str(), F_OK) != -1) {
        return false; // this file has been existed.
    } else {
        FILE *fp = fopen((Newtable->FileName + ".struct").c_str(), "w");
        for (int i = 0; i < 4096; i++)
            fputc(Newtable->Data[i], fp);
        this->StructCacheQueue.emplace_back(Newtable);
        fclose(fp);

        FILE *Recordfp = fopen((Newtable->FileName + ".db").c_str(), "w");
        this->files.emplace_back(Recordfp);
    }
}

bool BufferManager::JudgeStructExistence(string TableName) {

    if (access((TableName + ".struct").c_str(), F_OK) != -1) {
        return true; // this file has been existed.
    } else {
        return false;
    }
}

BlockNode *BufferManager::GetStruct(string TableName) {
    auto iter = this->StructCacheQueue.begin();   //a struct like LRU to read struct in memory
    while (iter != this->StructCacheQueue.end()) {
        auto item = *iter;
        if (TableName == item->FileName)
            return item;
        iter++;
    }

    //can't find this table in memory, than read from disk
    FILE *fpRead = fopen((TableName + ".struct").c_str(), "r");
    auto *BN = new(BlockNode);


    BN->FileName = TableName;
    BN->dirty = false;
    BN->offset = 0;
    BN->Data = new char[BLOCKSIZE];
    if (fpRead) {
        while ((fread(BN->Data, 1, BLOCKSIZE, fpRead)) > 0)
            if (ferror(fpRead)) { ;/* deal with error */
            }
        fclose(fpRead);
        this->StructCacheQueue.emplace_back(BN);
        return BN;
    } else {
        throw logic_error("Can't find this file!");
    }
}

//================ Content part funcion===================
//Content Function interface with RecordManager
FileNode *BufferManager::GetFile(const string TableName) {
    auto *FN = new(FileNode);
    FN->FileName = TableName;
    FN->pin = false;
    return FN;
}

//