make zad6
rm wyn_6.txt
for ((j = 100; $j <= 1000; j = $j + 100))
do
    for (( i = 10000 ; $i <= 1000000; i = $i + 10000))
    do
        mpirun -np 2 ./zad6 $i $j >> wyn_6.txt
        echo -e "\r$i $j"
    done
done
