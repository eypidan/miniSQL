
//
// Created by eypidan on 6/4/2019.
//
#include "../include/BufferManager.h"

/*
 *  CatalogManager -> .struct file
 *  RecordManager  -> .db file
 */



BufferManager::~BufferManager() {
    for (auto FN : FileServices) {
        fclose(FN->fp);
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

        //create Record file at the same time
        auto *FN = new FileNode;
        FN->FileName = Newtable->FileName;
        FN->pin = false;
        FILE *Recordfp = fopen((Newtable->FileName + ".db").c_str(), "w");
        FN->fp = Recordfp;
        this->FileServices.emplace_back(FN);
    }
}

bool BufferManager::JudgeStructExistence(string TableName) {
    if (access((TableName + ".struct").c_str(), F_OK) != -1)
        return true; // this file has been existed.
    else
        return false;
}

BlockNode *BufferManager::GetStruct(string TableName) {
    auto iter = this->StructCacheQueue.begin();   //a struct like LRU to read struct in memory
    while (iter != this->StructCacheQueue.end()) {
        auto item = *iter;
        if (TableName == item->FileName)
            return item;
        iter++;
    }

    //can't find this table in memory, then read from disk
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
    auto iter = this->FileServices.begin();   //a struct like LRU to read Record in memory
    while (iter != this->FileServices.end()) {
        auto item = *iter;
        if (TableName == item->FileName)
            return item;
        iter++;
    }

    FILE *fp = fopen((TableName + ".db").c_str(), "r");
    auto *FN = new(FileNode);
    FN->FileName = TableName;
    FN->pin = false;
    FN->fp = fp;
    return FN;
}

void BufferManager::DeleteFile(const string TableName) {
    //need to delete both struct file and db file
    auto iter = this->FileServices.begin();   //a struct like LRU to read Record in memory
    while (iter != this->FileServices.end()) {
        auto item = *iter;
        if (TableName == item->FileName) {
            fclose(item->fp);
            this->FileServices.erase(iter);
            break;
        }
        iter++;
    }

    auto iter2 = this->StructCacheQueue.begin();   //a struct like LRU to read Record in memory
    while (iter2 != this->StructCacheQueue.end()) {
        auto item = *iter;
        if (TableName == item->FileName) {
            this->StructCacheQueue.erase(iter2);
            break;
        }
        iter++;
    }

    if (remove((TableName + ".db").c_str()) != 0) throw logic_error("Delete File error!");
    if (remove((TableName + ".struct").c_str()) != 0) throw logic_error("Delete File error!");
}
//