CC=c++ -std=c++11

ifeq ($(shell uname), Darwin)
	include Makefile.macos
else
	include Makefile.pi
endif

clean:
	@rm *.o toy

