//  File implement/oglplus/enums/named_string_type_def.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/named_string_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2017 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
#ifdef OGLPLUS_LIST_NEEDS_COMMA
# undef OGLPLUS_LIST_NEEDS_COMMA
#endif

#if defined GL_SHADER_INCLUDE_ARB
# ifdef OGLPLUS_LIST_NEEDS_COMMA
   OGLPLUS_ENUM_CLASS_COMMA
# endif
# if defined ShaderInclude
#  pragma push_macro("ShaderInclude")
#  undef ShaderInclude
   OGLPLUS_ENUM_CLASS_VALUE(ShaderInclude, GL_SHADER_INCLUDE_ARB)
#  pragma pop_macro("ShaderInclude")
# else
   OGLPLUS_ENUM_CLASS_VALUE(ShaderInclude, GL_SHADER_INCLUDE_ARB)
# endif
# ifndef OGLPLUS_LIST_NEEDS_COMMA
#  define OGLPLUS_LIST_NEEDS_COMMA 1
# endif
#endif
#ifdef OGLPLUS_LIST_NEEDS_COMMA
# undef OGLPLUS_LIST_NEEDS_COMMA
#endif

