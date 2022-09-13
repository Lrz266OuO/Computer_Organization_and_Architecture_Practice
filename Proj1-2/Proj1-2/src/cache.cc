/*
 * @Author: LiRunze lirunze.me@gmail.com
 * @Date: 2022-09-12 00:05:42
 * @LastEditors: LiRunze
 * @LastEditTime: 2022-09-13 01:36:40
 * @Description:  
 */

#include "cache.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <assert.h>

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

    block       = (int)log2(BLOCKSIZE);
    set         = (int)log2(SET);
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

    char rw;
    unsigned int address;

    while(scanf("%c %x",&rw,&address) != EOF) {
        if(rw=='r' || rw=='R') {
            readFromAddress(L1_Cache, address, Victim_Cache.SIZE);
        }
        else if(rw=='w' || rw=='W') {
            writeToAddress(L1_Cache, address, Victim_Cache.SIZE);
        }
        else {
            printf("Error: operation valid.\n");
            assert(0);
        }
    }

}

void CACHE::output() {

    // TODO: 输出格式
    printf("===== Simulator configuration =====\n");
    printf("BLOCKSIZE:                    %d\n", L1_Cache.BLOCKSIZE);
    printf("L1_SIZE:                      %d\n", L1_Cache.SIZE);
    printf("L1_ASSOC:                     %d\n", L1_Cache.ASSOC);
    printf("Victim_Cache_SIZE:            %d\n", Victim_Cache.SIZE);
    printf("L2_SIZE:                      %d\n", L2_Cache.SIZE);
    printf("L2_ASSOC:                     %d\n", L2_Cache.ASSOC);
    printf("trace_file:                   %s\n", TRACE_FILE);
    printf("===================================\n");

    int i, j, k;

    // output L1 cache
    printf("===== L1 contents =====\n");
    for(i=0; i<(int)L1_Cache.SET; i++) {
        // bubble sort L1 cache
        for(j=0; j<(int)L1_Cache.ASSOC; j++) {
            lru_key = L1_Cache.LRU_C[i+j*L1_Cache.SET];
            tag_key = L1_Cache.TAGS[i+j*L1_Cache.SET];
            dir_key = L1_Cache.DIRTY[i+j*L1_Cache.SET];

            for(k=j-1; k>=0 && lru_key<L1_Cache.LRU_C[i+k*L1_Cache.SET]; k--) {
                L1_Cache.LRU_C[i+(k+1)*L1_Cache.SET] = L1_Cache.LRU_C[i+k*L1_Cache.SET];
                L1_Cache.TAGS[i+(k+1)*L1_Cache.SET] = L1_Cache.TAGS[i+k*L1_Cache.SET];
                L1_Cache.DIRTY[i+(k+1)*L1_Cache.SET] = L1_Cache.DIRTY[i+k*L1_Cache.SET];
            }

            L1_Cache.LRU_C[i+(k+1)*L1_Cache.SET] = lru_key;
            L1_Cache.TAGS[i+(k+1)*L1_Cache.SET] = tag_key;
            L1_Cache.DIRTY[i+(k+1)*L1_Cache.SET] = dir_key;
        }
        // print bubble sorting set
        printf("set %d: ", i);
        for(j=0; j<(int)L1_Cache.ASSOC; j++) {
            printf("%x ", L1_Cache.TAGS[i+j*L1_Cache.SET]);
            printf("%c ", L1_Cache.DIRTY[i+j*L1_Cache.SET] ? 'D ' : '  ');
        }
        printf("\n");
    }

    // output victim cache
    if(Victim_Cache.SIZE) {
        printf("===== Victim Cache contents =====\n");
        printf("set 0: ");
        for(i=1; i<(int)Victim_Cache.ASSOC; i++) {
            lru_key = Victim_Cache.LRU_C[i];
            tag_key = Victim_Cache.TAGS[i];
            dir_key = Victim_Cache.DIRTY[i];
            for( j=i-1; j>=0 && lru_key<Victim_Cache.LRU_C[j]; j--) {
                Victim_Cache.LRU_C[j+1] = Victim_Cache.LRU_C[j];
                Victim_Cache.TAGS[j+1] = Victim_Cache.TAGS[j];
                Victim_Cache.DIRTY[j+1] = Victim_Cache.DIRTY[j];
            }
            Victim_Cache.LRU_C[j+1] = lru_key;
            Victim_Cache.TAGS[j+1] = tag_key;
            Victim_Cache.DIRTY[j+1] = dir_key;
        }
        for(i=0; i<(int)Victim_Cache.ASSOC; i++) {
            printf("%x ", Victim_Cache.TAGS[i]);
            printf("%c ", Victim_Cache.DIRTY[i] ? 'D ' : '  ');
        }
        printf("\n");
    }

    // output L2 cache
    if(L2_Cache.SIZE) {
        printf("===== L2 contents =====\n");
        for(i=0; i<(int)L2_Cache.SET; i++) {
            for(j=1; j<(int)L2_Cache.ASSOC; j++) {
                lru_key = L2_Cache.LRU_C[i+(j*L2_Cache.SET)];
                tag_key = L2_Cache.TAGS[i+(j*L2_Cache.SET)];
                dir_key = L2_Cache.DIRTY[i+(j*L2_Cache.SET)];
                for(k=j-1; k>=0 && lru_key<L2_Cache.LRU_C[i+(k*L2_Cache.SET)]; k--) {
                    L2_Cache.LRU_C[i+((k+1)*L2_Cache.SET)] = L2_Cache.LRU_C[i+(k*L2_Cache.SET)];
                    L2_Cache.TAGS[i+((k+1)*L2_Cache.SET)] = L2_Cache.TAGS[i+(k*L2_Cache.SET)];
                    L2_Cache.DIRTY[i+((k+1)*L2_Cache.SET)] = L2_Cache.DIRTY[i+(k*L2_Cache.SET)];
                }
                L2_Cache.LRU_C[i+((k+1)*L2_Cache.SET)] = lru_key;
                L2_Cache.TAGS[i+((k+1)*L2_Cache.SET)] = tag_key;
                L2_Cache.DIRTY[i+((k+1)*L2_Cache.SET)] = dir_key;

            }
            printf("set %d: ", i);
            for(j=0; j<(int)L2_Cache.ASSOC; j++) {
                printf("%x ", L2_Cache.TAGS[i+(j*L2_Cache.SET)]);
                printf("%c ", L2_Cache.DIRTY[i+(j*L2_Cache.SET)] ? 'D ' : '  ');
            }
            printf("\n");
        }
    }

    // calculate the data
    L1_Cache.MISS_RATE      = ((double)(L1_Cache.NUM_OF_READ_MISS+L1_Cache.NUM_OF_WRITE_MISS) / (double)(L1_Cache.NUM_OF_READ+L1_Cache.NUM_OF_WRITE));
    L1_Cache.MISS_PENALTY   = (20 + 0.5*((double)L1_Cache.BLOCKSIZE/16));
    L1_Cache.HIT_TIME       = (0.25 + (2.5*L1_Cache.SIZE/(512*1024)) + (0.025*L1_Cache.BLOCKSIZE/16) + (0.025*L1_Cache.ASSOC));
    L1_Cache.ACCESS_TIME    = (L1_Cache.HIT_TIME + L1_Cache.MISS_RATE*L1_Cache.MISS_PENALTY);

    L2_Cache.MISS_RATE      = ((double)L2_Cache.NUM_OF_READ_MISS / (double)L2_Cache.NUM_OF_READ);
    L2_Cache.MISS_PENALTY   = (20 + 0.5*((double)L2_Cache.BLOCKSIZE/16));
    L2_Cache.HIT_TIME       = (2.5 + (2.5*L2_Cache.SIZE/(512*1024)) + (0.025*L2_Cache.BLOCKSIZE/16) + (0.025*L2_Cache.ASSOC));
    L2_Cache.ACCESS_TIME    = (L1_Cache.HIT_TIME + L1_Cache.MISS_RATE * (L2_Cache.HIT_TIME + L2_Cache.MISS_RATE * L2_Cache.MISS_PENALTY));

    // output the data
    printf("====== Simulation results (raw) ======\n");
    printf("a. number of L1 reads:                %d\n", L1_Cache.NUM_OF_READ);
    printf("b. number of L1 read misses:          %d\n", L1_Cache.NUM_OF_READ_MISS);
    printf("c. number of L1 writes:\t\t     %d\n",       L1_Cache.NUM_OF_WRITE);
    printf("d. number of L1 write misses:         %d\n", L1_Cache.NUM_OF_WRITE_MISS);
    printf("e. L1 miss rate:                      %.4f\n", L1_Cache.MISS_RATE);
    printf("f. number of swaps:                   %d\n", Victim_Cache.NUM_OF_SWAP);
    printf("g. number of victim cache writeback:  %d\n", Victim_Cache.NUM_OF_WRITE_BACK);
    printf("h. number of L2 reads:                %d\n", L2_Cache.NUM_OF_READ);
    printf("i. number of L2 read misses:          %d\n", L2_Cache.NUM_OF_READ_MISS);
    printf("j. number of L2 writes:               %d\n", L2_Cache.NUM_OF_WRITE);
    printf("k. number of L2 write misses:         %d\n", L2_Cache.NUM_OF_WRITE_MISS);
    printf("l. L2 miss rate:                      %d\n", L2_Cache.SIZE ? L2_Cache.MISS_RATE : 0);
    printf("m. number of L2 writebacks:           %d\n", L2_Cache.NUM_OF_WRITE_BACK);
    printf("n. total memory traffic:              %d\n", L2_Cache.SIZE ? L2_Cache.TOTAL_MEMORY_TRAFFIC : L1_Cache.TOTAL_MEMORY_TRAFFIC);
    printf("==== Simulation results (performance) ====\n");
    printf("1. average access time:         %.4f ns\n", L2_Cache.SIZE ? L2_Cache.ACCESS_TIME : L1_Cache.ACCESS_TIME);

}

