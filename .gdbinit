# Set the architecture
set architecture i386

# Set the disassembly flavor
set disassembly-flavor intel

set disassemble-next-line

set pagination enable

set print address

set logging enable

set logging file gdb.log

# Set the prompt
set prompt (gdb)

# Set the history file
set history save on
set history filename ~/.gdb_history
set history size 1000

# Set the default file search path
set solib-search-path .

# Set the default directory
d /home/user/Documents/ClassicOS

# Set the default target
file ClassicOS/build/kernel

# Set the symbol file
symbol-file kernel

# Set the debugging options
set disassemble-next-line on
set print pretty on

# Set breakpoints
break main