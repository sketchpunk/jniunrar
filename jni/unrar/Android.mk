LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := unrar

LOCAL_CFLAGS += -Wall -DNOVOLUME -DRARDLL -DRAR_NOCRYPT -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -fexceptions -frtti -fvisibility=hidden
 
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/sys

LOCAL_SRC_FILES += filestr.cpp scantree.cpp sha1.cpp sha256.cpp crc.cpp errhnd.cpp hash.cpp pathfn.cpp smallfn.cpp strfn.cpp \
	strlist.cpp timefn.cpp unicode.cpp list.cpp log.cpp match.cpp isnt.cpp extinfo.cpp errhnd.cpp headers.cpp global.cpp file.cpp \
	filcreat.cpp find.cpp getbits.cpp cmddata.cpp rs16.cpp rs.cpp consio.cpp encname.cpp options.cpp crypt.cpp consio.cpp cmddata.cpp \
	recvol.cpp volume.cpp resource.cpp secpassword.cpp qopen.cpp system.cpp rawread.cpp rdwrfn.cpp threadpool.cpp rar.cpp archive.cpp \
	arcread.cpp extract.cpp blake2s.cpp rarpch.cpp rarvm.cpp rijndael.cpp dll.cpp unpack.cpp filefn.cpp

#INCLUDE FILES, ALREADY COMPILED IN ONE OF THE FILES IN SRC.
#crypt5.cpp crypt3.cpp crypt1.cpp crypt2.cpp rarvmtbl.cpp threadmisc.cpp blake2s_sse.cpp blake2sp.cpp
#recvol3.cpp recvol5.cpp coder.cpp model.cpp suballoc.cpp unpackinline.cpp unpack50mt.cpp unpack50.cpp unpack50frag.cpp unpack15.cpp unpack20.cpp unpack30.cpp
#win32acl.cpp win32lnk.cpp win32stm.cpp uowners.cpp hardlinks.cpp arccmt.cpp ulinks.cpp

LOCAL_SHARED_LIBRARIES :=
LOCAL_STATIC_LIBRARIES :=

LOCAL_LDFLAGS := -Wl,--as-needed

include $(BUILD_STATIC_LIBRARY)