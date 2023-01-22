PROJECT=bloop-mcc
NUCLEO_VERS=L432KC
TARGET=NUCLEO_$(NUCLEO_VERS)
TOOLCHAIN=GCC_ARM
PROFILE=debug
OUTDIR=out/$(TARGET)

MBED_BUILD_FILES= mbed-os CMakeLists.txt

compile: $(MBED_BUILD_FILES)
	mbed-tools compile -t $(TOOLCHAIN) -m $(TARGET) -b $(PROFILE)
	make copy-out

copy-out:
	mkdir -p $(OUTDIR)
	cp cmake_build/$(TARGET)/$(PROFILE)/$(TOOLCHAIN)/$(PROJECT) $(OUTDIR)/$(TARGET)_$(PROFILE).bin

mbed-os:
	echo "Installing MBedOS... This may take a while"
	mbed-tools deploy

CMakeLists.txt:
	mbed-cli toolchain ${TOOLCHAIN}
	mbed-cli target ${TARGET}
	mbed-cli export -i cmake_gcc_arm
	rm GettingStarted.html

clean:
	rm -rf cmake_build/ out/ BUILD/ __pycache__/

rm-mbed:
	rm -rf $(MBED_BUILD_FILES)

