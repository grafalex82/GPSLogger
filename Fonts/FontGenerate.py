import os
import sys
import png

def parse_symbol(pixels, x_offset, sym_width, sym_height):
	symb = []
	for y in range(sym_height):
		line = []
		for x in range(sym_width):
			pixel = pixels[y][(x+x_offset)*4] # Greyscale is not supported yet. Assuming 4 bytes per pixel
			line.append(1 if (pixel == 0) else 0)
		symb.append(line)

	return symb

def strip_symbol(symb):
	#TODO: Perhaps this all could be easier with numpy

	y_padding = 0
	x_padding = 0

	# Strip top line
	while True:
		if 1 in symb[0]:
			break
		else:
			symb = symb[1:]
			y_padding += 1
			

	# Strip bottom line
	while True:
		if 1 in symb[-1]:
			break
		else:
			symb = symb[:-1]

	# Strip left column
	while True:
		if 1 in [symb[y][0] for y in range(len(symb))]:
			break
		else:
			x_padding += 1
			for y in range(len(symb)):
				symb[y] = symb[y][1:]

	# Strip right column
	while True:
		if 1 in [symb[y][-1] for y in range(len(symb))]:
			break
		else:
			for y in range(len(symb)):
				symb[y] = symb[y][:-1]

	for l in symb:
		print l

	return symb, x_padding, y_padding
			

def print_bitmap(symb):
	bitmap = ""
	for line in symb:
		b = 0
		for p in line:
			b = b << 1
			if p:
				b += 1
		bitmap += "0x" + format(b, "02x") + ", "

	print bitmap

def print_glyph(symb, sym_height):
	pixels = symb[0]
	x_padding = symb[1]
	y_padding = symb[2]

	print "?, %d, %d, ?, %d, %d" % (len(pixels[0]), len(pixels), x_padding, y_padding-sym_height) 

def main():
	if len(sys.argv) < 4:
		print "Usage: FontGenerate.py <font.png> <width> <height>"
		exit(1)

	filename = sys.argv[1]
	sym_width = int(sys.argv[2])
	sym_height = int(sys.argv[3])

	print "Reading", filename

	r=png.Reader(filename=filename)
	pic_width, pic_height, pixels_obj, pic_params = r.read()
	pixels = list(pixels_obj)

	symbols = []
	for x_offset in [10*offset for offset in range(pic_width/sym_width)]:
		symb = parse_symbol(pixels, x_offset, sym_width, sym_height)
		symbols.append(strip_symbol(symb))

	for symb in symbols:
		print_bitmap(symb[0])
	for symb in symbols:
		print_glyph(symb, sym_height)

if __name__ == "__main__":
	main()