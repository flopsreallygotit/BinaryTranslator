//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DEFINE_PUSH(IMM, 3, "\x48\xc7\xc7",    // mov rdi, IMM 
{
    codeCopy(code, code->dataSize, &immValue, sizeof(int));
    codeAppend(code, "\x57", 1);    // push rdi
})

DEFINE_PUSH(REG, 0, "", 
{
    registerValue += 0x50;    // push r*x
    codeCopy(code, code->dataSize, &registerValue, sizeof(char));
})

DEFINE_PUSH(IMM_MEM, 4, "\x48\x8b\x3c\x25",    // push rdi, qword IMM
{
    codeCopy(code, code->dataSize, &immValue, sizeof(int));
    codeAppend(code, "\x57", 1);            // push rdi
})

DEFINE_PUSH(REG_MEM, 2, "\x48\x8b",    // mov rdi, qword [r*x]  
{
    registerValue += 0x38;
         
    codeCopy(code, code->dataSize, &registerValue, sizeof(char));
    codeAppend(code, "\x57", 1);    // push rdi
})

DEFINE_PUSH(IMM_REG_MEM, 2, "\x48\x8b",    // mov rdi, qword [r*x + IMM]  
{
    registerValue += 0xb8;
         
    codeCopy(code, code->dataSize, &registerValue, sizeof(char));
    codeCopy(code, code->dataSize, &immValue, sizeof(int));
    codeAppend(code, "\x57", 1);        // push rdi
})

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
