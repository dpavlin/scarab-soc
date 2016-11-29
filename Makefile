CPU ?= lm32

minispartan6_base:
	rm -rf build
	./minispartan6_base.py --nocompile-gateware --cpu-type $(CPU)
	cd firmware && make clean all
	./minispartan6_base.py --cpu-type $(CPU)

load:
	./load.py

firmware:
	cd firmware && make clean all

load-firmware:
	litex_term --kernel firmware/firmware.bin COM17

.PHONY: load firmware load-firmware
