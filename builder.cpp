#include "builder.h"
#include "opcodes.h"
#include <cassert>


bsc_func_t * bsc_state_t::new_function(const std::string & name) {
    bsc_func_t * func = new bsc_func_t;
    func->clear();
    func_[name] = func;
    return func;
}

void bsc_state_t::add_cfunction(const std::string & name,
                                bsc_cfunc_t callback) {
    cfunc_[name] = callback;
}

void bsc_func_t::clear() {
    this->locals_.clear();
    this->name_.clear();
    this->strings_.clear();
    this->args_.clear();
    this->buffer_.clear();
}

void bsc_label_t::finalize() {
    assert(address_ != -1);
    for (const auto & patch : patch_) {
        func_.buffer_.patch(patch, address_);
    }
}

void func_builder_t::finalize() {
    // make sure we have a closing return
    emit_return();
}

func_builder_t::func_builder_t(bsc_func_t & func)
    : func_(func)
{
}

void func_builder_t::add_arg(const std::string & name) {
    func_.args_.push_back(name);
}

void func_builder_t::add_local(const std::string & name) {
    func_.locals_.push_back(name);
}

void func_builder_t::emit_label(bsc_label_t & label) {
    label.address_ = func_.buffer_.pos();
}

void func_builder_t::emit_null() {
    func_.buffer_.emit(bsc_null_e);
}

void func_builder_t::emit_string(const std::string & str) {
    func_.buffer_.emit(bsc_string_e);
    const uint32_t index = (uint32_t)func_.strings_.size();
    func_.strings_.push_back(str);
    func_.buffer_.emit<uint32_t>(index);
}

void func_builder_t::emit_const(int32_t value) {
    func_.buffer_.emit(bsc_const_e);
    func_.buffer_.emit<uint32_t>(value);
}

void func_builder_t::emit_load(const std::string & str) {
    func_.buffer_.emit(bsc_load_e);
    const uint32_t index = (uint32_t)func_.strings_.size();
    func_.strings_.push_back(str);
    func_.buffer_.emit<uint32_t>(index);
}

void func_builder_t::emit_store(const std::string & str) {
    func_.buffer_.emit(bsc_store_e);
    const uint32_t index = (uint32_t)func_.strings_.size();
    func_.strings_.push_back(str);
    func_.buffer_.emit<uint32_t>(index);
}

void func_builder_t::emit_iload(const std::string & str) {
    func_.buffer_.emit(bsc_iload_e);
    const uint32_t index = (uint32_t)func_.strings_.size();
    func_.strings_.push_back(str);
    func_.buffer_.emit<uint32_t>(index);
}

void func_builder_t::emit_istore(const std::string & str) {
    func_.buffer_.emit(bsc_istore_e);
    const uint32_t index = (uint32_t)func_.strings_.size();
    func_.strings_.push_back(str);
    func_.buffer_.emit<uint32_t>(index);
}

void func_builder_t::emit_call() {
    func_.buffer_.emit(bsc_call_e);
}

void func_builder_t::emit_return() {
    func_.buffer_.emit(bsc_return_e);
}

void func_builder_t::emit_drop() {
    func_.buffer_.emit(bsc_drop_e);
}

void func_builder_t::emit_add() {
    func_.buffer_.emit(bsc_add_e);
}

void func_builder_t::emit_sub() {
    func_.buffer_.emit(bsc_sub_e);
}

void func_builder_t::emit_mul() {
    func_.buffer_.emit(bsc_mul_e);
}

void func_builder_t::emit_and() {
    func_.buffer_.emit(bsc_and_e);
}

void func_builder_t::emit_not() {
    func_.buffer_.emit(bsc_not_e);
}

void func_builder_t::emit_or() {
    func_.buffer_.emit(bsc_or_e);
}

void func_builder_t::emit_xor() {
    func_.buffer_.emit(bsc_xor_e);
}

void func_builder_t::emit_jmp(bsc_label_t & label) {
    func_.buffer_.emit(bsc_jmp_e);
    label.patch_.push_back(func_.buffer_.pos());
    func_.buffer_.emit<uint32_t>(label.address_);
}

void func_builder_t::emit_jz(bsc_label_t & label) {
    func_.buffer_.emit(bsc_jz_e);
    label.patch_.push_back(func_.buffer_.pos());
    func_.buffer_.emit<uint32_t>(label.address_);
}

void func_builder_t::emit_jnz(bsc_label_t & label) {
    func_.buffer_.emit(bsc_jnz_e);
    label.patch_.push_back(func_.buffer_.pos());
    func_.buffer_.emit<uint32_t>(label.address_);
}

void func_builder_t::emit_je(bsc_label_t & label) {
    func_.buffer_.emit(bsc_je_e);
    label.patch_.push_back(func_.buffer_.pos());
    func_.buffer_.emit<uint32_t>(label.address_);
}

void func_builder_t::emit_jne(bsc_label_t & label) {
    func_.buffer_.emit(bsc_jne_e);
    label.patch_.push_back(func_.buffer_.pos());
    func_.buffer_.emit<uint32_t>(label.address_);
}

void func_builder_t::emit_jl(bsc_label_t & label) {
    func_.buffer_.emit(bsc_jl_e);
    label.patch_.push_back(func_.buffer_.pos());
    func_.buffer_.emit<uint32_t>(label.address_);
}

void func_builder_t::emit_jle(bsc_label_t & label) {
    func_.buffer_.emit(bsc_jle_e);
    label.patch_.push_back(func_.buffer_.pos());
    func_.buffer_.emit<uint32_t>(label.address_);
}

void func_builder_t::emit_jg(bsc_label_t & label) {
    func_.buffer_.emit(bsc_jg_e);
    label.patch_.push_back(func_.buffer_.pos());
    func_.buffer_.emit<uint32_t>(label.address_);
}

void func_builder_t::emit_jge(bsc_label_t & label) {
    func_.buffer_.emit(bsc_jge_e);
    label.patch_.push_back(func_.buffer_.pos());
    func_.buffer_.emit<uint32_t>(label.address_);
}
