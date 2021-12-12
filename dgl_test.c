#define DGL_IMPLEMENTATION
#include "dgl.h"

#include "dgl_test_helpers.h"

int
main(int argc, char **argv)
{
    DGL_BEGIN_TEST("Memory alignment");
    {
        uint8 memory[1024] = {};
        DGL_Mem_Arena arena = {};

        dgl_mem_arena_init(&arena, memory, 1024);

        uint8 *mem1 = (uint8 *)dgl_mem_arena_alloc_align(&arena, 13, 8);
        uint8 *mem2 = (uint8 *)dgl_mem_arena_alloc_align(&arena, 2, 8);
        uint8 *mem3 = (uint8 *)dgl_mem_arena_alloc_align(&arena, 6, 32);
        uint8 *mem4 = (uint8 *)dgl_mem_arena_alloc_align(&arena, 1, 32);

        DGL_EXPECT_int32((uintptr)mem1 % 8, ==, 0);
        DGL_EXPECT_int32((uintptr)mem2 % 8, ==, 0);
        DGL_EXPECT_int32((uintptr)mem3 % 32, ==, 0);
        DGL_EXPECT_int32((uintptr)mem4 % 32, ==, 0);
    }
    DGL_END_TEST();

    if(dgl_test_result()) { return(0); }
    else { return(1); }
}
