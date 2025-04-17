/*
 * Tiny Code Generator for QEMU
 *
 * Copyright (c) 2008 Fabrice Bellard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "qemu/osdep.h"
#include "qemu/host-utils.h"
#include "cpu.h"


   // Compile-time dispatch
   #define CAT(a, ...) PRIMITIVE_CAT(a, __VA_ARGS__)
   #define PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__
   
   #define IIF(c) PRIMITIVE_CAT(IIF_, c)
   #define IIF_0(t, ...) __VA_ARGS__
   #define IIF_1(t, ...) t
   
   #define PROBE(x) x, 1
   #define CHECK(...) CHECK_N(__VA_ARGS__, 0)
   #define CHECK_N(x, n, ...) n
   
   // Void type detection
   #define VOID_TYPE_void ()
   #define VOID_TYPE_noreturn ()
   #define VOID_PROBE(type)            VOID_PROBE_PROXY(VOID_TYPE_##type)
   #define VOID_PROBE_PROXY(...)       VOID_PROBE_PRIMITIVE(__VA_ARGS__)
   #define VOID_PROBE_PRIMITIVE(x)     VOID_PROBE_COMBINE_ x
   #define VOID_PROBE_COMBINE_(...)    PROBE(~)
   #define IS_VOID(type)               CHECK(VOID_PROBE(type))
   
   // Global helper detection
   #define GLOB_NAME_uc_tracecode ()
   #define GLOB_NAME_div_i32 ()
   #define GLOB_NAME_rem_i32 ()
   #define GLOB_NAME_divu_i32 ()
   #define GLOB_NAME_remu_i32 ()
   #define GLOB_NAME_div_i64 ()
   #define GLOB_NAME_rem_i64 ()
   #define GLOB_NAME_divu_i64 ()
   #define GLOB_NAME_remu_i64 ()
   #define GLOB_NAME_shl_i64 ()
   #define GLOB_NAME_shr_i64 ()
   #define GLOB_NAME_sar_i64 ()
   #define GLOB_NAME_mulsh_i64 ()
   #define GLOB_PROBE(name)            GLOB_PROBE_PROXY(GLOB_NAME_##name)
   #define GLOB_PROBE_PROXY(...)       GLOB_PROBE_PRIMITIVE(__VA_ARGS__)
   #define GLOB_PROBE_PRIMITIVE(x)     GLOB_PROBE_COMBINE_ x
   #define GLOB_PROBE_COMBINE_(...)    PROBE(~)
   #define IS_GLOB(name)               CHECK(GLOB_PROBE(name))
   
   // Arguments
   #define A1 (a1 | ((uint64_t)a2  << 32))
   #define A2 (a3 | ((uint64_t)a4  << 32))
   #define A3 (a5 | ((uint64_t)a6  << 32))
   #define A4 (a7 | ((uint64_t)a8  << 32))
   #define A5 (a9 | ((uint64_t)a10 << 32))
   #define A6 (a11 | ((uint64_t)a12 << 32))
   #define A7 (a13 | ((uint64_t)a14 << 32))
   #define GEN_ADAPTER_ARGS \
     uint32_t a1, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5, \
     uint32_t a6, uint32_t a7, uint32_t a8, uint32_t a9, uint32_t a10, uint32_t a11, uint32_t a12
   
   // Adapter definition
   #define GEN_ADAPTER_0_VOID(name) \
       HELPER(name)(); return 0;
   #define GEN_ADAPTER_0_NONVOID(name) \
       return HELPER(name)();
   #define GEN_ADAPTER_0_DEFINE(name, ret) \
   uint64_t glue(adapter_helper_, name)(GEN_ADAPTER_ARGS) { \
       IIF(IS_VOID(ret)) (GEN_ADAPTER_0_VOID(name), GEN_ADAPTER_0_NONVOID(name)) \
   }
   
   #define GEN_ADAPTER_1_VOID(name, t1) \
       HELPER(name)((dh_ctype(t1))A1); return 0;
   #define GEN_ADAPTER_1_NONVOID(name, t1) \
       return HELPER(name)((dh_ctype(t1))A1);
   #define GEN_ADAPTER_1_DEFINE(name, ret, t1) \
   uint64_t glue(adapter_helper_, name)(GEN_ADAPTER_ARGS) { \
       IIF(IS_VOID(ret)) (GEN_ADAPTER_1_VOID(name, t1), GEN_ADAPTER_1_NONVOID(name, t1)) \
   }
   
   #define GEN_ADAPTER_2_VOID(name, t1, t2) \
       HELPER(name)((dh_ctype(t1))A1, (dh_ctype(t2))A2); return 0;
   #define GEN_ADAPTER_2_NONVOID(name, t1, t2) \
       return HELPER(name)((dh_ctype(t1))A1, (dh_ctype(t2))A2);
   #define GEN_ADAPTER_2_DEFINE(name, ret, t1, t2) \
   uint64_t glue(adapter_helper_, name)(GEN_ADAPTER_ARGS) { \
       IIF(IS_VOID(ret)) (GEN_ADAPTER_2_VOID(name, t1, t2), GEN_ADAPTER_2_NONVOID(name, t1, t2)) \
   }
   
   #define GEN_ADAPTER_3_VOID(name, t1, t2, t3) \
       HELPER(name)((dh_ctype(t1))A1, (dh_ctype(t2))A2, (dh_ctype(t3))A3); return 0;
   #define GEN_ADAPTER_3_NONVOID(name, t1, t2, t3) \
       return HELPER(name)((dh_ctype(t1))A1, (dh_ctype(t2))A2, (dh_ctype(t3))A3);
   #define GEN_ADAPTER_3_DEFINE(name, ret, t1, t2, t3) \
   uint64_t glue(adapter_helper_, name)(GEN_ADAPTER_ARGS) { \
       IIF(IS_VOID(ret)) (GEN_ADAPTER_3_VOID(name, t1, t2, t3), GEN_ADAPTER_3_NONVOID(name, t1, t2, t3)) \
   }
   
   #define GEN_ADAPTER_4_VOID(name, t1, t2, t3, t4) \
       HELPER(name)((dh_ctype(t1))A1, (dh_ctype(t2))A2, (dh_ctype(t3))A3, (dh_ctype(t4))A4); return 0;
   #define GEN_ADAPTER_4_NONVOID(name, t1, t2, t3, t4) \
       return HELPER(name)((dh_ctype(t1))A1, (dh_ctype(t2))A2, (dh_ctype(t3))A3, (dh_ctype(t4))A4);
   #define GEN_ADAPTER_4_DEFINE(name, ret, t1, t2, t3, t4) \
   uint64_t glue(adapter_helper_, name)(GEN_ADAPTER_ARGS) { \
       IIF(IS_VOID(ret)) (GEN_ADAPTER_4_VOID(name, t1, t2, t3, t4), GEN_ADAPTER_4_NONVOID(name, t1, t2, t3, t4)) \
   }
   
   #define GEN_ADAPTER_5_VOID(name, t1, t2, t3, t4, t5) \
       HELPER(name)((dh_ctype(t1))A1, (dh_ctype(t2))A2, (dh_ctype(t3))A3, (dh_ctype(t4))A4, (dh_ctype(t5))A5); return 0;
   #define GEN_ADAPTER_5_NONVOID(name, t1, t2, t3, t4, t5) \
       return HELPER(name)((dh_ctype(t1))A1, (dh_ctype(t2))A2, (dh_ctype(t3))A3, (dh_ctype(t4))A4, (dh_ctype(t5))A5);
   #define GEN_ADAPTER_5_DEFINE(name, ret, t1, t2, t3, t4, t5) \
   uint64_t glue(adapter_helper_, name)(GEN_ADAPTER_ARGS) { \
       IIF(IS_VOID(ret)) (GEN_ADAPTER_5_VOID(name, t1, t2, t3, t4, t5), GEN_ADAPTER_5_NONVOID(name, t1, t2, t3, t4, t5)) \
   }
      
   #define GEN_ADAPTER_6_VOID(name, t1, t2, t3, t4, t5, t6) \
       HELPER(name)((dh_ctype(t1))A1, (dh_ctype(t2))A2, (dh_ctype(t3))A3, (dh_ctype(t4))A4, (dh_ctype(t5))A5, (dh_ctype(t6))A6); return 0;
   #define GEN_ADAPTER_6_NONVOID(name, t1, t2, t3, t4, t5, t6) \
       return HELPER(name)((dh_ctype(t1))A1, (dh_ctype(t2))A2, (dh_ctype(t3))A3, (dh_ctype(t4))A4, (dh_ctype(t5))A5, (dh_ctype(t6))A6);
   #define GEN_ADAPTER_6_DEFINE(name, ret, t1, t2, t3, t4, t5, t6) \
   uint64_t glue(adapter_helper_, name)(GEN_ADAPTER_ARGS) { \
       IIF(IS_VOID(ret)) (GEN_ADAPTER_6_VOID(name, t1, t2, t3, t4, t5, t6), GEN_ADAPTER_6_NONVOID(name, t1, t2, t3, t4, t5, t6)) \
   }

   #define GEN_ADAPTER_7_VOID(name, t1, t2, t3, t4, t5, t6, t7) \
   HELPER(name)((dh_ctype(t1))A1, (dh_ctype(t2))A2, (dh_ctype(t3))A3, (dh_ctype(t4))A4, (dh_ctype(t5))A5, (dh_ctype(t6))A6, (dh_ctype(t7))A7); return 0;
#define GEN_ADAPTER_7_NONVOID(name, t1, t2, t3, t4, t5, t6, t7) \
   return HELPER(name)((dh_ctype(t1))A1, (dh_ctype(t2))A2, (dh_ctype(t3))A3, (dh_ctype(t4))A4, (dh_ctype(t5))A5, (dh_ctype(t6))A6, (dh_ctype(t7))A7);
#define GEN_ADAPTER_7_DEFINE(name, ret, t1, t2, t3, t4, t5, t6, t7) \
uint64_t glue(adapter_helper_, name)(GEN_ADAPTER_ARGS) { \
   IIF(IS_VOID(ret)) (GEN_ADAPTER_7_VOID(name, t1, t2, t3, t4, t5, t6, t7), GEN_ADAPTER_7_NONVOID(name, t1, t2, t3, t4, t5, t6, t7)) \
}
   
   #define GEN_ADAPTER_BLANK
   #ifdef GEN_ADAPTER_DEFINE
   #define GEN_ADAPTER_0(name, ret) \
       IIF(IS_GLOB(name)) (GEN_ADAPTER_BLANK, GEN_ADAPTER_0_DEFINE(name, ret))
   #define GEN_ADAPTER_1(name, ret, t1) \
       IIF(IS_GLOB(name)) (GEN_ADAPTER_BLANK, GEN_ADAPTER_1_DEFINE(name, ret, t1))
   #define GEN_ADAPTER_2(name, ret, t1, t2) \
       IIF(IS_GLOB(name)) (GEN_ADAPTER_BLANK, GEN_ADAPTER_2_DEFINE(name, ret, t1, t2))
   #define GEN_ADAPTER_3(name, ret, t1, t2, t3) \
       IIF(IS_GLOB(name)) (GEN_ADAPTER_BLANK, GEN_ADAPTER_3_DEFINE(name, ret, t1, t2, t3))
   #define GEN_ADAPTER_4(name, ret, t1, t2, t3, t4) \
       IIF(IS_GLOB(name)) (GEN_ADAPTER_BLANK, GEN_ADAPTER_4_DEFINE(name, ret, t1, t2, t3, t4))
   #define GEN_ADAPTER_5(name, ret, t1, t2, t3, t4, t5) \
       IIF(IS_GLOB(name)) (GEN_ADAPTER_BLANK, GEN_ADAPTER_5_DEFINE(name, ret, t1, t2, t3, t4, t5))
       #define GEN_ADAPTER_6(name, ret, t1, t2, t3, t4, t5, t6) \
       IIF(IS_GLOB(name)) (GEN_ADAPTER_BLANK, GEN_ADAPTER_6_DEFINE(name, ret, t1, t2, t3, t4, t5, t6))
       #define GEN_ADAPTER_7(name, ret, t1, t2, t3, t4, t5, t6, t7) \
       IIF(IS_GLOB(name)) (GEN_ADAPTER_BLANK, GEN_ADAPTER_7_DEFINE(name, ret, t1, t2, t3, t4, t5, t6, t7))
   #else
   #define GEN_ADAPTER_0(name, ret) GEN_ADAPTER_BLANK
   #define GEN_ADAPTER_1(name, ret, t1) GEN_ADAPTER_BLANK
   #define GEN_ADAPTER_2(name, ret, t1, t2) GEN_ADAPTER_BLANK
   #define GEN_ADAPTER_3(name, ret, t1, t2, t3) GEN_ADAPTER_BLANK
   #define GEN_ADAPTER_4(name, ret, t1, t2, t3, t4) GEN_ADAPTER_BLANK
   #define GEN_ADAPTER_5(name, ret, t1, t2, t3, t4, t5) GEN_ADAPTER_BLANK
   #define GEN_ADAPTER_6(name, ret, t1, t2, t3, t4, t5, t6) GEN_ADAPTER_BLANK
   #define GEN_ADAPTER_7(name, ret, t1, t2, t3, t4, t5, t6, t7) GEN_ADAPTER_BLANK
   #endif
   

#include "exec/helper-proto.h"
#include "exec/cpu_ldst.h"
#include "exec/exec-all.h"
#include "exec/tb-lookup.h"
#include "tcg/tcg.h"

#include <uc_priv.h>


/* 32-bit helpers */

