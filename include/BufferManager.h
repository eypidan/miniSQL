//
// Created by PangSiyuan on 6/9/2019.
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

#define BLOCKSIZE 4096           //define BLOCKSIZE 4096 bit
#define CACHESIZE 20000          //the MAX number of block in cache
#define LRU_TIME_VALUE 2         // LRU Time
#include <iostream>
#include <string>
#include <list>
//#include <unistd.h>
#include <io.h>
#include <process.h>
#ifndef F_OK
#define F_OK 0
#endif // F_OK
#include <time.h>
#include "MetaData.h"
#include "Exception.h"
extern int blockNum;               //use a global varible to record the block number
using namespace std;

struct BlockNode {
    char *Data = NULL;
    bool dirty = false;                 //When the block.dirty == true, this block need to write back to disk
    int offset;                 //offset in FileNode
    bool pin = false;                     // pin a Blocknode
    char flag = 0;                  //the priority of this BlockNode
    string FileName;              //Which file this block belongs to    
};

class FileNode {
    string FileName;              // A table maps a File i.e. BookTable -> Book.db
    list<BlockNode *> accessQueue;
    list<BlockNode *> cacheQueue;  // store recently used block
    FILE *fp = NULL;

	char *readBlock(int offset);
	void cleanup();
	void writeBack(int offset, char *Data);
	friend class BufferManager;
public:

    FileNode() = default;

    ~FileNode();  //write dirty block to disk
    int getBlockNum();   //get this FileNode's block number
    BlockNode *getblock(int index); //get index's block
    int
    allocNewNode(BlockNode *NewBlock); //add a new block to a fileNode ,return the offset of this block in this fileNode
    void synchronize();                //deal with dirty block
};

//BufferManager contains operation about `Memory` and `Disk`
class BufferManager {
private:
	vector<FileNode *> FileServices;
public:
	BufferManager() = default;
	~BufferManager();
	void globalSynchronize();  // //synchronize all the files in BufferManager's FileServices
	bool JudgeFileExistence(string FileName);    //return true => file exist
	bool CreateFile(string FileName);        //return true => create table sucessfully.
	FileNode *GetFile(const string FileName);    //get this FileName's FileNode
	void DeleteFile(const string FileName);      //delte this file
};

static BufferManager bufferManager;

#endif //BUFFERMANAGER_BUFFERMANAGER_H
