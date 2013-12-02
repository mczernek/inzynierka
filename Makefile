CC=g++ -lboost_system -lboost_iostreams -lboost_filesystem
OBJS = main.o i2c8Bit.o mpu6050.o

main: $(OBJS)
	$(CC) $^ -o $@

$(OBJS): %.o: %.cpp
	$(CC) -c $< -o $@
	
clean:
	rm -f *.o *~
	
.PHONY: all clean
