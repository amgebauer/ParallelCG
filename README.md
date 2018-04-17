# Parallel Conjugate Gradient method

## Profiling results
We used a problem size of N=1000. We profiled on a MacBook Pro (13-inch, 2017, 3.1 GHz Intel Core i5).

| Total seconds | Percentage | Self weight | Symbol Name |
|---------------|------------|-------------|-------------|
| 1.38 s | 100.0% | 0 s	| 	CGMethod (11690) |
| 1.38 s | 100.0% | 0 s	| 	 Main Thread  0x18624a |
| 1.37 s |  99.6% | 0 s	| 	  start |
| 1.37 s |  99.6% | 0 s	| 	   main |
| 1.36 s |  98.6% | 0 s	| 	    SOLVE::CGSolver::solve(LINALG::SymmetricMatrix const&, LINALG::Vector const&, LINALG::Vector&, double) |
| 1.35 s |  98.4% | 680.00 ms |	 	     LINALG::SymmetricMatrix::operator*(LINALG::Vector&) const |
| 435.00 ms |  31.6% | 144.00 ms |	 	      LINALG::SymmetricMatrix::get(unsigned long, unsigned long) const |
| 291.00 ms |  21.1% | 291.00 ms |	 	       LINALG::SymmetricMatrix::getGlobalIndex(unsigned long, unsigned long) const |
| 193.00 ms |  14.0% | 193.00 ms |	 	      LINALG::Vector::operator()(unsigned long) |
| 45.00 ms |   3.2% | 45.00 ms |	 	      LINALG::Vector::getSize() const |
| 1.00 ms |   0.0% | 0 s |	 	      LINALG::Vector::Vector(unsigned long) |
| 1.00 ms |   0.0% | 0 s |	 	     LINALG::Vector::add(double, LINALG::Vector const&, double) |
| 1.00 ms |   0.0% | 0 s |	 	     LINALG::SymmetricMatrix::~SymmetricMatrix() |
| 1.00 ms |   0.0% | 0 s |	 	     LINALG::Vector::operator-(LINALG::Vector const&) const |
| 6.00 ms |   0.4% | 3.00 ms |	 	    LINALG::SymmetricMatrix::operator()(unsigned long, unsigned long) |
| 5.00 ms |   0.3% | 0 s |	 	    LINALG::SymmetricMatrix::SymmetricMatrix(unsigned long) |
| 2.00 ms |   0.1% | 0 s |	 	    LINALG::SymmetricMatrix::~SymmetricMatrix() |
| 5.00 ms |   0.3% | 0 s |	 	  _dyld_start |
