
//
// Created by eypidan on 6/4/2019.
//
#include "../include/BufferManager.h"

/*
 *  CatalogManager -> .struct file
 *  RecordManager  -> .db file
 */

int blockNum = 0;

BufferManager::~BufferManager() {
//    for (auto FN : FileServices) {
//        fclose(FN->fp);
//    }
};
//================ Struct part funcion===================
//struct Function interface with CatalogManager
bool BufferManager::CreateFile(string FileName) {
    FILE *fp = fopen(FileName.c_str(), "w");
    if (fp == nullptr)
        throw logic_error("Create file error!");

    //create Record file at the same time
    auto *FN = new FileNode;
    FN->FileName = FileName;
    FN->pin = false;
    FN->fp = fp;
    this->FileServices.emplace_back(FN);
}

bool BufferManager::JudgeFileExistence(string FileName) {
    auto iter = this->FileServices.begin();
    while (iter != this->FileServices.end()) {
        auto item = *iter;
        if (FileName == item->FileName)
            return true;
        iter++;
    }

    if (access(FileName.c_str(), F_OK) != -1)
        return true; // this file has been existed.
    else
        return false;
}


//================ Content part funcion===================
//Content Function interface with RecordManager
FileNode *BufferManager::GetFile(const string FileName) {
    auto iter = this->FileServices.begin();   //a struct like LRU to read Record in memory
    while (iter != this->FileServices.end()) {
        auto item = *iter;
        if (FileName == item->FileName)
            return item;
        iter++;
    }

    FILE *fp = fopen(FileName.c_str(), "r+");
    if (fp == nullptr) throw logic_error("Didn't create this file yet.");
    auto *FN = new(FileNode);
    FN->FileName = FileName;
    FN->pin = false;
    FN->fp = fp;
    this->FileServices.emplace_back(FN);
    return FN;
}

void BufferManager::DeleteFile(const string FileName) {
    //need to delete both struct file and db file
    auto iter = this->FileServices.begin();   //a struct like LRU to read Record in memory
    while (iter != this->FileServices.end()) {
        auto item = *iter;
        if (FileName == item->FileName) {
            fclose(item->fp);
            this->FileServices.erase(iter);
            break;
        }
        iter++;
    }

    if (remove(FileName.c_str()) != 0) throw logic_error("Delete File error!");
}
//
FileNode::~FileNode() {
    auto iter = accessQueue.begin();
    while (iter != accessQueue.end()) {
        auto item = *iter;
        if (item->dirty) {
            this->writeBack(item->offset, item->Data);
        }

        delete item;
        iter = accessQueue.erase(iter);
        blockNum--;
    }

    iter = cacheQueue.begin();
    while (iter != cacheQueue.end()) {
        auto item = *iter;
        if (item->dirty) {
            this->writeBack(item->offset, item->Data);
        }
        delete item;
        iter = cacheQueue.erase(iter);
        blockNum--;
    }
}


char *FileNode::readBlock(int offset) {
    fseek(this->fp, offset * BLOCKSIZE, SEEK_SET);
    auto *Data = new char[BLOCKSIZE];
    fread(Data, BLOCKSIZE, 1, fp);
    return Data;
}


int FileNode::allocNewNode(BlockNode *NewBlock) {
    fseek(this->fp, 0, SEEK_END);
    auto offset_char = (int) ftell(this->fp);
    fwrite(NewBlock->Data, BLOCKSIZE, 1, this->fp);
////    fflush(this->fp);
    NewBlock->FileName = this->FileName;
    NewBlock->offset = offset_char / BLOCKSIZE;

    accessQueue.push_front(NewBlock);
    blockNum++;
    return offset_char / BLOCKSIZE;
}

BlockNode *FileNode::getblock(int index) { //Use LRU to store recently used block
    auto iter = cacheQueue.begin();
    while (iter != cacheQueue.end()) {
        auto item = *iter;
        if (item->offset == index) {
            item->flag = (char) time(nullptr);  //??
            cacheQueue.push_front(item);
            cacheQueue.erase(iter);
            return item;
        }
        iter++;
    }

    iter = accessQueue.begin();
    while (iter != accessQueue.end()) {
        auto item = *iter;
        if (item->offset == index) {
            item->flag++;
            if (item->flag == LRU_TIME_VALUE) {
                cacheQueue.push_front(item);
                accessQueue.erase(iter);
            } else {
                accessQueue.push_front(item);
                accessQueue.erase(iter);
            }
            return item;
        }
        iter++;
    }
    if (blockNum >= CACHESIZE) {
        cleanup();
    }
    blockNum++;
    auto *item = new BlockNode;
    item->offset = index;
    item->dirty = false;
    item->flag = 0;
    item->Data = this->readBlock(index);
    accessQueue.push_front(item);
    return item;
}

void FileNode::writeBack(int offset, char *Data) {
    fseek(this->fp, offset * BLOCKSIZE, SEEK_SET);
    fwrite(Data, BLOCKSIZE, 1, fp);
    fflush(fp);
}

void FileNode::synchronize() {
    auto iter = accessQueue.begin();
    while (iter != accessQueue.end()) {
        auto item = *iter;
        if (item->dirty) {
            this->writeBack(item->offset, item->Data);
        }
        iter++;
    }
    iter = cacheQueue.begin();
    while (iter != cacheQueue.end()) {
        auto item = *iter;
        if (item->dirty) {
            this->writeBack(item->offset, item->Data);
        }
        iter++;
    }
}

void FileNode::cleanup() {
    auto iter = accessQueue.begin();
    while (iter != accessQueue.end()) {
        auto item = *iter;
        if (item->dirty) {
            this->writeBack(item->offset, item->Data);
        }
        delete item->Data;
        item->Data=nullptr;
        delete item;
        iter = accessQueue.erase(iter);
        blockNum--;
    }
    if (accessQueue.size() <= CACHESIZE / 3) {
        iter = cacheQueue.begin();
        while (iter != cacheQueue.end()) {
            auto item = *iter;
            if (item->dirty) {
                this->writeBack(item->offset, item->Data);
            }
            delete item->Data;
            delete item;
            iter = cacheQueue.erase(iter);
            blockNum--;
        }
    }
}