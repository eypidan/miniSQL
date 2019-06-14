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
#include "MetaData.h"

using namespace std;

struct Block {
    char Data[BlOCKSIZE];
    bool dirty;                 //When the block.dirty == true, this block need to write back to disk
    int offset;
    string FileName;              //Which file this block belongs to    
};
typedef struct Block BlockNode;
struct FileNode {
    string FileName;              // A table maps a File i.e. BookTable -> Book.db
    vector<BlockNode> BN;
    FileNode *NextFile;
    FileNode *PreFile;
};


class BufferManager {
private:
    FileNode *HeaderFile;
public:
    BufferManager();
    ~BufferManager();

    bool CreateStruct(string TableName);//return true => create table sucessfully, return false => table has existed.

    FileNode *GetFile(const string TableName);             //get this TableName's FileNode
    void DeleteFile(const string TableName);               //delte this table

};


#endif //BUFFERMANAGER_BUFFERMANAGER_H
