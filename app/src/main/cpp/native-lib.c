#include "helixdown_testing_com_ndk_file_encrypt_Cryptor.h"
#include "helixdown_testing_com_ndk_file_encrypt_NDKFilePatch.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <android/log.h>
#include <unistd.h>
#include <errno.h>

#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"Derek",__VA_ARGS__)
#define  LOGI(...) __android_log_print(ANDROID_LOG_INFO,"Derek",__VA_ARGS__)

char passward[] = "ILoveDilireba";
/*
 * Class:     helixdown_testing_com_ndk_file_encrypt_Cryptor
 * Method:    Encrypt
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_helixdown_testing_com_ndk_1file_1encrypt_Cryptor_Encrypt
        (JNIEnv * env, jclass cls, jstring normal_path_jstr, jstring crypt_path_jstr){
    //jstring -> char*
    const char * normal_path = (*env)->GetStringUTFChars(env,normal_path_jstr,NULL);
    const char * crypt_path = (*env)->GetStringUTFChars(env,crypt_path_jstr,NULL);
    LOGI(" %s",normal_path);

    FILE* pNormalFile = fopen(normal_path,"rb");
    if (pNormalFile == NULL){
        LOGE("Open file failed %s",strerror(errno));
        return;
    }
    FILE* pCryptFile = fopen(crypt_path,"wb");
    if (pCryptFile == NULL) {
        LOGE("Open file failed %s",strerror(errno));
        return;

    }
    int ch;
    size_t length = strlen(passward);

    int i = 0;
    while ( (ch =fgetc(pNormalFile)) != EOF){
        fputc(ch^passward[i%length],pCryptFile);
        i++;
    }

    fclose(pCryptFile);
    fclose(pNormalFile);

    (*env)->ReleaseStringUTFChars(env,normal_path_jstr,normal_path);
    (*env)->ReleaseStringUTFChars(env,crypt_path_jstr,crypt_path);
}

/*
 * Class:     helixdown_testing_com_ndk_file_encrypt_Cryptor
 * Method:    Decrypt
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_helixdown_testing_com_ndk_1file_1encrypt_Cryptor_Decrypt
        (JNIEnv * env, jclass cls, jstring crypt_path_jstr, jstring decrypt_path_jstr){

    const char * crypt_path = (*env)->GetStringUTFChars(env,crypt_path_jstr,NULL);
    const char * decrypt_path = (*env)->GetStringUTFChars(env,decrypt_path_jstr,NULL);

    FILE* pCryptFile = fopen(crypt_path,"rb");
    if (pCryptFile == NULL){
        LOGE(" pCryptFile open file failed : %s",strerror(errno));
        return;
    }

    FILE* pDecryptFile = fopen(decrypt_path,"wb");
    if (pDecryptFile == NULL) {
        LOGE(" pDecryptFile open file failed : %s ",strerror(errno));
        return;
    }
    int ch;
    size_t length = strlen(passward);

    int i = 0;
    while ( (ch =fgetc(pCryptFile)) != EOF){
        fputc(ch^passward[i%length],pDecryptFile);
        i++;
    }

    fclose(pCryptFile);
    fclose(pDecryptFile);

    (*env)->ReleaseStringUTFChars(env,crypt_path_jstr,crypt_path);
    (*env)->ReleaseStringUTFChars(env,decrypt_path_jstr,decrypt_path);
}

long  get_file_size(const char* path){
    FILE * fp = fopen(path,"rb");
    fseek(fp,0,SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return size;
}

JNIEXPORT void JNICALL Java_helixdown_testing_com_ndk_1file_1encrypt_NDKFilePatch_Diff
        (JNIEnv * env, jclass cls, jstring path_jstr, jstring path_pattern_jstr, jint file_num){
    const  char* path = (*env)->GetStringUTFChars(env,path_jstr,NULL);
    const char* path_pattern = (*env)->GetStringUTFChars(env,path_pattern_jstr,NULL);

    char** diffes = malloc(sizeof(char*) * file_num );

    for (int i = 0; i < file_num; ++i) {
        diffes[i] = malloc(sizeof(char) * 50);
        sprintf(diffes[i],path_pattern,i + 1 );
    }

    FILE * fpr = fopen(path,"rb");
    if (fpr == NULL){
        LOGE("open %s file failed error msg : %s",path,strerror(errno));
        return;
    }

    long filesize = get_file_size(path);
    if (filesize % file_num == 0){
        long partsize = filesize / file_num;
        for (int i = 0; i < file_num; ++i) {
            FILE * fpw = fopen(diffes[i],"wb");
            if (fpw == NULL){
                LOGE("open %s file failed error msg : %s",diffes[i],strerror(errno));
                return;
            }
            for (int j = 0; j < partsize; ++j) {
                fputc(fgetc(fpr),fpw);
            }

            fclose(fpw);
        }
    }else{
        long partsize = filesize /( file_num - 1);
        for (int i = 0; i < file_num - 1 ; ++i) {
            FILE * fpw = fopen(diffes[i],"wb");
            if (fpw == NULL){
                LOGE("open %s file failed error msg : %s",diffes[i],strerror(errno));
                return;
            }
            for (int j = 0; j < partsize; ++j) {
                fputc(fgetc(fpr),fpw);
            }

            fclose(fpw);
        }

        FILE* fpw = fopen(diffes[file_num-1],"wb");
        if (fpw == NULL){
            LOGE("open %s file failed error msg : %s",diffes[file_num - 1],strerror(errno));
            return;
        }
        long lastsize = filesize % (file_num - 1);
        for (int i = 0; i < lastsize; ++i) {
            fputc(fgetc(fpr),fpw);
        }
        fclose(fpw);
    }

    fclose(fpr);

//    释放
    for (int i = 0; i < file_num; ++i) {
        free(diffes[i]);
    }
    free(diffes);

    (*env)->ReleaseStringUTFChars(env,path_jstr,path);
    (*env)->ReleaseStringUTFChars(env,path_pattern_jstr,path_pattern);
}


JNIEXPORT void JNICALL Java_helixdown_testing_com_ndk_1file_1encrypt_NDKFilePatch_Patch
        (JNIEnv * env , jclass cls, jstring path_pattern_jstr, jint file_num, jstring path_patch_jstr){

    const char* path_pattern = (*env)->GetStringUTFChars(env,path_pattern_jstr,NULL);
    const char* path_patch = (*env)->GetStringUTFChars(env,path_patch_jstr,NULL);

    char** diffes = malloc(sizeof(char*) * file_num );

    FILE * fpw = fopen(path_patch,"wb");
    if (fpw == NULL){
        LOGE("open %s file failed error msg : %s",path_patch,strerror(errno));
        return;
    }

    for (int i = 0; i < file_num; ++i) {
        diffes[i] = malloc(sizeof(char) * 50);
        sprintf(diffes[i],path_pattern,i + 1 );

        FILE* fpr = fopen(diffes[i],"rb");
        if (fpr == NULL){
            LOGE("open %s file failed error msg : %s",diffes[i],strerror(errno));
            return;
        }

        long filesize = get_file_size(diffes[i]);

        for (int j  = 0; j < filesize; ++j) {
            fputc(fgetc(fpr),fpw);
        }
        fclose(fpr);
    }

    fclose(fpw);

//    释放
    for (int i = 0; i < file_num; ++i) {
        free(diffes[i]);
    }
    free(diffes);

    (*env)->ReleaseStringUTFChars(env,path_patch_jstr,path_patch);
    (*env)->ReleaseStringUTFChars(env,path_pattern_jstr,path_pattern);
}


