# Nome do compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -Wall -g

# Nome do executável
TARGET = main

# Lista de arquivos fonte
SOURCES = main.cpp color.cpp vec.cpp

# Arquivos de cabeçalho
HEADERS = ball.h bezier.h color.h vec.h

# Compila o executável
all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(SOURCES) -o $(TARGET) $(CXXFLAGS) -lGL -lGLU -lglut

# Limpa arquivos gerados
clean:
	rm -f $(TARGET)
