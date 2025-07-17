compile:
	g++ -std=c++17 main.cpp airplane.cpp animator.cpp circle.cpp -o DNA -lsfml-graphics -lsfml-window -lsfml-system

compile_noNotes:
	g++ -std=c++17 -Wno-psabi main.cpp airplane.cpp animator.cpp circle.cpp -o DNA -lsfml-graphics -lsfml-window -lsfml-system

