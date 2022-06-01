all: serial cuda

serial: lab4Serial.c
	icc -O3 lab4Serial.c -o lab4_serial

cuda: lab4Cuda.cu
	nvcc -O3 lab4Cuda.cu -o lab4_cuda

clean:
	rm -rf *.o serial cuda