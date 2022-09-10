/*
 * @Author: LiRunze lirunze.me@gmail.com
 * @Date: 2022-09-09 06:00:02
 * @LastEditors: LiRunze
 * @LastEditTime: 2022-09-10 06:21:38
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
        unsigned int TOT_MEM_TRAFFIC;

        double MISS_RATE;                   // miss rate        = (readmisses+writemisses)/(reads+writes)
        double MISS_PENALTY;                // miss penalty     = 20ns+0.5*(blocksize/16Byte_per_ns)
        double HIT_TIME;                    // hit time         = 0.25ns+2.5ns*(size/512KB)+0.025ns*(blocksize/16B)+0.025*assoc
        double ACCESS_TIME;                 // access time      = (reads+writes)*hittime+(readmisses+writemisses)*misspenalty

        CACHE *nextLevel;                   // pointer to the next level of cache

    public:
        unsigned int TAG;                   // TAG = SIZE/BLOCKSIZE;
        unsigned int SET;                   // SET = TAG/ASSOC;
        unsigned int INDEX;
        unsigned int TAG_LOC;
        unsigned int TAG_ADD;

        unsigned int *TAGS;
        unsigned int *DIRTY;
        unsigned int *VALID;
        int *NUM_OF_TAG;
        int *NUM_OF_SET;

        void init(unsigned int block, unsigned int size, unsigned int assoc, unsigned int replace, unsigned int write, char *tracefile);
        void input();                       // input the operation and address
        void output();                      // output the raw data
        
        void readFromAddress();
        void writeToAddress();
        void transAddress(unsigned int add);

        void hit(int index);
        void lru();                         // Least Recently Used:
        void lfu();                         // Least Frequently Used: select the least recently referenced data block to replace

};

class CACHE {
    
    private:
        Cache L1, L2;

    public:
        void init();

};

#endif
