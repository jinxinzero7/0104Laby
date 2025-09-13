#!/bin/bash

echo "введи число:"
read num

if (( number % 2 )); then
    echo "число $num является четным"
else
    echo "число $num является нечетным"
fi

echo "введи свой возраст:"
read age

if [ $age -ge 18 ]; then
	echo "красава"
else
	echo "уходи быстро фу дизлайк"
fi

