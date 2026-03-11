source = vm.c
testfile = test.asm
cc = gcc
target = vm

test: $(source) 
	@echo "compiling the test..."
	$(cc) $< -o $(target)
	@echo "executing the test..."
	./$(target) $(testfile)

clean: 
	@echo "deleting executables..."
	rm $(target)

.PHONY: test clean
