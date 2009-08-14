
SOURCES = cblas_module.c

ARGS = -c -O3 -DADD_  -fpic -fopenmp -DCOMPILE_DL_FIRST_MODULE=1

LIB = 
LIBDIR =
INCDIR = -I/usr/local/include -I. -I/usr/include/php5 -I/usr/include/php5/Zend -I/usr/include/php5//main -I/usr/include/php5/TSRM

ARGSSO = -shared -rdynamic -fopenmp
LIBSO = -lblas 
LIBDIRSO = -L/usr/local/lib

cblas_module.o: cblas_module.c
	g++ $(SOURCES) $(LIBDIR) $(INCDIR) $(LIB) $(ARGS)
cblas_module.so: cblas_module.o
	g++ -o cblas_module.so cblas_module.o $(ARGSSO) $(LIBSO) $(LIBDIRSO)

