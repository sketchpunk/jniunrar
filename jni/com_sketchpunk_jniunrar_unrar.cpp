#include <com_sketchpunk_jniunrar_unrar.h>
#include <cstddef> //Needed else get an error about NULL Being undefined.
//#include <stdlib.h>
//#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
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

/*===================================================================================================*/
//Declare some private functions
int EndsWith(const char *str, const char *suffix);
std::string newLowerStr(char* cstr);
std::vector<std::string> jStringToVector(JNIEnv *env, jstring jstr);


/*===================================================================================================*/
//Get some weird warning in the console that JNI_Onload is not found, added it just incase.
jint JNI_OnLoad(JavaVM* vm, void* reserved){
		JNIEnv* env;
		if (vm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) return -1;
		return JNI_VERSION_1_6;
}//func
void JNI_OnUnload(JavaVM *, void *){}


/*===================================================================================================*/
JNIEXPORT jstring JNICALL Java_com_sketchpunk_jniunrar_unrar_getVersion(JNIEnv * env,jclass cls){
	char rtn[12];
	sprintf(rtn,"%d.%d.%d",RARVER_MAJOR,RARVER_MINOR,RARVER_BETA);
	return env->NewStringUTF(rtn);
}//func


/*===================================================================================================*/
//TODO, use string.find to filter out entries. Save time filtering at this level then to do it after the fact in java.
//http://www.cplusplus.com/reference/string/string/find_last_of/
JNIEXPORT jobjectArray JNICALL Java_com_sketchpunk_jniunrar_unrar_getEntries(JNIEnv * env,jclass cls,jstring jArcPath,jstring jExtList){
	jobjectArray rtn = NULL;
	
	//std::string extList;
	std::vector<std::string> extList;
	int extListLen = 0;
	
	if(jExtList != NULL){
		extList = jStringToVector(env,jExtList);
		extListLen = extList.size();

		for(int i=0; i < extListLen; i++){
			LOGI(extList[i].c_str());
		}//for

		//extList = jstring2string(env,jExtList);
		//filterEnabled = 1;
		//LOGI(extList.c_str());

		//splitToVector(extList.c_str());		
	}//if

	//-----------------------------------
	//Convert values to something useable in C
	const char *arcPath = env->GetStringUTFChars(jArcPath,JNI_FALSE);

	//Setup Rar struct
	RAROpenArchiveData stRar;
	memset(&stRar,0,sizeof(RAROpenArchiveData));
	stRar.ArcName = (char*)arcPath;
	stRar.OpenMode = RAR_OM_LIST;

	//-----------------------------------
	//open file with data.
	HANDLE rHandle = RAROpenArchive(&stRar);
	if(rHandle && !stRar.OpenResult){
		//Setup header struct and other variables
		int result,doAppend;
		std::vector<std::string> aryStr;

		RARHeaderData header;
		memset(&header,0,sizeof(RARHeaderData));
		
		//...............................
		//loop through entry items
		while (RARReadHeader(rHandle, &header) == 0){
			//only save the files, not directories
			if((header.Flags & LHD_DIRECTORY) != LHD_DIRECTORY){
				
				//if extension filter exists do some validation on the filename
				doAppend = 0;
				if(extListLen){
					for(int i=0; i < extListLen; i++){
						if(EndsWith(header.FileName,extList[i].c_str())){ doAppend = 1; break; }
					}//for
				}//if

				//Add to the list if has the right extensions
				if(doAppend || extListLen == 0) aryStr.push_back(header.FileName);
			}//if

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
	env->ReleaseStringUTFChars(jArcPath,arcPath);

	return rtn;
}//func


/*=====================================================================================================================
NOTES
Instead of loading the whole file into memory in the form of a byte array. I would perfer to be able to open the archive,
save the handle back to java. On the java end create a custom implementation of (or extend)  InputStream. By using the handle, keep
reading chucks of bytes at a time. The input stream would have to be able to cache some of the bytes since decompressing creates 
more bytes then what would be requested by the byte array buffer. It's not like streaming from a file where you know exactly which byte 
you're on. Skipping might not be implemented because of this. Even rolling back would not be easily possible. So the stream would be one
way direction from beginning to end. This would be enough to have Bitmap on the java end to read the header, then stream the content out of the rar
while rending the image. In theory this would improve on memory useage. Thinking that loading a whole file in memory, then having the bitmap object
copy that data would result in using up twice the memory at one given time. By streaming we can keep the memory usage lower where only
the bitmap object would contain the byte information in its entirely.

dll.cpp
	- Line 264 - ProcessFile
		this function starts the Extraction Process.
	- Line 325 - Data->Extract.ExtractCurrentFile(&Data->Cmd,Data->Arc,Data->HeaderSize,Repeat);
		this seems to be where the real extraction is beginning. I'm having problems following the code in ProcessFile
		Idealy, I would add a new function to CmdExtract that instead of extract current file would only extract X amount of 
		bytes from some starting position.

extract.cpp
	- Line 213 - CmdExtract::ExtractCurrentFile
		Pretty complicated function, but this is the function that needs to be repurposed to create a function that
		steps through the requested entry's data and stream it back out.

		Can probably slim down the function by removing all the multi-volume stuff. For comics, all the data really exists
		within a single file. So hopefully that can help simplify the Extract function some.

http://grepcode.com/file/repository.grepcode.com/java/root/jdk/openjdk/6-b14/java/io/InputStream.java

----------
http://sourceforge.net/projects/jniunrar/files/jniunrar/

Pretty good project in how to do some really complex things with JNI. Dont understand how, but looking through the java 
and cpp files shows that C++ can call a method from a java object. There also seems to be a way to hold the archive state since much
of the functionality is broken out, like there's an open, process and close functions.
=====================================================================================================================*/

JNIEXPORT jbyteArray JNICALL Java_com_sketchpunk_jniunrar_unrar_extractEntryToArray(JNIEnv * env, jclass cls,jstring jArcPath,jstring jEntryName){
	jbyteArray rtn = NULL;

	return rtn;
}//func


/*============================================================
Helper functions to manage string duties.
============================================================*/
//Used for the delimited list of extensions. Just parse it into an array
std::vector<std::string> jStringToVector(JNIEnv *env, jstring jstr){
	std::vector<std::string> rtn;
	const char *str;
	str = env->GetStringUTFChars(jstr,JNI_FALSE);
	if (str == NULL) return rtn; // OutOfMemoryError
	
	//..................................
	//Split string to a string vector
    char sep[] = ",";
    char* tok;

    tok = strtok((char*)str,sep);
    while(tok){
        rtn.push_back(newLowerStr(tok));
        tok = strtok(0,sep);
    }//while

    //..................................
    //Cleanup
	env->ReleaseStringUTFChars(jstr,str);
	return rtn;
}//func

std::string newLowerStr(char* cstr){
	std::string rtn(cstr);
	std::transform(rtn.begin(), rtn.end(), rtn.begin(), ::tolower);
	return rtn;
}//func

//Tests the end of a string.
int EndsWith(const char *str, const char *suffix){
    if (!str || !suffix) return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);

    //..................
    //ext will be lowercase, so make the main string lowercase as well to make the compare work.
	char lStr[lenstr];
	for(int i=0; i < lenstr; i++) lStr[i] = std::tolower(str[i]);

	//..................
    if (lensuffix > lenstr) return 0;
    return strncmp(lStr + lenstr-lensuffix,suffix,lensuffix) == 0;
}//func

//jstring str2jString(JNIEnv *env,std::string str){
//	return env->NewStringUTF(str.c_str());
//}//func

//function that converts jString to std::string
/*
std::string jStr2Str(JNIEnv *env, jstring jstr){
	const char *str;
	str = env->GetStringUTFChars(jstr, NULL); //env->GetStringUTFChars(jstr,JNI_FALSE);
	if (str == NULL) return ""; // OutOfMemoryError
	
	std::string rtn(str);
	env->ReleaseStringUTFChars(jstr,str);
	return rtn;
}//func
*/