void CACHE::readFromAddress(Cache &cache, unsigned int address, unsigned int victim_cache) {

    unsigned int flag = 0;
    cache.NUM_OF_READ++;
    cache.transformAddress(address);

    unsigned int i;
    for(i=0; i<cache.ASSOC; i++) {
        // Hit
        if((cache.TAGS[cache.INDEX+i*cache.SET] == cache.TAG_ADD) && 
           (cache.VALID[cache.INDEX+i*cache.SET])) {
            cache.hit(cache.INDEX+i*cache.SET);
            return;
        }
    }

    // Judge whether invalid read or not.
    for(i=0; i<cache.ASSOC; i++) {
        if(cache.VALID[cache.INDEX+i*cache.SET] == 0) {
            cache.VALID[cache.INDEX+i*cache.SET] = 1;
            cache.TAG_LOC = cache.INDEX + i * cache.SET;
            flag = 1;
            break;
        }
    }
    if(flag) {
        // Read Invalid
        cache.NUM_OF_READ_MISS++;
        if(cache.nextLevel != NULL) {
            // This cache is L1, next is L2
            readFromAddress(L2_Cache, address, 0);
        }
        cache.TOTAL_MEMORY_TRAFFIC++;
        cache.TAGS[cache.TAG_LOC]   = cache.TAG_ADD;
        cache.DIRTY[cache.TAG_LOC]  = 0;
        cache.LRU_C[cache.TAG_LOC]  = 0;
        cache.miss();
        return;
    }

    if(victim_cache) {
        // Read Victim cache
        cache.miss();
        cache.LRU_C[cache.TAG_LOC] = 0;
        readFromVictim(cache, address, 'r');
        return;
    }

    // Read miss
    cache.NUM_OF_READ_MISS++;
    cache.TOTAL_MEMORY_TRAFFIC++;
    cache.miss();
    cache.LRU_C[cache.TAG_LOC] = 0;
    if(cache.DIRTY[cache.TAG_LOC]) {
        cache.TOTAL_MEMORY_TRAFFIC++;
        if(cache.nextLevel != NULL) {
            unsigned int shift = cache.TAGS[cache.TAG_LOC];
            shift = (((shift << cache.set) | (cache.TAG_LOC % cache.SET)) << cache.block);
            writeToAddress(L2_Cache, shift, 0);
        }
        cache.NUM_OF_WRITE_BACK++;
        cache.DIRTY[cache.TAG_LOC] = 0;
    }

    if(cache.nextLevel != NULL) {
        // Read L2 cache
        readFromAddress(L2_Cache, address, 0);
    }

    cache.TAGS[cache.TAG_LOC] = cache.TAG_ADD;

}

