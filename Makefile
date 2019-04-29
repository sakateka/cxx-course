.PHONY: clean build cover

TARGET?=test_main.cc

converter: *.cc ui/*.cpp ui/*.cc
	clang++ -g -I/usr/lib/x86_64-linux-gnu/wx/include/gtk2-unicode-3.0 -I/usr/include/wx-3.0 -D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D__WXGTK__ -pthread -L/usr/lib/x86_64-linux-gnu -pthread -lwx_gtk2u_core-3.0 -lwx_baseu-3.0 main.cc ui/converter.cpp -o converter

run: converter
	./converter

test:
	clang++ $(TARGET) -o $(TARGET).bin

cover:
	clang++ -fprofile-instr-generate -fcoverage-mapping $(TARGET) -o $(TARGET).bin
	LLVM_PROFILE_FILE="$(TARGET).profraw" ./$(TARGET).bin
	llvm-profdata merge -sparse $(TARGET).profraw -o $(TARGET).profdata
	llvm-cov show -format=html -ignore-filename-regex=acutest.h \
		./$(TARGET).bin -instr-profile=$(TARGET).profdata > $(TARGET).html
	xdg-open $(TARGET).html

clean:
	@rm -vf *.cc.html a.out *.bin *.profdata *.profraw converter
