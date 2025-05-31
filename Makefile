CXX=g++
SOURCES=sources/main.cpp
TARGET=build/tobas
LUAU_INC_DIR=-Iluau/Common/include -Iluau/Ast/include -Iluau/Compiler/include -Iluau/VM/include -Iluau/CodeGen/include -IRequire/Runtime/include -Iextern -Iextern/isocline/include -ICLI/include
ODE_INC_DIR=-Iode/include
RL_INC_DIR=-Iraylib/src
LJ_INC_DIR=-Iluajit/src
INC_DIRS=$(ODE_INC_DIR) $(RL_INC_DIR) $(LJ_INC_DIR)

LUAU_LIB_DIR=-Lluau/build/release
ODE_LIB_DIR=-Lode/ode/src/.libs
RL_LIB_DIR=-Lraylib/src
LJ_LIB_DIR=-Lluajit/src
LIB_DIRS=$(ODE_LIB_DIR) $(RL_LIB_DIR) $(LJ_LIB_DIR)

LUAU_LIB=-lluauast -lluaucompiler -lluauvm
ODE_LIB=-lode
RL_LIB=-lraylib
LJ_LIB=-lluajit
WIN_LIBS=-lwinmm -lgdi32 -lopengl32
LIBS=$(ODE_LIB) $(RL_LIB) $(LJ_LIB)

linux: $(TARGET)

windows: $(TARGET).exe

$(TARGET): $(SOURCES) 
	$(CXX) $(SOURCES) $(LIBS) $(LIB_DIRS) $(INC_DIRS) -o $(TARGET)
$(TARGET).exe: $(SOURCES)
	$(CXX) $(SOURCES) $(WIN_LIBS) $(LIBS) $(LIB_DIRS) $(INC_DIRS) -o $(TARGET).exe
