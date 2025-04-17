// Compile-time dispatch
#define CAT(a, ...) PRIMITIVE_CAT(a, __VA_ARGS__)
#define PRIMITIVE_CAT(a, ...) a##__VA_ARGS__

#define IIF(c) PRIMITIVE_CAT(IIF_, c)
#define IIF_0(t, ...) __VA_ARGS__
#define IIF_1(t, ...) t

#define PROBE(x) x, 1
#define CHECK(...) CHECK_N(__VA_ARGS__, 0)
#define CHECK_N(x, n, ...) n

// Void type detection
#define VOID_TYPE_void ()
#define VOID_TYPE_noreturn ()
#define VOID_PROBE(type) VOID_PROBE_PROXY(VOID_TYPE_##type)
#define VOID_PROBE_PROXY(...) VOID_PROBE_PRIMITIVE(__VA_ARGS__)
#define VOID_PROBE_PRIMITIVE(x) VOID_PROBE_COMBINE_ x
#define VOID_PROBE_COMBINE_(...) PROBE(~)
#define IS_VOID(type) CHECK(VOID_PROBE(type))

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
#define GLOB_PROBE(name) GLOB_PROBE_PROXY(GLOB_NAME_##name)
#define GLOB_PROBE_PROXY(...) GLOB_PROBE_PRIMITIVE(__VA_ARGS__)
#define GLOB_PROBE_PRIMITIVE(x) GLOB_PROBE_COMBINE_ x
#define GLOB_PROBE_COMBINE_(...) PROBE(~)
#define IS_GLOB(name) 0 // CHECK(GLOB_PROBE(name))

// Arguments
#define GEN_ADAPTER_ARGS                                                       \
    size_t a1, size_t a2, size_t a3, size_t a4, size_t a5, size_t a6,          \
        size_t a7, size_t a8, size_t a9, size_t a10, size_t a11, size_t a12

#define GEN_ADAPTER_ARGS_ARRAY                                                 \
    int current_arg = 0;                                                       \
    size_t arg_arr[] = {a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12};

#define SELECT_NEXT_PARTIAL_ARG() (arg_arr[current_arg++])

#if UINTPTR_MAX == UINT32_MAX
#define SELECT_NEXT_FULL_ARG()                                                 \
    (SELECT_NEXT_PARTIAL_ARG() | ((uint64_t)(SELECT_NEXT_PARTIAL_ARG()) << 32))

#define SELECT_NEXT_ARG(type)                                                  \
    ((sizeof(type) <= sizeof(uint32_t)) ? ((type)SELECT_NEXT_PARTIAL_ARG())    \
                                        : ((type)SELECT_NEXT_FULL_ARG()))
#else
#define SELECT_NEXT_ARG(type) ((type)SELECT_NEXT_PARTIAL_ARG())
#endif

#define SELECT_ARG(type, index) SELECT_NEXT_ARG(type)

// Adapter definition
#define GEN_ADAPTER_0_VOID(name)                                               \
    HELPER(name)();                                                            \
    return 0;
#define GEN_ADAPTER_0_NONVOID(name) return HELPER(name)();
#define GEN_ADAPTER_0_DEFINE(name, ret)                                        \
    uint64_t glue(adapter_helper_, name)(GEN_ADAPTER_ARGS)                     \
    {                                                                          \
        IIF(IS_VOID(ret))(GEN_ADAPTER_0_VOID(name),                            \
                          GEN_ADAPTER_0_NONVOID(name))                         \
    }

#define GEN_ADAPTER_1_VOID(name, t1)                                           \
    HELPER(name)(SELECT_ARG(dh_ctype(t1), A1));                                \
    return 0;
#define GEN_ADAPTER_1_NONVOID(name, t1)                                        \
    return HELPER(name)(SELECT_ARG(dh_ctype(t1), A1));
#define GEN_ADAPTER_1_DEFINE(name, ret, t1)                                    \
    uint64_t glue(adapter_helper_, name)(GEN_ADAPTER_ARGS)                     \
    {                                                                          \
        GEN_ADAPTER_ARGS_ARRAY                                                 \
        IIF(IS_VOID(ret))(GEN_ADAPTER_1_VOID(name, t1),                        \
                          GEN_ADAPTER_1_NONVOID(name, t1))                     \
    }

#define GEN_ADAPTER_2_VOID(name, t1, t2)                                       \
    HELPER(name)(SELECT_ARG(dh_ctype(t1), A1), SELECT_ARG(dh_ctype(t2), A2));  \
    return 0;
#define GEN_ADAPTER_2_NONVOID(name, t1, t2)                                    \
    return HELPER(name)(SELECT_ARG(dh_ctype(t1), A1),                          \
                        SELECT_ARG(dh_ctype(t2), A2));
