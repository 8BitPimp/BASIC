#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <map>
#include <memory>

typedef void bsc_cfunc_t(struct bsc_state_t &);

struct bsc_buffer_t {
    template <typename type_t>
    void emit(const type_t in) {
        emit(&in, sizeof(type_t));
    }

    template <typename type_t>
    void patch(uint32_t offset, const type_t in) {
        patch(offset, &in, sizeof(type_t));
    }

    uint32_t pos() const;

protected:
    uint32_t offset_;
    std::unique_ptr<uint8_t> data_;

    void reserve(uint32_t size);

    void emit(const void * src, uint32_t size);
    void patch(uint32_t offset, const void * src, uint32_t size);
};

struct bsc_func_t {
    std::string name_;
    std::vector<std::string> strings_;
    std::vector<std::string> args_;
    std::vector<std::string> locals_;
    bsc_buffer_t buffer_;

    void clear();
};

struct bsc_state_t {
    std::map<std::string, bsc_func_t *> func_;
    std::map<std::string, bsc_cfunc_t > cfunc_;

    bsc_func_t * new_function(const std::string & name);
    void add_cfunction(const std::string & name, bsc_cfunc_t callback);

    void run(const std::string & name);
};

struct bsc_label_t {

    bsc_label_t(bsc_func_t & func)
        : func_(func)
        , address_(-1)
    {
    }

    uint32_t address_;
    std::vector<uint32_t> patch_;
    bsc_func_t & func_;

    ~bsc_label_t() { finalize(); }
    void finalize();
};

struct func_builder_t {

    func_builder_t(bsc_func_t & func);

    void add_arg    (const std::string & name);
    void add_local  (const std::string & name);

    void emit_label (bsc_label_t & label);

    void emit_null  ();
    void emit_string(const std::string & str);
    void emit_const (int32_t value);

    void emit_iload (const std::string & str);
    void emit_istore(const std::string & str);

    void emit_load  (const std::string & str);
    void emit_store (const std::string & str);

    void emit_call  ();
    void emit_return();

    void emit_add   ();
    void emit_sub   ();
    void emit_mul   ();
    void emit_and   ();
    void emit_not   ();
    void emit_or    ();
    void emit_xor   ();

    void emit_jmp   (bsc_label_t & label);
    void emit_jz    (bsc_label_t & label);
    void emit_jnz   (bsc_label_t & label);
    void emit_je    (bsc_label_t & label);
    void emit_jne   (bsc_label_t & label);
    void emit_jl    (bsc_label_t & label);
    void emit_jle   (bsc_label_t & label);
    void emit_jg    (bsc_label_t & label);
    void emit_jge   (bsc_label_t & label);

    void emit_drop  ();

protected:
    std::vector<std::string> strings_;
    std::map<std::string, uint32_t> ident_;
    bsc_func_t & func_;
};
