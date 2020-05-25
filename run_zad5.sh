make zad5
rm wyn_5.txt
for ((j = 100; $j <= 1000; j = $j + 100))
do
    for (( i = 10000 ; $i < 1000000; i = $i + 10000))
    do
        mpirun -np 2 ./zad5 $i $j >> wyn_5.txt
        echo -e "\r$i $j"
    done
done
