#ifndef CHESS_COMPILER_MACROS_H
#define CHESS_COMPILER_MACROS_H

// Compiler intrinsics should optimize by default
// Loop unrolling hint to silence clang-tidy warning
#if defined(__clang__)
#define UNROLL_LOOP _Pragma("unroll")
#else
#define UNROLL_LOOP /* _Pragma("GCC ivdep") */
#endif

#endif // CHESS_COMPILER_MACROS_H