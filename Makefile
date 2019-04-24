.PHONY: clean build cover

TARGET?=test_main.cc

build:
	clang++ $(TARGET) -o $(TARGET).bin

cover:
	clang++ -fprofile-instr-generate -fcoverage-mapping $(TARGET) -o $(TARGET).bin
	LLVM_PROFILE_FILE="$(TARGET).profraw" ./$(TARGET).bin
	llvm-profdata merge -sparse $(TARGET).profraw -o $(TARGET).profdata
	llvm-cov show -format=html -ignore-filename-regex=acutest.h \
		./$(TARGET).bin -instr-profile=$(TARGET).profdata > $(TARGET).html
	xdg-open $(TARGET).html

clean:
	@rm -vf *.html a.out *.bin *.profdata *.profraw
