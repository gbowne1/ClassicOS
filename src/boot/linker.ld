 ENTRY(boot)
     SECTIONS {
       . = 0x7c00;
       .text : {
         *(.text)
       }
       /* Add other sections as needed */
     }