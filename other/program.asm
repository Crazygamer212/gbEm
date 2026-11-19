    NOP     ; 0x100
    NOP     ; 0x101
    NOP     ; 0x102

.kewel
    DEC A     ; 0x103
    DEC A     ; 0x104
    INC B     ; 0x105
    DEC B     ; 0x106
    OR A, A   ; 0x109
    XOR A, A  ; 0x107
    AND A, A  ; 0x108

    JP .kewel  ; 0x10a  -> 0x103



amogus:
    JP amogus