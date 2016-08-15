#include <cassert>

#include "builder.h"
#include "opcodes.h"

bsc_frame_t::bsc_frame_t(bsc_func_t & func)
    : func_(func)
    , reader_(func.buffer_)
{
    for (const auto & itt:func_.locals_) {
        local_[itt].reset(new bsc_obj_null_t);
    }
}

void bsc_frame_t::set_local(const std::string & id, bsc_obj_t * obj) {
    auto itt = local_.find(id);
    assert(itt!=local_.end());
    itt->second.reset(obj);
}

bsc_obj_t * bsc_frame_t::get_local(const std::string & id) {
    auto itt = local_.find(id);
    return itt==local_.end() ? nullptr : itt->second.get();
}

bsc_obj_t * bsc_obj_t::copy() {
    return nullptr;
}

void bsc_vm_t::run(const std::string & func) {
}

void bsc_vm_t::do_call() {
}

void bsc_vm_t::step() {
    // pick next opcode
    uint8_t opcode = 0;
    assert(frame_.size());
    frame_.back().reader_.read<uint8_t>(opcode);
    
    // dispatch opcode
    switch (opcode) {
    case(bsc_null_e): {
        std::unique_ptr<bsc_obj_t> obj(new bsc_obj_null_t);
        push(obj);
    }
    break;

    case(bsc_string_e): {
        int32_t operand = 0;
        frame_.back().reader_.read(operand);
        std::unique_ptr<bsc_obj_t> obj(new bsc_obj_string_t);
        obj->cast<bsc_obj_string_t>().string_ = frame_.back().func_.strings_[operand];
        push(obj);
    } break;

    case(bsc_const_e): {
        int32_t operand = 0;
        frame_.back().reader_.read(operand);
        std::unique_ptr<bsc_obj_t> obj(new bsc_obj_integer_t);
        obj->cast<bsc_obj_integer_t>().value_ = operand;
        push(obj);
    } break;

    case(bsc_load_e): {
        // find local / global / arg
        // copy local / global / arg
        // push local
    } break;

    case(bsc_store_e): {

        //todo: set local / global

        int32_t operand = 0;
        frame_.back().reader_.read(operand);
        std::unique_ptr<bsc_obj_t> obj;
        pop(obj);
        frame_.back().set_local(frame_.back().func_.strings_[operand], obj.release());
    } break;

    case(bsc_call_e):
        do_call();
        break;
    case(bsc_iload_e): break;
    case(bsc_istore_e): break;
    case(bsc_return_e): break;
    case(bsc_drop_e): {
            std::unique_ptr<bsc_obj_t> obj;
            pop(obj);
        }
        break;
    case(bsc_add_e): break;
    case(bsc_sub_e): break;
    case(bsc_mul_e): break;
    case(bsc_and_e): break;
    case(bsc_not_e): break;
    case(bsc_or_e): break;
    case(bsc_xor_e): break;
    case(bsc_jmp_e): break;
    case(bsc_jz_e): break;
    case(bsc_jnz_e): break;
    case(bsc_je_e): break;
    case(bsc_jne_e): break;
    case(bsc_jg_e): break;
    case(bsc_jge_e): break;
    case(bsc_jl_e): break;
    case(bsc_jle_e): break;
    }
}
