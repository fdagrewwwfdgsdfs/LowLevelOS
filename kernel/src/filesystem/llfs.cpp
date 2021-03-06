#include "llfs.h"
#include "../memory/efiMemory.h"

LLFSEntry* LLFSOpenFile(LLFSHeader* fs,const char* filename) {
    LLFSEntry* firstEntry = (LLFSEntry*)((uint64_t)fs+sizeof(LLFSHeader));
    for(int i = 0;i<fs->Entries;i++) {
        if(memcmp(firstEntry->Filename,filename,16) == 0) {
            return firstEntry;
        }
        firstEntry = (LLFSEntry*)((uint64_t)firstEntry+sizeof(LLFSEntry)+firstEntry->FileSize);
    }
    return 0;
}

void* LLFSReadFile(LLFSEntry* entry) {
    return (void*)((uint64_t)entry+sizeof(LLFSEntry));
}

uint64_t LLFSGetFileSystemSize(LLFSHeader* fs) {
    LLFSEntry* firstEntry = (LLFSEntry*)((uint64_t)fs+sizeof(LLFSHeader));
    uint64_t fsize = sizeof(LLFSHeader);
    for(int i = 0;i<fs->Entries;i++) {
        fsize += sizeof(LLFSEntry)+firstEntry->FileSize;
        firstEntry = (LLFSEntry*)((uint64_t)firstEntry+sizeof(LLFSEntry)+firstEntry->FileSize);
    }
    return fsize;
}

int LLFSCheck(LLFSHeader* fs) {
    if(fs->Signature[0] != 'L' || fs->Signature[1] != 'L' || fs->Signature[2] != 'F' || fs->Signature[3] != 'S') return 0;
    if(fs->Version != 1) return 0;
    return 1;
}