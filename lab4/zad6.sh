#!/bin/bash

function greet() {
    local name=$1
    echo "ну привет, $name"
}

echo "пример вызова функции:"
greet "Санёк"

echo "теперь назови свое имя:"
read name

greet "$name"