#define GEN_ADAPTER_2_DEFINE(name, ret, t1, t2)                                \
    uint64_t glue(adapter_helper_, name)(GEN_ADAPTER_ARGS)                     \
    {                                                                          \
        GEN_ADAPTER_ARGS_ARRAY                                                 \
        IIF(IS_VOID(ret))(GEN_ADAPTER_2_VOID(name, t1, t2),                    \
                          GEN_ADAPTER_2_NONVOID(name, t1, t2))                 \
    }

#define GEN_ADAPTER_3_VOID(name, t1, t2, t3)                                   \
    HELPER(name)(SELECT_ARG(dh_ctype(t1), A1), SELECT_ARG(dh_ctype(t2), A2),   \
                 SELECT_ARG(dh_ctype(t3), A3));                                \
    return 0;
#define GEN_ADAPTER_3_NONVOID(name, t1, t2, t3)                                \
    return HELPER(name)(SELECT_ARG(dh_ctype(t1), A1),                          \
                        SELECT_ARG(dh_ctype(t2), A2),                          \
                        SELECT_ARG(dh_ctype(t3), A3));
#define GEN_ADAPTER_3_DEFINE(name, ret, t1, t2, t3)                            \
    uint64_t glue(adapter_helper_, name)(GEN_ADAPTER_ARGS)                     \
    {                                                                          \
        GEN_ADAPTER_ARGS_ARRAY                                                 \
        IIF(IS_VOID(ret))(GEN_ADAPTER_3_VOID(name, t1, t2, t3),                \
                          GEN_ADAPTER_3_NONVOID(name, t1, t2, t3))             \
    }

#define GEN_ADAPTER_4_VOID(name, t1, t2, t3, t4)                               \
    HELPER(name)(SELECT_ARG(dh_ctype(t1), A1), SELECT_ARG(dh_ctype(t2), A2),   \
                 SELECT_ARG(dh_ctype(t3), A3), SELECT_ARG(dh_ctype(t4), A4));  \
    return 0;
#define GEN_ADAPTER_4_NONVOID(name, t1, t2, t3, t4)                            \
    return HELPER(name)(                                                       \
        SELECT_ARG(dh_ctype(t1), A1), SELECT_ARG(dh_ctype(t2), A2),            \
        SELECT_ARG(dh_ctype(t3), A3), SELECT_ARG(dh_ctype(t4), A4));
#define GEN_ADAPTER_4_DEFINE(name, ret, t1, t2, t3, t4)                        \
    uint64_t glue(adapter_helper_, name)(GEN_ADAPTER_ARGS)                     \
    {                                                                          \
        GEN_ADAPTER_ARGS_ARRAY                                                 \
        IIF(IS_VOID(ret))(GEN_ADAPTER_4_VOID(name, t1, t2, t3, t4),            \
                          GEN_ADAPTER_4_NONVOID(name, t1, t2, t3, t4))         \
    }

#define GEN_ADAPTER_5_VOID(name, t1, t2, t3, t4, t5)                           \
    HELPER(name)(SELECT_ARG(dh_ctype(t1), A1), SELECT_ARG(dh_ctype(t2), A2),   \
                 SELECT_ARG(dh_ctype(t3), A3), SELECT_ARG(dh_ctype(t4), A4),   \
                 SELECT_ARG(dh_ctype(t5), A5));                                \
    return 0;
#define GEN_ADAPTER_5_NONVOID(name, t1, t2, t3, t4, t5)                        \
    return HELPER(name)(                                                       \
        SELECT_ARG(dh_ctype(t1), A1), SELECT_ARG(dh_ctype(t2), A2),            \
        SELECT_ARG(dh_ctype(t3), A3), SELECT_ARG(dh_ctype(t4), A4),            \
        SELECT_ARG(dh_ctype(t5), A5));
#define GEN_ADAPTER_5_DEFINE(name, ret, t1, t2, t3, t4, t5)                    \
    uint64_t glue(adapter_helper_, name)(GEN_ADAPTER_ARGS)                     \
    {                                                                          \
        GEN_ADAPTER_ARGS_ARRAY                                                 \
        IIF(IS_VOID(ret))(GEN_ADAPTER_5_VOID(name, t1, t2, t3, t4, t5),        \
                          GEN_ADAPTER_5_NONVOID(name, t1, t2, t3, t4, t5))     \
    }

#define GEN_ADAPTER_6_VOID(name, t1, t2, t3, t4, t5, t6)                       \
    HELPER(name)(SELECT_ARG(dh_ctype(t1), A1), SELECT_ARG(dh_ctype(t2), A2),   \
                 SELECT_ARG(dh_ctype(t3), A3), SELECT_ARG(dh_ctype(t4), A4),   \
                 SELECT_ARG(dh_ctype(t5), A5), SELECT_ARG(dh_ctype(t6), A6));  \
    return 0;
