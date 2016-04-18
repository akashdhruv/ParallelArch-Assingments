FF = ifort # Fortran Compiler

CPUFLAGS = -qopenmp -xhost # CPU Flags
MICFLAGS = -qopenmp -mmic  # MIC Flags

#CPUFLAGS = -fopenmp
#MICFLAGS = -fopenmp

# Targets
all: loop_cpu loop_mic instructions

# CPU target
loop_cpu: loop_module.modCPU loop_module.oCPU loop_subroutines.oCPU loop_main.oCPU
	$(FF) $(CPUFLAGS) -o loop_cpu loop_main.o loop_subroutines.o

# MIC target
loop_mic: loop_module.modMIC loop_module.oMIC loop_subroutines.oMIC loop_main.oMIC
	$(FF) $(MICFLAGS) -o loop_mic loop_main.o loop_subroutines.o

# Compilation of CPU Objects
%.oCPU %.modCPU: %.F90
	$(FF) $(CPUFLAGS) -c "$<"

# Compilation of MIC Objects
%.oMIC %.modMIC: %.F90
	$(FF) $(MICFLAGS) -c "$<"

# Execution instructions
instructions: 
	$(info )
	$(info TO EXECUTE THE APPLICATION: )
	$(info "make run-cpu" to run the application on the host CPU)
	$(info "make run-mic" to run the application on the coprocessor)
	$(info )

# run-cpu
run-cpu:
	./loop_cpu

# run-mic
run-mic:
	micnativeloadex ./loop_mic

clean:
	rm *.o *.mod loop_cpu loop_mic
