SECTIONS
{
    .blob :
    {
        QUAD(0xcafebabe);
        QUAD(root);
        KEEP(*(.rodata));
        KEEP(*(.data*));
    }

    /DISCARD/ : { *(.comment); }
}

