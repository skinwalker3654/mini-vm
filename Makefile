source = vm.c
testfile = test.asm
calcfile = calc.asm
cc = gcc
target = vm

test: $(source) 
	@echo "compiling the vm..."
	$(cc) $< -o $(target)
	@echo "executing the test..."
	./$(target) $(testfile)

calculator: $(source)
	@echo "compiling the vm..."
	$(cc) $< -o $(target)
	@echo "executing the calculator.."
	./$(target) $(calcfile)

clean: 
	@echo "deleting executables..."
	rm $(target)

.PHONY: test clean
