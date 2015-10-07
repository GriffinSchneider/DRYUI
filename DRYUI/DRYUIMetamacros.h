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

// This file just defines a macro 'dryui_metamacro_foreach_comma' which has the same behavior that
// calling libextobjc'smetamacro_foreach would have if you could pass a comma (,) as SEP.


////////////////////////////////////////////////////////////////////////////////////////////////////
#define dryui_metamacro_foreach_comma(MACRO, ...) \
dryui_metamacro_foreach_cxt(dryui_metamacro_foreach_iter, MACRO, __VA_ARGS__)

#define dryui_metamacro_foreach_cxt(MACRO, CONTEXT, ...) \
metamacro_concat(dryui_metamacro_foreach_cxt, metamacro_argcount(__VA_ARGS__))(MACRO, CONTEXT, __VA_ARGS__)

#define dryui_metamacro_foreach_iter(INDEX, MACRO, ARG) MACRO(INDEX, ARG)

////////////////////////////////////////////////////////////////////////////////////////////////////
#define dryui_metamacro_for_cxt0(MACRO, CONTEXT)
#define dryui_metamacro_for_cxt1(MACRO, CONTEXT) MACRO(0, CONTEXT)

#define dryui_metamacro_for_cxt2(MACRO, CONTEXT) \
dryui_metamacro_for_cxt1(MACRO, CONTEXT) \
, \
MACRO(1, CONTEXT)

#define dryui_metamacro_for_cxt3(MACRO, CONTEXT) \
dryui_metamacro_for_cxt2(MACRO, CONTEXT) \
, \
MACRO(2, CONTEXT)

#define dryui_metamacro_for_cxt4(MACRO, CONTEXT) \
dryui_metamacro_for_cxt3(MACRO, CONTEXT) \
, \
MACRO(3, CONTEXT)

#define dryui_metamacro_for_cxt5(MACRO, CONTEXT) \
dryui_metamacro_for_cxt4(MACRO, CONTEXT) \
, \
MACRO(4, CONTEXT)

#define dryui_metamacro_for_cxt6(MACRO, CONTEXT) \
dryui_metamacro_for_cxt5(MACRO, CONTEXT) \
, \
MACRO(5, CONTEXT)

#define dryui_metamacro_for_cxt7(MACRO, CONTEXT) \
dryui_metamacro_for_cxt6(MACRO, CONTEXT) \
, \
MACRO(6, CONTEXT)

#define dryui_metamacro_for_cxt8(MACRO, CONTEXT) \
dryui_metamacro_for_cxt7(MACRO, CONTEXT) \
, \
MACRO(7, CONTEXT)

#define dryui_metamacro_for_cxt9(MACRO, CONTEXT) \
dryui_metamacro_for_cxt8(MACRO, CONTEXT) \
, \
MACRO(8, CONTEXT)

#define dryui_metamacro_for_cxt10(MACRO, CONTEXT) \
dryui_metamacro_for_cxt9(MACRO, CONTEXT) \
, \
MACRO(9, CONTEXT)

#define dryui_metamacro_for_cxt11(MACRO, CONTEXT) \
dryui_metamacro_for_cxt10(MACRO, CONTEXT) \
, \
MACRO(10, CONTEXT)

#define dryui_metamacro_for_cxt12(MACRO, CONTEXT) \
dryui_metamacro_for_cxt11(MACRO, CONTEXT) \
, \
MACRO(11, CONTEXT)

#define dryui_metamacro_for_cxt13(MACRO, CONTEXT) \
dryui_metamacro_for_cxt12(MACRO, CONTEXT) \
, \
MACRO(12, CONTEXT)

#define dryui_metamacro_for_cxt14(MACRO, CONTEXT) \
dryui_metamacro_for_cxt13(MACRO, CONTEXT) \
, \
MACRO(13, CONTEXT)

#define dryui_metamacro_for_cxt15(MACRO, CONTEXT) \
dryui_metamacro_for_cxt14(MACRO, CONTEXT) \
, \
MACRO(14, CONTEXT)

#define dryui_metamacro_for_cxt16(MACRO, CONTEXT) \
dryui_metamacro_for_cxt15(MACRO, CONTEXT) \
, \
MACRO(15, CONTEXT)

#define dryui_metamacro_for_cxt17(MACRO, CONTEXT) \
dryui_metamacro_for_cxt16(MACRO, CONTEXT) \
, \
MACRO(16, CONTEXT)

#define dryui_metamacro_for_cxt18(MACRO, CONTEXT) \
dryui_metamacro_for_cxt17(MACRO, CONTEXT) \
, \
MACRO(17, CONTEXT)

#define dryui_metamacro_for_cxt19(MACRO, CONTEXT) \
dryui_metamacro_for_cxt18(MACRO, CONTEXT) \
, \
MACRO(18, CONTEXT)

#define dryui_metamacro_for_cxt20(MACRO, CONTEXT) \
dryui_metamacro_for_cxt19(MACRO, CONTEXT) \
, \
MACRO(19, CONTEXT)

#endif