#define GEN_ADAPTER_6_NONVOID(name, t1, t2, t3, t4, t5, t6)                    \
    return HELPER(name)(                                                       \
        SELECT_ARG(dh_ctype(t1), A1), SELECT_ARG(dh_ctype(t2), A2),            \
        SELECT_ARG(dh_ctype(t3), A3), SELECT_ARG(dh_ctype(t4), A4),            \
        SELECT_ARG(dh_ctype(t5), A5), SELECT_ARG(dh_ctype(t6), A6));
#define GEN_ADAPTER_6_DEFINE(name, ret, t1, t2, t3, t4, t5, t6)                \
    uint64_t glue(adapter_helper_, name)(GEN_ADAPTER_ARGS)                     \
    {                                                                          \
        GEN_ADAPTER_ARGS_ARRAY                                                 \
        IIF(IS_VOID(ret))(GEN_ADAPTER_6_VOID(name, t1, t2, t3, t4, t5, t6),    \
                          GEN_ADAPTER_6_NONVOID(name, t1, t2, t3, t4, t5, t6)) \
    }

#define GEN_ADAPTER_7_VOID(name, t1, t2, t3, t4, t5, t6, t7)                   \
    HELPER(name)(SELECT_ARG(dh_ctype(t1), A1), SELECT_ARG(dh_ctype(t2), A2),   \
                 SELECT_ARG(dh_ctype(t3), A3), SELECT_ARG(dh_ctype(t4), A4),   \
                 SELECT_ARG(dh_ctype(t5), A5), SELECT_ARG(dh_ctype(t6), A6),   \
                 SELECT_ARG(dh_ctype(t7), A7));                                \
    return 0;
#define GEN_ADAPTER_7_NONVOID(name, t1, t2, t3, t4, t5, t6, t7)                \
    return HELPER(name)(                                                       \
        SELECT_ARG(dh_ctype(t1), A1), SELECT_ARG(dh_ctype(t2), A2),            \
        SELECT_ARG(dh_ctype(t3), A3), SELECT_ARG(dh_ctype(t4), A4),            \
        SELECT_ARG(dh_ctype(t5), A5), SELECT_ARG(dh_ctype(t6), A6),            \
        SELECT_ARG(dh_ctype(t7), A7));
#define GEN_ADAPTER_7_DEFINE(name, ret, t1, t2, t3, t4, t5, t6, t7)            \
    uint64_t glue(adapter_helper_, name)(GEN_ADAPTER_ARGS)                     \
    {                                                                          \
        GEN_ADAPTER_ARGS_ARRAY                                                 \
        IIF(IS_VOID(ret))(                                                     \
            GEN_ADAPTER_7_VOID(name, t1, t2, t3, t4, t5, t6, t7),              \
            GEN_ADAPTER_7_NONVOID(name, t1, t2, t3, t4, t5, t6, t7))           \
    }

#define GEN_ADAPTER_BLANK
#ifdef GEN_ADAPTER_DEFINE
#define GEN_ADAPTER_0(name, ret)                                               \
    IIF(IS_GLOB(name))(GEN_ADAPTER_BLANK, GEN_ADAPTER_0_DEFINE(name, ret))
#define GEN_ADAPTER_1(name, ret, t1)                                           \
    IIF(IS_GLOB(name))(GEN_ADAPTER_BLANK, GEN_ADAPTER_1_DEFINE(name, ret, t1))
#define GEN_ADAPTER_2(name, ret, t1, t2)                                       \
    IIF(IS_GLOB(name))(GEN_ADAPTER_BLANK,                                      \
                       GEN_ADAPTER_2_DEFINE(name, ret, t1, t2))
#define GEN_ADAPTER_3(name, ret, t1, t2, t3)                                   \
    IIF(IS_GLOB(name))(GEN_ADAPTER_BLANK,                                      \
                       GEN_ADAPTER_3_DEFINE(name, ret, t1, t2, t3))
#define GEN_ADAPTER_4(name, ret, t1, t2, t3, t4)                               \
    IIF(IS_GLOB(name))(GEN_ADAPTER_BLANK,                                      \
                       GEN_ADAPTER_4_DEFINE(name, ret, t1, t2, t3, t4))
#define GEN_ADAPTER_5(name, ret, t1, t2, t3, t4, t5)                           \
    IIF(IS_GLOB(name))(GEN_ADAPTER_BLANK,                                      \
                       GEN_ADAPTER_5_DEFINE(name, ret, t1, t2, t3, t4, t5))
#define GEN_ADAPTER_6(name, ret, t1, t2, t3, t4, t5, t6)                       \
    IIF(IS_GLOB(name))(                                                        \
        GEN_ADAPTER_BLANK,                                                     \
        GEN_ADAPTER_6_DEFINE(name, ret, t1, t2, t3, t4, t5, t6))
#define GEN_ADAPTER_7(name, ret, t1, t2, t3, t4, t5, t6, t7)                   \
    IIF(IS_GLOB(name))(                                                        \
        GEN_ADAPTER_BLANK,                                                     \
        GEN_ADAPTER_7_DEFINE(name, ret, t1, t2, t3, t4, t5, t6, t7))
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
