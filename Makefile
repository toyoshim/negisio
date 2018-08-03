DEVICE  = attiny2313
F_CPU   = 20000000
FUSE_L  = 0xEF
FUSE_H  = 0xDB
FUSE_X  = 0xFF
HIDSPX  = hidspx -d1

CFLAGS  = -Iv-usb/usbdrv -I. -DDEBUG_LEVEL=0
OBJECTS = v-usb/usbdrv/usbdrv.o v-usb/usbdrv/usbdrvasm.o v-usb/usbdrv/oddebug.o main.o

COMPILE = avr-gcc -Wall -Os -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE)

# symbolic targets:
help:
	@echo "This Makefile has no default rule. Use one of the following:"
	@echo "make hex ....... to build main.hex"
	@echo "make program ... to flash fuses and firmware"
	@echo "make fuse ...... to flash the fuses"
	@echo "make flash ..... to flash the firmware (use this on metaboard)"
	@echo "make clean ..... to delete objects and hex file"

hex: main.hex

program: flash fuse

# rule for programming fuse bits:
fuse:
	@[ "$(FUSE_H)" != "" -a "$(FUSE_L)" != "" ] || \
		{ echo "*** Edit Makefile and choose values for FUSE_L and FUSE_H!"; exit 1; }
	$(HIDSPX) -fL$(FUSE_L) -fH$(FUSE_L) -fX$(FUSE_X)

# rule for uploading firmware:
flash: main.hex
	$(HIDSPX) main.hex

# rule for deleting dependent files (those which can be built by Make):
clean:
	rm -f main.hex main.lst main.obj main.cof main.list main.map main.eep.hex main.elf *.o v-usb/usbdrv/*.o main.s v-usb/usbdrv/oddebug.s v-usb/usbdrv/usbdrv.s

# generic rule for compiling C files:
.c.o:
	$(COMPILE) -c $< -o $@

# generic rule for assembling Assembler source files:
.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

# specific file dependencies:
main.o:: usbconfig.h

# file targets:
main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex main.eep.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size main.hex
