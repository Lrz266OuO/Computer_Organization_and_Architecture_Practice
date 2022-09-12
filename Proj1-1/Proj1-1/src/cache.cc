/*
 * @Author: LiRunze lirunze.me@gmail.com
 * @Date: 2022-09-09 05:59:55
 * @LastEditors: LiRunze
 * @LastEditTime: 2022-09-12 00:23:48
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
    TOTAL_MEMORY_TRAFFIC= 0;

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

    // nextLevel           = NULL;

}

void Cache::input() {

    char rw;
    unsigned int address;

    while(scanf("%c %x",&rw,&address) != EOF) {
        transformAddress(address);
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

    printf("  ===== Simulator configuration =====\n");
    printf("  L1_BLOCKSIZE:%22d\n", BLOCKSIZE);
    printf("  L1_SIZE:%27d\n", SIZE);
    printf("  L1_ASSOC:%26d\n", ASSOC);
    printf("  L1_REPLACEMENT_POLICY:%13d\n", REPLACEMENT_POLICY);
    printf("  L1_WRITE_POLICY:%19d\n", WRITE_POLICY);
    printf("  trace_file:%24s\n", TRACE_FILE);
    printf("  ===================================\n\n");

    printf("===== L1 contents =====\n");
    unsigned int i, j;
    for(i=0; i<SET; i++) {
        printf("set%4d:", i);
        for(j=0; j<ASSOC; j++) {
            printf("%8x", TAGS[i+j*SET]);
            printf((WRITE_POLICY==0 && DIRTY[i+j*SET]==1) ? " D" : "  ");
        }
        printf("\n");
    }

    printf("\n");
    printf("  ====== Simulation results (raw) ======\n");
    printf("  a. number of L1 reads:%16d\n", NUM_OF_READ);
    printf("  b. number of L1 read misses:%10d\n", NUM_OF_READ_MISS);
    printf("  c. number of L1 writes:%15d\n", NUM_OF_WRITE);
    printf("  d. number of L1 write misses:%9d\n", NUM_OF_WRITE_MISS);
    printf("  e. L1 miss rate:%22.4f\n", MISS_RATE);
    printf("  f. number of writebacks from L1:%6d\n", NUM_OF_WRITE_BACK);
    printf("  g. total memory traffic:%14d\n", TOTAL_MEMORY_TRAFFIC);
    
    printf("\n");
    printf("  ==== Simulation results (performance) ====\n");
    printf("  1. average access time:%15.4f ns\n", ACCESS_TIME);

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
    TOTAL_MEMORY_TRAFFIC++;
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
            TOTAL_MEMORY_TRAFFIC++;
            NUM_OF_WRITE_BACK++;
            DIRTY[TAG_ADD] = 0;
        }
    }
    
    return;

}

void Cache::writeToAddress() {

    unsigned int i;
    for(i=0; i<ASSOC; i++) {
        int index = INDEX+i*SET;
        if(TAGS[index] == TAG_LOC) {
            if(!WRITE_POLICY) {
                // write policy = 0: WBWA
                DIRTY[index] = 1;
            }
            else {
                // write policy = 1: WTNA
                TOTAL_MEMORY_TRAFFIC++;
            }
            if(REPLACEMENT_POLICY) {
                NUM_OF_TAG[index] = NUM_OF_TAG[index] + 1;
            }
            else {
                hit(index);
            }
            return;
        }
    }

    // write miss
    NUM_OF_WRITE_MISS++;
    TOTAL_MEMORY_TRAFFIC++;
    if(!WRITE_POLICY) {
        if(REPLACEMENT_POLICY) {
            lfu();
            NUM_OF_TAG[TAG_ADD] = NUM_OF_SET[INDEX] + 1;
        }
        else {
            lru();
            NUM_OF_TAG[TAG_ADD] = 0;
        }
        if(DIRTY[TAG_ADD]) {
            NUM_OF_WRITE_BACK++;
            TOTAL_MEMORY_TRAFFIC++;
        }
        DIRTY[TAG_ADD] = 1;
        TAGS[TAG_ADD] = TAG_LOC;
    }

    return;

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
    TAG_LOC     = add>>(block+set);

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

    unsigned int i;
    int max = -1;
    for(i=0; i<ASSOC; i++) {
        if(NUM_OF_TAG[INDEX+i*SET] > max) {
            max = NUM_OF_TAG[INDEX+i*SET];
            TAG_ADD = INDEX + i*SET;
        }
    }
    for(i=0; i<ASSOC; i++) {
        NUM_OF_TAG[INDEX+i*SET] = NUM_OF_TAG[INDEX+i*SET] + 1;
    }
    NUM_OF_TAG[TAG_ADD] = 0;

}

void Cache::lfu() {
    
    unsigned int i;
    int min = 1 << 24;
    for(i=0; i<ASSOC; i++) {
        if(NUM_OF_TAG[INDEX+i*SET] < min) {
            min = NUM_OF_TAG[INDEX+i*SET];
            TAG_ADD = INDEX + i*SET;
        }
    }
    NUM_OF_SET[INDEX] = NUM_OF_TAG[TAG_ADD];

}


