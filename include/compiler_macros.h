#ifndef CHESS_COMPILER_MACROS_H
#define CHESS_COMPILER_MACROS_H

// Compiler intrinsics should optimize by default
// Loop unrolling hint to silence `altera-unroll-loops`
#if defined(__clang__)
#define UNROLL_LOOP _Pragma("unroll")
#define UNROLL_PARTIAL _Pragma("unroll 4")
#else
#define UNROLL_LOOP
#define UNROLL_PARTIAL
#endif

#endif // CHESS_COMPILER_MACROS_H