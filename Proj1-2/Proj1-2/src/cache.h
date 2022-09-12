/*
 * @Author: LiRunze lirunze.me@gmail.com
 * @Date: 2022-09-12 00:05:34
 * @LastEditors: LiRunze
 * @LastEditTime: 2022-09-12 00:52:08
 * @Description:  
 */

#ifndef CACHE_H
#define CACHE_H

class Cache {

    private:
        unsigned int BLOCKSIZE;             // block capacity of cache
        unsigned int SIZE;                  // total capacity of cache
        unsigned int ASSOC;                 // connectivity of cache

        unsigned int NUM_OF_READ;           // number of read
        unsigned int NUM_OF_READ_MISS;      // number of read_miss
        unsigned int NUM_OF_WRITE;          // number of write
        unsigned int NUM_OF_WRITE_MISS;     // number of write_miss
        unsigned int NUM_OF_WRITE_BACK;     // number of write_back
        unsigned int NUM_OF_SWAP;           // number of swap
      
        unsigned int TOTAL_MEMORY_TRAFFIC;  // total memory traffic between cache and main memory

        double MISS_RATE;                   // miss rate        = (readmisses+writemisses)/(reads+writes)
        double MISS_PENALTY;                // miss penalty     = 20ns+0.5*(blocksize/16Byte_per_ns)
        double HIT_TIME;                    // hit time         = 0.25ns+2.5ns*(size/512KB)+0.025ns*(blocksize/16B)+0.025*assoc
        double ACCESS_TIME;                 // access time      = (reads+writes)*hittime+(readmisses+writemisses)*misspenalty

        
    public:

        Cache *nextLevel;                   // pointer to the next level of cache



};

class CACHE {

    private:
        Cache L1_Cache, L2_Cache;           // L1 cache & L2 cache
        Cache Victim_Cache;                 // Victim cache
        char *TRACE_FILE;                   // filename of address stream


};

#endif
