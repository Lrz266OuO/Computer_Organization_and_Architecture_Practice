/*
 * @Author: LiRunze lirunze.me@gmail.com
 * @Date: 2022-09-12 00:05:49
 * @LastEditors: LiRunze
 * @LastEditTime: 2022-09-12 02:25:11
 * @Description:  
 */

#include "cache.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <assert.h>

CACHE c;

int main(int argc, char *argv[]) {

    if(argc < 7) {
        // invalid command
		printf("Error: sim_cache <BLOCKSIZE> <L1_SIZE> <L1_ASSOC> <VICTIM_CACHE_SIZE> <L2_SIZE> <L2_ASSOC> <trace_file>\n");
		return 0;
    }

    if(!freopen(argv[7], "r", stdin)) return 0;
    unsigned int blocksize      = (unsigned)atoi(argv[1]);
    unsigned int size1          = (unsigned)atoi(argv[2]);
    unsigned int assoc1         = (unsigned)atoi(argv[3]);
    unsigned int size_victim    = (unsigned)atoi(argv[4]);
    unsigned int size2          = (unsigned)atoi(argv[5]);
    unsigned int assoc2         = (unsigned)atoi(argv[6]);
    char *tracefile             = argv[7];

    int length = (int)strlen(tracefile);
    int i;
    for(i=length; i>=0; i--) {
        if(tracefile[i] == '/') break;
    }
    tracefile = tracefile + i + 1;

    c.init(blocksize, size1, assoc1, size_victim, size2, assoc2, tracefile);
    c.input();
    c.output();

    return 0;

}
