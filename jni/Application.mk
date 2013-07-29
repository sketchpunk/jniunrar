#armeabi armeabi-v7a x86 mips
APP_ABI := armeabi
APP_PLATFORM := android-4

#Some how these three options help make two cpp files in unrar compile.
#Has gnustl has C++ Exceptions, RTTI and Stand Library
APP_CPPFLAGS := -frtti
APP_OPTIM := release
#APP_STL := stlport_static
APP_STL := gnustl_static
LOCAL_ARM_MODE := thumb

#NDK Does not automaticly compile static libraries. The following line will force a build
#APP_MODULES := unrar