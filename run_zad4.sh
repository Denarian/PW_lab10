make zad4
rm wyn_4.txt
mpirun -np 2 ./zad4 1 1 >> wyn_4.txt
for (( i = 10000 ; $i < 1000000; i = $i + 10000))
do
    mpirun -np 2 ./zad4 $i 1 >> wyn_4.txt
    echo -e "\r$i"
done
