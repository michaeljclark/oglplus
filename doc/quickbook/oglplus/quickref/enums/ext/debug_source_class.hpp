//  File doc/quickbook/oglplus/quickref/enums/ext/debug_source_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/debug_source.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2017 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_debug_source_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__DebugSource> class Transform>
class __EnumToClass<Base, __DebugSource, Transform> /*<
Specialization of __EnumToClass for the __DebugSource enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<DebugSource::API>
		API;
	Transform<DebugSource::WindowSystem>
		WindowSystem;
	Transform<DebugSource::ShaderCompiler>
		ShaderCompiler;
	Transform<DebugSource::ThirdParty>
		ThirdParty;
	Transform<DebugSource::Application>
		Application;
	Transform<DebugSource::Other>
		Other;
	Transform<DebugSource::DontCare>
		DontCare;
};

} // namespace enums
#endif
//]

