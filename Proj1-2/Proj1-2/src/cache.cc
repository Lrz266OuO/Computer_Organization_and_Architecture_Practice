/*
 * @Author: LiRunze lirunze.me@gmail.com
 * @Date: 2022-09-12 00:05:42
 * @LastEditors: LiRunze
 * @LastEditTime: 2022-09-12 02:02:58
 * @Description:  
 */

#include "cache.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>

/*
 * class Cache's function
 *
 */

void Cache::init(unsigned int block, unsigned int size, unsigned int assoc, unsigned int set, unsigned int tag) {

    BLOCKSIZE               = block;
    SIZE                    = size;
    ASSOC                   = assoc;
    SET                     = set;
    TAG                     = tag;

    NUM_OF_READ             = 0;
    NUM_OF_WRITE            = 0;
    NUM_OF_READ_MISS        = 0;
    NUM_OF_WRITE_MISS       = 0;
    NUM_OF_WRITE_BACK       = 0;
    TOTAL_MEMORY_TRAFFIC    = 0;

    MISS_RATE               = 0;
    MISS_PENALTY            = 0;
    HIT_TIME                = 0;
    ACCESS_TIME             = 0;

    TAGS                    = (unsigned int*)malloc((TAG*sizeof(unsigned int)));
    DIRTY                   = (unsigned int*)malloc((TAG*sizeof(unsigned int)));
    VALID                   = (unsigned int*)malloc((TAG*sizeof(unsigned int)));
    LRU_C                   = (unsigned int*)malloc((TAG*sizeof(unsigned int)));

    memset(TAGS, 0, sizeof(TAGS[0]*TAG));
    memset(DIRTY, 0, sizeof(DIRTY[0]*TAG));
    memset(VALID, 0, sizeof(VALID[0]*TAG));
    memset(LRU_C, 0, sizeof(LRU_C[0]*TAG));

    nextLevel               = NULL;

}

void Cache::transformAddress(unsigned int add) {

    int block   = (int)log2(BLOCKSIZE);
    int set     = (int)log2(SET);
    int shift   = 0;
    int i;
    for(i=0; i<set; i++) {
        shift = (shift<<1) | 1;
    }
    INDEX       = (add>>block) & shift;
    TAG_ADD     = add >> (block+set);

}

void Cache::hit(int index) {

    unsigned int i;
    for(i=0; i<ASSOC; i++) {
        if(LRU_C[INDEX+i*SET] < LRU_C[index]) {
            LRU_C[INDEX+i*SET] = LRU_C[INDEX+i*SET] + 1;
        }
    }
    LRU_C[index] = 0;

}

void Cache::miss() {

    TAG_LOC = 0;

    unsigned int max = 0;
    unsigned int i;
    for(i=0; i<ASSOC; i++) {
        if(LRU_C[INDEX+i*SET] > max) {
            max = LRU_C[INDEX+i*SET];
            TAG_LOC = INDEX + i*SET;
        }
    }
    for(i=0; i<ASSOC; i++) {
        LRU_C[INDEX+i*SET] = LRU_C[INDEX+i*SET] + 1;
    }

}



/*
 * TODO: class CACHE's function
 *
 */

void CACHE::init(unsigned int block, unsigned int size1, unsigned int assoc1, unsigned int size2, unsigned int assoc2, unsigned int size_victim, char *tracefile) {

    TRACE_FILE = tracefile;
    
    // Init L1, L2 and Victim cache
    L1_Cache.init(block, size1, assoc1, size1/(block*assoc1), size1/block);
    if(size2 > 0) {
        L1_Cache.nextLevel = &L2_Cache;
        L2_Cache.init(block, size2, assoc2, size2/(block*assoc2), size2/block);
    }
    if(size_victim > 0) {
        Victim_Cache.init(block, size_victim, size_victim/block, 1, size_victim/block);
    }

}

void CACHE::input() {

}

void CACHE::output() {

}

void CACHE::readFromAddress(Cache &c, unsigned int add, unsigned int vc) {

}

void CACHE::writeToAddress(Cache &c, unsigned int add, unsigned int vc) {

}

void CACHE::readFromVictim(Cache &c, unsigned int add, char rw) {

}
