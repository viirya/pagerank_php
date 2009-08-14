// Evgenii B. Rudnyi, http://MatrixProgramming.com

#ifdef USECLOCK
#include <time.h>
#else
#include <sys/time.h>
#endif
#include <vector>

#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;

class Matrix : public std::vector<double>
{
  size_t m;
  size_t n;

public:
  Matrix() : m(0), n(0) {}
  Matrix(size_t m_, size_t n_) : std::vector<double>(m_*n_), m(m_), n(n_) {}
  Matrix(size_t m_, size_t n_, double val) : std::vector<double>(m_*n_, val), m(m_), n(n_) {}

  void resize(size_t m_, size_t n_)
    {m = m_; n = n_; std::vector<double>::resize(m_*n_);}
  void reserve(size_t m_, size_t n_)
    {std::vector<double>::reserve(m_*n_);}
  void clear()
    {m = n = 0; std::vector<double>::clear();}

  size_t NRows() const {return m;}
  size_t NCols() const {return n;}

  double& operator()(size_t i, size_t j)
  {
    return operator[](i + j*m);
  }
  const double& operator()(size_t i, size_t j) const
  {
    return operator[](i + j*m);
  }

  void print() {

    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        cout << operator[](i + j*m) << " ";
      }
      cout << endl;
    }    

  }

	void swap(Matrix &y)
	{
		std::vector<double>::swap(y);
		std::swap(n, y.n);
		std::swap(m, y.m);
	}
	void clearMemory()
	{
		Matrix empty;
	  swap(empty);
	}
};

/// Simple timing
#ifdef USECLOCK
class Timing
{
	double st;
	double gettime() {return clock();}
public:
	Timing() {st = gettime();}
	void reset() {st = gettime();}
	double time() {return (gettime() - st)/CLK_TCK;}
};
#else
class Timing
{
	timeval tim;
//Sun is different with gettimeofday from others
#ifndef __sun__
	struct timezone zone;
#endif	
	double st;
	double gettime()
	{
#ifdef __sun__
		gettimeofday(&tim, (void*)0);
#else
		gettimeofday(&tim, &zone);
#endif	
		return double(tim.tv_sec) + tim.tv_usec*1e-6;
	}
public:
	Timing() {st = gettime();}
	void reset() {st = gettime();}
	double time() {return (gettime() - st);}
};
#endif


