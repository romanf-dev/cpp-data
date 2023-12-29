
all:
	$(PREFIX)g++ -include node.hpp -pedantic -std=c++20 -c -o cppdata.o example_data.cpp
	$(PREFIX)ld --gc-sections -T gnu.ld -o cppdata.elf cppdata.o
	$(PREFIX)objcopy -O binary --only-section .blob cppdata.elf cppdata.bin

clean:
	rm *.o *.elf *.bin
