# Set the architecture
set architecture i386

# Set the target executable
file kernel

# Set the symbol file
symbol-file kernel

# Set the debugging options
set disassembly-flavor intel
set disassemble-next-line on
set print pretty on

# Set breakpoints
break main