RELEASE = 102

PROGRAMC = babaliba testkey testsnd # Update this to match actual .c filenames in src/
PROGRAMASM = # babacol # Update this to match actual .s filenames in src/
GRAPHICS = src/graphics.h src/font.h src/numbers.h src/graphicsmod.h src/map.h src/marges.h src/loading.h src/blocks_main_color.h
HEADERS = src/lib_graphics.h src/lib_screen.h  src/speccypal.h  src/map_special.h src/lib_screen.h src/speccypal.h src/sounds.h src/sound.h src/strings.h
VASM_FLAGS = -Felf
VASM = tools/stcmd vasm
VLINK = tools/stcmd vlink
CC = tools/stcmd m68k-atari-mint-gcc

BDIR = build
ODIR = obj
SDIR = src
RDIR = release

OUT = $(addprefix $(BDIR)/, $(PROGRAMC:=.tos))
OBJASM = $(addprefix $(ODIR)/, $(PROGRAMASM:=.o))
LSTASM = $(addprefix $(ODIR)/, $(PROGRAMASM:=.o.lst))
OUTASM = $(addprefix $(BDIR)/, $(PROGRAMASM:=.tos))

LIBCMINI = /freemint/libcmini/lib
CFLAGS = -std=gnu99 -I m68k-atari-mint/sys-include -I/freemint/libcmini/include -nostdlib  
EXTOBJS = $(LIBCMINI)/crt0.o 
CFLAGSEXT = -s -Ofast # -O3 Additional CFLAGS can be added here
LINKFLAGS = -L$(LIBCMINI) -lcmini -lgcc 

all: $(OUT) $(OUTASM)
	@echo "Construcció completa."

$(BDIR)/%.tos: $(SDIR)/%.c $(GRAPHICS) $(HEADERS)
	@mkdir -p $(BDIR)
	$(CC) $(CFLAGS) $(CFLAGSEXT) $(EXTOBJS) $< -o $@ $(LINKFLAGS)
	@#$(CC) $(CFLAGS) $(CFLAGSEXT) -S $< -o $@.asm $(LINKFLAGS)

$(ODIR)/%.o: $(SDIR)/%.s
	@mkdir -p $(ODIR)
	$(VASM) $< -o $@ $(VASM_FLAGS) -L $@.lst 

$(BDIR)/%.tos: $(ODIR)/%.o	
	@mkdir -p $(BDIR)
	$(VLINK) $< -bataritos -o $@

clean:
	rm -rf $(OUT)
	rm -rf $(OUTASM)
	rm -rf $(OBJASM)
	rm -rf $(LSTASM)
	rm -rf src/graphics.h
	rm -rf src/graphicsmod.h
	rm -rf src/font.h
	rm -rf src/numbers.h
	rm -rf src/map.h

.PHONY : hdd clean all compress release

compress: all 
	upx --best $(OUT)

