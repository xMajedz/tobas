# Depends on
* Open Dynamics Engine => 0.16.0 ([odedevs/ode](https://bitbucket.org/odedevs/ode)) 
* Raylib >= 5.0 ([raysan5/raylib](https://github.com/raysan5/raylib))
* LuaJIT >= 2.1 ([LuaJIT/LuaJIT](https://github.com/LuaJIT/LuaJIT))
# Building
clone tobas
```
git clone https://github.com/xMajedz/tobas.git
cd tobas
```
clone ODE
```
git clone https://bitbucket.org/odedevs/ode.git
```
build ODE
```
cd ode
./bootstrap
./configure
make
```
clone Raylib
```
git clone https://github.com/raysan5/raylib.git
```
build Raylib
```
make -C raylib/src
```
clone LuaJIT
```
git clone https://luajit.org/git/luajit.git
```
build LuaJIT
```
make -C luajit/src
```
build tobas
```
cd build
make -C ..
```
run tobas
```
./tobas
```
# Downloads
check the [releases](https://github.com/xMajedz/tobas/releases) page for updates 