int32_t HELPER(div_i32)(int32_t arg1, int32_t arg2)
{
    return arg1 / arg2;
}

int32_t HELPER(rem_i32)(int32_t arg1, int32_t arg2)
{
    return arg1 % arg2;
}

uint32_t HELPER(divu_i32)(uint32_t arg1, uint32_t arg2)
{
    return arg1 / arg2;
}

uint32_t HELPER(remu_i32)(uint32_t arg1, uint32_t arg2)
{
    return arg1 % arg2;
}

/* 64-bit helpers */

uint64_t HELPER(shl_i64)(uint64_t arg1, uint64_t arg2)
{
    return arg1 << arg2;
}

uint64_t HELPER(shr_i64)(uint64_t arg1, uint64_t arg2)
{
    return arg1 >> arg2;
}

int64_t HELPER(sar_i64)(int64_t arg1, int64_t arg2)
{
    return arg1 >> arg2;
}

int64_t HELPER(div_i64)(int64_t arg1, int64_t arg2)
{
    return arg1 / arg2;
}

int64_t HELPER(rem_i64)(int64_t arg1, int64_t arg2)
{
    return arg1 % arg2;
}

uint64_t HELPER(divu_i64)(uint64_t arg1, uint64_t arg2)
{
    return arg1 / arg2;
}

