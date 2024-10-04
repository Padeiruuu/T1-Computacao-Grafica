
main: main.cpp color.o vec.o ball.h bezier.h
	g++ -o main -lGL -lGLU -lglut -lm main.cpp color.o vec.o ball.h bezier.h

color.o: color.cpp
	g++ -c -lGL -lGLU -lglut color.cpp

vec.o: vec.cpp
	g++ -c -lGL -lGLU -lglut vec.cpp -lm

ball.o: ball.h vec.o bezier.o
	g++ -c -lGL -lGLU -lglut ball.h vec.o bezier.o -lm

bezier.o: bezier.h vec.o
	g++ -c -lGL -lGLU -lglut bezier.h vec.o -lm




arquivos: gera_arquivos.py
	python gera_arquivos.py

