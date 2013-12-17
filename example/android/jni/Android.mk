# Copyright 2013 Matus Chochlik. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

LOCAL_PATH:= $(call my-dir)
OGLPLUS_ROOT := /home/chochlik/devel/oglplus

include $(CLEAR_VARS)

LOCAL_MODULE := libandroid_001_triangle

LOCAL_CXXFLAGS := \
	-std=c++0x \
	-frtti \
	-fexceptions \
	-I/$(OGLPLUS_ROOT)/third_party/include \
	-I/$(OGLPLUS_ROOT)/include \
	-I$(OGLPLUS_ROOT)/implement

LOCAL_SRC_FILES := android_001_triangle.cpp

LOCAL_LDLIBS := -lGLESv3 -lEGL

include $(BUILD_SHARED_LIBRARY)
