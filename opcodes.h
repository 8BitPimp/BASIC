#pragma once
#include <cstdint>

enum bsc_opcode_t : uint8_t
{
    bsc_null_e = 0, // (push none object)
    bsc_string_e,	// (string index)
    bsc_const_e,	// (constant index)
    bsc_load_e,		// (push variable value)
    bsc_store_e,	// (store value to variable)
    bsc_call_e,		// (call )
    bsc_iload_e,	// (name operand, pop index, push value)
    bsc_istore_e,	// (name operand, pop value, push to index)
    bsc_return_e,	// (pop value to return stack)
    bsc_drop_e,     // ()
    bsc_add_e,		// (pop two push one)
    bsc_sub_e,		// (pop two push one)
    bsc_mul_e,		// (pop two push one)
    bsc_and_e,
    bsc_not_e,
    bsc_or_e,
    bsc_xor_e,
    bsc_jmp_e,		// ()
    bsc_jz_e,		// (pop == 0)
    bsc_jnz_e,		// (pop != 0)
    bsc_je_e,		// (pop == pop)
    bsc_jne_e,		// (pop != pop)
    bsc_jg_e,		// (pop >  pop)
    bsc_jge_e,		// (pop >= pop)
    bsc_jl_e,		// (pop <  pop)
    bsc_jle_e,		// (pop <= pop)
    bsc_local_e		// (name index)
};
