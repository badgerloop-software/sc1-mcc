PROJECT=mcc
NUCLEO_VERS=L432KC
TARGET=NUCLEO_$(NUCLEO_VERS)
TOOLCHAIN=GCC_ARM
PROFILE=develop
OUTDIR=out/$(TARGET)

compile:
	mbed-tools compile -t $(TOOLCHAIN) -m $(TARGET) -b $(PROFILE)
	make copy-out

copy-out:
	mkdir -p $(OUTDIR)
	cp cmake_build/$(TARGET)/$(PROFILE)/$(TOOLCHAIN)/out.bin $(OUTDIR)/$(TARGET)_$(PROFILE).bin

clean:
	rm -rf cmake_build/ out/

