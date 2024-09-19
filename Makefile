# Nome do compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -Wall -Iinclude -std=c++11

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

# Lista de fontes e arquivos objeto
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Nome do executável
TARGET = sm

# Regra padrão para compilar o programa
all: $(TARGET)

# Regra para compilar o executável a partir dos arquivos objeto
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regra para compilar cada arquivo .cpp em um arquivo .o na pasta obj
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Criar o diretório obj, se ele não existir
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Limpeza dos arquivos objeto
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

.PHONY: all clean