uint64_t HELPER(remu_i64)(uint64_t arg1, uint64_t arg2)
{
    return arg1 % arg2;
}

uint64_t HELPER(muluh_i64)(uint64_t arg1, uint64_t arg2)
{
    uint64_t l, h;
    mulu64(&l, &h, arg1, arg2);
    return h;
}

int64_t HELPER(mulsh_i64)(int64_t arg1, int64_t arg2)
{
    uint64_t l, h;
    muls64(&l, &h, arg1, arg2);
    return h;
}

uint32_t HELPER(clz_i32)(uint32_t arg, uint32_t zero_val)
{
    return arg ? clz32(arg) : zero_val;
}

uint32_t HELPER(ctz_i32)(uint32_t arg, uint32_t zero_val)
{
    return arg ? ctz32(arg) : zero_val;
}

uint64_t HELPER(clz_i64)(uint64_t arg, uint64_t zero_val)
{
    return arg ? clz64(arg) : zero_val;
}

uint64_t HELPER(ctz_i64)(uint64_t arg, uint64_t zero_val)
{
    return arg ? ctz64(arg) : zero_val;
}

uint32_t HELPER(clrsb_i32)(uint32_t arg)
{
    return clrsb32(arg);
}

uint64_t HELPER(clrsb_i64)(uint64_t arg)
{
    return clrsb64(arg);
}

uint32_t HELPER(ctpop_i32)(uint32_t arg)
{
    return ctpop32(arg);
}

uint64_t HELPER(ctpop_i64)(uint64_t arg)
{
    return ctpop64(arg);
}

void *HELPER(lookup_tb_ptr)(CPUArchState *env)
{
    CPUState *cpu = env_cpu(env);
    TranslationBlock *tb;
    target_ulong cs_base, pc;
    uint32_t flags;
    struct uc_struct *uc = (struct uc_struct *)cpu->uc;

    tb = tb_lookup__cpu_state(cpu, &pc, &cs_base, &flags, curr_cflags());
    if (tb == NULL) {
        return uc->tcg_ctx->code_gen_epilogue;
    }
    return tb->tc.ptr;
}

void HELPER(exit_atomic)(CPUArchState *env)
{
    cpu_loop_exit_atomic(env_cpu(env), GETPC());
}
