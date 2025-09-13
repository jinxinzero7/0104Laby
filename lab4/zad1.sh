#!/bin/bash

# zadanie 1
echo "тя как звать?"
read name
echo "здарова, $name!"

# zadanie 2

echo "введи первое число"
read num1
echo "введи второе число"
read num2

sum=$((num1+num2))
echo "sum of numbers: $sum"
