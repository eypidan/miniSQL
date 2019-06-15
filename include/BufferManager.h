//
// Created by px on 6/9/2019.
//
/* Instruction to BuuferManager :
 * 1. FileNode contain BlockNode
 * 2. FileNode and BlockNode are both link structure
 * 3. Just use the public functions of the class !
 * 4. Dirty page: the content in pool which is different from its former
 *    data in disk (and not write back to dist yet)
 *    see https://www.cnblogs.com/Arlen/articles/1768557.html for detail
 * 5. Normally, CatalogManager And RecordManager Need to get FileNode first, and
 *    use this FileNode to get BlockNode
 * 6. !======= File <=> Table ======== !
 * 7. While the size of datas is not big enough, a FileNode just have a BlockNode
 * */

#ifndef BUFFERMANAGER_BUFFERMANAGER_H
#define BUFFERMANAGER_BUFFERMANAGER_H

#define BLOCKSIZE 4096         //define BLOCKSIZE 4096 bit
#define CACHESIZE 20000          //the MAX number of block in cache
#include <iostream>
#include <string>
#include <list>
#include <unistd.h>
#include "MetaData.h"

using namespace std;

struct BlockNode {
    char *Data;
    bool dirty;                 //When the block.dirty == true, this block need to write back to disk
    int offset;                 //offset in FileNode
    string FileName;              //Which file this block belongs to    
};

class FileNode {
    string FileName;              // A table maps a File i.e. BookTable -> Book.db
    bool pin;                     // pin a node
    list<BlockNode *> accessQueue;
    list<BlockNode *> cacheQueue;  // store recently used block
    FILE *fp;
    friend class BufferManager;
public:
    BlockNode *operator[](int index); //get index's block
    int *
    allocNewNode(BlockNode *NewBlock); //add a new block to a fileNode ,return the offset of this block in this fileNode
    void synchronize();

};

//BufferManager contains operation about `Memory` and `Disk`
class BufferManager {
private:
    vector<FileNode *> FileServices;
    list<BlockNode *> StructCacheQueue;  // store recently used struct
public:
    BufferManager() = default;
    ~BufferManager();

    bool JudgeStructExistence(string TableName);    //return true => file exist
    bool CreateStruct(BlockNode *Newtable);             //return true => create table sucessfully, return false => table has existed.
    BlockNode *GetStruct(string TableName);


    FileNode *GetFile(const string TableName);          //get this TableName's FileNode
    void DeleteFile(const string TableName);            //delte this table
};


#endif //BUFFERMANAGER_BUFFERMANAGER_H
