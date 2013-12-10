CC=g++ -std=c++0x -pthread
OBJS = main.o i2c8Bit.o mpu6050.o MeasurmentsHandler.o Measurement.o ConfigResolver.o

main: $(OBJS)
	$(CC) $^ -o $@

$(OBJS): %.o: %.cpp
	$(CC) -c $< -o $@
	
clean:
	rm -f *.o *~
	
.PHONY: all clean
