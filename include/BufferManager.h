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

using namespace std;
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
    string FileName;              //Which file this block belongs to
    friend class BufferManager;


};

class FileNode {
private:
    string FileName;              // A table maps a File i.e. BookTable -> Book.db
    BlockNode *HeaderBlockNode;
    bool lock;                    //whether this file is locked
    FileNode *NextFile;
    FileNode *PreFile;

    friend class BufferManager;
};


class BufferManager {
private:
    FileNode *HeaderFile;
public:
    BufferManager();
    ~BufferManager();

    FileNode *GetTable(const string TableName);             //get this TableName's FileNode
    void DeleteTable(const string TableName);               //delte this table
    void LockFileNode(FileNode *File, bool lockstate);       //set File's lock state to lock or unlock
    void LockBlockNode(BlockNode *Block, bool lockstate);    //set Block's lock state to lock or unlock
    void DirtyBlock(BlockNode *Block, bool dirtystate);      //set Block's dirty state to dirty or clean
    const int GetBlockSize();

    BlockNode *Get_x_Block(FileNode *File, int index);       //get index BlockNode of a FileNode
};


#endif //BUFFERMANAGER_BUFFERMANAGER_H
