PROJECT=bloop-mcc
NUCLEO_VERS=F767ZI
TARGET=NUCLEO_$(NUCLEO_VERS)
TOOLCHAIN=GCC_ARM
PROFILE=debug
OUTDIR=out/$(TARGET)
PROJECT_SOURCE_DIR=/src

MBED_BUILD_FILES= mbed-os CMakeLists.txt .mbed

compile: $(MBED_BUILD_FILES)
	mbed-tools compile -t $(TOOLCHAIN) -m $(TARGET) -b $(PROFILE)
	make copy-out
	echo "Done building ${PROJECT}"

copy-out:
	mkdir -p $(OUTDIR)
	cp cmake_build/$(TARGET)/$(PROFILE)/$(TOOLCHAIN)/src $(OUTDIR)/$(TARGET)_$(PROFILE).bin

mbed-os:
	echo "Installing MBedOS... This may take a while"
	mbed-tools deploy

CMakeLists.txt: .mbed
	mbed-cli toolchain ${TOOLCHAIN}
	mbed-cli target ${TARGET}
	mbed-cli export -i cmake_gcc_arm
	rm GettingStarted.html

.mbed:
	mbed config root ${PROJECT_SOURCE_DIR}

clean:
	rm -rf cmake_build/ out/ BUILD/ __pycache__/

rm-mbed:
	rm -rf $(MBED_BUILD_FILES)

