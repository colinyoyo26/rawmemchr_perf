FILE=data.txt



for i in `seq 10 1000 100000`
do
    printf "$i "
    ./bench $i 
done >> $FILE


while IFS=" " read -r d1 d2 d3 d4 d5
do
    num=`expr $num + 1`
    slowCPE=`expr $slowCPE + $d4`
    fastCPE=`expr $fastCPE + $d5`
done < $FILE

slowCPE=`expr $slowCPE / $num`
fastCPE=`expr $fastCPE / $num`

gnuplot -e "fast = 'rawmemchr (CPE = $fastCPE)'; \
            slow = 'orig (CPE = $slowCPE)'"      \
            ./script/plotscript.gp && rm $FILE