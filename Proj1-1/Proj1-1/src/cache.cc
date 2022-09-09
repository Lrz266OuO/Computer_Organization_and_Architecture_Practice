/*
 * @Author: LiRunze lirunze.me@gmail.com
 * @Date: 2022-09-09 05:59:55
 * @LastEditors: LiRunze
 * @LastEditTime: 2022-09-09 07:36:13
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
