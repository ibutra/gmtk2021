FILES = 3d-hammer.h basketball-jersey.h cooking-pot.h gamepad.h gardening-shears.h journey.h paint-brush.h party-popper.h shopping-bag.h sloth.h wave-surfer.h white-book.h character.h 3d-hammer-bw.h basketball-jersey-bw.h cooking-pot-bw.h gamepad-bw.h gardening-shears-bw.h journey-bw.h paint-brush-bw.h party-popper-bw.h shopping-bag-bw.h sloth-bw.h wave-surfer-bw.h white-book-bw.h help.h credits.h Gidole-Regular.h logo.h

all: $(FILES)

%.h: %.png
	xxd -i $< $@

%.h: %.txt
	xxd -i $< | sed 's/}/,0x00}/' > $@

%.h: %.ttf
	xxd -i $< $@


.PHONY: clean
clean:
	rm -fr $(FILES)
