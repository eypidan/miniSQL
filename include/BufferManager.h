//
// Created by px on 6/9/2019.
//
/* Instruction to BuuferManager :
 * 1. FileNode contain BlockNode
 * 2. FileNode and BlockNode are both link structure
 * 3. Just use the public functions of the class !
 * */

#ifndef BUFFERMANAGER_BUFFERMANAGER_H
#define BUFFERMANAGER_BUFFERMANAGER_H

#define BlOCKSIZE 4096         //define BlOCKSIZE 4096 bit
#define TABLESIZE 100          //define the max number of table(<=> file in disk) is 100
#define BLOCKSIZE 200          //define the max number of block is 200

#include <iostream>
#include <string>

enum class BlockType {
    RecordBlock,
    IndexBlock,
    RecordCatalogBlock,
    IndexCatalogBlock
};

class BlockNode {
private:
    char __Data[BlOCKSIZE];
    bool __lock;                  //whether this block is locked
    bool __dirty;                 //When the block.dirty == true, this block need to write back to disk
    BlockNode *NextBlockNode;
    BlockNode *PreBlockNode;
    std::string FileName;         //Which file this block belongs to

public:

};

class FileNode {
public:
    std::string FileName;       // A table maps a File i.e. BookTable -> Book.db
    BlockNode *HeaderBlockNode;
    bool lock;                  //whether this file is locked
    FileNode *NextFile;
    FileNode *PreFile;
};


class BufferManager {
private:
    FileNode *HeaderFile;
public:
    BufferManager();

    ~BufferManager();

    void
};


#endif //BUFFERMANAGER_BUFFERMANAGER_H
