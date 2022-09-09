/*
 * @Author: LiRunze lirunze.me@gmail.com
 * @Date: 2022-09-09 06:00:02
 * @LastEditors: LiRunze
 * @LastEditTime: 2022-09-09 06:56:22
 * @Description:  
 */

#ifndef CACHE_H
#define CACHE_H

class Cache {

    private:
        unsigned int BLOCKSIZE;             // block capacity of cache
        unsigned int SIZE;                  // total capacity of cache
        unsigned int ASSOC;                 // connectivity of cache

        unsigned int REPLACEMENT_POLICY;    // replacement policy 0:LRU, 1:LFU
        unsigned int WRITE_POLICY;          // write policy 0:WBWA, 1:WTNA

        char *TRACE_FILE;                   // filename of address stream

        unsigned int NUM_OF_READ;           // number of read
        unsigned int NUM_OF_READ_MISS;      // number of read_miss
        unsigned int NUM_OF_WRITE;          // number of write
        unsigned int NUM_OF_WRITE_MISS;     // number of write_miss
        unsigned int NUM_OF_WRITE_BACK;     // number of write_back
        // unsigned int TOT_MEM_TRAFFIC;

        // total_access_time = (reads+writes)*hit_time+(read_misses+write_misses)*miss_penalty
        double MISS_RATE;                   // miss rate
        double MISS_PENALTY;                // miss penalty
        double HIT_TIME;                    // hit time
        double ACCESS_TIME;                 // total access time

        CACHE *nextLevel;                   // pointer to the next level of cache

    public:
        unsigned int SET;
        unsigned int TAG;
        unsigned int INDEX;
        unsigned int TAG_LOC;
        unsigned int TAG_ADD;

        unsigned int *TAGS;
        unsigned int *DIRTY;
        unsigned int *VALID;

        int *NUM_OF_SET;
        int *NUM_OF_TAG;

        void init(unsigned int block, unsigned int size, unsigned int assoc, unsigned int replace, unsigned int write, char *tracefile);
        void input();
        void output();
        
        void readFromAddress();
        void writeToAddress();
        void transAddress(unsigned int address);

        void hit(int index);
        void lru();
        void lfu();

};

class CACHE {
    
    private:
        Cache L1, L2;

    public:
        void init();

};

#endif