release: all
	rm -r -f $(RDIR)/*
	rm $(BDIR)/ba*.tos
	tools/stcmd m68k-atari-mint-gcc -std=gnu99 -DCATALAN -I m68k-atari-mint/sys-include -I/freemint/libcmini/include -nostdlib   -s -Ofast  /freemint/libcmini/lib/crt0.o  src/babaliba.c -o build/ba$(RELEASE)ca.tos -L/freemint/libcmini/lib -lcmini -lgcc
	upx --best build/ba$(RELEASE)ca.tos                                                                                             
	mkdir -p $(RDIR)	
	cp build/ba$(RELEASE)ca.tos $(RDIR)/ba$(RELEASE)ca.tos

	tools/stcmd m68k-atari-mint-gcc -std=gnu99 -DSPANISH -I m68k-atari-mint/sys-include -I/freemint/libcmini/include -nostdlib   -s -Ofast  /freemint/libcmini/lib/crt0.o  src/babaliba.c -o build/ba$(RELEASE)es.tos -L/freemint/libcmini/lib -lcmini -lgcc
	upx --best  build/ba$(RELEASE)es.tos 
	mkdir -p $(RDIR)
	cp build/ba$(RELEASE)es.tos $(RDIR)/ba$(RELEASE)es.tos
	
	tools/stcmd m68k-atari-mint-gcc -std=gnu99 -DENGLISH -I m68k-atari-mint/sys-include -I/freemint/libcmini/include -nostdlib   -s -Ofast  /freemint/libcmini/lib/crt0.o  src/babaliba.c -o build/ba$(RELEASE)en.tos -L/freemint/libcmini/lib -lcmini -lgcc
	upx --best  build/ba$(RELEASE)en.tos
	mkdir -p $(RDIR)
	cp build/ba$(RELEASE)en.tos $(RDIR)/ba$(RELEASE)en.tos
	
	cp README.md $(RDIR)/
	pandoc -f markdown -t plain README.md -o $(RDIR)/README.TXT
	convert ./info/anunci/anunci.png -resize 75% -quality 75 ./release/anunci.jpg
	convert ./info/caràtules/floppy.png -quality 60 ./release/floppy.jpg

	dd if=/dev/zero of=$(RDIR)/babaliba.st bs=1024 count=720
	mformat -i $(RDIR)/babaliba.st -f 720 :: 
	mcopy -i $(RDIR)/babaliba.st $(BDIR)/ba$(RELEASE)ca.tos ::
	mcopy -i $(RDIR)/babaliba.st $(BDIR)/ba$(RELEASE)es.tos ::
	mcopy -i $(RDIR)/babaliba.st $(BDIR)/ba$(RELEASE)en.tos ::
	mcopy -i $(RDIR)/babaliba.st $(RDIR)/README.TXT ::	

hdd:
	rm -rf /tmp/babaliba
	mkdir -p /tmp/babaliba/hdd
	ln -s $$(pwd)/build/ /tmp/babaliba/hdd/C
	ln -s $$(pwd)/../../tools/hdd  /tmp/babaliba/hdd/D

src/graphics.h: data/babalibapal.ase tools/buildgraphics_c.py tools/extra_blocks.py 
	@rm -rf src/graphics.h
	@rm -rf ./data/babalibac/*.png
	libresprite  ./data/babalibapal.ase --save-as ./data/babalibac/babalibapal1.png -b
	./tools/buildgraphics_c.py > src/graphics.h

src/graphicsmod.h: data/babalibapal.ase tools/export_extra_blocks.py tools/extra_blocks.py
	@rm -rf src/graphicsmod.h
	./tools/export_extra_blocks.py > ./src/graphicsmod.h

src/font.h: data/font.ase tools/png2stbaba8w.py
	@rm -rf src/font.h
	libresprite -b data/font.ase --sheet data/font.png
	python3 tools/png2stbaba8w.py data/font.png > src/font.h

src/numbers.h: data/numbers.ase tools/png2stbaba8w.py
	@rm -rf src/numbers.hdata/map.bin
	libresprite -b data/numbers.ase --sheet data/numbers.png
	python3 tools/png2stbaba8w.py data/numbers.png > src/numbers.h

src/map.h: data/map.bin	tools/map_export.py
	@rm -rf src/map.h
	python3 tools/map_export.py data/map.bin > src/map.h

src/marges.h: data/marges.ase tools/png2stbaba8w.py
	@rm -rf src/marges.h
	libresprite -b data/marges.ase --sheet data/marges.png
	python3 tools/png2stbaba8w.py --num hex --mode col data/marges.png  > src/marges.h

src/loading.h: data/loading.png tools/png2stbaba.py
	@rm -rf src/loading.h
	python3 tools/png2stbaba.py --dataonly --lang c data/loading.png > src/loading.h

src/blocks_main_color.h: src/graphics.h tools/map_creator.py tools/extra_blocks.py
	@rm -rf src/blocks_main_color.h
	./tools/map_creator.py --exportcolorindexes > src/blocks_main_color.h	