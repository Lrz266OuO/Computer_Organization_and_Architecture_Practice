/*
 * @Author: LiRunze lirunze.me@gmail.com
 * @Date: 2022-09-09 05:59:55
 * @LastEditors: LiRunze
 * @LastEditTime: 2022-09-10 03:09:51
 * @Description:  
 */

#include "cache.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>

void Cache::init(unsigned int block, unsigned int size, unsigned int assoc, unsigned int replace, unsigned int write, char *tracefile) {
    
    BLOCKSIZE           = block;
    SIZE                = size;
    ASSOC               = assoc;
    REPLACEMENT_POLICY  = replace;
    WRITE_POLICY        = write;
    TRACE_FILE          = tracefile;

    NUM_OF_READ         = 0;
    NUM_OF_READ_MISS    = 0;
    NUM_OF_WRITE        = 0;
    NUM_OF_WRITE_MISS   = 0;
    NUM_OF_WRITE_BACK   = 0;
    TOT_MEM_TRAFFIC     = 0;

    TAG                 = SIZE/BLOCKSIZE;
    SET                 = TAG/ASSOC;
    TAGS                = (unsigned int*)malloc(TAG*sizeof(unsigned int));
    DIRTY               = (unsigned int*)malloc(TAG*sizeof(unsigned int));
    VALID               = (unsigned int*)malloc(TAG*sizeof(unsigned int));
    NUM_OF_TAG          = (int*)malloc(TAG*sizeof(int));
    NUM_OF_SET          = (int*)malloc(SET*sizeof(int));

    memset(TAGS, 0, sizeof(TAGS[0])*TAG);
    memset(DIRTY, 0, sizeof(DIRTY[0])*TAG);
    memset(VALID, 0, sizeof(VALID[0])*TAG);
    memset(NUM_OF_TAG, 0, sizeof(NUM_OF_TAG[0])*TAG);
    memset(NUM_OF_SET, 0, sizeof(NUM_OF_SET[0])*SET);

    nextLevel           = NULL;

}

void Cache::input() {

    char rw;
    unsigned int address;

    while(scanf("%c %x",&rw,&address) != EOF) {
        transAddress(address);
        if(rw=='r' || rw=='R') {
            NUM_OF_READ++;
            readFromAddress();
        }
        else if(rw=='w' || rw=='W') {
            NUM_OF_WRITE++;
            writeToAddress();
        }
    }

    MISS_RATE       = (double(NUM_OF_READ_MISS+NUM_OF_WRITE_MISS)/(NUM_OF_READ+NUM_OF_WRITE));
    MISS_PENALTY    = (20+0.5*((double)BLOCKSIZE/16));
    HIT_TIME        = (0.25+2.5*((double)SIZE/(512*1024))+0.025*((double)BLOCKSIZE/16)+0.025*ASSOC);
    ACCESS_TIME     = (HIT_TIME+(MISS_RATE*MISS_PENALTY)); 

}

void Cache::output() {

}

void Cache::readFromAddress() {
    
    TAG_ADD = 0;
    unsigned int i;
    for(i=0; i<ASSOC; i++) {
        int index = INDEX+i*SET;
        if(TAGS[index] == TAG_LOC) {
            if(REPLACEMENT_POLICY) {
                // replacement policy = 1: LFU
                NUM_OF_TAG[index] = NUM_OF_TAG[index] + 1;
            }
            else {
                // replacement policy = 0: LRU
                hit(index);
            }
            return;
        }
    }

    // read miss
    NUM_OF_READ_MISS++;
    TOT_MEM_TRAFFIC++;
    if(REPLACEMENT_POLICY) {
        lfu();
        NUM_OF_TAG[TAG_ADD] = NUM_OF_SET[INDEX] + 1;
    }
    else {
        lru();
    }
    TAGS[TAG_ADD] = TAG_LOC;
    if(!WRITE_POLICY) {
        // WBWA
        if(DIRTY[TAG_ADD] == 1) {
            TOT_MEM_TRAFFIC++;
            NUM_OF_WRITE_BACK++;
            DIRTY[TAG_ADD] = 0;
        }
    }
    
    return;

}

void Cache::writeToAddress() {

}

void Cache::transAddress(unsigned int add) {

}

void Cache::hit(int index) {

    unsigned int i;
    for(i=0; i<ASSOC; i++) {
        if(NUM_OF_TAG[INDEX+i*SET] < NUM_OF_TAG[index]) {
            NUM_OF_TAG[INDEX+i*SET] = NUM_OF_TAG[INDEX+(i*SET)]+1;
        }
    }
    NUM_OF_TAG[index] = 0;
    
}

void Cache::lru() {

}

void Cache::lfu() {
    
}


