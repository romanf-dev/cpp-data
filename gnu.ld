SECTIONS
{
    .blob :
    {
        QUAD(0xcafebabe);
        QUAD(root);
        KEEP(*(.rodata));
        KEEP(*(.data.rel.ro.local));
    }

    /DISCARD/ : { *(.comment); }
}

