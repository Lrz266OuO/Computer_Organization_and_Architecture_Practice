/*
 * @Author: LiRunze lirunze.me@gmail.com
 * @Date: 2022-09-12 00:05:34
 * @LastEditors: LiRunze
 * @LastEditTime: 2022-09-13 01:40:31
 * @Description:  
 */

#ifndef CACHE_H
#define CACHE_H

class Cache {

    private:
        unsigned int BLOCKSIZE;             // block capacity of cache
        unsigned int SIZE;                  // total capacity of cache
        unsigned int ASSOC;                 // connectivity of cache
        unsigned int SET;                   // SET = SIZE/(BLOCKSIZE*ASSOC)
        unsigned int TAG;                   // TAG = SIZE/BLOCKSIZE

        unsigned int NUM_OF_READ;           // number of read
        unsigned int NUM_OF_READ_MISS;      // number of read_miss
        unsigned int NUM_OF_WRITE;          // number of write
        unsigned int NUM_OF_WRITE_MISS;     // number of write_miss
        unsigned int NUM_OF_WRITE_BACK;     // number of write_back
        unsigned int NUM_OF_SWAP;           // number of swap: L1_cache miss but Victim_cache hit
        unsigned int TOTAL_MEMORY_TRAFFIC;  // total memory traffic between caches and main memory

        /*
        * L1.MISS_RATE      = (L1.readmisses+L1.writemisses) / (L1.reads+L1.writes));
        * L1.MISS_PENALTY   = (20 + 0.5*L1.blocksize/16);
        * L1.HIT_TIME       = (0.25 + 2.5*L1.size/(512*1024) + (0.025*L1.blocksize/16) + (0.025*L1_Cache.ASSOC));
        * L1.ACCESS_TIME    = (L1.HIT_TIME + L1.MISS_RATE*L1.MISS_PENALTY;
        * L2.MISS_RATE      = (L2.readmisses / L2.reads);
        * L2.MISS_PENALTY   = (20 + 0.5*L2.blocksize/16);
        * L2.HIT_TIME       = (2.5 + 2.5*L2.size/(512*1024) + 0.025*L2.blocksize/16 + 0.025*L2.assoc);
        * L2.ACCESS_TIME    = L1.HIT_TIME + L1.MISS_RATE * (L2.HIT_TIME + L2.MISS_RATE * L2.MISS_PENALTY);
        */
        double MISS_RATE;                   // miss rate
        double MISS_PENALTY;                // miss penalty
        double HIT_TIME;                    // hit time
        double ACCESS_TIME;                 // access time

        
    public:

        Cache *nextLevel;                   // pointer to the next level of cache

        unsigned int INDEX;
        unsigned int TAG_LOC;
        unsigned int TAG_ADD;

        unsigned int *TAGS;
        unsigned int *DIRTY;
        unsigned int *VALID;
        unsigned int *LRU_C;

        int block;                          // block = log2(BLOCKSIZE)
        int set;                            // set = log2(SET)

        void init(unsigned int block, unsigned int size, unsigned int assoc, unsigned int set, unsigned int tag);
        
        void transformAddress(unsigned int add);
        
        void hit(int index);
        void miss();

        friend class CACHE;

};

class CACHE {

    private:
        Cache L1_Cache;                     // L1 cache
        Cache L2_Cache;                     // L2 cache
        Cache Victim_Cache;                 // Victim cache

        // These keys are used for bubble sort.
        unsigned int lru_key;
        unsigned int tag_key;
        unsigned int dir_key;
        
        char *TRACE_FILE;                   // filename of address stream

    public:
        void init(unsigned int block, unsigned int size1, unsigned int assoc1, unsigned int size2, unsigned int assoc2, unsigned int size_victim, char *tracefile);
        void input();
        void output();
        
        void readFromAddress(Cache &cache, unsigned int address, unsigned int victim_cache);
        void writeToAddress(Cache &cache, unsigned int address, unsigned int victim_cache);
        void readFromVictim(Cache &cache, unsigned int address, char rw);

};

#endif
