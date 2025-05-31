CXX=g++
SOURCES=sources/main.cpp
OBJECTS=build/main.cpp.o
TARGET=build/tobas

LUAU_INC_DIRS=-Iluau/Compiler/include -Iluau/VM/include
ODE_INC_DIR=-Iode/include
RL_INC_DIR=-Iraylib/src
LJ_INC_DIR=-Iluajit/src
INC_DIRS=$(LUAU_INC_DIRS) $(ODE_INC_DIR) $(RL_INC_DIR)

LUAU_LIBS_DIR=luau/build/release
COMPILER_LIB=$(LUAU_LIBS_DIR)/libluaucompiler.a
AST_LIB=$(LUAU_LIBS_DIR)/libluauast.a
VM_LIB=$(LUAU_LIBS_DIR)/libluauvm.a

ODE_LIB_DIR=-Lode/ode/src/.libs
RL_LIB_DIR=-Lraylib/src
LJ_LIB_DIR=-Lluajit/src
LIB_DIRS=$(ODE_LIB_DIR) $(RL_LIB_DIR)

LUAU_LIBS=$(COMPILER_LIB) $(AST_LIB) $(VM_LIB)
ODE_LIB=-lode
RL_LIB=-lraylib
LJ_LIB=-lluajit
WIN_LIBS=-lwinmm -lgdi32 -lopengl32
LIBS=$(LUAU_LIBS) $(ODE_LIB) $(RL_LIB)

linux: $(TARGET)

windows: $(TARGET).exe

$(TARGET): $(OBJECTS) 
	$(CXX) $< $(LIBS) $(LIB_DIRS) $(INC_DIRS) -o $@
$(TARGET).exe: $(OBJECTS)
	$(CXX) $< $(LIBS) $(WIN_LIBS) $(LIB_DIRS) $(INC_DIRS) -o $@
$(OBJECTS): $(SOURCES)
	$(CXX) $< -c -MMD -MP $(INC_DIRS) -o $@
