py: main.cpp bigNumber.cpp
	swig -c++ -python bigNumber.i
	g++ -fPIC -c bigNumber.cpp
	g++ -fPIC -c bigNumber_wrap.cxx -I/usr/include/python2.7
	g++ -shared bigNumber.o bigNumber_wrap.o -o _bigNumber.so
	
yn: main.cpp bigNumber.cpp
	g++ -c -fPIC bigNumber.cpp
	g++ -shared -o libbignumber.so bigNumber.o
	LD_LIBRARY_PATH=./ g++ main.cpp -lbignumber -I. -Wl,-rpath,. -L. -o TCHMK_1