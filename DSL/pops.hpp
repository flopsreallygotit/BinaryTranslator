//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DEFINE_POP(IMM_MEM, 4, "\x5f\x48\x89\x3d",                     // pop rdi
{
    codeCopy(code, code->dataSize, &immValue, sizeof(int)); // mov [IMM], rdi
})

DEFINE_POP(REG_MEM, 3, "\x5f\x48\x89",                                 // pop rdi
{
    registerValue += 0x38;

    codeCopy(code, code->dataSize, &registerValue, sizeof(char));   // mov [r*x], rdi
})

DEFINE_POP(REG, 0, "", // pop r*x 
{
    registerValue += 0x58; 

    codeCopy(code, code->dataSize, &registerValue, sizeof(char));
})

DEFINE_POP(IMM_REG_MEM, 3, "\x5f\x48\x89",                             // pop rdi
{
    registerValue += 0xb8;
    
    codeCopy(code, code->dataSize, &registerValue, sizeof(char));   // mov [r*x + IMM], rdi
    codeCopy(code, code->dataSize, &immValue,      sizeof(int));
})

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~