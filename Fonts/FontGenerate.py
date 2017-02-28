import os
import sys
import png

def parse_symbol(pixels, x_offset, y_offset, sym_width, sym_height):
	print "Parsing symbol at %d:%d" % (x_offset, y_offset)
	symb = []
	for y in range(sym_height):
		line = []
		for x in range(sym_width):
			pixel = pixels[y+y_offset][(x+x_offset)*4] # Greyscale is not supported yet. Assuming 4 bytes per pixel
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

#	for l in symb:
#		print l

	return symb, x_padding, y_padding
			

def print_bitmap(symb, idx):
	bitmap = ""
	b = 0
	bits = 0

	for line in symb:
		for p in line:
			b = b << 1
			if p:
				b += 1
				
			# Store the bit
			bits += 1
			if bits == 8:
				bitmap += "0x" + format(b, "02x") + ", "
				b = 0
				bits = 0
			
	if bits != 0:
		b = b << (8-bits)
		bitmap += "0x" + format(b, "02x") + ','
		
	print "%s    //0x%s '%c'" % (bitmap, format(idx, "02x"), chr(idx))

def print_glyph(symb, sym_width, sym_height, idx, offset):
	pixels = symb[0]
	x_padding = symb[1]
	y_padding = symb[2]

	print "%d, %d, %d, %d, %d, %d,    //0x%s '%c'" % (offset, len(pixels[0]), len(pixels), sym_width, x_padding, y_padding-sym_height, format(idx, "02x"), chr(idx)) 
	
	size = len(pixels) * len(pixels[0])
	if size % 8 != 0:
		size += 8 - size % 8
	
	return size / 8

def main():
	if len(sys.argv) < 4:
		print "Usage: FontGenerate.py <font.png> <width> <height> <base_idx>"
		exit(1)
		
	filename = sys.argv[1]
	sym_width = int(sys.argv[2])
	sym_height = int(sys.argv[3])
	base_idx = int(sys.argv[4])

	print "Reading", filename

	r=png.Reader(filename=filename)
	pic_width, pic_height, pixels_obj, pic_params = r.read()
	pixels = list(pixels_obj)
	
	print pic_params
	print pixels[0]

	symbols = []
	for y_offset in [sym_height*offset for offset in range(pic_height/sym_height)]:
		for x_offset in [sym_width*offset for offset in range(pic_width/sym_width)]:
			symb = parse_symbol(pixels, x_offset, y_offset, sym_width, sym_height)
			symbols.append(strip_symbol(symb))

	idx = base_idx
	for symb in symbols:
		print_bitmap(symb[0], idx)
		idx += 1
		
	idx = base_idx
	offset = 0
	for symb in symbols:
		offset += print_glyph(symb, sym_width, sym_height, idx, offset)
		idx += 1

if __name__ == "__main__":
	main()