.PHONY: clean build cover

TARGET?=test_main.cc

build:
	clang++ $(TARGET) -o $(TARGET).bin

cover:
	clang++ -fprofile-instr-generate -fcoverage-mapping $(TARGET) -o $(TARGET).bin
	./$(TARGET).bin
	llvm-profdata merge -sparse default.profraw -o default.profdata
	llvm-cov show -format=html -ignore-filename-regex=acutest.h ./$(TARGET).bin -instr-profile=default.profdata > cover.html
	xdg-open cover.html

clean:
	@rm -vf cover.html a.out *.bin default.profdata default.profraw
