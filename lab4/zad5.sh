#!/bin/bash

echo "вывод от 1 до 10:"

for i in {1..10}; do
    echo "$i"
done

# задание 2
echo "введи число для просмотра умножения на него:"
read num

echo "таблица умножения для $num:"

for j in {1..10}; do
    result=$(( num * j ))
    echo "$num x $j = $result"
done
