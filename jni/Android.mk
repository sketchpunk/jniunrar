TOP_LOCAL_PATH := $(call my-dir)
include $(call all-subdir-makefiles)
LOCAL_PATH := $(TOP_LOCAL_PATH)
include $(CLEAR_VARS)

LOCAL_MODULE := jniunrar
#LOCAL_CFLAGS += -Wall -fvisibility=hidden
#LOCAL_CFLAGS += -Wall -DNOVOLUME -DRARDLL -DRAR_NOCRYPT -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -fexceptions -frtti -fvisibility=hidden

LOCAL_C_INCLUDES := $(LOCAL_PATH)/unrar
LOCAL_SRC_FILES := com_sketchpunk_jniunrar_unrar.cpp
LOCAL_STATIC_LIBRARIES := unrar

# Optional compiler flags.
#LOCAL_LDLIBS   = -lz -lm
#LOCAL_CFLAGS   = -Wall -pedantic -std=c99 -g

#LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
#-llog lets you use android log functions
LOCAL_LDLIBS := -llog
#LOCAL_LDFLAGS := -Wl,--as-needed 

include $(BUILD_SHARED_LIBRARY)