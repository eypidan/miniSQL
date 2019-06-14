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

#define BlOCKSIZE 4096         //define BlOCKSIZE 4096 bit
#define TABLESIZE 100          //define the max number of table(<=> file in disk) is 100
#define BLOCKSIZE 200          //define the max number of block is 200
#include <iostream>
#include <string>
#include <list>
#include "MetaData.h"

using namespace std;

struct BlockNode {
    char Data[BlOCKSIZE];
    bool dirty;                 //When the block.dirty == true, this block need to write back to disk
    int offset;                 //offset in FileNode
    string FileName;              //Which file this block belongs to    
};

class FileNode {
    string FileName;              // A table maps a File i.e. BookTable -> Book.db
    bool pin;                     // pin a node
    list<BlockNode *> accessQueue;
    list<BlockNode *> cacheQueue;
public:
    BlockNode *operator[](int index); //get index's block
    void synchronize();
};


class BufferManager {
private:
    vector<FileNode> FileService;
public:
    BufferManager();
    ~BufferManager();

    bool CreateStruct(BlockNode *Newtable);             //return true => create table sucessfully, return false => table has existed.
    BlockNode *GetStruct(string TableName);
    FileNode *GetFile(const string TableName);             //get this TableName's FileNode
    void DeleteFile(const string TableName);               //delte this table
};


#endif //BUFFERMANAGER_BUFFERMANAGER_H
