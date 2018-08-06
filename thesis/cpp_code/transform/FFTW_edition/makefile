LIBS = -lfftw3f -larmadillo
CXX = g++ 

all:	detect

detect: FFTW_STFT.cpp STFT_header/STFT.cpp makefile
	${CXX} FFTW_STFT.cpp STFT_header/STFT.cpp ${LIBS} -o detect 
clean:
	rm -f core *.o detect
