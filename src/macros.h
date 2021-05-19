/*  Copyright 2021-2022 MarcosHCK
 *  This file is part of lglm.
 *
 *  lglm is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  lglm is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with lglm.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef LGLM_MACROS_H_INCLUDED
#define LGLM_MACROS_H_INCLUDED
#include <stddef.h>

/*
 * Some macros I use to trust they are defined
 * in ANSI C, but they aren't
 *
 */
#ifndef NULL
#  ifdef __cplusplus
#  define NULL        (0L)
#  else /* !__cplusplus */
#  define NULL        ((void*) 0)
#  endif /* !__cplusplus */
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE  (!FALSE)
#endif

/*
 * _LIKELY & _UNLIKELY (and _GNUC_EXTENSION)
 * copied from GLIB source (G_LIKELY & G_UNLIKELY macro (and G_GNUC_EXTENSION))
 * thanks you for this amazing code!
 *
 */
#if     __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 8)
#define _GNUC_EXTENSION __extension__
#else
#define _GNUC_EXTENSION
#endif

#if defined(__GNUC__) && (__GNUC__ > 2) && defined(__OPTIMIZE__)
#define __BOOLEAN_EXPR(expr)                    \
 _GNUC_EXTENSION ({                             \
   int _g_boolean_var_;                         \
   if (expr)                                    \
      _g_boolean_var_ = 1;                      \
   else                                         \
      _g_boolean_var_ = 0;                      \
   _g_boolean_var_;                             \
})
#define _LIKELY(expr) (__builtin_expect (__BOOLEAN_EXPR(expr), 1))
#define _UNLIKELY(expr) (__builtin_expect (__BOOLEAN_EXPR(expr), 0))
#else
#define _LIKELY(expr) (expr)
#define _UNLIKELY(expr) (expr)
#endif

/*
 * _N_ELEMENTS
 * copied from GLIB source (G_N_ELEMENTS macro)
 * thanks you again for this amazing code!
 *
 */
#define _N_ELEMENTS(arr) (sizeof (arr) / sizeof ((arr)[0]))

/*
 * _STATIC_ASSERT
 * copied from GLIB source (G_STATIC_ASSERT macro)
 * thanks you again for this amazing code!
 * (god, I won't get tired of thank GLIB developers).
 *
 */
#define _PASTE_ARGS(identifier1,identifier2) identifier1 ## identifier2
#define _PASTE(identifier1,identifier2)      _PASTE_ARGS (identifier1, identifier2)
#if !defined(__cplusplus) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define _STATIC_ASSERT(expr) _Static_assert (expr, "Expression evaluates to false")
#elif (defined(__cplusplus) && __cplusplus >= 201103L) || \
      (defined(__cplusplus) && defined (_MSC_VER) && (_MSC_VER >= 1600)) || \
      (defined (_MSC_VER) && (_MSC_VER >= 1800))
#define _STATIC_ASSERT(expr) static_assert (expr, "Expression evaluates to false")
#else
#ifdef __COUNTER__
#define _STATIC_ASSERT(expr) typedef char _PASTE (_GStaticAssertCompileTimeAssertion_, __COUNTER__)[(expr) ? 1 : -1] G_GNUC_UNUSED
#else
#define _STATIC_ASSERT(expr) typedef char _PASTE (_GStaticAssertCompileTimeAssertion_, __LINE__)[(expr) ? 1 : -1] G_GNUC_UNUSED
#endif
#endif /* __STDC_VERSION__ */
#define _STATIC_ASSERT_EXPR(expr) ((void) sizeof (char[(expr) ? 1 : -1]))

#endif // LGLM_MACROS_H_INCLUDED
