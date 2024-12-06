create day:
    mkdir ./{{day}} && touch ./{{day}}/main.cpp && touch ./{{day}}/input.in

build day:
    mkdir -p ./build
    g++-14 --std=c++20 ./{{day}}/main.cpp -o ./build/{{day}}

run day: 
    just build {{day}}
    ./build/{{day}} < ./{{day}}/input.in

clean:
    rm -rf ./build