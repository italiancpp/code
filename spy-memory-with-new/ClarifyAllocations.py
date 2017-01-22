#!/usr/bin/python
#
# C++filt demangles names.
# 
# addr2line converts code pointers (e. g. functions' addresses) 
# into the file:line couple corresponding to the code (if there are debug symbols).
#
# The python code should be portable, but the called utilities aren't.
#

import re
import subprocess
#

# Opens a sub-process and passes shell commands to it. Returns the results as a string.
# Not very efficient, but easy.
def run_shell(command):
	return subprocess.Popen(command, stdout=subprocess.PIPE).communicate()[0]
#
#
if __name__ == "__main__":
	total_size = 0;
#	
	# There are 2 types of lines in the output: stack frames and allocation sizes.
	size_line  = re.compile("Allocation, size = (\d+) at (\d+)")  # Allocation, size = <byte> at <pointer somewhere in the heap>
	stack_line = re.compile(".*\((.*)\+.*\) \[(.*)\]")  # <rubbish>(mangled name) [<code pointer>]
#
	allocations_file = open("allocations.txt")
	for line in allocations_file:
		match_size = size_line.match(line)
		match_stack = stack_line.match(line)
#	
		# For a demo, I compute the sum of all the used memory.
		# The things you can do with an overridden new!
		if (match_size):
			allocation_size = int(match_size.group(1)) 
			total_size += allocation_size
			print "Used " + str(allocation_size)
#	
		elif (match_stack):
			mangled_name = match_stack.group(1)
			line_address = match_stack.group(2)
			demangled_name = run_shell(["c++filt",  "-n", mangled_name])
			line_number = run_shell(["addr2line",  "-e",   "./overridenew",  line_address])
#		
			# This is not professional-grade formatting. The -1 cuts away the newlines.
			print"\t" + demangled_name[:-1] + "\n\t\t" + line_number,
#
		# Copy the separator as they were.
		else:
			print line
#
	print "\n total allocated size "  + str(total_size)