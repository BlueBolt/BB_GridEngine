/*___INFO__MARK_BEGIN__*/
/*************************************************************************
 * 
 *  The Contents of this file are made available subject to the terms of
 *  the Sun Industry Standards Source License Version 1.2
 * 
 *  Sun Microsystems Inc., March, 2001
 * 
 * 
 *  Sun Industry Standards Source License Version 1.2
 *  =================================================
 *  The contents of this file are subject to the Sun Industry Standards
 *  Source License Version 1.2 (the "License"); You may not use this file
 *  except in compliance with the License. You may obtain a copy of the
 *  License at http://gridengine.sunsource.net/Gridengine_SISSL_license.html
 * 
 *  Software provided under this License is provided on an "AS IS" basis,
 *  WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING,
 *  WITHOUT LIMITATION, WARRANTIES THAT THE SOFTWARE IS FREE OF DEFECTS,
 *  MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE, OR NON-INFRINGING.
 *  See the License for the specific provisions governing your rights and
 *  obligations concerning the Software.
 * 
 *   The Initial Developer of the Original Code is: Sun Microsystems, Inc.
 * 
 *   Copyright: 2001 by Sun Microsystems, Inc.
 * 
 *   All Rights Reserved.
 * 
 ************************************************************************/
/*___INFO__MARK_END__*/

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <jni.h>
#include "jgdi.h"
#include "basis_types.h"
#include "cull.h"
#include "commlib.h"
#include "sgermon.h"
#include "sge_all_listsL.h"
#include "sge_answer.h"
#include "sge_prog.h"
#include "sge_bootstrap.h"
#include "sge_gdi_ctx.h"
#include "cl_errors.h"
#include "sge_log.h"
#include "sge_error_class.h"
#include "jgdi_common.h"
#include "jgdi_wrapper.h"


static jclass find_class(JNIEnv *env, const char *fullClassname, lList** alpp) {
   jclass clazz = NULL;
   jclass tmpclazz = NULL;

   DENTER(BASIS_LAYER, "find_class");

   if (fullClassname == NULL) {
      answer_list_add(alpp, "fullClassname is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DEXIT;
      return NULL;
   }
   tmpclazz = (*env)->FindClass(env, fullClassname);

   if (test_jni_error(env, "Class not found", alpp)) {
       DEXIT;
       return NULL;
   }

   /* aquire a global ref for the class object */
   clazz = (jclass)(*env)->NewGlobalRef(env, tmpclazz);
   if (clazz == NULL) {
      answer_list_add_sprintf(alpp, STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR,
                      "Can not get a global reference on the %s class object",
                      fullClassname);
      DEXIT;
      return NULL;
   }
   DEXIT;
   return clazz;
}

static jgdi_result_t get_static_method_id_for_fullClassname(JNIEnv *env, jclass *clazzref, jmethodID *midref, const char *fullClassname,
                                      const char *methodName, const char *methodSignature, lList **alpp) {                                      

   DENTER(BASIS_LAYER, "get_static_method_id_for_fullClassname");

   /* Test preconditions */
   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DEXIT;
      return JGDI_NULL_POINTER;
   }
   if (midref == NULL) {
      answer_list_add(alpp, "midref is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DEXIT;
      return JGDI_NULL_POINTER;
   }
   if (clazzref == NULL) {
      answer_list_add(alpp, "clazzref is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DEXIT;
      return JGDI_NULL_POINTER;
   }
   
   *clazzref = find_class(env, fullClassname, alpp);
   if (*clazzref == NULL) {
      answer_list_add_sprintf(alpp, STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR,
                              "class %s not found", fullClassname);
      DEXIT;
      return JGDI_ILLEGAL_STATE;
   }
   
   *midref = get_static_methodid(env, *clazzref, methodName, methodSignature, alpp);
   if (*midref == NULL) {
      DEXIT;
      return JGDI_ILLEGAL_STATE;
   }
   
   DEXIT;
   return JGDI_SUCCESS;
}


static jgdi_result_t get_method_id_for_fullClassname(JNIEnv *env, jobject obj, jmethodID *midref, const char *fullClassname,
                                      const char *methodName, const char *methodSignature, lList **alpp) {                                      

   jclass clazz = NULL;

   DENTER(BASIS_LAYER, "get_method_id_for_fullClassname");

   /* Test preconditions */
   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DEXIT;
      return JGDI_NULL_POINTER;
   }
   if (obj == NULL) {
      answer_list_add(alpp, "obj is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DEXIT;
      return JGDI_NULL_POINTER;
   }
   if (midref == NULL) {
      answer_list_add(alpp, "midref is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DEXIT;
      return JGDI_NULL_POINTER;
   }
   
   clazz = find_class(env, fullClassname, alpp);
   if (clazz == NULL) {
      answer_list_add_sprintf(alpp, STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR,
                              "class %s not found", fullClassname);
      DEXIT;
      return JGDI_ILLEGAL_STATE;
   }
   if (!(*env)->IsInstanceOf(env, obj, clazz)) {
      answer_list_add_sprintf(alpp, STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR,
                      "obj is not an instanceof %s", fullClassname);
      DEXIT;
      return JGDI_ILLEGAL_STATE;
   }
   
   *midref = get_methodid(env, clazz, methodName, methodSignature, alpp);
   if (*midref == NULL) {
      DEXIT;
      return JGDI_ILLEGAL_STATE;
   }
   
   DEXIT;
   return JGDI_SUCCESS;
}



/*==== BEGIN  java/util/Iterator */

jclass Iterator_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "Iterator_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "java/util/Iterator", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t Iterator_hasNext(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Iterator_hasNext");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Iterator", "hasNext", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "Iterator_hasNext failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Iterator_next(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Iterator_next");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Iterator", "next", "()Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Iterator_next failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Iterator_remove(JNIEnv *env, jobject obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "Iterator_remove");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Iterator", "remove", "()V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid);  
   if (test_jni_error(env, "Iterator_remove failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}

/*==== END  java/util/Iterator */



/*==== BEGIN  java/util/Set */

jclass Set_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "Set_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "java/util/Set", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t Set_add(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Set_add");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Set", "add", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Set_add failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Set_addAll(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Set_addAll");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Set", "addAll", "(Ljava/util/Collection;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Set_addAll failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Set_clear(JNIEnv *env, jobject obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "Set_clear");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Set", "clear", "()V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid);  
   if (test_jni_error(env, "Set_clear failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t Set_contains(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Set_contains");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Set", "contains", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Set_contains failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Set_containsAll(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Set_containsAll");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Set", "containsAll", "(Ljava/util/Collection;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Set_containsAll failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Set_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Set_equals");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Set", "equals", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Set_equals failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Set_hashCode(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Set_hashCode");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Set", "hashCode", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "Set_hashCode failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Set_isEmpty(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Set_isEmpty");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Set", "isEmpty", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "Set_isEmpty failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Set_iterator(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Set_iterator");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Set", "iterator", "()Ljava/util/Iterator;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Set_iterator failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Set_remove(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Set_remove");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Set", "remove", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Set_remove failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Set_removeAll(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Set_removeAll");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Set", "removeAll", "(Ljava/util/Collection;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Set_removeAll failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Set_retainAll(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Set_retainAll");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Set", "retainAll", "(Ljava/util/Collection;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Set_retainAll failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Set_size(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Set_size");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Set", "size", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "Set_size failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Set_toArray(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "Set_toArray");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Set", "toArray", "()[Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Set_toArray failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;
      
   } 
       if (temp == NULL) {
          *result = NULL;
          *len = 0;
       } else {
           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);
           if (test_jni_error(env, "Can not get the array length of the result", alpp)) {
              ret = JGDI_ILLEGAL_STATE;
           } if (arrayLen > 0) {
               jobject *array = NULL;
               int i = 0;
               
               array = (jobject*)malloc(sizeof(jobject)* arrayLen);
               for(i = 0; i < arrayLen; i++) {
                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); 
                  if (test_jni_error(env, "Can not get object from array", alpp)) {
                     free(array);
                     array = NULL;
                     break;
                  }
               }
               if(array != NULL) {
                  *result = array;
                  *len = arrayLen;
               }
               
          } else {
             *result = NULL;
             *len = 0;
          }
       }
      
   DRETURN(ret);
}
jgdi_result_t Set_toArray_0(JNIEnv *env, jobject obj , jobject* p0 , jobject** result, int* len, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "Set_toArray_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Set", "toArray", "([Ljava/lang/Object;)[Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Set_toArray_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;
      
   } 
       if (temp == NULL) {
          *result = NULL;
          *len = 0;
       } else {
           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);
           if (test_jni_error(env, "Can not get the array length of the result", alpp)) {
              ret = JGDI_ILLEGAL_STATE;
           } if (arrayLen > 0) {
               jobject *array = NULL;
               int i = 0;
               
               array = (jobject*)malloc(sizeof(jobject)* arrayLen);
               for(i = 0; i < arrayLen; i++) {
                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); 
                  if (test_jni_error(env, "Can not get object from array", alpp)) {
                     free(array);
                     array = NULL;
                     break;
                  }
               }
               if(array != NULL) {
                  *result = array;
                  *len = arrayLen;
               }
               
          } else {
             *result = NULL;
             *len = 0;
          }
       }
      
   DRETURN(ret);
}

/*==== END  java/util/Set */



/*==== BEGIN  java/util/List */

jclass List_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "List_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "java/util/List", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t List_add(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "List_add");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "add", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "List_add failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t List_addAll(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "List_addAll");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "addAll", "(Ljava/util/Collection;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "List_addAll failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t List_addAll_0(JNIEnv *env, jobject obj , jint p0 , jobject p1 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "List_addAll_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "addAll", "(ILjava/util/Collection;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "List_addAll_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t List_add_0(JNIEnv *env, jobject obj , jint p0 , jobject p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "List_add_0");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "add", "(ILjava/lang/Object;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "List_add_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t List_clear(JNIEnv *env, jobject obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "List_clear");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "clear", "()V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid);  
   if (test_jni_error(env, "List_clear failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t List_contains(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "List_contains");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "contains", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "List_contains failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t List_containsAll(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "List_containsAll");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "containsAll", "(Ljava/util/Collection;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "List_containsAll failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t List_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "List_equals");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "equals", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "List_equals failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t List_get(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "List_get");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "get", "(I)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "List_get failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t List_hashCode(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "List_hashCode");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "hashCode", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "List_hashCode failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t List_indexOf(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "List_indexOf");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "indexOf", "(Ljava/lang/Object;)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "List_indexOf failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t List_isEmpty(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "List_isEmpty");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "isEmpty", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "List_isEmpty failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t List_iterator(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "List_iterator");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "iterator", "()Ljava/util/Iterator;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "List_iterator failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t List_lastIndexOf(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "List_lastIndexOf");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "lastIndexOf", "(Ljava/lang/Object;)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "List_lastIndexOf failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t List_listIterator(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "List_listIterator");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "listIterator", "()Ljava/util/ListIterator;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "List_listIterator failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t List_listIterator_0(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "List_listIterator_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "listIterator", "(I)Ljava/util/ListIterator;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "List_listIterator_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t List_remove(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "List_remove");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "remove", "(I)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "List_remove failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t List_removeAll(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "List_removeAll");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "removeAll", "(Ljava/util/Collection;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "List_removeAll failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t List_remove_0(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "List_remove_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "remove", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "List_remove_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t List_retainAll(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "List_retainAll");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "retainAll", "(Ljava/util/Collection;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "List_retainAll failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t List_set(JNIEnv *env, jobject obj , jint p0 , jobject p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "List_set");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "set", "(ILjava/lang/Object;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "List_set failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t List_size(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "List_size");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "size", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "List_size failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t List_subList(JNIEnv *env, jobject obj , jint p0 , jint p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "List_subList");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "subList", "(II)Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "List_subList failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t List_toArray(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "List_toArray");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "toArray", "()[Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "List_toArray failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;
      
   } 
       if (temp == NULL) {
          *result = NULL;
          *len = 0;
       } else {
           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);
           if (test_jni_error(env, "Can not get the array length of the result", alpp)) {
              ret = JGDI_ILLEGAL_STATE;
           } if (arrayLen > 0) {
               jobject *array = NULL;
               int i = 0;
               
               array = (jobject*)malloc(sizeof(jobject)* arrayLen);
               for(i = 0; i < arrayLen; i++) {
                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); 
                  if (test_jni_error(env, "Can not get object from array", alpp)) {
                     free(array);
                     array = NULL;
                     break;
                  }
               }
               if(array != NULL) {
                  *result = array;
                  *len = arrayLen;
               }
               
          } else {
             *result = NULL;
             *len = 0;
          }
       }
      
   DRETURN(ret);
}
jgdi_result_t List_toArray_0(JNIEnv *env, jobject obj , jobject* p0 , jobject** result, int* len, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "List_toArray_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/List", "toArray", "([Ljava/lang/Object;)[Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "List_toArray_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;
      
   } 
       if (temp == NULL) {
          *result = NULL;
          *len = 0;
       } else {
           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);
           if (test_jni_error(env, "Can not get the array length of the result", alpp)) {
              ret = JGDI_ILLEGAL_STATE;
           } if (arrayLen > 0) {
               jobject *array = NULL;
               int i = 0;
               
               array = (jobject*)malloc(sizeof(jobject)* arrayLen);
               for(i = 0; i < arrayLen; i++) {
                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); 
                  if (test_jni_error(env, "Can not get object from array", alpp)) {
                     free(array);
                     array = NULL;
                     break;
                  }
               }
               if(array != NULL) {
                  *result = array;
                  *len = arrayLen;
               }
               
          } else {
             *result = NULL;
             *len = 0;
          }
       }
      
   DRETURN(ret);
}

