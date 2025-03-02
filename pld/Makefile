all: gal1.jed gal2.jed

# Build the two JED files for the GALs
#
gal1.jed: all_logic.pld
	cpp -DGAL1 all_logic.pld | grep -v '^#' > gal1.pld
	galasm gal1.pld

gal2.jed: all_logic.pld
	cpp -DGAL2 all_logic.pld | grep -v '^#' > gal2.pld
	galasm gal2.pld

# Burn GAL 1
#
gal1burn:
	minipro -p 'ATF22V10C(UES)' -w gal1.jed

# Burn GAL 2
#
gal2burn:
	minipro -p 'ATF22V10C(UES)' -w gal2.jed

# Cleanup
#
# Keep the jed files, lose the rest
keepjed:
	rm -f *.chp *.fus *.pin gal?.pld

# Clean fully
clean:
	rm -f output.vcd a.out gal1.* gal2.*
