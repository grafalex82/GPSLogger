import os
import sys
import re

def main():
	if len(sys.argv) < 2:
		print("Usage: dump_firmware.py <disassembly_file>")
		exit(1)

	symb_table_start = re.compile("SYMBOL TABLE:")
	symb_table_end = re.compile("^[\\s]*$")
	line_re = re.compile("([0-9a-fA-F]+)\\s+[^\\s]+\\s+[^\\s]*\\s*([^\\s]+)\\s+([0-9a-fA-F]+)\\s+(.*)")
	
	
	dump = open(sys.argv[1], "r")
	skip = True
	items = []
	for line in dump:
		if skip and symb_table_start.match(line):
			skip = False
		elif not skip and symb_table_end.match(line):
			skip = True
		elif not skip:
			m = line_re.match(line)

			item = (int(m.group(1), 16), m.group(1), m.group(2), int(m.group(3), 16), m.group(4))
			items.append(item)

	for item in sorted(items):
		print("\"0x" + item[1] + "\"\t" + item[2] + "\t" + str(item[3]) + "\t" + item[4])



if __name__ == "__main__":
	main()