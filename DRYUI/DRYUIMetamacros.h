//
//  DRYUIMetamacros.h
//  Pods
//
//  Created by Griffin Schneider on 10/7/15.
//
//

#ifndef DRYUIMetamacros_h
#define DRYUIMetamacros_h

#import <libextobjc/metamacros.h>


////////////////////////////////////////////////////////////////////////////////////////////////////
#define dryui_metamacro_foreach_iter(INDEX, MACRO, ARG) MACRO(INDEX, ARG)

#define dryui_metamacro_foreach_even_comma(MACRO, ...) \
dryui_metamacro_foreach_even_comma_cxt(dryui_metamacro_foreach_iter, MACRO, __VA_ARGS__)

#define dryui_metamacro_foreach_even_comma_cxt(MACRO, CONTEXT, ...) \
metamacro_concat(dryui_metamacro_foreach_even_comma_cxt, metamacro_argcount(__VA_ARGS__))(MACRO, CONTEXT, __VA_ARGS__)


////////////////////////////////////////////////////////////////////////////////////////////////////
#define dryui_metamacro_foreach_even_comma_cxt0(MACRO, CONTEXT)
#define dryui_metamacro_foreach_even_comma_cxt1(MACRO, CONTEXT, _0) MACRO(0, CONTEXT, _0)

#define dryui_metamacro_foreach_even_comma_cxt2(MACRO, CONTEXT, _0, _1) \
dryui_metamacro_foreach_even_comma_cxt1(MACRO, CONTEXT, _0) \
MACRO(1, CONTEXT, _1)

#define dryui_metamacro_foreach_even_comma_cxt3(MACRO, CONTEXT, _0, _1, _2) \
dryui_metamacro_foreach_even_comma_cxt2(MACRO, CONTEXT, _0, _1) \
, \
MACRO(2, CONTEXT, _2)

#define dryui_metamacro_foreach_even_comma_cxt4(MACRO, CONTEXT, _0, _1, _2, _3) \
dryui_metamacro_foreach_even_comma_cxt3(MACRO, CONTEXT, _0, _1, _2) \
MACRO(3, CONTEXT, _3)

#define dryui_metamacro_foreach_even_comma_cxt5(MACRO, CONTEXT, _0, _1, _2, _3, _4) \
dryui_metamacro_foreach_even_comma_cxt4(MACRO, CONTEXT, _0, _1, _2, _3) \
, \
MACRO(4, CONTEXT, _4)

#define dryui_metamacro_foreach_even_comma_cxt6(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5) \
dryui_metamacro_foreach_even_comma_cxt5(MACRO, CONTEXT, _0, _1, _2, _3, _4) \
MACRO(5, CONTEXT, _5)

#define dryui_metamacro_foreach_even_comma_cxt7(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6) \
dryui_metamacro_foreach_even_comma_cxt6(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5) \
, \
MACRO(6, CONTEXT, _6)

#define dryui_metamacro_foreach_even_comma_cxt8(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7) \
dryui_metamacro_foreach_even_comma_cxt7(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6) \
MACRO(7, CONTEXT, _7)

#define dryui_metamacro_foreach_even_comma_cxt9(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8) \
dryui_metamacro_foreach_even_comma_cxt8(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7) \
, \
MACRO(8, CONTEXT, _8)

#define dryui_metamacro_foreach_even_comma_cxt10(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9) \
dryui_metamacro_foreach_even_comma_cxt9(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8) \
MACRO(9, CONTEXT, _9)

#define dryui_metamacro_foreach_even_comma_cxt11(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) \
dryui_metamacro_foreach_even_comma_cxt10(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9) \
, \
MACRO(10, CONTEXT, _10)

#define dryui_metamacro_foreach_even_comma_cxt12(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) \
dryui_metamacro_foreach_even_comma_cxt11(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) \
MACRO(11, CONTEXT, _11)

#define dryui_metamacro_foreach_even_comma_cxt13(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) \
dryui_metamacro_foreach_even_comma_cxt12(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) \
, \
MACRO(12, CONTEXT, _12)

#define dryui_metamacro_foreach_even_comma_cxt14(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) \
dryui_metamacro_foreach_even_comma_cxt13(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) \
MACRO(13, CONTEXT, _13)

#define dryui_metamacro_foreach_even_comma_cxt15(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) \
dryui_metamacro_foreach_even_comma_cxt14(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) \
, \
MACRO(14, CONTEXT, _14)

#define dryui_metamacro_foreach_even_comma_cxt16(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) \
dryui_metamacro_foreach_even_comma_cxt15(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) \
MACRO(15, CONTEXT, _15)

#define dryui_metamacro_foreach_even_comma_cxt17(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) \
dryui_metamacro_foreach_even_comma_cxt16(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) \
, \
MACRO(16, CONTEXT, _16)

#define dryui_metamacro_foreach_even_comma_cxt18(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17) \
dryui_metamacro_foreach_even_comma_cxt17(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) \
MACRO(17, CONTEXT, _17)

#define dryui_metamacro_foreach_even_comma_cxt19(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18) \
dryui_metamacro_foreach_even_comma_cxt18(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17) \
, \
MACRO(18, CONTEXT, _18)

#define dryui_metamacro_foreach_even_comma_cxt20(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19) \
dryui_metamacro_foreach_even_comma_cxt19(MACRO, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18) \
MACRO(19, CONTEXT, _19)

#endif