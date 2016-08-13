#include "builder.h"

void bsc_call(bsc_state_t & state) {
}

int main()
{
    bsc_state_t state;

    // external api
    state.add_cfunction("call", bsc_call);

    // test function
    {
        bsc_func_t * func = state.new_function("test");
        func_builder_t b = func_builder_t(*func);
        // return value
        b.emit_const(1);
    }

    // main function
    {
        bsc_func_t * func = state.new_function("main");
        func_builder_t b = func_builder_t(*func);
        // argument string
        b.emit_string("Hello World ");
        b.emit_load("test");
        b.emit_call();
        b.emit_add();
        // print to screen
        b.emit_load("print");
        b.emit_call();
        b.emit_drop();
        // return value
        b.emit_null();
    }

    // execute main
    state.run("main");

    return 0;
}
