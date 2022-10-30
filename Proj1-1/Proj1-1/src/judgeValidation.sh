###
 # @Author: LiRunze lirunze.me@gmail.com
 # @Date: 2022-10-29 21:56:09
 # @LastEditors: LiRunze
 # @LastEditTime: 2022-10-29 23:19:58
 # @Description:  
### 

#!/bin/bash
function run()
{
    echo "Judging test 1."
    ./sim_cache 16 16384 1 0 0 ../traces/gcc_trace.txt > ../judgeResults/ValidationTest1.txt
    diff -iw ../judgeResults/ValidationTest1.txt ../validation/ValidationRun1.txt

    echo "Judging test 2."
    ./sim_cache 128 2048 8 0 1 ../traces/go_trace.txt > ../judgeResults/ValidationTest2.txt
    diff -iw ../judgeResults/ValidationTest2.txt ../validation/ValidationRun2.txt

    echo "Judging test 3."
    ./sim_cache 32 4096 4 0 1 ../traces/perl_trace.txt > ../judgeResults/ValidationTest3.txt
    diff -iw ../judgeResults/ValidationTest3.txt ../validation/ValidationRun3.txt

    echo "Judging test 4."
    ./sim_cache 64 8192 2 1 0 ../traces/gcc_trace.txt > ../judgeResults/ValidationTest4.txt
    diff -iw ../judgeResults/ValidationTest4.txt ../validation/ValidationRun4.txt

    echo "Judging test 5."
    ./sim_cache 32 1024 4 1 1 ../traces/go_trace.txt > ../judgeResults/ValidationTest5.txt
    diff -iw ../judgeResults/ValidationTest5.txt ../validation/ValidationRun5.txt
}

make clean
make
mkdir -p ../judgeResults
run 
echo "Finish all."
exit 0
