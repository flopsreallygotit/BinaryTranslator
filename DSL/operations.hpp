//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// 0:   |   5F          | pop rdi
// 1:   |   5E          | pop rsi

// 2:   |   48 01 FE    | add rsi, rdi

// 5:   |   56          | push rsi

DEFINE_OPERATION(ADD, 6, "\x5F\x5E\x48\x01\xFE\x56")

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// 0:   |   5F          | pop rdi
// 1:   |   5E          | pop rsi

// 2:   |   48 29 FE    | sub rsi, rdi

// 5:   |   56          | push rsi

DEFINE_OPERATION(SUB, 6, "\x5F\x5E\x48\x29\xFE\x56")

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DEFINE_OPERATION(MUL, 15, "\x5F"            // 0:   |   5F          |   pop rdi     // TODO double check + extension for vertical paste
                          "\x5E"            // 1:   |   5E          |   pop rsi
                          "\x48\x89\xC2"    // 2:   |   49 89 C2    |   mov r10, rax
                          "\x48\x89\xF0"    // 5:   |   48 89 F0    |   mov rax, rsi
                          "\x48\xF7\xE7"    // 8:   |   48 F7 E7    |   mul rdi
                          "\x50"           // B:   |   50          |   push rax 
                          "\x4C\x89\xD0")   // C:   |   4C 89 D0    |   mov rax, r10

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DEFINE_OPERATION(DIV, 24, {"\x5F",            /* 0:   |   5F          |*/ "pop rdi"},
                          {"\x5E",            /* 1:   |   5E          |*/ "pop rsi"},
                          {"\x49\x89\xC2",    /* 2:   |   49 89 C2    |*/ "mov r10, rax"},
                          {"\x48\x89\xF0",    /* 5:   |   48 89 F0    |*/ "mov rax, rsi"},
                          {"\x52",            /* 8:   |   52          |*/ "push rdx"},
                          {"\x48\x31\xD2",    /* 9:   |   48 31 D2    |*/ "xor rdx, rdx"},
                          {"\x48\xF7\xF7",    /* C:   |   48 F7 F7    |*/ "div rdi"},
                          {"\x5A",            /* F:   |   5A          |*/ "pop rdx"},
                          {"\x50",            /* 10:  |   50          |*/ "push rax"},
                          {"\x4C\x89\xD0",    /* 11:  |   4C 89 D0    |*/ "mov rax, r10"})

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// 0:   |   F2 0F 10 04 24  |   movsd xmm0, QWORD PTR [rsp]
// 5:   |   66 0F 51 C0     |   sqrtpd xmm0, xmm0
// 9:   |   F2 0F 11 04 24  |   movsd QWORD PTR [rsp], xmm0

DEFINE_OPERATION(POW, 17, "\xf2\x0f\x10\x04\x24\x66\x0f\x51\xc0\xf2\x0f\x11\x04\x24") // TODO SQRT

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// 0:   |   5e          |   pop rsi
// 1:   |   5f          |   pop rdi

// 2:   |   48 39 f7    |   cmp rdi, rsi

// 5:   |   7c 04       |   jl 0xb  <--+
// 7:   |   6a 01       |   push 0x1   |
//                                     |
// 9:   |   eb 02       |   jmp 0xd <--+--+
// B:   |   6a 00       |   push 0x0      |
//                                        |
// D:   |                           <-----+

DEFINE_OPERATION(IS_BT, 13, "\x5E\x5F\x48\x39\xF7\x7C\x04\x6A\x01\xEB\x02\x6A\x00") // TODO use it

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
