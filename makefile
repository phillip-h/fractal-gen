all:
	g++ *.cpp -std=c++11 -Wall -Wextra -pedantic -O3 -march=native -o fractal-gen 

clean:
	@$(RM) fractal-gen 
