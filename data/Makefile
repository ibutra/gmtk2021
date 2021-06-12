FILES = 3d-hammer.h basketball-jersey.h cooking-pot.h gamepad.h gardening-shears.h journey.h paint-brush.h party-popper.h shopping-bag.h sloth.h wave-surfer.h white-book.h character.h

all: $(FILES)

%.h: %.png
	xxd -i $< $@

.PHONY: clean
clean:
	rm -fr $(FILES)