void CACHE::writeToAddress(Cache &cache, unsigned int address, unsigned int victim_cache) {

    cache.NUM_OF_WRITE++;
    cache.transformAddress(address);
    unsigned int flag = 0;

    unsigned int i;
    for(i=0; i<cache.ASSOC; i++) {
        if((cache.TAGS[cache.INDEX+i*cache.SET] == cache.TAG_ADD) && 
           (cache.VALID[cache.INDEX+i*cache.SET])) {
            cache.DIRTY[cache.INDEX+i*cache.SET] = 1;
            cache.hit(cache.INDEX+i*cache.SET);
            return;
        }
    }

    for(i=0; i<cache.ASSOC; i++) {
        if(cache.VALID[cache.INDEX+i*cache.SET] == 0) {
            cache.VALID[cache.INDEX+i*cache.SET] = 1;
            cache.TAG_LOC = cache.INDEX + i * cache.SET;
            flag = 1;
            break;
        }
    }
    if(flag) {
        cache.NUM_OF_WRITE_MISS++;
        if(cache.nextLevel != NULL) {
            readFromAddress(L2_Cache, address, 0);
        }
        cache.TOTAL_MEMORY_TRAFFIC++;
        cache.TAGS[cache.TAG_LOC]   = cache.TAG_ADD;
        cache.DIRTY[cache.TAG_LOC]  = 1;
        cache.LRU_C[cache.TAG_LOC]  = 0;
        cache.miss();
        return;
    }

    if(victim_cache) {
        // Write Victim cache
        cache.miss();
        cache.LRU_C[cache.TAG_LOC] = 0;
        readFromVictim(cache, address, 'w');
        return;
    }

    // Write miss
    cache.NUM_OF_WRITE_MISS++;
    cache.TOTAL_MEMORY_TRAFFIC++;
    cache.miss();
    cache.LRU_C[cache.TAG_LOC] = 0;

    if(cache.DIRTY[cache.TAG_LOC]) {
        cache.TOTAL_MEMORY_TRAFFIC++;
        if(cache.nextLevel != NULL) {
            unsigned int shift = cache.TAGS[cache.TAG_LOC];
            shift = (((shift<<cache.set) | (cache.TAG_LOC % cache.SET)) << cache.block);
            writeToAddress(L2_Cache, shift, 0);
        }
        cache.NUM_OF_WRITE_BACK++;
    }

    if(cache.nextLevel != NULL) {
        readFromAddress(L2_Cache, address, 0);
    }

    cache.DIRTY[cache.TAG_LOC] = 1;
    cache.TAGS[cache.TAG_LOC] = cache.TAG_ADD;

}

