#include <com_sketchpunk_jniunrar_unrar.h>
#include <cstddef> //Needed else get an error about NULL Being undefined.
//#include <stdlib.h>
//#include <cstdlib>
#include <vector>
#include <string>
#include <stdio.h> //Needed for sprintf
#include <android/log.h>
#include "version.hpp"
#include "rar.hpp"
#include "array.hpp"
#include "dll.hpp" //Structs are in this file

//Define shortcuts to android's c version of System.out.println
#define  LOG_TAG "jnirar"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

//#ifdef __cplusplus
//extern "C" {
//#endif

jint JNI_OnLoad(JavaVM* vm, void* reserved){
        JNIEnv* env;
        if (vm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) return -1;
        return JNI_VERSION_1_6;
}//func
void JNI_OnUnload(JavaVM *, void *){}


JNIEXPORT jstring JNICALL Java_com_sketchpunk_jniunrar_unrar_getVersion(JNIEnv * env,jclass cls){
	char rtn[12];
	sprintf(rtn,"%d.%d.%d",RARVER_MAJOR,RARVER_MINOR,RARVER_BETA);
	return env->NewStringUTF(rtn);
}//func


JNIEXPORT jobjectArray JNICALL Java_com_sketchpunk_jniunrar_unrar_getListing(JNIEnv * env,jclass cls,jstring jFilePath){
	jobjectArray rtn = NULL;
	//TODO, use string.find to filter out entries. Save time filtering at this level then to do it after the fact in java.

	//-----------------------------------
	//Convert values to something useable in C
	const char *filePath = env->GetStringUTFChars(jFilePath,JNI_FALSE);

	//Setup Rar struct
    RAROpenArchiveData stRar;
    memset(&stRar,0,sizeof(RAROpenArchiveData));
	stRar.ArcName = (char*)filePath;
	stRar.OpenMode = RAR_OM_LIST;

	//-----------------------------------
	//open file with data.
	HANDLE rHandle = RAROpenArchive(&stRar);
	if(rHandle && !stRar.OpenResult){
		//Setup header struct and other variables
		int result;
		std::vector<std::string> aryStr;

		RARHeaderData header;
		memset(&header,0,sizeof(RARHeaderData));
		
		//...............................
		//loop through entry items
		while (RARReadHeader(rHandle, &header) == 0){
			//only save the files, not directories
			if ((header.Flags & LHD_DIRECTORY) != LHD_DIRECTORY) aryStr.push_back(header.FileName);

			result = RARProcessFile(rHandle, RAR_SKIP, NULL, NULL); //skip entry content
			if(result) LOGE("Unable to process %s, error: %d", header.FileName, result); //error skipping content
		}//while

		RARCloseArchive(rHandle); //Close file, done reading from it.

		//...............................
		//Save vector back to a basic string array that java can understand.
		size_t aryLen = aryStr.size(); //LOGI("Count %d",aryStr.size());

		if(aryLen != 0){
			rtn = (jobjectArray)env->NewObjectArray(aryLen,env->FindClass("java/lang/String"),NULL);

			for(std::vector<std::string>::size_type i = 0; i != aryLen; i++){
				env->SetObjectArrayElement(rtn,i,env->NewStringUTF(aryStr[i].c_str()));
			}//for
		}//if
	}else{
		LOGE("unrar_getListing Error %i",stRar.OpenResult);
	}//if

	//-----------------------------------
	//Cleanup
	env->ReleaseStringUTFChars(jFilePath,filePath);

	return rtn;
}//func


JNIEXPORT jbyteArray JNICALL Java_com_sketchpunk_jniunrar_unrar_getBytes(JNIEnv * env, jclass cls,jstring jFilePath,jstring jItemName){
	jbyteArray rtn = NULL;

	return rtn;
}//func


//#ifdef __cplusplus
//}
//#endif