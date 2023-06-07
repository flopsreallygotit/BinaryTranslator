//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// 0:   |   E9  |   jmp ...

DEFINE_JMP(JMP, 1, "\xE9")

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// 0:   |   E8  |   call

DEFINE_JMP(CALL, 1, "\xE8")

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// 0:    | 5E        |   pop rsi; 
// 1:    | 5F        |   pop rdi; 

// 2:    | 48 39 F7  |   cmp rsi, rdi; 
// 5:    | 0F 84     |   je ...

DEFINE_JMP(JE, 7, "\x5E\x5F\x48\x39\xF7\x0F\x84")

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// 0:    | 5E        |   pop rsi; 
// 1:    | 5F        |   pop rdi; 

// 2:    | 48 39 F7  |   cmp rsi, rdi; 
// 5:    | 0F 85     |   jne ...

DEFINE_JMP(JNE, 7, "\x5E\x5F\x48\x39\xF7\x0F\x85")

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// 0:    | 5E        |   pop rsi; 
// 1:    | 5F        |   pop rdi; 

// 2:    | 48 39 F7  |   cmp rsi, rdi; 
// 5:    | 0F 8F     |   jg ...

DEFINE_JMP(JG, 7, "\x5E\x5F\x48\x39\xF7\x0F\x8F")

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// 0:    | 5E        |   pop rsi; 
// 1:    | 5F        |   pop rdi; 

// 2:    | 48 39 F7  |   cmp rsi, rdi; 
// 5:    | 0F 8D     |   jge ...

DEFINE_JMP(JGE, 7, "\x5E\x5F\x48\x39\xF7\x0F\x8D")

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// 0:    | 5E        |   pop rsi; 
// 1:    | 5F        |   pop rdi; 

// 2:    | 48 39 F7  |   cmp rsi, rdi; 
// 5:    | 0F 8C     |   jl ...

DEFINE_JMP(JL, 7, "\x5E\x5F\x48\x39\xF7\x0F\x8C")

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// 0:    | 5E        |   pop rsi; 
// 1:    | 5F        |   pop rdi; 

// 2:    | 48 39 F7  |   cmp rsi, rdi; 
// 5:    | 0F 8E     |   jle ...

DEFINE_JMP(JLE, 7, "\x5E\x5F\x48\x39\xF7\x0F\x8E")

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~