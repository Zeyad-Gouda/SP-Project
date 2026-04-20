# Replace this with the path you get from `brew info sfml`
SFML_PATH = /opt/homebrew/Cellar/sfml/3.0.2

# Replace "src" with the name of the folder where all your cpp code is
cppFileNames := SP_Project.cpp

all: compile

compile:	
	mkdir -p bin
	g++ -std=c++17 $(cppFileNames) -I/opt/homebrew/Cellar/sfml/3.0.2/include -o main -L/opt/homebrew/Cellar/sfml/3.0.2/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network