# Set the architecture
set architecture i386

# Set the disassembly flavor
set disassembly-flavor intel

# Set the prompt
set prompt (gdb)

# Set the history file
set history save on
set history filename ~/.gdb_history
set history size 1000

# Set the default file search path
set solib-search-path .

# Set the default directory
cd /path/to/project

# Set the default target
file kernel

# Set the symbol file
symbol-file kernel

# Set the debugging options
set disassemble-next-line on
set print pretty on

# Set breakpoints
break main