/*==== END  java/util/List */



/*==== BEGIN  java/util/ArrayList */

jclass ArrayList_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "ArrayList_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "java/util/ArrayList", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t ArrayList_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "ArrayList_init");
      
   clazz = ArrayList_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "()V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }    
   *obj = (*env)->NewObject(env, clazz, mid );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t ArrayList_init_0(JNIEnv *env, jobject *obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "ArrayList_init_0");
      
   clazz = ArrayList_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(I)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }    
   *obj = (*env)->NewObject(env, clazz, mid , p0 );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t ArrayList_init_1(JNIEnv *env, jobject *obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "ArrayList_init_1");
      
   clazz = ArrayList_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(Ljava/util/Collection;)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }    
   *obj = (*env)->NewObject(env, clazz, mid , p0 );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t ArrayList_add(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "ArrayList_add");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/ArrayList", "add", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ArrayList_add failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ArrayList_addAll(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "ArrayList_addAll");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/ArrayList", "addAll", "(Ljava/util/Collection;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ArrayList_addAll failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ArrayList_addAll_0(JNIEnv *env, jobject obj , jint p0 , jobject p1 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "ArrayList_addAll_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/ArrayList", "addAll", "(ILjava/util/Collection;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "ArrayList_addAll_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ArrayList_add_0(JNIEnv *env, jobject obj , jint p0 , jobject p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ArrayList_add_0");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/ArrayList", "add", "(ILjava/lang/Object;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "ArrayList_add_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ArrayList_clear(JNIEnv *env, jobject obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ArrayList_clear");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/ArrayList", "clear", "()V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid);  
   if (test_jni_error(env, "ArrayList_clear failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ArrayList_clone(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ArrayList_clone");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/ArrayList", "clone", "()Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ArrayList_clone failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ArrayList_contains(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "ArrayList_contains");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/ArrayList", "contains", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ArrayList_contains failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ArrayList_ensureCapacity(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ArrayList_ensureCapacity");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/ArrayList", "ensureCapacity", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ArrayList_ensureCapacity failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ArrayList_get(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ArrayList_get");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/ArrayList", "get", "(I)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ArrayList_get failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ArrayList_indexOf(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ArrayList_indexOf");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/ArrayList", "indexOf", "(Ljava/lang/Object;)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ArrayList_indexOf failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ArrayList_isEmpty(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "ArrayList_isEmpty");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/ArrayList", "isEmpty", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "ArrayList_isEmpty failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ArrayList_lastIndexOf(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ArrayList_lastIndexOf");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/ArrayList", "lastIndexOf", "(Ljava/lang/Object;)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ArrayList_lastIndexOf failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ArrayList_remove(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ArrayList_remove");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/ArrayList", "remove", "(I)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ArrayList_remove failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ArrayList_remove_0(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "ArrayList_remove_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/ArrayList", "remove", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ArrayList_remove_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ArrayList_set(JNIEnv *env, jobject obj , jint p0 , jobject p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ArrayList_set");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/ArrayList", "set", "(ILjava/lang/Object;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "ArrayList_set failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ArrayList_size(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ArrayList_size");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/ArrayList", "size", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "ArrayList_size failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ArrayList_toArray(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "ArrayList_toArray");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/ArrayList", "toArray", "()[Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ArrayList_toArray failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;
      
   } 
       if (temp == NULL) {
          *result = NULL;
          *len = 0;
       } else {
           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);
           if (test_jni_error(env, "Can not get the array length of the result", alpp)) {
              ret = JGDI_ILLEGAL_STATE;
           } if (arrayLen > 0) {
               jobject *array = NULL;
               int i = 0;
               
               array = (jobject*)malloc(sizeof(jobject)* arrayLen);
               for(i = 0; i < arrayLen; i++) {
                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); 
                  if (test_jni_error(env, "Can not get object from array", alpp)) {
                     free(array);
                     array = NULL;
                     break;
                  }
               }
               if(array != NULL) {
                  *result = array;
                  *len = arrayLen;
               }
               
          } else {
             *result = NULL;
             *len = 0;
          }
       }
      
   DRETURN(ret);
}
jgdi_result_t ArrayList_toArray_0(JNIEnv *env, jobject obj , jobject* p0 , jobject** result, int* len, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "ArrayList_toArray_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/ArrayList", "toArray", "([Ljava/lang/Object;)[Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ArrayList_toArray_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;
      
   } 
       if (temp == NULL) {
          *result = NULL;
          *len = 0;
       } else {
           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);
           if (test_jni_error(env, "Can not get the array length of the result", alpp)) {
              ret = JGDI_ILLEGAL_STATE;
           } if (arrayLen > 0) {
               jobject *array = NULL;
               int i = 0;
               
               array = (jobject*)malloc(sizeof(jobject)* arrayLen);
               for(i = 0; i < arrayLen; i++) {
                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); 
                  if (test_jni_error(env, "Can not get object from array", alpp)) {
                     free(array);
                     array = NULL;
                     break;
                  }
               }
               if(array != NULL) {
                  *result = array;
                  *len = arrayLen;
               }
               
          } else {
             *result = NULL;
             *len = 0;
          }
       }
      
   DRETURN(ret);
}
jgdi_result_t ArrayList_trimToSize(JNIEnv *env, jobject obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ArrayList_trimToSize");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/ArrayList", "trimToSize", "()V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid);  
   if (test_jni_error(env, "ArrayList_trimToSize failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}

/*==== END  java/util/ArrayList */



/*==== BEGIN  java/util/Calendar */

jclass Calendar_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "Calendar_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "java/util/Calendar", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t Calendar_static_ALL_STYLES(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_ALL_STYLES");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "ALL_STYLES", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_ALL_STYLES failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_AM(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_AM");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "AM", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_AM failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_AM_PM(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_AM_PM");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "AM_PM", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_AM_PM failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_APRIL(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_APRIL");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "APRIL", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_APRIL failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_AUGUST(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_AUGUST");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "AUGUST", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_AUGUST failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_DATE(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_DATE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "DATE", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_DATE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_DAY_OF_MONTH(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_DAY_OF_MONTH");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "DAY_OF_MONTH", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_DAY_OF_MONTH failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_DAY_OF_WEEK(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_DAY_OF_WEEK");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "DAY_OF_WEEK", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_DAY_OF_WEEK failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_DAY_OF_WEEK_IN_MONTH(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_DAY_OF_WEEK_IN_MONTH");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "DAY_OF_WEEK_IN_MONTH", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_DAY_OF_WEEK_IN_MONTH failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_DAY_OF_YEAR(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_DAY_OF_YEAR");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "DAY_OF_YEAR", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_DAY_OF_YEAR failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_DECEMBER(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_DECEMBER");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "DECEMBER", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_DECEMBER failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_DST_OFFSET(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_DST_OFFSET");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "DST_OFFSET", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_DST_OFFSET failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_ERA(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_ERA");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "ERA", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_ERA failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_FEBRUARY(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_FEBRUARY");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "FEBRUARY", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_FEBRUARY failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_FIELD_COUNT(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_FIELD_COUNT");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "FIELD_COUNT", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_FIELD_COUNT failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_FRIDAY(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_FRIDAY");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "FRIDAY", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_FRIDAY failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_HOUR(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_HOUR");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "HOUR", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_HOUR failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_HOUR_OF_DAY(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_HOUR_OF_DAY");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "HOUR_OF_DAY", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_HOUR_OF_DAY failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_JANUARY(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_JANUARY");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "JANUARY", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_JANUARY failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_JULY(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_JULY");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "JULY", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_JULY failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_JUNE(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_JUNE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "JUNE", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_JUNE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_LONG(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_LONG");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "LONG", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_LONG failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_MARCH(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_MARCH");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "MARCH", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_MARCH failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_MAY(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_MAY");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "MAY", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_MAY failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_MILLISECOND(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_MILLISECOND");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "MILLISECOND", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_MILLISECOND failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_MINUTE(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_MINUTE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "MINUTE", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_MINUTE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_MONDAY(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_MONDAY");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "MONDAY", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_MONDAY failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_MONTH(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_MONTH");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "MONTH", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_MONTH failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_NOVEMBER(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_NOVEMBER");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "NOVEMBER", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_NOVEMBER failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_OCTOBER(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_OCTOBER");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "OCTOBER", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_OCTOBER failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_PM(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_PM");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "PM", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_PM failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_SATURDAY(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_SATURDAY");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "SATURDAY", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_SATURDAY failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_SECOND(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_SECOND");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "SECOND", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_SECOND failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_SEPTEMBER(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_SEPTEMBER");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "SEPTEMBER", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_SEPTEMBER failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_SHORT(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_SHORT");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "SHORT", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_SHORT failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_SUNDAY(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_SUNDAY");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "SUNDAY", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_SUNDAY failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_THURSDAY(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_THURSDAY");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "THURSDAY", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_THURSDAY failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_TUESDAY(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_TUESDAY");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "TUESDAY", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_TUESDAY failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_UNDECIMBER(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_UNDECIMBER");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "UNDECIMBER", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_UNDECIMBER failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_WEDNESDAY(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_WEDNESDAY");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "WEDNESDAY", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_WEDNESDAY failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_WEEK_OF_MONTH(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_WEEK_OF_MONTH");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "WEEK_OF_MONTH", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_WEEK_OF_MONTH failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_WEEK_OF_YEAR(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_WEEK_OF_YEAR");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "WEEK_OF_YEAR", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_WEEK_OF_YEAR failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_YEAR(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_YEAR");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "YEAR", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_YEAR failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_ZONE_OFFSET(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Calendar_static_ZONE_OFFSET");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Calendar_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/util/Calendar not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "ZONE_OFFSET", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Calendar_static_ZONE_OFFSET failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Calendar_static_getAvailableLocales(JNIEnv *env , jobject** result, int* len, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "Calendar_static_getAvailableLocales");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/util/Calendar", "getAvailableLocales", "()[Ljava/util/Locale;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid );  
   if (test_jni_error(env, "Calendar_getAvailableLocales failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;
      
   }  else {
  
       if (temp == NULL) {
          *result = NULL;
          *len = 0;
       } else {
           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);
           if (test_jni_error(env, "Can not get the array length of the result", alpp)) {
              ret = JGDI_ILLEGAL_STATE;
           } if (arrayLen > 0) {
               jobject *array = NULL;
               int i = 0;
               
               array = (jobject*)malloc(sizeof(jobject)* arrayLen);
               for(i = 0; i < arrayLen; i++) {
                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); 
                  if (test_jni_error(env, "Can not get object from array", alpp)) {
                     free(array);
                     array = NULL;
                     break;
                  }
               }
               if(array != NULL) {
                  *result = array;
                  *len = arrayLen;
               }
               
          } else {
             *result = NULL;
             *len = 0;
          }
       }
      
   }

   DRETURN(ret);
}
jgdi_result_t Calendar_static_getInstance(JNIEnv *env , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Calendar_static_getInstance");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/util/Calendar", "getInstance", "()Ljava/util/Calendar;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid );  
   if (test_jni_error(env, "Calendar_getInstance failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Calendar_static_getInstance_0(JNIEnv *env , jobject p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Calendar_static_getInstance_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/util/Calendar", "getInstance", "(Ljava/util/Locale;)Ljava/util/Calendar;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Calendar_getInstance_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Calendar_static_getInstance_1(JNIEnv *env , jobject p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Calendar_static_getInstance_1");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/util/Calendar", "getInstance", "(Ljava/util/TimeZone;)Ljava/util/Calendar;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Calendar_getInstance_1 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Calendar_static_getInstance_2(JNIEnv *env , jobject p0 , jobject p1 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Calendar_static_getInstance_2");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/util/Calendar", "getInstance", "(Ljava/util/TimeZone;Ljava/util/Locale;)Ljava/util/Calendar;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 , p1 );  
   if (test_jni_error(env, "Calendar_getInstance_2 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Calendar_add(JNIEnv *env, jobject obj , jint p0 , jint p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "Calendar_add");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "add", "(II)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "Calendar_add failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t Calendar_after(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Calendar_after");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "after", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Calendar_after failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_before(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Calendar_before");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "before", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Calendar_before failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_clear(JNIEnv *env, jobject obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "Calendar_clear");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "clear", "()V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid);  
   if (test_jni_error(env, "Calendar_clear failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t Calendar_clear_0(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "Calendar_clear_0");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "clear", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Calendar_clear_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t Calendar_clone(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Calendar_clone");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "clone", "()Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Calendar_clone failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_compareTo(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Calendar_compareTo");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "compareTo", "(Ljava/lang/Object;)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Calendar_compareTo failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_compareTo_0(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Calendar_compareTo_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "compareTo", "(Ljava/util/Calendar;)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Calendar_compareTo_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Calendar_equals");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "equals", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Calendar_equals failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_get(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Calendar_get");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "get", "(I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Calendar_get failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_getActualMaximum(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Calendar_getActualMaximum");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "getActualMaximum", "(I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Calendar_getActualMaximum failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_getActualMinimum(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Calendar_getActualMinimum");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "getActualMinimum", "(I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Calendar_getActualMinimum failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_getAvailableLocales(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "Calendar_getAvailableLocales");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "getAvailableLocales", "()[Ljava/util/Locale;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Calendar_getAvailableLocales failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;
      
   } 
       if (temp == NULL) {
          *result = NULL;
          *len = 0;
       } else {
           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);
           if (test_jni_error(env, "Can not get the array length of the result", alpp)) {
              ret = JGDI_ILLEGAL_STATE;
           } if (arrayLen > 0) {
               jobject *array = NULL;
               int i = 0;
               
               array = (jobject*)malloc(sizeof(jobject)* arrayLen);
               for(i = 0; i < arrayLen; i++) {
                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); 
                  if (test_jni_error(env, "Can not get object from array", alpp)) {
                     free(array);
                     array = NULL;
                     break;
                  }
               }
               if(array != NULL) {
                  *result = array;
                  *len = arrayLen;
               }
               
          } else {
             *result = NULL;
             *len = 0;
          }
       }
      
   DRETURN(ret);
}
jgdi_result_t Calendar_getDisplayName(JNIEnv *env, jobject obj , jint p0 , jint p1 , jobject p2 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Calendar_getDisplayName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "getDisplayName", "(IILjava/util/Locale;)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1, p2);  
   if (test_jni_error(env, "Calendar_getDisplayName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_getDisplayNames(JNIEnv *env, jobject obj , jint p0 , jint p1 , jobject p2 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Calendar_getDisplayNames");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "getDisplayNames", "(IILjava/util/Locale;)Ljava/util/Map;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1, p2);  
   if (test_jni_error(env, "Calendar_getDisplayNames failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_getFirstDayOfWeek(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Calendar_getFirstDayOfWeek");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "getFirstDayOfWeek", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "Calendar_getFirstDayOfWeek failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_getGreatestMinimum(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Calendar_getGreatestMinimum");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "getGreatestMinimum", "(I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Calendar_getGreatestMinimum failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_getInstance(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Calendar_getInstance");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "getInstance", "()Ljava/util/Calendar;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Calendar_getInstance failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_getInstance_0(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Calendar_getInstance_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "getInstance", "(Ljava/util/Locale;)Ljava/util/Calendar;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Calendar_getInstance_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_getInstance_1(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Calendar_getInstance_1");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "getInstance", "(Ljava/util/TimeZone;)Ljava/util/Calendar;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Calendar_getInstance_1 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_getInstance_2(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Calendar_getInstance_2");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "getInstance", "(Ljava/util/TimeZone;Ljava/util/Locale;)Ljava/util/Calendar;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "Calendar_getInstance_2 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_getLeastMaximum(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Calendar_getLeastMaximum");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "getLeastMaximum", "(I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Calendar_getLeastMaximum failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_getMaximum(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Calendar_getMaximum");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "getMaximum", "(I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Calendar_getMaximum failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_getMinimalDaysInFirstWeek(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Calendar_getMinimalDaysInFirstWeek");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "getMinimalDaysInFirstWeek", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "Calendar_getMinimalDaysInFirstWeek failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_getMinimum(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Calendar_getMinimum");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "getMinimum", "(I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Calendar_getMinimum failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_getTime(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Calendar_getTime");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "getTime", "()Ljava/util/Date;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Calendar_getTime failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_getTimeInMillis(JNIEnv *env, jobject obj , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "Calendar_getTimeInMillis");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "getTimeInMillis", "()J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid);  
   if (test_jni_error(env, "Calendar_getTimeInMillis failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_getTimeZone(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Calendar_getTimeZone");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "getTimeZone", "()Ljava/util/TimeZone;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Calendar_getTimeZone failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_hashCode(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Calendar_hashCode");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "hashCode", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "Calendar_hashCode failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_isLenient(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Calendar_isLenient");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "isLenient", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "Calendar_isLenient failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_isSet(JNIEnv *env, jobject obj , jint p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Calendar_isSet");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "isSet", "(I)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Calendar_isSet failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Calendar_roll(JNIEnv *env, jobject obj , jint p0 , jboolean p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "Calendar_roll");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "roll", "(IZ)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "Calendar_roll failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t Calendar_roll_0(JNIEnv *env, jobject obj , jint p0 , jint p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "Calendar_roll_0");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "roll", "(II)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "Calendar_roll_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t Calendar_set(JNIEnv *env, jobject obj , jint p0 , jint p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "Calendar_set");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "set", "(II)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "Calendar_set failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t Calendar_setFirstDayOfWeek(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "Calendar_setFirstDayOfWeek");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "setFirstDayOfWeek", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Calendar_setFirstDayOfWeek failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t Calendar_setLenient(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "Calendar_setLenient");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "setLenient", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Calendar_setLenient failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t Calendar_setMinimalDaysInFirstWeek(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "Calendar_setMinimalDaysInFirstWeek");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "setMinimalDaysInFirstWeek", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Calendar_setMinimalDaysInFirstWeek failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t Calendar_setTime(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "Calendar_setTime");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "setTime", "(Ljava/util/Date;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Calendar_setTime failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t Calendar_setTimeInMillis(JNIEnv *env, jobject obj , jlong p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "Calendar_setTimeInMillis");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "setTimeInMillis", "(J)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Calendar_setTimeInMillis failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t Calendar_setTimeZone(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "Calendar_setTimeZone");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "setTimeZone", "(Ljava/util/TimeZone;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Calendar_setTimeZone failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t Calendar_set_0(JNIEnv *env, jobject obj , jint p0 , jint p1 , jint p2 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "Calendar_set_0");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "set", "(III)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1, p2);  
   if (test_jni_error(env, "Calendar_set_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t Calendar_set_1(JNIEnv *env, jobject obj , jint p0 , jint p1 , jint p2 , jint p3 , jint p4 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "Calendar_set_1");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "set", "(IIIII)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1, p2, p3, p4);  
   if (test_jni_error(env, "Calendar_set_1 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t Calendar_set_2(JNIEnv *env, jobject obj , jint p0 , jint p1 , jint p2 , jint p3 , jint p4 , jint p5 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "Calendar_set_2");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "set", "(IIIIII)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1, p2, p3, p4, p5);  
   if (test_jni_error(env, "Calendar_set_2 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t Calendar_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Calendar_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/util/Calendar", "toString", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Calendar_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  java/util/Calendar */



/*==== BEGIN  java/lang/Class */

jclass Class_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "Class_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "java/lang/Class", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t Class_static_forName(JNIEnv *env , const char* p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_static_forName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Class", "forName", "(Ljava/lang/String;)Ljava/lang/Class;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "Class_forName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Class_static_forName_0(JNIEnv *env , const char* p0 , jboolean p1 , jobject p2 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_static_forName_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Class", "forName", "(Ljava/lang/String;ZLjava/lang/ClassLoader;)Ljava/lang/Class;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj , p1 , p2 );  
   if (test_jni_error(env, "Class_forName_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Class_asSubclass(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_asSubclass");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "asSubclass", "(Ljava/lang/Class;)Ljava/lang/Class;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Class_asSubclass failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_cast(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_cast");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "cast", "(Ljava/lang/Object;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Class_cast failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_desiredAssertionStatus(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Class_desiredAssertionStatus");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "desiredAssertionStatus", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_desiredAssertionStatus failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_forName(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_forName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "forName", "(Ljava/lang/String;)Ljava/lang/Class;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "Class_forName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_forName_0(JNIEnv *env, jobject obj , const char* p0 , jboolean p1 , jobject p2 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_forName_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "forName", "(Ljava/lang/String;ZLjava/lang/ClassLoader;)Ljava/lang/Class;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj, p1, p2);  
   if (test_jni_error(env, "Class_forName_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getAnnotation(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_getAnnotation");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getAnnotation", "(Ljava/lang/Class;)Ljava/lang/annotation/Annotation;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Class_getAnnotation failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getAnnotations(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "Class_getAnnotations");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getAnnotations", "()[Ljava/lang/annotation/Annotation;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getAnnotations failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;
      
   } 
       if (temp == NULL) {
          *result = NULL;
          *len = 0;
       } else {
           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);
           if (test_jni_error(env, "Can not get the array length of the result", alpp)) {
              ret = JGDI_ILLEGAL_STATE;
           } if (arrayLen > 0) {
               jobject *array = NULL;
               int i = 0;
               
               array = (jobject*)malloc(sizeof(jobject)* arrayLen);
               for(i = 0; i < arrayLen; i++) {
                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); 
                  if (test_jni_error(env, "Can not get object from array", alpp)) {
                     free(array);
                     array = NULL;
                     break;
                  }
               }
               if(array != NULL) {
                  *result = array;
                  *len = arrayLen;
               }
               
          } else {
             *result = NULL;
             *len = 0;
          }
       }
      
   DRETURN(ret);
}
jgdi_result_t Class_getCanonicalName(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_getCanonicalName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getCanonicalName", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getCanonicalName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getClassLoader(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_getClassLoader");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getClassLoader", "()Ljava/lang/ClassLoader;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getClassLoader failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getClasses(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "Class_getClasses");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getClasses", "()[Ljava/lang/Class;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getClasses failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;
      
   } 
       if (temp == NULL) {
          *result = NULL;
          *len = 0;
       } else {
           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);
           if (test_jni_error(env, "Can not get the array length of the result", alpp)) {
              ret = JGDI_ILLEGAL_STATE;
           } if (arrayLen > 0) {
               jobject *array = NULL;
               int i = 0;
               
               array = (jobject*)malloc(sizeof(jobject)* arrayLen);
               for(i = 0; i < arrayLen; i++) {
                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); 
                  if (test_jni_error(env, "Can not get object from array", alpp)) {
                     free(array);
                     array = NULL;
                     break;
                  }
               }
               if(array != NULL) {
                  *result = array;
                  *len = arrayLen;
               }
               
          } else {
             *result = NULL;
             *len = 0;
          }
       }
      
   DRETURN(ret);
}
jgdi_result_t Class_getComponentType(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_getComponentType");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getComponentType", "()Ljava/lang/Class;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getComponentType failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getConstructor(JNIEnv *env, jobject obj , jobject* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_getConstructor");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getConstructor", "([Ljava/lang/Class;)Ljava/lang/reflect/Constructor;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Class_getConstructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getConstructors(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "Class_getConstructors");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getConstructors", "()[Ljava/lang/reflect/Constructor;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getConstructors failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;
      
   } 
       if (temp == NULL) {
          *result = NULL;
          *len = 0;
       } else {
           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);
           if (test_jni_error(env, "Can not get the array length of the result", alpp)) {
              ret = JGDI_ILLEGAL_STATE;
           } if (arrayLen > 0) {
               jobject *array = NULL;
               int i = 0;
               
               array = (jobject*)malloc(sizeof(jobject)* arrayLen);
               for(i = 0; i < arrayLen; i++) {
                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); 
                  if (test_jni_error(env, "Can not get object from array", alpp)) {
                     free(array);
                     array = NULL;
                     break;
                  }
               }
               if(array != NULL) {
                  *result = array;
                  *len = arrayLen;
               }
               
          } else {
             *result = NULL;
             *len = 0;
          }
       }
      
   DRETURN(ret);
}
jgdi_result_t Class_getDeclaredAnnotations(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "Class_getDeclaredAnnotations");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getDeclaredAnnotations", "()[Ljava/lang/annotation/Annotation;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getDeclaredAnnotations failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;
      
   } 
       if (temp == NULL) {
          *result = NULL;
          *len = 0;
       } else {
           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);
           if (test_jni_error(env, "Can not get the array length of the result", alpp)) {
              ret = JGDI_ILLEGAL_STATE;
           } if (arrayLen > 0) {
               jobject *array = NULL;
               int i = 0;
               
               array = (jobject*)malloc(sizeof(jobject)* arrayLen);
               for(i = 0; i < arrayLen; i++) {
                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); 
                  if (test_jni_error(env, "Can not get object from array", alpp)) {
                     free(array);
                     array = NULL;
                     break;
                  }
               }
               if(array != NULL) {
                  *result = array;
                  *len = arrayLen;
               }
               
          } else {
             *result = NULL;
             *len = 0;
          }
       }
      
   DRETURN(ret);
}
jgdi_result_t Class_getDeclaredClasses(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "Class_getDeclaredClasses");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getDeclaredClasses", "()[Ljava/lang/Class;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getDeclaredClasses failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;
      
   } 
       if (temp == NULL) {
          *result = NULL;
          *len = 0;
       } else {
           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);
           if (test_jni_error(env, "Can not get the array length of the result", alpp)) {
              ret = JGDI_ILLEGAL_STATE;
           } if (arrayLen > 0) {
               jobject *array = NULL;
               int i = 0;
               
               array = (jobject*)malloc(sizeof(jobject)* arrayLen);
               for(i = 0; i < arrayLen; i++) {
                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); 
                  if (test_jni_error(env, "Can not get object from array", alpp)) {
                     free(array);
                     array = NULL;
                     break;
                  }
               }
               if(array != NULL) {
                  *result = array;
                  *len = arrayLen;
               }
               
          } else {
             *result = NULL;
             *len = 0;
          }
       }
      
   DRETURN(ret);
}
jgdi_result_t Class_getDeclaredConstructor(JNIEnv *env, jobject obj , jobject* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_getDeclaredConstructor");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getDeclaredConstructor", "([Ljava/lang/Class;)Ljava/lang/reflect/Constructor;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Class_getDeclaredConstructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getDeclaredConstructors(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "Class_getDeclaredConstructors");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getDeclaredConstructors", "()[Ljava/lang/reflect/Constructor;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getDeclaredConstructors failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;
      
   } 
       if (temp == NULL) {
          *result = NULL;
          *len = 0;
       } else {
           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);
           if (test_jni_error(env, "Can not get the array length of the result", alpp)) {
              ret = JGDI_ILLEGAL_STATE;
           } if (arrayLen > 0) {
               jobject *array = NULL;
               int i = 0;
               
               array = (jobject*)malloc(sizeof(jobject)* arrayLen);
               for(i = 0; i < arrayLen; i++) {
                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); 
                  if (test_jni_error(env, "Can not get object from array", alpp)) {
                     free(array);
                     array = NULL;
                     break;
                  }
               }
               if(array != NULL) {
                  *result = array;
                  *len = arrayLen;
               }
               
          } else {
             *result = NULL;
             *len = 0;
          }
       }
      
   DRETURN(ret);
}
jgdi_result_t Class_getDeclaredField(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_getDeclaredField");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getDeclaredField", "(Ljava/lang/String;)Ljava/lang/reflect/Field;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "Class_getDeclaredField failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getDeclaredFields(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "Class_getDeclaredFields");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getDeclaredFields", "()[Ljava/lang/reflect/Field;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getDeclaredFields failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;
      
   } 
       if (temp == NULL) {
          *result = NULL;
          *len = 0;
       } else {
           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);
           if (test_jni_error(env, "Can not get the array length of the result", alpp)) {
              ret = JGDI_ILLEGAL_STATE;
           } if (arrayLen > 0) {
               jobject *array = NULL;
               int i = 0;
               
               array = (jobject*)malloc(sizeof(jobject)* arrayLen);
               for(i = 0; i < arrayLen; i++) {
                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); 
                  if (test_jni_error(env, "Can not get object from array", alpp)) {
                     free(array);
                     array = NULL;
                     break;
                  }
               }
               if(array != NULL) {
                  *result = array;
                  *len = arrayLen;
               }
               
          } else {
             *result = NULL;
             *len = 0;
          }
       }
      
   DRETURN(ret);
}
jgdi_result_t Class_getDeclaredMethod(JNIEnv *env, jobject obj , const char* p0 , jobject* p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_getDeclaredMethod");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getDeclaredMethod", "(Ljava/lang/String;[Ljava/lang/Class;)Ljava/lang/reflect/Method;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj, p1);  
   if (test_jni_error(env, "Class_getDeclaredMethod failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getDeclaredMethods(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "Class_getDeclaredMethods");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getDeclaredMethods", "()[Ljava/lang/reflect/Method;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getDeclaredMethods failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;
      
   } 
       if (temp == NULL) {
          *result = NULL;
          *len = 0;
       } else {
           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);
           if (test_jni_error(env, "Can not get the array length of the result", alpp)) {
              ret = JGDI_ILLEGAL_STATE;
           } if (arrayLen > 0) {
               jobject *array = NULL;
               int i = 0;
               
               array = (jobject*)malloc(sizeof(jobject)* arrayLen);
               for(i = 0; i < arrayLen; i++) {
                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); 
                  if (test_jni_error(env, "Can not get object from array", alpp)) {
                     free(array);
                     array = NULL;
                     break;
                  }
               }
               if(array != NULL) {
                  *result = array;
                  *len = arrayLen;
               }
               
          } else {
             *result = NULL;
             *len = 0;
          }
       }
      
   DRETURN(ret);
}
jgdi_result_t Class_getDeclaringClass(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_getDeclaringClass");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getDeclaringClass", "()Ljava/lang/Class;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getDeclaringClass failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getEnclosingClass(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_getEnclosingClass");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getEnclosingClass", "()Ljava/lang/Class;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getEnclosingClass failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getEnclosingConstructor(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_getEnclosingConstructor");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getEnclosingConstructor", "()Ljava/lang/reflect/Constructor;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getEnclosingConstructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getEnclosingMethod(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_getEnclosingMethod");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getEnclosingMethod", "()Ljava/lang/reflect/Method;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getEnclosingMethod failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getEnumConstants(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "Class_getEnumConstants");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getEnumConstants", "()[Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getEnumConstants failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;
      
   } 
       if (temp == NULL) {
          *result = NULL;
          *len = 0;
       } else {
           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);
           if (test_jni_error(env, "Can not get the array length of the result", alpp)) {
              ret = JGDI_ILLEGAL_STATE;
           } if (arrayLen > 0) {
               jobject *array = NULL;
               int i = 0;
               
               array = (jobject*)malloc(sizeof(jobject)* arrayLen);
               for(i = 0; i < arrayLen; i++) {
                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); 
                  if (test_jni_error(env, "Can not get object from array", alpp)) {
                     free(array);
                     array = NULL;
                     break;
                  }
               }
               if(array != NULL) {
                  *result = array;
                  *len = arrayLen;
               }
               
          } else {
             *result = NULL;
             *len = 0;
          }
       }
      
   DRETURN(ret);
}
jgdi_result_t Class_getField(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_getField");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getField", "(Ljava/lang/String;)Ljava/lang/reflect/Field;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "Class_getField failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getFields(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "Class_getFields");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getFields", "()[Ljava/lang/reflect/Field;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getFields failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;
      
   } 
       if (temp == NULL) {
          *result = NULL;
          *len = 0;
       } else {
           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);
           if (test_jni_error(env, "Can not get the array length of the result", alpp)) {
              ret = JGDI_ILLEGAL_STATE;
           } if (arrayLen > 0) {
               jobject *array = NULL;
               int i = 0;
               
               array = (jobject*)malloc(sizeof(jobject)* arrayLen);
               for(i = 0; i < arrayLen; i++) {
                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); 
                  if (test_jni_error(env, "Can not get object from array", alpp)) {
                     free(array);
                     array = NULL;
                     break;
                  }
               }
               if(array != NULL) {
                  *result = array;
                  *len = arrayLen;
               }
               
          } else {
             *result = NULL;
             *len = 0;
          }
       }
      
   DRETURN(ret);
}
jgdi_result_t Class_getGenericInterfaces(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "Class_getGenericInterfaces");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getGenericInterfaces", "()[Ljava/lang/reflect/Type;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getGenericInterfaces failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;
      
   } 
       if (temp == NULL) {
          *result = NULL;
          *len = 0;
       } else {
           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);
           if (test_jni_error(env, "Can not get the array length of the result", alpp)) {
              ret = JGDI_ILLEGAL_STATE;
           } if (arrayLen > 0) {
               jobject *array = NULL;
               int i = 0;
               
               array = (jobject*)malloc(sizeof(jobject)* arrayLen);
               for(i = 0; i < arrayLen; i++) {
                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); 
                  if (test_jni_error(env, "Can not get object from array", alpp)) {
                     free(array);
                     array = NULL;
                     break;
                  }
               }
               if(array != NULL) {
                  *result = array;
                  *len = arrayLen;
               }
               
          } else {
             *result = NULL;
             *len = 0;
          }
       }
      
   DRETURN(ret);
}
jgdi_result_t Class_getGenericSuperclass(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_getGenericSuperclass");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getGenericSuperclass", "()Ljava/lang/reflect/Type;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getGenericSuperclass failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getInterfaces(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "Class_getInterfaces");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getInterfaces", "()[Ljava/lang/Class;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getInterfaces failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;
      
   } 
       if (temp == NULL) {
          *result = NULL;
          *len = 0;
       } else {
           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);
           if (test_jni_error(env, "Can not get the array length of the result", alpp)) {
              ret = JGDI_ILLEGAL_STATE;
           } if (arrayLen > 0) {
               jobject *array = NULL;
               int i = 0;
               
               array = (jobject*)malloc(sizeof(jobject)* arrayLen);
               for(i = 0; i < arrayLen; i++) {
                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); 
                  if (test_jni_error(env, "Can not get object from array", alpp)) {
                     free(array);
                     array = NULL;
                     break;
                  }
               }
               if(array != NULL) {
                  *result = array;
                  *len = arrayLen;
               }
               
          } else {
             *result = NULL;
             *len = 0;
          }
       }
      
   DRETURN(ret);
}
jgdi_result_t Class_getMethod(JNIEnv *env, jobject obj , const char* p0 , jobject* p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_getMethod");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getMethod", "(Ljava/lang/String;[Ljava/lang/Class;)Ljava/lang/reflect/Method;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj, p1);  
   if (test_jni_error(env, "Class_getMethod failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getMethods(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "Class_getMethods");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getMethods", "()[Ljava/lang/reflect/Method;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getMethods failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;
      
   } 
       if (temp == NULL) {
          *result = NULL;
          *len = 0;
       } else {
           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);
           if (test_jni_error(env, "Can not get the array length of the result", alpp)) {
              ret = JGDI_ILLEGAL_STATE;
           } if (arrayLen > 0) {
               jobject *array = NULL;
               int i = 0;
               
               array = (jobject*)malloc(sizeof(jobject)* arrayLen);
               for(i = 0; i < arrayLen; i++) {
                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); 
                  if (test_jni_error(env, "Can not get object from array", alpp)) {
                     free(array);
                     array = NULL;
                     break;
                  }
               }
               if(array != NULL) {
                  *result = array;
                  *len = arrayLen;
               }
               
          } else {
             *result = NULL;
             *len = 0;
          }
       }
      
   DRETURN(ret);
}
jgdi_result_t Class_getModifiers(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Class_getModifiers");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getModifiers", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getModifiers failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getName(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_getName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getName", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getPackage(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_getPackage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getPackage", "()Ljava/lang/Package;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getPackage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getProtectionDomain(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_getProtectionDomain");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getProtectionDomain", "()Ljava/security/ProtectionDomain;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getProtectionDomain failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getResource(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_getResource");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getResource", "(Ljava/lang/String;)Ljava/net/URL;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "Class_getResource failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getResourceAsStream(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_getResourceAsStream");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getResourceAsStream", "(Ljava/lang/String;)Ljava/io/InputStream;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "Class_getResourceAsStream failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getSigners(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "Class_getSigners");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getSigners", "()[Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getSigners failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;
      
   } 
       if (temp == NULL) {
          *result = NULL;
          *len = 0;
       } else {
           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);
           if (test_jni_error(env, "Can not get the array length of the result", alpp)) {
              ret = JGDI_ILLEGAL_STATE;
           } if (arrayLen > 0) {
               jobject *array = NULL;
               int i = 0;
               
               array = (jobject*)malloc(sizeof(jobject)* arrayLen);
               for(i = 0; i < arrayLen; i++) {
                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); 
                  if (test_jni_error(env, "Can not get object from array", alpp)) {
                     free(array);
                     array = NULL;
                     break;
                  }
               }
               if(array != NULL) {
                  *result = array;
                  *len = arrayLen;
               }
               
          } else {
             *result = NULL;
             *len = 0;
          }
       }
      
   DRETURN(ret);
}
jgdi_result_t Class_getSimpleName(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_getSimpleName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getSimpleName", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getSimpleName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getSuperclass(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_getSuperclass");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getSuperclass", "()Ljava/lang/Class;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getSuperclass failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_getTypeParameters(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "Class_getTypeParameters");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "getTypeParameters", "()[Ljava/lang/reflect/TypeVariable;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_getTypeParameters failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;
      
   } 
       if (temp == NULL) {
          *result = NULL;
          *len = 0;
       } else {
           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);
           if (test_jni_error(env, "Can not get the array length of the result", alpp)) {
              ret = JGDI_ILLEGAL_STATE;
           } if (arrayLen > 0) {
               jobject *array = NULL;
               int i = 0;
               
               array = (jobject*)malloc(sizeof(jobject)* arrayLen);
               for(i = 0; i < arrayLen; i++) {
                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); 
                  if (test_jni_error(env, "Can not get object from array", alpp)) {
                     free(array);
                     array = NULL;
                     break;
                  }
               }
               if(array != NULL) {
                  *result = array;
                  *len = arrayLen;
               }
               
          } else {
             *result = NULL;
             *len = 0;
          }
       }
      
   DRETURN(ret);
}
jgdi_result_t Class_isAnnotation(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Class_isAnnotation");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "isAnnotation", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_isAnnotation failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_isAnnotationPresent(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Class_isAnnotationPresent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "isAnnotationPresent", "(Ljava/lang/Class;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Class_isAnnotationPresent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_isAnonymousClass(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Class_isAnonymousClass");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "isAnonymousClass", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_isAnonymousClass failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_isArray(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Class_isArray");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "isArray", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_isArray failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_isAssignableFrom(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Class_isAssignableFrom");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "isAssignableFrom", "(Ljava/lang/Class;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Class_isAssignableFrom failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_isEnum(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Class_isEnum");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "isEnum", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_isEnum failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_isInstance(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Class_isInstance");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "isInstance", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Class_isInstance failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_isInterface(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Class_isInterface");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "isInterface", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_isInterface failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_isLocalClass(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Class_isLocalClass");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "isLocalClass", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_isLocalClass failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_isMemberClass(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Class_isMemberClass");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "isMemberClass", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_isMemberClass failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_isPrimitive(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Class_isPrimitive");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "isPrimitive", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_isPrimitive failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_isSynthetic(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Class_isSynthetic");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "isSynthetic", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_isSynthetic failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_newInstance(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_newInstance");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "newInstance", "()Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_newInstance failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Class_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Class_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Class", "toString", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Class_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  java/lang/Class */



/*==== BEGIN  java/lang/Object */

jclass Object_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "Object_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "java/lang/Object", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t Object_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "Object_init");
      
   clazz = Object_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "()V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }    
   *obj = (*env)->NewObject(env, clazz, mid );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t Object_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Object_equals");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Object", "equals", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Object_equals failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Object_getClass(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Object_getClass");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Object", "getClass", "()Ljava/lang/Class;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Object_getClass failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Object_hashCode(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Object_hashCode");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Object", "hashCode", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "Object_hashCode failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Object_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Object_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Object", "toString", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Object_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  java/lang/Object */



/*==== BEGIN  java/lang/Long */

jclass Long_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "Long_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "java/lang/Long", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t Long_init(JNIEnv *env, jobject *obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;     jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "Long_init");
      
   clazz = Long_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(Ljava/lang/String;)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   } 
   if ( p0 != NULL ) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }   
   *obj = (*env)->NewObject(env, clazz, mid , p0_obj );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t Long_init_0(JNIEnv *env, jobject *obj , jlong p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "Long_init_0");
      
   clazz = Long_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(J)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }    
   *obj = (*env)->NewObject(env, clazz, mid , p0 );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t Long_static_MAX_VALUE(JNIEnv *env, jlong *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jlong field = 0;*/
   
   DENTER(BASIS_LAYER, "Long_static_MAX_VALUE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Long_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Long not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "MAX_VALUE", "J", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticLongField(env, clazz, mid);
   if (test_jni_error(env, "Long_static_MAX_VALUE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Long_static_MIN_VALUE(JNIEnv *env, jlong *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jlong field = 0;*/
   
   DENTER(BASIS_LAYER, "Long_static_MIN_VALUE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Long_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Long not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "MIN_VALUE", "J", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticLongField(env, clazz, mid);
   if (test_jni_error(env, "Long_static_MIN_VALUE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Long_static_SIZE(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Long_static_SIZE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Long_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Long not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "SIZE", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Long_static_SIZE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Long_static_TYPE(JNIEnv *env, jobject *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jobject field = NULL;*/
   
   DENTER(BASIS_LAYER, "Long_static_TYPE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Long_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Long not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "TYPE", "Ljava/lang/Class;", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticObjectField(env, clazz, mid);
   if (test_jni_error(env, "Long_static_TYPE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Long_static_bitCount(JNIEnv *env , jlong p0 , jint* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Long_static_bitCount");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "bitCount", "(J)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticIntMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Long_bitCount failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_static_decode(JNIEnv *env , const char* p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_static_decode");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "decode", "(Ljava/lang/String;)Ljava/lang/Long;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "Long_decode failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_static_getLong(JNIEnv *env , const char* p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_static_getLong");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "getLong", "(Ljava/lang/String;)Ljava/lang/Long;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "Long_getLong failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_static_getLong_0(JNIEnv *env , const char* p0 , jobject p1 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_static_getLong_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "getLong", "(Ljava/lang/String;Ljava/lang/Long;)Ljava/lang/Long;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj , p1 );  
   if (test_jni_error(env, "Long_getLong_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_static_getLong_1(JNIEnv *env , const char* p0 , jlong p1 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_static_getLong_1");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "getLong", "(Ljava/lang/String;J)Ljava/lang/Long;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj , p1 );  
   if (test_jni_error(env, "Long_getLong_1 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_static_highestOneBit(JNIEnv *env , jlong p0 , jlong* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "Long_static_highestOneBit");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "highestOneBit", "(J)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticLongMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Long_highestOneBit failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_static_lowestOneBit(JNIEnv *env , jlong p0 , jlong* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "Long_static_lowestOneBit");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "lowestOneBit", "(J)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticLongMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Long_lowestOneBit failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_static_numberOfLeadingZeros(JNIEnv *env , jlong p0 , jint* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Long_static_numberOfLeadingZeros");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "numberOfLeadingZeros", "(J)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticIntMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Long_numberOfLeadingZeros failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_static_numberOfTrailingZeros(JNIEnv *env , jlong p0 , jint* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Long_static_numberOfTrailingZeros");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "numberOfTrailingZeros", "(J)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticIntMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Long_numberOfTrailingZeros failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_static_parseLong(JNIEnv *env , const char* p0 , jlong* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jlong temp = 0;

   DENTER(BASIS_LAYER, "Long_static_parseLong");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "parseLong", "(Ljava/lang/String;)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticLongMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "Long_parseLong failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_static_parseLong_0(JNIEnv *env , const char* p0 , jint p1 , jlong* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jlong temp = 0;

   DENTER(BASIS_LAYER, "Long_static_parseLong_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "parseLong", "(Ljava/lang/String;I)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticLongMethod(env, clazz, mid , p0_obj , p1 );  
   if (test_jni_error(env, "Long_parseLong_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_static_reverse(JNIEnv *env , jlong p0 , jlong* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "Long_static_reverse");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "reverse", "(J)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticLongMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Long_reverse failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_static_reverseBytes(JNIEnv *env , jlong p0 , jlong* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "Long_static_reverseBytes");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "reverseBytes", "(J)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticLongMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Long_reverseBytes failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_static_rotateLeft(JNIEnv *env , jlong p0 , jint p1 , jlong* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "Long_static_rotateLeft");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "rotateLeft", "(JI)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticLongMethod(env, clazz, mid , p0 , p1 );  
   if (test_jni_error(env, "Long_rotateLeft failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_static_rotateRight(JNIEnv *env , jlong p0 , jint p1 , jlong* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "Long_static_rotateRight");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "rotateRight", "(JI)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticLongMethod(env, clazz, mid , p0 , p1 );  
   if (test_jni_error(env, "Long_rotateRight failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_static_signum(JNIEnv *env , jlong p0 , jint* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Long_static_signum");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "signum", "(J)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticIntMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Long_signum failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_static_toBinaryString(JNIEnv *env , jlong p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_static_toBinaryString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "toBinaryString", "(J)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Long_toBinaryString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_static_toHexString(JNIEnv *env , jlong p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_static_toHexString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "toHexString", "(J)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Long_toHexString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_static_toOctalString(JNIEnv *env , jlong p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_static_toOctalString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "toOctalString", "(J)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Long_toOctalString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_static_toString(JNIEnv *env , jlong p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_static_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "toString", "(J)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Long_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_static_toString_0(JNIEnv *env , jlong p0 , jint p1 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_static_toString_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "toString", "(JI)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 , p1 );  
   if (test_jni_error(env, "Long_toString_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_static_valueOf(JNIEnv *env , const char* p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_static_valueOf");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "valueOf", "(Ljava/lang/String;)Ljava/lang/Long;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "Long_valueOf failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_static_valueOf_0(JNIEnv *env , jlong p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_static_valueOf_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "valueOf", "(J)Ljava/lang/Long;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Long_valueOf_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_static_valueOf_1(JNIEnv *env , const char* p0 , jint p1 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_static_valueOf_1");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Long", "valueOf", "(Ljava/lang/String;I)Ljava/lang/Long;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj , p1 );  
   if (test_jni_error(env, "Long_valueOf_1 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Long_bitCount(JNIEnv *env, jobject obj , jlong p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Long_bitCount");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "bitCount", "(J)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Long_bitCount failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_byteValue(JNIEnv *env, jobject obj , jbyte* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jbyte temp = 0;

   DENTER(BASIS_LAYER, "Long_byteValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "byteValue", "()B", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallByteMethod(env, obj, mid);  
   if (test_jni_error(env, "Long_byteValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_compareTo(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Long_compareTo");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "compareTo", "(Ljava/lang/Long;)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Long_compareTo failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_compareTo_0(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Long_compareTo_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "compareTo", "(Ljava/lang/Object;)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Long_compareTo_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_decode(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_decode");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "decode", "(Ljava/lang/String;)Ljava/lang/Long;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "Long_decode failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_doubleValue(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "Long_doubleValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "doubleValue", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "Long_doubleValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Long_equals");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "equals", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Long_equals failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_floatValue(JNIEnv *env, jobject obj , jfloat* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jfloat temp = 0.0f;

   DENTER(BASIS_LAYER, "Long_floatValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0f;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "floatValue", "()F", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallFloatMethod(env, obj, mid);  
   if (test_jni_error(env, "Long_floatValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0f;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_getLong(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_getLong");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "getLong", "(Ljava/lang/String;)Ljava/lang/Long;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "Long_getLong failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_getLong_0(JNIEnv *env, jobject obj , const char* p0 , jobject p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_getLong_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "getLong", "(Ljava/lang/String;Ljava/lang/Long;)Ljava/lang/Long;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj, p1);  
   if (test_jni_error(env, "Long_getLong_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_getLong_1(JNIEnv *env, jobject obj , const char* p0 , jlong p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_getLong_1");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "getLong", "(Ljava/lang/String;J)Ljava/lang/Long;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj, p1);  
   if (test_jni_error(env, "Long_getLong_1 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_hashCode(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Long_hashCode");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "hashCode", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "Long_hashCode failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_highestOneBit(JNIEnv *env, jobject obj , jlong p0 , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "Long_highestOneBit");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "highestOneBit", "(J)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Long_highestOneBit failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_intValue(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Long_intValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "intValue", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "Long_intValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_longValue(JNIEnv *env, jobject obj , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "Long_longValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "longValue", "()J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid);  
   if (test_jni_error(env, "Long_longValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_lowestOneBit(JNIEnv *env, jobject obj , jlong p0 , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "Long_lowestOneBit");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "lowestOneBit", "(J)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Long_lowestOneBit failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_numberOfLeadingZeros(JNIEnv *env, jobject obj , jlong p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Long_numberOfLeadingZeros");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "numberOfLeadingZeros", "(J)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Long_numberOfLeadingZeros failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_numberOfTrailingZeros(JNIEnv *env, jobject obj , jlong p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Long_numberOfTrailingZeros");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "numberOfTrailingZeros", "(J)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Long_numberOfTrailingZeros failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_parseLong(JNIEnv *env, jobject obj , const char* p0 , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jlong temp = 0;

   DENTER(BASIS_LAYER, "Long_parseLong");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "parseLong", "(Ljava/lang/String;)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallLongMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "Long_parseLong failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_parseLong_0(JNIEnv *env, jobject obj , const char* p0 , jint p1 , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jlong temp = 0;

   DENTER(BASIS_LAYER, "Long_parseLong_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "parseLong", "(Ljava/lang/String;I)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallLongMethod(env, obj, mid, p0_obj, p1);  
   if (test_jni_error(env, "Long_parseLong_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_reverse(JNIEnv *env, jobject obj , jlong p0 , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "Long_reverse");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "reverse", "(J)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Long_reverse failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_reverseBytes(JNIEnv *env, jobject obj , jlong p0 , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "Long_reverseBytes");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "reverseBytes", "(J)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Long_reverseBytes failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_rotateLeft(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "Long_rotateLeft");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "rotateLeft", "(JI)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "Long_rotateLeft failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_rotateRight(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "Long_rotateRight");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "rotateRight", "(JI)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "Long_rotateRight failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_shortValue(JNIEnv *env, jobject obj , jshort* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jshort temp = 0;

   DENTER(BASIS_LAYER, "Long_shortValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "shortValue", "()S", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallShortMethod(env, obj, mid);  
   if (test_jni_error(env, "Long_shortValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_signum(JNIEnv *env, jobject obj , jlong p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Long_signum");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "signum", "(J)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Long_signum failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_toBinaryString(JNIEnv *env, jobject obj , jlong p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_toBinaryString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "toBinaryString", "(J)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Long_toBinaryString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_toHexString(JNIEnv *env, jobject obj , jlong p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_toHexString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "toHexString", "(J)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Long_toHexString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_toOctalString(JNIEnv *env, jobject obj , jlong p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_toOctalString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "toOctalString", "(J)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Long_toOctalString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "toString", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Long_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_toString_0(JNIEnv *env, jobject obj , jlong p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_toString_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "toString", "(J)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Long_toString_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_toString_1(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_toString_1");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "toString", "(JI)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "Long_toString_1 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_valueOf(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_valueOf");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "valueOf", "(Ljava/lang/String;)Ljava/lang/Long;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "Long_valueOf failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_valueOf_0(JNIEnv *env, jobject obj , jlong p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_valueOf_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "valueOf", "(J)Ljava/lang/Long;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Long_valueOf_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Long_valueOf_1(JNIEnv *env, jobject obj , const char* p0 , jint p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Long_valueOf_1");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Long", "valueOf", "(Ljava/lang/String;I)Ljava/lang/Long;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj, p1);  
   if (test_jni_error(env, "Long_valueOf_1 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  java/lang/Long */



/*==== BEGIN  java/lang/Integer */

jclass Integer_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "Integer_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "java/lang/Integer", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t Integer_init(JNIEnv *env, jobject *obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "Integer_init");
      
   clazz = Integer_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(I)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }    
   *obj = (*env)->NewObject(env, clazz, mid , p0 );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t Integer_init_0(JNIEnv *env, jobject *obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;     jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "Integer_init_0");
      
   clazz = Integer_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(Ljava/lang/String;)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   } 
   if ( p0 != NULL ) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }   
   *obj = (*env)->NewObject(env, clazz, mid , p0_obj );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t Integer_static_MAX_VALUE(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Integer_static_MAX_VALUE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Integer_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Integer not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "MAX_VALUE", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Integer_static_MAX_VALUE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Integer_static_MIN_VALUE(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Integer_static_MIN_VALUE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Integer_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Integer not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "MIN_VALUE", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Integer_static_MIN_VALUE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Integer_static_SIZE(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Integer_static_SIZE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Integer_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Integer not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "SIZE", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Integer_static_SIZE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Integer_static_TYPE(JNIEnv *env, jobject *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jobject field = NULL;*/
   
   DENTER(BASIS_LAYER, "Integer_static_TYPE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Integer_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Integer not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "TYPE", "Ljava/lang/Class;", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticObjectField(env, clazz, mid);
   if (test_jni_error(env, "Integer_static_TYPE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Integer_static_bitCount(JNIEnv *env , jint p0 , jint* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_static_bitCount");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "bitCount", "(I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticIntMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Integer_bitCount failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_static_decode(JNIEnv *env , const char* p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_static_decode");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "decode", "(Ljava/lang/String;)Ljava/lang/Integer;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "Integer_decode failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_static_getInteger(JNIEnv *env , const char* p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_static_getInteger");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "getInteger", "(Ljava/lang/String;)Ljava/lang/Integer;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "Integer_getInteger failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_static_getInteger_0(JNIEnv *env , const char* p0 , jint p1 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_static_getInteger_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "getInteger", "(Ljava/lang/String;I)Ljava/lang/Integer;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj , p1 );  
   if (test_jni_error(env, "Integer_getInteger_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_static_getInteger_1(JNIEnv *env , const char* p0 , jobject p1 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_static_getInteger_1");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "getInteger", "(Ljava/lang/String;Ljava/lang/Integer;)Ljava/lang/Integer;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj , p1 );  
   if (test_jni_error(env, "Integer_getInteger_1 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_static_highestOneBit(JNIEnv *env , jint p0 , jint* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_static_highestOneBit");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "highestOneBit", "(I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticIntMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Integer_highestOneBit failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_static_lowestOneBit(JNIEnv *env , jint p0 , jint* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_static_lowestOneBit");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "lowestOneBit", "(I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticIntMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Integer_lowestOneBit failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_static_numberOfLeadingZeros(JNIEnv *env , jint p0 , jint* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_static_numberOfLeadingZeros");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "numberOfLeadingZeros", "(I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticIntMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Integer_numberOfLeadingZeros failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_static_numberOfTrailingZeros(JNIEnv *env , jint p0 , jint* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_static_numberOfTrailingZeros");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "numberOfTrailingZeros", "(I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticIntMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Integer_numberOfTrailingZeros failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_static_parseInt(JNIEnv *env , const char* p0 , jint* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_static_parseInt");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "parseInt", "(Ljava/lang/String;)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticIntMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "Integer_parseInt failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_static_parseInt_0(JNIEnv *env , const char* p0 , jint p1 , jint* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_static_parseInt_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "parseInt", "(Ljava/lang/String;I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticIntMethod(env, clazz, mid , p0_obj , p1 );  
   if (test_jni_error(env, "Integer_parseInt_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_static_reverse(JNIEnv *env , jint p0 , jint* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_static_reverse");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "reverse", "(I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticIntMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Integer_reverse failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_static_reverseBytes(JNIEnv *env , jint p0 , jint* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_static_reverseBytes");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "reverseBytes", "(I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticIntMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Integer_reverseBytes failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_static_rotateLeft(JNIEnv *env , jint p0 , jint p1 , jint* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_static_rotateLeft");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "rotateLeft", "(II)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticIntMethod(env, clazz, mid , p0 , p1 );  
   if (test_jni_error(env, "Integer_rotateLeft failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_static_rotateRight(JNIEnv *env , jint p0 , jint p1 , jint* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_static_rotateRight");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "rotateRight", "(II)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticIntMethod(env, clazz, mid , p0 , p1 );  
   if (test_jni_error(env, "Integer_rotateRight failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_static_signum(JNIEnv *env , jint p0 , jint* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_static_signum");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "signum", "(I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticIntMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Integer_signum failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_static_toBinaryString(JNIEnv *env , jint p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_static_toBinaryString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "toBinaryString", "(I)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Integer_toBinaryString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_static_toHexString(JNIEnv *env , jint p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_static_toHexString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "toHexString", "(I)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Integer_toHexString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_static_toOctalString(JNIEnv *env , jint p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_static_toOctalString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "toOctalString", "(I)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Integer_toOctalString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_static_toString(JNIEnv *env , jint p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_static_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "toString", "(I)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Integer_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_static_toString_0(JNIEnv *env , jint p0 , jint p1 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_static_toString_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "toString", "(II)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 , p1 );  
   if (test_jni_error(env, "Integer_toString_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_static_valueOf(JNIEnv *env , jint p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_static_valueOf");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "valueOf", "(I)Ljava/lang/Integer;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Integer_valueOf failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_static_valueOf_0(JNIEnv *env , const char* p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_static_valueOf_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "valueOf", "(Ljava/lang/String;)Ljava/lang/Integer;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "Integer_valueOf_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_static_valueOf_1(JNIEnv *env , const char* p0 , jint p1 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_static_valueOf_1");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Integer", "valueOf", "(Ljava/lang/String;I)Ljava/lang/Integer;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj , p1 );  
   if (test_jni_error(env, "Integer_valueOf_1 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Integer_bitCount(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_bitCount");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "bitCount", "(I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Integer_bitCount failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_byteValue(JNIEnv *env, jobject obj , jbyte* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jbyte temp = 0;

   DENTER(BASIS_LAYER, "Integer_byteValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "byteValue", "()B", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallByteMethod(env, obj, mid);  
   if (test_jni_error(env, "Integer_byteValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_compareTo(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_compareTo");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "compareTo", "(Ljava/lang/Integer;)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Integer_compareTo failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_compareTo_0(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_compareTo_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "compareTo", "(Ljava/lang/Object;)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Integer_compareTo_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_decode(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_decode");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "decode", "(Ljava/lang/String;)Ljava/lang/Integer;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "Integer_decode failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_doubleValue(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "Integer_doubleValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "doubleValue", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "Integer_doubleValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Integer_equals");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "equals", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Integer_equals failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_floatValue(JNIEnv *env, jobject obj , jfloat* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jfloat temp = 0.0f;

   DENTER(BASIS_LAYER, "Integer_floatValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0f;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "floatValue", "()F", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallFloatMethod(env, obj, mid);  
   if (test_jni_error(env, "Integer_floatValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0f;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_getInteger(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_getInteger");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "getInteger", "(Ljava/lang/String;)Ljava/lang/Integer;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "Integer_getInteger failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_getInteger_0(JNIEnv *env, jobject obj , const char* p0 , jint p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_getInteger_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "getInteger", "(Ljava/lang/String;I)Ljava/lang/Integer;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj, p1);  
   if (test_jni_error(env, "Integer_getInteger_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_getInteger_1(JNIEnv *env, jobject obj , const char* p0 , jobject p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_getInteger_1");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "getInteger", "(Ljava/lang/String;Ljava/lang/Integer;)Ljava/lang/Integer;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj, p1);  
   if (test_jni_error(env, "Integer_getInteger_1 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_hashCode(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_hashCode");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "hashCode", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "Integer_hashCode failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_highestOneBit(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_highestOneBit");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "highestOneBit", "(I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Integer_highestOneBit failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_intValue(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_intValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "intValue", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "Integer_intValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_longValue(JNIEnv *env, jobject obj , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "Integer_longValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "longValue", "()J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid);  
   if (test_jni_error(env, "Integer_longValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_lowestOneBit(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_lowestOneBit");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "lowestOneBit", "(I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Integer_lowestOneBit failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_numberOfLeadingZeros(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_numberOfLeadingZeros");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "numberOfLeadingZeros", "(I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Integer_numberOfLeadingZeros failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_numberOfTrailingZeros(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_numberOfTrailingZeros");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "numberOfTrailingZeros", "(I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Integer_numberOfTrailingZeros failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_parseInt(JNIEnv *env, jobject obj , const char* p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_parseInt");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "parseInt", "(Ljava/lang/String;)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallIntMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "Integer_parseInt failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_parseInt_0(JNIEnv *env, jobject obj , const char* p0 , jint p1 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_parseInt_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "parseInt", "(Ljava/lang/String;I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallIntMethod(env, obj, mid, p0_obj, p1);  
   if (test_jni_error(env, "Integer_parseInt_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_reverse(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_reverse");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "reverse", "(I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Integer_reverse failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_reverseBytes(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_reverseBytes");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "reverseBytes", "(I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Integer_reverseBytes failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_rotateLeft(JNIEnv *env, jobject obj , jint p0 , jint p1 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_rotateLeft");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "rotateLeft", "(II)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "Integer_rotateLeft failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_rotateRight(JNIEnv *env, jobject obj , jint p0 , jint p1 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_rotateRight");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "rotateRight", "(II)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "Integer_rotateRight failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_shortValue(JNIEnv *env, jobject obj , jshort* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jshort temp = 0;

   DENTER(BASIS_LAYER, "Integer_shortValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "shortValue", "()S", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallShortMethod(env, obj, mid);  
   if (test_jni_error(env, "Integer_shortValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_signum(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Integer_signum");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "signum", "(I)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Integer_signum failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_toBinaryString(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_toBinaryString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "toBinaryString", "(I)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Integer_toBinaryString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_toHexString(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_toHexString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "toHexString", "(I)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Integer_toHexString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_toOctalString(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_toOctalString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "toOctalString", "(I)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Integer_toOctalString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "toString", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Integer_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_toString_0(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_toString_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "toString", "(I)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Integer_toString_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_toString_1(JNIEnv *env, jobject obj , jint p0 , jint p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_toString_1");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "toString", "(II)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "Integer_toString_1 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_valueOf(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_valueOf");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "valueOf", "(I)Ljava/lang/Integer;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Integer_valueOf failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_valueOf_0(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_valueOf_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "valueOf", "(Ljava/lang/String;)Ljava/lang/Integer;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "Integer_valueOf_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Integer_valueOf_1(JNIEnv *env, jobject obj , const char* p0 , jint p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Integer_valueOf_1");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Integer", "valueOf", "(Ljava/lang/String;I)Ljava/lang/Integer;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj, p1);  
   if (test_jni_error(env, "Integer_valueOf_1 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  java/lang/Integer */



/*==== BEGIN  java/lang/Boolean */

jclass Boolean_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "Boolean_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "java/lang/Boolean", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t Boolean_init(JNIEnv *env, jobject *obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "Boolean_init");
      
   clazz = Boolean_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(Z)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }    
   *obj = (*env)->NewObject(env, clazz, mid , p0 );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t Boolean_init_0(JNIEnv *env, jobject *obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;     jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "Boolean_init_0");
      
   clazz = Boolean_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(Ljava/lang/String;)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   } 
   if ( p0 != NULL ) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }   
   *obj = (*env)->NewObject(env, clazz, mid , p0_obj );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t Boolean_static_FALSE(JNIEnv *env, jobject *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jobject field = NULL;*/
   
   DENTER(BASIS_LAYER, "Boolean_static_FALSE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Boolean_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Boolean not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "FALSE", "Ljava/lang/Boolean;", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticObjectField(env, clazz, mid);
   if (test_jni_error(env, "Boolean_static_FALSE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Boolean_static_TRUE(JNIEnv *env, jobject *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jobject field = NULL;*/
   
   DENTER(BASIS_LAYER, "Boolean_static_TRUE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Boolean_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Boolean not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "TRUE", "Ljava/lang/Boolean;", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticObjectField(env, clazz, mid);
   if (test_jni_error(env, "Boolean_static_TRUE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Boolean_static_TYPE(JNIEnv *env, jobject *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jobject field = NULL;*/
   
   DENTER(BASIS_LAYER, "Boolean_static_TYPE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Boolean_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Boolean not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "TYPE", "Ljava/lang/Class;", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticObjectField(env, clazz, mid);
   if (test_jni_error(env, "Boolean_static_TYPE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Boolean_static_getBoolean(JNIEnv *env , const char* p0 , jboolean* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Boolean_static_getBoolean");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Boolean", "getBoolean", "(Ljava/lang/String;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticBooleanMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "Boolean_getBoolean failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Boolean_static_parseBoolean(JNIEnv *env , const char* p0 , jboolean* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Boolean_static_parseBoolean");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Boolean", "parseBoolean", "(Ljava/lang/String;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticBooleanMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "Boolean_parseBoolean failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Boolean_static_toString(JNIEnv *env , jboolean p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Boolean_static_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Boolean", "toString", "(Z)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Boolean_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Boolean_static_valueOf(JNIEnv *env , jboolean p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Boolean_static_valueOf");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Boolean", "valueOf", "(Z)Ljava/lang/Boolean;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Boolean_valueOf failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Boolean_static_valueOf_0(JNIEnv *env , const char* p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Boolean_static_valueOf_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Boolean", "valueOf", "(Ljava/lang/String;)Ljava/lang/Boolean;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "Boolean_valueOf_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Boolean_booleanValue(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Boolean_booleanValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Boolean", "booleanValue", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "Boolean_booleanValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Boolean_compareTo(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Boolean_compareTo");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Boolean", "compareTo", "(Ljava/lang/Boolean;)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Boolean_compareTo failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Boolean_compareTo_0(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Boolean_compareTo_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Boolean", "compareTo", "(Ljava/lang/Object;)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Boolean_compareTo_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Boolean_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Boolean_equals");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Boolean", "equals", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Boolean_equals failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Boolean_getBoolean(JNIEnv *env, jobject obj , const char* p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Boolean_getBoolean");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Boolean", "getBoolean", "(Ljava/lang/String;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallBooleanMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "Boolean_getBoolean failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Boolean_hashCode(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Boolean_hashCode");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Boolean", "hashCode", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "Boolean_hashCode failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Boolean_parseBoolean(JNIEnv *env, jobject obj , const char* p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Boolean_parseBoolean");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Boolean", "parseBoolean", "(Ljava/lang/String;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallBooleanMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "Boolean_parseBoolean failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Boolean_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Boolean_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Boolean", "toString", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Boolean_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Boolean_toString_0(JNIEnv *env, jobject obj , jboolean p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Boolean_toString_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Boolean", "toString", "(Z)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Boolean_toString_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Boolean_valueOf(JNIEnv *env, jobject obj , jboolean p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Boolean_valueOf");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Boolean", "valueOf", "(Z)Ljava/lang/Boolean;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Boolean_valueOf failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Boolean_valueOf_0(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Boolean_valueOf_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Boolean", "valueOf", "(Ljava/lang/String;)Ljava/lang/Boolean;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "Boolean_valueOf_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  java/lang/Boolean */



/*==== BEGIN  java/lang/Double */

jclass Double_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "Double_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "java/lang/Double", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t Double_init(JNIEnv *env, jobject *obj , jdouble p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "Double_init");
      
   clazz = Double_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(D)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }    
   *obj = (*env)->NewObject(env, clazz, mid , p0 );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t Double_init_0(JNIEnv *env, jobject *obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;     jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "Double_init_0");
      
   clazz = Double_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(Ljava/lang/String;)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   } 
   if ( p0 != NULL ) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }   
   *obj = (*env)->NewObject(env, clazz, mid , p0_obj );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t Double_static_MAX_EXPONENT(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Double_static_MAX_EXPONENT");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Double_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Double not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "MAX_EXPONENT", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Double_static_MAX_EXPONENT failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Double_static_MAX_VALUE(JNIEnv *env, jdouble *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jdouble field = 0.0;*/
   
   DENTER(BASIS_LAYER, "Double_static_MAX_VALUE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Double_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Double not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "MAX_VALUE", "D", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticDoubleField(env, clazz, mid);
   if (test_jni_error(env, "Double_static_MAX_VALUE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Double_static_MIN_EXPONENT(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Double_static_MIN_EXPONENT");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Double_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Double not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "MIN_EXPONENT", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Double_static_MIN_EXPONENT failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Double_static_MIN_NORMAL(JNIEnv *env, jdouble *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jdouble field = 0.0;*/
   
   DENTER(BASIS_LAYER, "Double_static_MIN_NORMAL");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Double_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Double not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "MIN_NORMAL", "D", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticDoubleField(env, clazz, mid);
   if (test_jni_error(env, "Double_static_MIN_NORMAL failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Double_static_MIN_VALUE(JNIEnv *env, jdouble *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jdouble field = 0.0;*/
   
   DENTER(BASIS_LAYER, "Double_static_MIN_VALUE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Double_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Double not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "MIN_VALUE", "D", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticDoubleField(env, clazz, mid);
   if (test_jni_error(env, "Double_static_MIN_VALUE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Double_static_NEGATIVE_INFINITY(JNIEnv *env, jdouble *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jdouble field = 0.0;*/
   
   DENTER(BASIS_LAYER, "Double_static_NEGATIVE_INFINITY");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Double_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Double not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "NEGATIVE_INFINITY", "D", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticDoubleField(env, clazz, mid);
   if (test_jni_error(env, "Double_static_NEGATIVE_INFINITY failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Double_static_NaN(JNIEnv *env, jdouble *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jdouble field = 0.0;*/
   
   DENTER(BASIS_LAYER, "Double_static_NaN");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Double_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Double not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "NaN", "D", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticDoubleField(env, clazz, mid);
   if (test_jni_error(env, "Double_static_NaN failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Double_static_POSITIVE_INFINITY(JNIEnv *env, jdouble *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jdouble field = 0.0;*/
   
   DENTER(BASIS_LAYER, "Double_static_POSITIVE_INFINITY");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Double_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Double not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "POSITIVE_INFINITY", "D", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticDoubleField(env, clazz, mid);
   if (test_jni_error(env, "Double_static_POSITIVE_INFINITY failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Double_static_SIZE(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Double_static_SIZE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Double_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Double not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "SIZE", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Double_static_SIZE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Double_static_TYPE(JNIEnv *env, jobject *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jobject field = NULL;*/
   
   DENTER(BASIS_LAYER, "Double_static_TYPE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Double_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Double not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "TYPE", "Ljava/lang/Class;", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticObjectField(env, clazz, mid);
   if (test_jni_error(env, "Double_static_TYPE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Double_static_compare(JNIEnv *env , jdouble p0 , jdouble p1 , jint* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Double_static_compare");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Double", "compare", "(DD)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticIntMethod(env, clazz, mid , p0 , p1 );  
   if (test_jni_error(env, "Double_compare failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Double_static_doubleToLongBits(JNIEnv *env , jdouble p0 , jlong* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "Double_static_doubleToLongBits");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Double", "doubleToLongBits", "(D)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticLongMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Double_doubleToLongBits failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Double_static_doubleToRawLongBits(JNIEnv *env , jdouble p0 , jlong* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "Double_static_doubleToRawLongBits");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Double", "doubleToRawLongBits", "(D)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticLongMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Double_doubleToRawLongBits failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Double_static_isInfinite(JNIEnv *env , jdouble p0 , jboolean* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Double_static_isInfinite");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Double", "isInfinite", "(D)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticBooleanMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Double_isInfinite failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Double_static_isNaN(JNIEnv *env , jdouble p0 , jboolean* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Double_static_isNaN");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Double", "isNaN", "(D)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticBooleanMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Double_isNaN failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Double_static_longBitsToDouble(JNIEnv *env , jlong p0 , jdouble* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "Double_static_longBitsToDouble");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Double", "longBitsToDouble", "(J)D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticDoubleMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Double_longBitsToDouble failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Double_static_parseDouble(JNIEnv *env , const char* p0 , jdouble* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "Double_static_parseDouble");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Double", "parseDouble", "(Ljava/lang/String;)D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticDoubleMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "Double_parseDouble failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Double_static_toHexString(JNIEnv *env , jdouble p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Double_static_toHexString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Double", "toHexString", "(D)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Double_toHexString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Double_static_toString(JNIEnv *env , jdouble p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Double_static_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Double", "toString", "(D)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Double_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Double_static_valueOf(JNIEnv *env , jdouble p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Double_static_valueOf");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Double", "valueOf", "(D)Ljava/lang/Double;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Double_valueOf failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Double_static_valueOf_0(JNIEnv *env , const char* p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Double_static_valueOf_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Double", "valueOf", "(Ljava/lang/String;)Ljava/lang/Double;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "Double_valueOf_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Double_byteValue(JNIEnv *env, jobject obj , jbyte* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jbyte temp = 0;

   DENTER(BASIS_LAYER, "Double_byteValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "byteValue", "()B", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallByteMethod(env, obj, mid);  
   if (test_jni_error(env, "Double_byteValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Double_compare(JNIEnv *env, jobject obj , jdouble p0 , jdouble p1 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Double_compare");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "compare", "(DD)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "Double_compare failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Double_compareTo(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Double_compareTo");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "compareTo", "(Ljava/lang/Double;)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Double_compareTo failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Double_compareTo_0(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Double_compareTo_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "compareTo", "(Ljava/lang/Object;)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Double_compareTo_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Double_doubleToLongBits(JNIEnv *env, jobject obj , jdouble p0 , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "Double_doubleToLongBits");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "doubleToLongBits", "(D)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Double_doubleToLongBits failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Double_doubleToRawLongBits(JNIEnv *env, jobject obj , jdouble p0 , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "Double_doubleToRawLongBits");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "doubleToRawLongBits", "(D)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Double_doubleToRawLongBits failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Double_doubleValue(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "Double_doubleValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "doubleValue", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "Double_doubleValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Double_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Double_equals");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "equals", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Double_equals failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Double_floatValue(JNIEnv *env, jobject obj , jfloat* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jfloat temp = 0.0f;

   DENTER(BASIS_LAYER, "Double_floatValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0f;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "floatValue", "()F", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallFloatMethod(env, obj, mid);  
   if (test_jni_error(env, "Double_floatValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0f;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Double_hashCode(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Double_hashCode");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "hashCode", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "Double_hashCode failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Double_intValue(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Double_intValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "intValue", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "Double_intValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Double_isInfinite(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Double_isInfinite");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "isInfinite", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "Double_isInfinite failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Double_isInfinite_0(JNIEnv *env, jobject obj , jdouble p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Double_isInfinite_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "isInfinite", "(D)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Double_isInfinite_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Double_isNaN(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Double_isNaN");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "isNaN", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "Double_isNaN failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Double_isNaN_0(JNIEnv *env, jobject obj , jdouble p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Double_isNaN_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "isNaN", "(D)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Double_isNaN_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Double_longBitsToDouble(JNIEnv *env, jobject obj , jlong p0 , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "Double_longBitsToDouble");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "longBitsToDouble", "(J)D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Double_longBitsToDouble failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Double_longValue(JNIEnv *env, jobject obj , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "Double_longValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "longValue", "()J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid);  
   if (test_jni_error(env, "Double_longValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Double_parseDouble(JNIEnv *env, jobject obj , const char* p0 , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "Double_parseDouble");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "parseDouble", "(Ljava/lang/String;)D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallDoubleMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "Double_parseDouble failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Double_shortValue(JNIEnv *env, jobject obj , jshort* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jshort temp = 0;

   DENTER(BASIS_LAYER, "Double_shortValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "shortValue", "()S", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallShortMethod(env, obj, mid);  
   if (test_jni_error(env, "Double_shortValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Double_toHexString(JNIEnv *env, jobject obj , jdouble p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Double_toHexString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "toHexString", "(D)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Double_toHexString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Double_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Double_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "toString", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Double_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Double_toString_0(JNIEnv *env, jobject obj , jdouble p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Double_toString_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "toString", "(D)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Double_toString_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Double_valueOf(JNIEnv *env, jobject obj , jdouble p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Double_valueOf");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "valueOf", "(D)Ljava/lang/Double;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Double_valueOf failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Double_valueOf_0(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Double_valueOf_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Double", "valueOf", "(Ljava/lang/String;)Ljava/lang/Double;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "Double_valueOf_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  java/lang/Double */



/*==== BEGIN  java/lang/Float */

jclass Float_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "Float_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "java/lang/Float", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t Float_init(JNIEnv *env, jobject *obj , jdouble p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "Float_init");
      
   clazz = Float_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(D)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }    
   *obj = (*env)->NewObject(env, clazz, mid , p0 );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t Float_init_0(JNIEnv *env, jobject *obj , jfloat p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "Float_init_0");
      
   clazz = Float_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(F)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }    
   *obj = (*env)->NewObject(env, clazz, mid , p0 );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t Float_init_1(JNIEnv *env, jobject *obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;     jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "Float_init_1");
      
   clazz = Float_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(Ljava/lang/String;)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   } 
   if ( p0 != NULL ) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }   
   *obj = (*env)->NewObject(env, clazz, mid , p0_obj );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t Float_static_MAX_EXPONENT(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Float_static_MAX_EXPONENT");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Float_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Float not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "MAX_EXPONENT", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Float_static_MAX_EXPONENT failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Float_static_MAX_VALUE(JNIEnv *env, jfloat *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jfloat field = 0.0f;*/
   
   DENTER(BASIS_LAYER, "Float_static_MAX_VALUE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Float_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Float not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "MAX_VALUE", "F", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticFloatField(env, clazz, mid);
   if (test_jni_error(env, "Float_static_MAX_VALUE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Float_static_MIN_EXPONENT(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Float_static_MIN_EXPONENT");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Float_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Float not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "MIN_EXPONENT", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Float_static_MIN_EXPONENT failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Float_static_MIN_NORMAL(JNIEnv *env, jfloat *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jfloat field = 0.0f;*/
   
   DENTER(BASIS_LAYER, "Float_static_MIN_NORMAL");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Float_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Float not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "MIN_NORMAL", "F", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticFloatField(env, clazz, mid);
   if (test_jni_error(env, "Float_static_MIN_NORMAL failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Float_static_MIN_VALUE(JNIEnv *env, jfloat *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jfloat field = 0.0f;*/
   
   DENTER(BASIS_LAYER, "Float_static_MIN_VALUE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Float_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Float not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "MIN_VALUE", "F", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticFloatField(env, clazz, mid);
   if (test_jni_error(env, "Float_static_MIN_VALUE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Float_static_NEGATIVE_INFINITY(JNIEnv *env, jfloat *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jfloat field = 0.0f;*/
   
   DENTER(BASIS_LAYER, "Float_static_NEGATIVE_INFINITY");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Float_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Float not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "NEGATIVE_INFINITY", "F", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticFloatField(env, clazz, mid);
   if (test_jni_error(env, "Float_static_NEGATIVE_INFINITY failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Float_static_NaN(JNIEnv *env, jfloat *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jfloat field = 0.0f;*/
   
   DENTER(BASIS_LAYER, "Float_static_NaN");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Float_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Float not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "NaN", "F", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticFloatField(env, clazz, mid);
   if (test_jni_error(env, "Float_static_NaN failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Float_static_POSITIVE_INFINITY(JNIEnv *env, jfloat *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jfloat field = 0.0f;*/
   
   DENTER(BASIS_LAYER, "Float_static_POSITIVE_INFINITY");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Float_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Float not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "POSITIVE_INFINITY", "F", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticFloatField(env, clazz, mid);
   if (test_jni_error(env, "Float_static_POSITIVE_INFINITY failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Float_static_SIZE(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "Float_static_SIZE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Float_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Float not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "SIZE", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "Float_static_SIZE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Float_static_TYPE(JNIEnv *env, jobject *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jobject field = NULL;*/
   
   DENTER(BASIS_LAYER, "Float_static_TYPE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = Float_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class java/lang/Float not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "TYPE", "Ljava/lang/Class;", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticObjectField(env, clazz, mid);
   if (test_jni_error(env, "Float_static_TYPE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t Float_static_compare(JNIEnv *env , jfloat p0 , jfloat p1 , jint* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Float_static_compare");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Float", "compare", "(FF)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticIntMethod(env, clazz, mid , p0 , p1 );  
   if (test_jni_error(env, "Float_compare failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Float_static_floatToIntBits(JNIEnv *env , jfloat p0 , jint* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Float_static_floatToIntBits");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Float", "floatToIntBits", "(F)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticIntMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Float_floatToIntBits failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Float_static_floatToRawIntBits(JNIEnv *env , jfloat p0 , jint* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Float_static_floatToRawIntBits");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Float", "floatToRawIntBits", "(F)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticIntMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Float_floatToRawIntBits failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Float_static_intBitsToFloat(JNIEnv *env , jint p0 , jfloat* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jfloat temp = 0.0f;

   DENTER(BASIS_LAYER, "Float_static_intBitsToFloat");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0f;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Float", "intBitsToFloat", "(I)F", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticFloatMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Float_intBitsToFloat failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0f;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Float_static_isInfinite(JNIEnv *env , jfloat p0 , jboolean* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Float_static_isInfinite");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Float", "isInfinite", "(F)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticBooleanMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Float_isInfinite failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Float_static_isNaN(JNIEnv *env , jfloat p0 , jboolean* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Float_static_isNaN");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Float", "isNaN", "(F)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticBooleanMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Float_isNaN failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Float_static_parseFloat(JNIEnv *env , const char* p0 , jfloat* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jfloat temp = 0.0f;

   DENTER(BASIS_LAYER, "Float_static_parseFloat");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0f;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Float", "parseFloat", "(Ljava/lang/String;)F", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticFloatMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "Float_parseFloat failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0f;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Float_static_toHexString(JNIEnv *env , jfloat p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Float_static_toHexString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Float", "toHexString", "(F)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Float_toHexString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Float_static_toString(JNIEnv *env , jfloat p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Float_static_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Float", "toString", "(F)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Float_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Float_static_valueOf(JNIEnv *env , jfloat p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Float_static_valueOf");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Float", "valueOf", "(F)Ljava/lang/Float;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Float_valueOf failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Float_static_valueOf_0(JNIEnv *env , const char* p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Float_static_valueOf_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "java/lang/Float", "valueOf", "(Ljava/lang/String;)Ljava/lang/Float;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "Float_valueOf_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Float_byteValue(JNIEnv *env, jobject obj , jbyte* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jbyte temp = 0;

   DENTER(BASIS_LAYER, "Float_byteValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "byteValue", "()B", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallByteMethod(env, obj, mid);  
   if (test_jni_error(env, "Float_byteValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Float_compare(JNIEnv *env, jobject obj , jfloat p0 , jfloat p1 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Float_compare");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "compare", "(FF)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "Float_compare failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Float_compareTo(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Float_compareTo");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "compareTo", "(Ljava/lang/Float;)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Float_compareTo failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Float_compareTo_0(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Float_compareTo_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "compareTo", "(Ljava/lang/Object;)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Float_compareTo_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Float_doubleValue(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "Float_doubleValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "doubleValue", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "Float_doubleValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Float_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Float_equals");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "equals", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Float_equals failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Float_floatToIntBits(JNIEnv *env, jobject obj , jfloat p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Float_floatToIntBits");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "floatToIntBits", "(F)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Float_floatToIntBits failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Float_floatToRawIntBits(JNIEnv *env, jobject obj , jfloat p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Float_floatToRawIntBits");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "floatToRawIntBits", "(F)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Float_floatToRawIntBits failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Float_floatValue(JNIEnv *env, jobject obj , jfloat* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jfloat temp = 0.0f;

   DENTER(BASIS_LAYER, "Float_floatValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0f;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "floatValue", "()F", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallFloatMethod(env, obj, mid);  
   if (test_jni_error(env, "Float_floatValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0f;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Float_hashCode(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Float_hashCode");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "hashCode", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "Float_hashCode failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Float_intBitsToFloat(JNIEnv *env, jobject obj , jint p0 , jfloat* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jfloat temp = 0.0f;

   DENTER(BASIS_LAYER, "Float_intBitsToFloat");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0f;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "intBitsToFloat", "(I)F", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallFloatMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Float_intBitsToFloat failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0f;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Float_intValue(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Float_intValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "intValue", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "Float_intValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Float_isInfinite(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Float_isInfinite");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "isInfinite", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "Float_isInfinite failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Float_isInfinite_0(JNIEnv *env, jobject obj , jfloat p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Float_isInfinite_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "isInfinite", "(F)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Float_isInfinite_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Float_isNaN(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Float_isNaN");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "isNaN", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "Float_isNaN failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Float_isNaN_0(JNIEnv *env, jobject obj , jfloat p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "Float_isNaN_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "isNaN", "(F)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Float_isNaN_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Float_longValue(JNIEnv *env, jobject obj , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "Float_longValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "longValue", "()J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid);  
   if (test_jni_error(env, "Float_longValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Float_parseFloat(JNIEnv *env, jobject obj , const char* p0 , jfloat* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jfloat temp = 0.0f;

   DENTER(BASIS_LAYER, "Float_parseFloat");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0f;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "parseFloat", "(Ljava/lang/String;)F", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallFloatMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "Float_parseFloat failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0f;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Float_shortValue(JNIEnv *env, jobject obj , jshort* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jshort temp = 0;

   DENTER(BASIS_LAYER, "Float_shortValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "shortValue", "()S", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallShortMethod(env, obj, mid);  
   if (test_jni_error(env, "Float_shortValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Float_toHexString(JNIEnv *env, jobject obj , jfloat p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Float_toHexString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "toHexString", "(F)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Float_toHexString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Float_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Float_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "toString", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Float_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Float_toString_0(JNIEnv *env, jobject obj , jfloat p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Float_toString_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "toString", "(F)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Float_toString_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Float_valueOf(JNIEnv *env, jobject obj , jfloat p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Float_valueOf");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "valueOf", "(F)Ljava/lang/Float;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Float_valueOf failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Float_valueOf_0(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Float_valueOf_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Float", "valueOf", "(Ljava/lang/String;)Ljava/lang/Float;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "Float_valueOf_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  java/lang/Float */



/*==== BEGIN  java/lang/Number */

jclass Number_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "Number_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "java/lang/Number", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t Number_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "Number_init");
      
   clazz = Number_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "()V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }    
   *obj = (*env)->NewObject(env, clazz, mid );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t Number_byteValue(JNIEnv *env, jobject obj , jbyte* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jbyte temp = 0;

   DENTER(BASIS_LAYER, "Number_byteValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Number", "byteValue", "()B", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallByteMethod(env, obj, mid);  
   if (test_jni_error(env, "Number_byteValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Number_doubleValue(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "Number_doubleValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Number", "doubleValue", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "Number_doubleValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Number_floatValue(JNIEnv *env, jobject obj , jfloat* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jfloat temp = 0.0f;

   DENTER(BASIS_LAYER, "Number_floatValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0f;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Number", "floatValue", "()F", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallFloatMethod(env, obj, mid);  
   if (test_jni_error(env, "Number_floatValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0f;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Number_intValue(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Number_intValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Number", "intValue", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "Number_intValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Number_longValue(JNIEnv *env, jobject obj , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "Number_longValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Number", "longValue", "()J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid);  
   if (test_jni_error(env, "Number_longValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Number_shortValue(JNIEnv *env, jobject obj , jshort* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jshort temp = 0;

   DENTER(BASIS_LAYER, "Number_shortValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "java/lang/Number", "shortValue", "()S", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallShortMethod(env, obj, mid);  
   if (test_jni_error(env, "Number_shortValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  java/lang/Number */
