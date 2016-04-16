FF = ifort

CPUFLAGS = -qopenmp -xhost
MICFLAGS = -qopenmp -mmic

all: loop_cpu loop_mic instructions

loop_cpu: loop_module.modCPU loop_module.oCPU loop_subroutines.oCPU loop_main.oCPU
	$(FF) $(CPUFLAGS) -o loop_cpu loop_main.o loop_subroutines.o

loop_mic: loop_module.modMIC loop_module.oMIC loop_subroutines.oMIC loop_main.oMIC
	$(FF) $(MICFLAGS) -o loop_mic loop_main.o loop_subroutines.o

%.oCPU %.modCPU: %.F90
	$(FF) $(CPUFLAGS) -c "$<"

%.oMIC %.modMIC: %.F90
	$(FF) $(MICFLAGS) -c "$<"

instructions: 
	$(info )
	$(info TO EXECUTE THE APPLICATION: )
	$(info "make run-cpu" to run the application on the host CPU)
	$(info "make run-mic" to run the application on the coprocessor)
	$(info )

run-cpu:
	./loop_cpu

run-mic:
	micnativeloadex ./loop_mic

clean:
	rm *.o *.mod loop_cpu loop_mic
