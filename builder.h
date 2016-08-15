#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <map>
#include <memory>

#include "buffer.h"

typedef void (*bsc_cfunc_t)(struct bsc_state_t &);

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
};

struct bsc_label_t {

    bsc_label_t(bsc_func_t & func)
        : func_(func)
        , address_(-1) {
    }

    uint32_t address_;
    std::vector<uint32_t> patch_;
    bsc_func_t & func_;

    ~bsc_label_t() { finalize(); }
    void finalize();
};

struct func_builder_t {

    func_builder_t(bsc_func_t & func);
    ~func_builder_t() { finalize(); }
    
    void finalize();

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

struct bsc_obj_t {

	enum type_t	{
		null,
		integer,
		string,
		array,
		function,
	};

	const type_t type_;

	template <typename type_t>
	const type_t & cast() const {
		assert(type_t::type() == type_);
		return *static_cast<type_t*>(this);
	}

	template <typename type_t>
	type_t & cast() {
		assert(type_t::type() == type_);
		return *static_cast<type_t*>(this);
	}

	template <typename type_t>
	const type_t * try_cast() const {
		return (type_t::type() == type_) ?
			*static_cast<type_t*>(this) :
			nullptr;
	}

	template <typename type_t>
	type_t * try_cast() {
		return (type_t::type() == type_) ?
			*static_cast<type_t*>(this) :
			nullptr;
	}

    bsc_obj_t * copy();

protected:
	bsc_obj_t(type_t type)
		: type_(type) {
	}
};

struct bsc_obj_null_t
	: public bsc_obj_t {

	bsc_obj_null_t()
		: bsc_obj_t(bsc_obj_t::null)
	{
	}

	static bsc_obj_t::type_t type() {
		return bsc_obj_t::null;
	}
};

struct bsc_obj_integer_t
	: public bsc_obj_t {
    
	bsc_obj_integer_t()
		: bsc_obj_t(bsc_obj_t::integer)
	{
	}

	int32_t value_;

	static bsc_obj_t::type_t type() {
		return bsc_obj_t::integer;
	}
};

struct bsc_obj_string_t
	: public bsc_obj_t {
    
	bsc_obj_string_t()
		: bsc_obj_t(bsc_obj_t::string)
	{
	}

	std::string string_;

	static bsc_obj_t::type_t type() {
		return bsc_obj_t::string;
	}
};

struct bsc_obj_array_t
	: public bsc_obj_t {
    
	bsc_obj_array_t()
		: bsc_obj_t(bsc_obj_t::array)
	{
	}

    std::vector<bsc_obj_t> element_;

	static bsc_obj_t::type_t type() {
		return bsc_obj_t::array;
	}
};

struct bsc_obj_function_t
	: public bsc_obj_t {
    
	bsc_obj_function_t()
		: bsc_obj_t(bsc_obj_t::function)
	{
	}

	std::string name_;
	std::vector<std::string> args_;

	static bsc_obj_t::type_t type() {
		return bsc_obj_t::function;
	}
};

struct bsc_frame_t {

    bsc_frame_t(bsc_func_t & func);

    bsc_reader_t reader_;
    bsc_func_t & func_;
    uint32_t sp_;

    void set_local(const std::string & id, bsc_obj_t * obj);
    bsc_obj_t * get_local(const std::string & id);

protected:
    std::map<std::string, std::unique_ptr<bsc_obj_t>> local_;
};

struct bsc_vm_t {

    bsc_vm_t(bsc_state_t & state)
        : state_(state) {
    }

    void run(const std::string & func);

protected:

    void pop(std::unique_ptr<bsc_obj_t> & out) {
        out.reset(stack_.back());
        stack_.pop_back();
    }

    void push(std::unique_ptr<bsc_obj_t> & out) {
        stack_.push_back(out.release());
    }

    void do_call();

    void step();

    std::vector<bsc_obj_t*> stack_;
    std::vector<bsc_frame_t> frame_;
    bsc_state_t & state_;
};
