//  File doc/quickbook/oglplus/quickref/enums/query_target.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/query_target.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2014 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_query_target
namespace oglplus {

enum class QueryTarget : GLenum
{
	TimeElapsed                        = GL_TIME_ELAPSED,
	Timestamp                          = GL_TIMESTAMP,
	SamplesPassed                      = GL_SAMPLES_PASSED,
	AnySamplesPassed                   = GL_ANY_SAMPLES_PASSED,
	PrimitivesGenerated                = GL_PRIMITIVES_GENERATED,
	TransformFeedbackPrimitivesWritten = GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN
};

template <>
__Range<QueryTarget> __EnumValueRange<QueryTarget>(void);

__StrCRef __EnumValueName(QueryTarget);

} // namespace oglplus
//]