g++ -o hello hello.cpp `wx-config --cxxflags --libs`

make hello

g++ -c gl0.cpp `wx-config --cxxflags`
g++ -o gl0 gl0.o `wx-config --libs --gl-libs` -lglut

g++ -o gl0 gl0.cpp `wx-config --cxxflags --libs --gl-libs` -lglut -lGL

make gl0

g++ -c gl1.cpp wx-config --cxxflags
g++ -o gl1 gl1.o wx-config --libs --gl-libs -lglut

make gl1