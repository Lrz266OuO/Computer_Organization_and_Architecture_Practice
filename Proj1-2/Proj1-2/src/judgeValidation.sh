###
 # @Author: LiRunze lirunze.me@gmail.com
 # @Date: 2022-10-29 23:27:44
 # @LastEditors: LiRunze
 # @LastEditTime: 2022-10-29 23:35:36
 # @Description:  
### 

#!/bin/bash
function run()
{
    echo "Judging test 6."
    ./sim_cache 32 2048 4 0 4096 8 ../traces/gcc_trace.txt > ../judgeResults/ValidationTest6.txt
    diff -iw ../judgeResults/ValidationTest6.txt ../validation/Validation6_PartB.txt

    echo "Judging test 7."
    ./sim_cache 16 1024 8 0 8192 4 ../traces/go_trace.txt > ../judgeResults/ValidationTest7.txt
    diff -iw ../judgeResults/ValidationTest7.txt ../validation/Validation7_PartB.txt

    echo "Judging test 8."
    ./sim_cache 32 1024 8 256 0 0 ../traces/perl_trace.txt > ../judgeResults/ValidationTest8.txt
    diff -iw ../judgeResults/ValidationTest8.txt ../validation/Validation8_PartB.txt

    echo "Judging test 9."
    ./sim_cache 128 1024 2 1024 4096 4 ../traces/gcc_trace.txt > ../judgeResults/ValidationTest9.txt
    diff -iw ../judgeResults/ValidationTest9.txt ../validation/Validation9_PartB.txt

    echo "Judging test 10."
    ./sim_cache 64 8192 2 1024 16384 4 ../traces/perl_trace.txt > ../judgeResults/ValidationTest10.txt
    diff -iw ../judgeResults/ValidationTest10.txt ../validation/Validation10_PartB.txt
}

make clean
make
mkdir -p ../judgeResults
run 
echo "Finish all."
exit 0