void CACHE::readFromVictim(Cache &cache, unsigned int address, char rw) {

    cache.transformAddress(address);

    unsigned int temp;

    unsigned int i;
    for(i=0; i<Victim_Cache.ASSOC; i++) {
        if((Victim_Cache.TAGS[i] == address>>cache.block) && (Victim_Cache.VALID[i])) {
            temp                        = cache.TAGS[cache.TAG_LOC];
            cache.TAGS[cache.TAG_LOC]   = cache.TAG_ADD;
            Victim_Cache.TAGS[i]        = ((temp << cache.set) | (cache.INDEX % cache.SET));

            temp                        = cache.DIRTY[cache.TAG_LOC];
            cache.DIRTY[cache.TAG_LOC]  = Victim_Cache.DIRTY[i];
            Victim_Cache.DIRTY[i]       = temp;
            
            Victim_Cache.hit(i);
            Victim_Cache.NUM_OF_SWAP++;

            if(rw=='w' || rw=='W') {
                cache.DIRTY[cache.TAG_LOC] = 1;
            }

            return;
        }
    }

    // cache miss
    if(rw=='r' || rw=='R') {
        cache.NUM_OF_READ_MISS++;
    }
    else {
        cache.NUM_OF_WRITE_MISS;
    }
    cache.TOTAL_MEMORY_TRAFFIC++;
    Victim_Cache.miss();
    Victim_Cache.LRU_C[Victim_Cache.TAG_LOC] = 0;

    if(Victim_Cache.DIRTY[Victim_Cache.TAG_LOC]) {
        if(cache.nextLevel != NULL) {
            temp = Victim_Cache.TAGS[Victim_Cache.TAG_LOC] << cache.block;
            writeToAddress(L2_Cache, temp, 0);
        }
        Victim_Cache.NUM_OF_WRITE_BACK++;
        cache.TOTAL_MEMORY_TRAFFIC++;
    }

    if(cache.nextLevel != NULL) {
        readFromAddress(L2_Cache, address, 0);
    }

    temp                                        = cache.TAGS[cache.TAG_LOC];
    Victim_Cache.TAGS[Victim_Cache.TAG_LOC]     = ((temp << cache.set) | (cache.INDEX % cache.SET));
    temp                                        = cache.DIRTY[cache.TAG_LOC];
    Victim_Cache.DIRTY[Victim_Cache.TAG_LOC]    = temp;
    Victim_Cache.VALID[Victim_Cache.TAG_LOC]    = 1;
    cache.TAGS[cache.TAG_LOC]                   = cache.TAG_ADD;
    // cache.DIRTY[cache.TAG_LOC]                  = rw-'r' ? 1 : 0;
    if(rw=='r' || rw=='R') {
        cache.DIRTY[cache.TAG_LOC] = 0;
    }
    else {
        cache.DIRTY[cache.TAG_LOC] = 1;
    }

}
