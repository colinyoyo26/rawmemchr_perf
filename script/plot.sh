
for i in `seq 10 1000 100000`
do
    printf "$i "
    ./bench $i 
done >> data.txt

gnuplot script/plotscript.gp && rm *.txt