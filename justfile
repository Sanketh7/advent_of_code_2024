create day:
    mkdir ./{{day}} && touch ./{{day}}/main.cpp && touch ./{{day}}/input.in

build day:
    mkdir -p ./build
    g++-14 -Wall --std=c++20 ./{{day}}/main.cpp -o ./build/{{day}}

run day: (build day)
    ./build/{{day}} < ./{{day}}/input.in

clean:
    rm -rf ./build