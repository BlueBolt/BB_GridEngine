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



/*==== BEGIN  com/sun/grid/jgdi/configuration/reflect/ClassDescriptor */

jclass ClassDescriptor_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "ClassDescriptor_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/configuration/reflect/ClassDescriptor", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t ClassDescriptor_clone(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ClassDescriptor_clone");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/ClassDescriptor", "clone", "(Ljava/lang/Object;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ClassDescriptor_clone failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClassDescriptor_getBeanClass(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ClassDescriptor_getBeanClass");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/ClassDescriptor", "getBeanClass", "()Ljava/lang/Class;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ClassDescriptor_getBeanClass failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClassDescriptor_getCullName(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ClassDescriptor_getCullName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/ClassDescriptor", "getCullName", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ClassDescriptor_getCullName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClassDescriptor_getProperties(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ClassDescriptor_getProperties");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/ClassDescriptor", "getProperties", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ClassDescriptor_getProperties failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClassDescriptor_getProperty(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ClassDescriptor_getProperty");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/ClassDescriptor", "getProperty", "(I)Lcom/sun/grid/jgdi/configuration/reflect/PropertyDescriptor;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ClassDescriptor_getProperty failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClassDescriptor_getPropertyByCullFieldName(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ClassDescriptor_getPropertyByCullFieldName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/ClassDescriptor", "getPropertyByCullFieldName", "(I)Lcom/sun/grid/jgdi/configuration/reflect/PropertyDescriptor;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ClassDescriptor_getPropertyByCullFieldName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClassDescriptor_getPropertyCount(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ClassDescriptor_getPropertyCount");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/ClassDescriptor", "getPropertyCount", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "ClassDescriptor_getPropertyCount failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClassDescriptor_getPropertyNames(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;    

   DENTER(BASIS_LAYER, "ClassDescriptor_getPropertyNames");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;    
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/ClassDescriptor", "getPropertyNames", "()[Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ClassDescriptor_getPropertyNames failed", alpp)) {
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
jgdi_result_t ClassDescriptor_getProperty_0(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ClassDescriptor_getProperty_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/ClassDescriptor", "getProperty", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/configuration/reflect/PropertyDescriptor;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ClassDescriptor_getProperty_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClassDescriptor_newInstance(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ClassDescriptor_newInstance");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/ClassDescriptor", "newInstance", "()Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ClassDescriptor_newInstance failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClassDescriptor_toString(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ClassDescriptor_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/ClassDescriptor", "toString", "(Ljava/lang/Object;)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ClassDescriptor_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClassDescriptor_validate(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ClassDescriptor_validate");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/ClassDescriptor", "validate", "(Ljava/lang/Object;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ClassDescriptor_validate failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/configuration/reflect/ClassDescriptor */



/*==== BEGIN  com/sun/grid/jgdi/configuration/reflect/PropertyDescriptor */

jclass PropertyDescriptor_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "PropertyDescriptor_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/configuration/reflect/PropertyDescriptor", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t PropertyDescriptor_clone(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "PropertyDescriptor_clone");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/PropertyDescriptor", "clone", "(Ljava/lang/Object;Ljava/lang/Object;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "PropertyDescriptor_clone failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t PropertyDescriptor_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "PropertyDescriptor_equals");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/PropertyDescriptor", "equals", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "PropertyDescriptor_equals failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t PropertyDescriptor_getBeanClass(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "PropertyDescriptor_getBeanClass");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/PropertyDescriptor", "getBeanClass", "()Ljava/lang/Class;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "PropertyDescriptor_getBeanClass failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t PropertyDescriptor_getCullContentField(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "PropertyDescriptor_getCullContentField");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/PropertyDescriptor", "getCullContentField", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "PropertyDescriptor_getCullContentField failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t PropertyDescriptor_getCullFieldName(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "PropertyDescriptor_getCullFieldName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/PropertyDescriptor", "getCullFieldName", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "PropertyDescriptor_getCullFieldName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t PropertyDescriptor_getCullType(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "PropertyDescriptor_getCullType");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/PropertyDescriptor", "getCullType", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "PropertyDescriptor_getCullType failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t PropertyDescriptor_getJNIPropertyType(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "PropertyDescriptor_getJNIPropertyType");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/PropertyDescriptor", "getJNIPropertyType", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "PropertyDescriptor_getJNIPropertyType failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t PropertyDescriptor_getPropertyName(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "PropertyDescriptor_getPropertyName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/PropertyDescriptor", "getPropertyName", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "PropertyDescriptor_getPropertyName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t PropertyDescriptor_getPropertyType(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "PropertyDescriptor_getPropertyType");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/PropertyDescriptor", "getPropertyType", "()Ljava/lang/Class;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "PropertyDescriptor_getPropertyType failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t PropertyDescriptor_hasCullWrapper(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "PropertyDescriptor_hasCullWrapper");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/PropertyDescriptor", "hasCullWrapper", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "PropertyDescriptor_hasCullWrapper failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t PropertyDescriptor_hashCode(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "PropertyDescriptor_hashCode");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/PropertyDescriptor", "hashCode", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "PropertyDescriptor_hashCode failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t PropertyDescriptor_isBrowsable(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "PropertyDescriptor_isBrowsable");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/PropertyDescriptor", "isBrowsable", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "PropertyDescriptor_isBrowsable failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t PropertyDescriptor_isConfigurable(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "PropertyDescriptor_isConfigurable");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/PropertyDescriptor", "isConfigurable", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "PropertyDescriptor_isConfigurable failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t PropertyDescriptor_isReadOnly(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "PropertyDescriptor_isReadOnly");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/PropertyDescriptor", "isReadOnly", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "PropertyDescriptor_isReadOnly failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t PropertyDescriptor_isSet(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "PropertyDescriptor_isSet");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/PropertyDescriptor", "isSet", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "PropertyDescriptor_isSet failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t PropertyDescriptor_setBrowsable(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "PropertyDescriptor_setBrowsable");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/PropertyDescriptor", "setBrowsable", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "PropertyDescriptor_setBrowsable failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t PropertyDescriptor_setConfigurable(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "PropertyDescriptor_setConfigurable");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/PropertyDescriptor", "setConfigurable", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "PropertyDescriptor_setConfigurable failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t PropertyDescriptor_setCullContentField(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "PropertyDescriptor_setCullContentField");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/PropertyDescriptor", "setCullContentField", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "PropertyDescriptor_setCullContentField failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t PropertyDescriptor_setHasCullWrapper(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "PropertyDescriptor_setHasCullWrapper");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/PropertyDescriptor", "setHasCullWrapper", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "PropertyDescriptor_setHasCullWrapper failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t PropertyDescriptor_setReadOnly(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "PropertyDescriptor_setReadOnly");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/PropertyDescriptor", "setReadOnly", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "PropertyDescriptor_setReadOnly failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/configuration/reflect/PropertyDescriptor */



/*==== BEGIN  com/sun/grid/jgdi/configuration/reflect/SimplePropertyDescriptor */

jclass SimplePropertyDescriptor_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "SimplePropertyDescriptor_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/configuration/reflect/SimplePropertyDescriptor", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t SimplePropertyDescriptor_init(JNIEnv *env, jobject *obj , jobject p0 , const char* p1 , jobject p2 , const char* p3 , jint p4 , jboolean p5 , jboolean p6 , jboolean p7 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;     jstring p1_obj = NULL;     jstring p3_obj = NULL;
   DENTER(BASIS_LAYER, "SimplePropertyDescriptor_init");
      
   clazz = SimplePropertyDescriptor_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(Ljava/lang/Class;, Ljava/lang/String;, Ljava/lang/Class;, Ljava/lang/String;, I, Z, Z, Z)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   } 
   if ( p1 != NULL ) {
      p1_obj = (*env)->NewStringUTF(env, p1 ); 
   }
   if ( p3 != NULL ) {
      p3_obj = (*env)->NewStringUTF(env, p3 ); 
   }   
   *obj = (*env)->NewObject(env, clazz, mid , p0 , p1_obj , p2 , p3_obj , p4 , p5 , p6 , p7 );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t SimplePropertyDescriptor_clone(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "SimplePropertyDescriptor_clone");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/SimplePropertyDescriptor", "clone", "(Ljava/lang/Object;Ljava/lang/Object;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "SimplePropertyDescriptor_clone failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t SimplePropertyDescriptor_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "SimplePropertyDescriptor_getValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/SimplePropertyDescriptor", "getValue", "(Ljava/lang/Object;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "SimplePropertyDescriptor_getValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t SimplePropertyDescriptor_isBrowsable(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "SimplePropertyDescriptor_isBrowsable");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/SimplePropertyDescriptor", "isBrowsable", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "SimplePropertyDescriptor_isBrowsable failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t SimplePropertyDescriptor_isRequired(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "SimplePropertyDescriptor_isRequired");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/SimplePropertyDescriptor", "isRequired", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "SimplePropertyDescriptor_isRequired failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t SimplePropertyDescriptor_setValue(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "SimplePropertyDescriptor_setValue");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/SimplePropertyDescriptor", "setValue", "(Ljava/lang/Object;Ljava/lang/Object;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "SimplePropertyDescriptor_setValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t SimplePropertyDescriptor_toString(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "SimplePropertyDescriptor_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/SimplePropertyDescriptor", "toString", "(Ljava/lang/Object;)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "SimplePropertyDescriptor_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/configuration/reflect/SimplePropertyDescriptor */



/*==== BEGIN  com/sun/grid/jgdi/configuration/reflect/ListPropertyDescriptor */

jclass ListPropertyDescriptor_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "ListPropertyDescriptor_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/configuration/reflect/ListPropertyDescriptor", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t ListPropertyDescriptor_init(JNIEnv *env, jobject *obj , jobject p0 , const char* p1 , jobject p2 , const char* p3 , jint p4 , jboolean p5 , jboolean p6 , jboolean p7 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;     jstring p1_obj = NULL;     jstring p3_obj = NULL;
   DENTER(BASIS_LAYER, "ListPropertyDescriptor_init");
      
   clazz = ListPropertyDescriptor_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(Ljava/lang/Class;, Ljava/lang/String;, Ljava/lang/Class;, Ljava/lang/String;, I, Z, Z, Z)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   } 
   if ( p1 != NULL ) {
      p1_obj = (*env)->NewStringUTF(env, p1 ); 
   }
   if ( p3 != NULL ) {
      p3_obj = (*env)->NewStringUTF(env, p3 ); 
   }   
   *obj = (*env)->NewObject(env, clazz, mid , p0 , p1_obj , p2 , p3_obj , p4 , p5 , p6 , p7 );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t ListPropertyDescriptor_add(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ListPropertyDescriptor_add");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/ListPropertyDescriptor", "add", "(Ljava/lang/Object;Ljava/lang/Object;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "ListPropertyDescriptor_add failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ListPropertyDescriptor_clone(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ListPropertyDescriptor_clone");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/ListPropertyDescriptor", "clone", "(Ljava/lang/Object;Ljava/lang/Object;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "ListPropertyDescriptor_clone failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ListPropertyDescriptor_get(JNIEnv *env, jobject obj , jobject p0 , jint p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ListPropertyDescriptor_get");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/ListPropertyDescriptor", "get", "(Ljava/lang/Object;I)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "ListPropertyDescriptor_get failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ListPropertyDescriptor_getCount(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ListPropertyDescriptor_getCount");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/ListPropertyDescriptor", "getCount", "(Ljava/lang/Object;)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ListPropertyDescriptor_getCount failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ListPropertyDescriptor_remove(JNIEnv *env, jobject obj , jobject p0 , jint p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ListPropertyDescriptor_remove");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/ListPropertyDescriptor", "remove", "(Ljava/lang/Object;I)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "ListPropertyDescriptor_remove failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ListPropertyDescriptor_removeAll(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ListPropertyDescriptor_removeAll");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/ListPropertyDescriptor", "removeAll", "(Ljava/lang/Object;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ListPropertyDescriptor_removeAll failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ListPropertyDescriptor_remove_0(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "ListPropertyDescriptor_remove_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/ListPropertyDescriptor", "remove", "(Ljava/lang/Object;Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "ListPropertyDescriptor_remove_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ListPropertyDescriptor_set(JNIEnv *env, jobject obj , jobject p0 , jint p1 , jobject p2 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ListPropertyDescriptor_set");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/ListPropertyDescriptor", "set", "(Ljava/lang/Object;ILjava/lang/Object;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1, p2);  
   if (test_jni_error(env, "ListPropertyDescriptor_set failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ListPropertyDescriptor_toString(JNIEnv *env, jobject obj , jobject p0 , jint p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ListPropertyDescriptor_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/ListPropertyDescriptor", "toString", "(Ljava/lang/Object;I)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "ListPropertyDescriptor_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/configuration/reflect/ListPropertyDescriptor */



/*==== BEGIN  com/sun/grid/jgdi/configuration/reflect/MapPropertyDescriptor */

jclass MapPropertyDescriptor_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "MapPropertyDescriptor_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/configuration/reflect/MapPropertyDescriptor", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t MapPropertyDescriptor_get(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "MapPropertyDescriptor_get");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapPropertyDescriptor", "get", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "MapPropertyDescriptor_get failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t MapPropertyDescriptor_getCount(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "MapPropertyDescriptor_getCount");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapPropertyDescriptor", "getCount", "(Ljava/lang/Object;)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "MapPropertyDescriptor_getCount failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t MapPropertyDescriptor_getDefaultKey(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "MapPropertyDescriptor_getDefaultKey");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapPropertyDescriptor", "getDefaultKey", "()Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "MapPropertyDescriptor_getDefaultKey failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t MapPropertyDescriptor_getKeyCullFieldName(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "MapPropertyDescriptor_getKeyCullFieldName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapPropertyDescriptor", "getKeyCullFieldName", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "MapPropertyDescriptor_getKeyCullFieldName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t MapPropertyDescriptor_getKeyType(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "MapPropertyDescriptor_getKeyType");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapPropertyDescriptor", "getKeyType", "()Ljava/lang/Class;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "MapPropertyDescriptor_getKeyType failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t MapPropertyDescriptor_getKeys(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "MapPropertyDescriptor_getKeys");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapPropertyDescriptor", "getKeys", "(Ljava/lang/Object;)Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "MapPropertyDescriptor_getKeys failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t MapPropertyDescriptor_getValueCullFieldName(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "MapPropertyDescriptor_getValueCullFieldName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapPropertyDescriptor", "getValueCullFieldName", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "MapPropertyDescriptor_getValueCullFieldName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t MapPropertyDescriptor_put(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jobject p2 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "MapPropertyDescriptor_put");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapPropertyDescriptor", "put", "(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1, p2);  
   if (test_jni_error(env, "MapPropertyDescriptor_put failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t MapPropertyDescriptor_remove(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "MapPropertyDescriptor_remove");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapPropertyDescriptor", "remove", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "MapPropertyDescriptor_remove failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t MapPropertyDescriptor_removeAll(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "MapPropertyDescriptor_removeAll");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapPropertyDescriptor", "removeAll", "(Ljava/lang/Object;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "MapPropertyDescriptor_removeAll failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t MapPropertyDescriptor_toString(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "MapPropertyDescriptor_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapPropertyDescriptor", "toString", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "MapPropertyDescriptor_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/configuration/reflect/MapPropertyDescriptor */



/*==== BEGIN  com/sun/grid/jgdi/configuration/reflect/MapListPropertyDescriptor */

jclass MapListPropertyDescriptor_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "MapListPropertyDescriptor_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/configuration/reflect/MapListPropertyDescriptor", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t MapListPropertyDescriptor_add(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jobject p2 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "MapListPropertyDescriptor_add");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapListPropertyDescriptor", "add", "(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1, p2);  
   if (test_jni_error(env, "MapListPropertyDescriptor_add failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t MapListPropertyDescriptor_addEmpty(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "MapListPropertyDescriptor_addEmpty");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapListPropertyDescriptor", "addEmpty", "(Ljava/lang/Object;Ljava/lang/Object;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "MapListPropertyDescriptor_addEmpty failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t MapListPropertyDescriptor_get(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jint p2 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "MapListPropertyDescriptor_get");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapListPropertyDescriptor", "get", "(Ljava/lang/Object;Ljava/lang/Object;I)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1, p2);  
   if (test_jni_error(env, "MapListPropertyDescriptor_get failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t MapListPropertyDescriptor_getCount(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "MapListPropertyDescriptor_getCount");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapListPropertyDescriptor", "getCount", "(Ljava/lang/Object;Ljava/lang/Object;)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "MapListPropertyDescriptor_getCount failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t MapListPropertyDescriptor_getCullListType(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "MapListPropertyDescriptor_getCullListType");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapListPropertyDescriptor", "getCullListType", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "MapListPropertyDescriptor_getCullListType failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t MapListPropertyDescriptor_getDefaultKey(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "MapListPropertyDescriptor_getDefaultKey");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapListPropertyDescriptor", "getDefaultKey", "()Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "MapListPropertyDescriptor_getDefaultKey failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t MapListPropertyDescriptor_getKeyCullFieldName(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "MapListPropertyDescriptor_getKeyCullFieldName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapListPropertyDescriptor", "getKeyCullFieldName", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "MapListPropertyDescriptor_getKeyCullFieldName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t MapListPropertyDescriptor_getKeyType(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "MapListPropertyDescriptor_getKeyType");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapListPropertyDescriptor", "getKeyType", "()Ljava/lang/Class;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "MapListPropertyDescriptor_getKeyType failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t MapListPropertyDescriptor_getKeys(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "MapListPropertyDescriptor_getKeys");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapListPropertyDescriptor", "getKeys", "(Ljava/lang/Object;)Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "MapListPropertyDescriptor_getKeys failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t MapListPropertyDescriptor_getList(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "MapListPropertyDescriptor_getList");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapListPropertyDescriptor", "getList", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "MapListPropertyDescriptor_getList failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t MapListPropertyDescriptor_getValueCullFieldName(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "MapListPropertyDescriptor_getValueCullFieldName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapListPropertyDescriptor", "getValueCullFieldName", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "MapListPropertyDescriptor_getValueCullFieldName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t MapListPropertyDescriptor_remove(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jint p2 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "MapListPropertyDescriptor_remove");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapListPropertyDescriptor", "remove", "(Ljava/lang/Object;Ljava/lang/Object;I)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1, p2);  
   if (test_jni_error(env, "MapListPropertyDescriptor_remove failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t MapListPropertyDescriptor_removeAll(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "MapListPropertyDescriptor_removeAll");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapListPropertyDescriptor", "removeAll", "(Ljava/lang/Object;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "MapListPropertyDescriptor_removeAll failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t MapListPropertyDescriptor_removeAll_0(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "MapListPropertyDescriptor_removeAll_0");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapListPropertyDescriptor", "removeAll", "(Ljava/lang/Object;Ljava/lang/Object;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "MapListPropertyDescriptor_removeAll_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t MapListPropertyDescriptor_remove_0(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jobject p2 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "MapListPropertyDescriptor_remove_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapListPropertyDescriptor", "remove", "(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0, p1, p2);  
   if (test_jni_error(env, "MapListPropertyDescriptor_remove_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t MapListPropertyDescriptor_set(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jint p2 , jobject p3 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "MapListPropertyDescriptor_set");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapListPropertyDescriptor", "set", "(Ljava/lang/Object;Ljava/lang/Object;ILjava/lang/Object;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1, p2, p3);  
   if (test_jni_error(env, "MapListPropertyDescriptor_set failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t MapListPropertyDescriptor_toString(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jint p2 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "MapListPropertyDescriptor_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/reflect/MapListPropertyDescriptor", "toString", "(Ljava/lang/Object;Ljava/lang/Object;I)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1, p2);  
   if (test_jni_error(env, "MapListPropertyDescriptor_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/configuration/reflect/MapListPropertyDescriptor */



/*==== BEGIN  com/sun/grid/jgdi/configuration/JGDIAnswer */

jclass JGDIAnswer_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "JGDIAnswer_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/configuration/JGDIAnswer", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t JGDIAnswer_dump(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JGDIAnswer_dump");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/JGDIAnswer", "dump", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JGDIAnswer_dump failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JGDIAnswer_getQuality(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "JGDIAnswer_getQuality");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/JGDIAnswer", "getQuality", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "JGDIAnswer_getQuality failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JGDIAnswer_getStatus(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "JGDIAnswer_getStatus");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/JGDIAnswer", "getStatus", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "JGDIAnswer_getStatus failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JGDIAnswer_getText(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JGDIAnswer_getText");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/JGDIAnswer", "getText", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JGDIAnswer_getText failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JGDIAnswer_isSetQuality(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JGDIAnswer_isSetQuality");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/JGDIAnswer", "isSetQuality", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "JGDIAnswer_isSetQuality failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JGDIAnswer_isSetStatus(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JGDIAnswer_isSetStatus");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/JGDIAnswer", "isSetStatus", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "JGDIAnswer_isSetStatus failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JGDIAnswer_isSetText(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JGDIAnswer_isSetText");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/JGDIAnswer", "isSetText", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "JGDIAnswer_isSetText failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JGDIAnswer_setQuality(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JGDIAnswer_setQuality");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/JGDIAnswer", "setQuality", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JGDIAnswer_setQuality failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JGDIAnswer_setStatus(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JGDIAnswer_setStatus");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/JGDIAnswer", "setStatus", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JGDIAnswer_setStatus failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JGDIAnswer_setText(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JGDIAnswer_setText");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/JGDIAnswer", "setText", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JGDIAnswer_setText failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/configuration/JGDIAnswer */



/*==== BEGIN  com/sun/grid/jgdi/configuration/Util$Difference */

jclass Util_Difference_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "Util_Difference_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/configuration/Util$Difference", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t Util_Difference_init(JNIEnv *env, jobject *obj , const char* p0 , const char* p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;     jstring p0_obj = NULL;     jstring p1_obj = NULL;
   DENTER(BASIS_LAYER, "Util_Difference_init");
      
   clazz = Util_Difference_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(Ljava/lang/String;, Ljava/lang/String;)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   } 
   if ( p0 != NULL ) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   if ( p1 != NULL ) {
      p1_obj = (*env)->NewStringUTF(env, p1 ); 
   }   
   *obj = (*env)->NewObject(env, clazz, mid , p0_obj , p1_obj );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t Util_Difference_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Util_Difference_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/Util$Difference", "toString", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "Util_Difference_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/configuration/Util$Difference */



/*==== BEGIN  com/sun/grid/jgdi/configuration/Util */

jclass Util_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "Util_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/configuration/Util", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t Util_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "Util_init");
      
   clazz = Util_find_class(env, alpp);
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

jgdi_result_t Util_static_addDescriptor(JNIEnv *env , jobject p0 , jobject p1 , lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "Util_static_addDescriptor");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/configuration/Util", "addDescriptor", "(Ljava/lang/Class;Lcom/sun/grid/jgdi/configuration/reflect/ClassDescriptor;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }
   
 (*env)->CallStaticVoidMethod(env, clazz, mid , p0 , p1 );  
   if (test_jni_error(env, "Util_addDescriptor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   } 
   DRETURN(ret);
}
jgdi_result_t Util_static_clone(JNIEnv *env , jobject p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Util_static_clone");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/configuration/Util", "clone", "(Ljava/lang/Object;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Util_clone failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Util_static_findObject(JNIEnv *env , const char* p0 , jobject p1 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Util_static_findObject");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/configuration/Util", "findObject", "(Ljava/lang/String;Lcom/sun/grid/jgdi/configuration/GEObject;)Lcom/sun/grid/jgdi/configuration/GEObject;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj , p1 );  
   if (test_jni_error(env, "Util_findObject failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Util_static_getDescriptor(JNIEnv *env , jobject p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Util_static_getDescriptor");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/configuration/Util", "getDescriptor", "(Ljava/lang/Class;)Lcom/sun/grid/jgdi/configuration/reflect/ClassDescriptor;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "Util_getDescriptor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Util_static_getDescriptorForCullType(JNIEnv *env , const char* p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Util_static_getDescriptorForCullType");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/configuration/Util", "getDescriptorForCullType", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/configuration/reflect/ClassDescriptor;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "Util_getDescriptorForCullType failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Util_static_getDifferences(JNIEnv *env , jobject p0 , jobject p1 , jobject p2 , lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "Util_static_getDifferences");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/configuration/Util", "getDifferences", "(Lcom/sun/grid/jgdi/configuration/GEObject;Lcom/sun/grid/jgdi/configuration/GEObject;Ljava/util/List;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }
   
 (*env)->CallStaticVoidMethod(env, clazz, mid , p0 , p1 , p2 );  
   if (test_jni_error(env, "Util_getDifferences failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   } 
   DRETURN(ret);
}
jgdi_result_t Util_static_getDifferences_0(JNIEnv *env , jobject p0 , jobject p1 , const char* p2 , jobject p3 , lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p2_obj = NULL;
   DENTER(BASIS_LAYER, "Util_static_getDifferences_0");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/configuration/Util", "getDifferences", "(Lcom/sun/grid/jgdi/configuration/GEObject;Lcom/sun/grid/jgdi/configuration/GEObject;Ljava/lang/String;Ljava/util/List;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p2 != NULL) {
      p2_obj = (*env)->NewStringUTF(env, p2); 
   }
   
 (*env)->CallStaticVoidMethod(env, clazz, mid , p0 , p1 , p2_obj , p3 );  
   if (test_jni_error(env, "Util_getDifferences_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   } 
   DRETURN(ret);
}
jgdi_result_t Util_static_nextObjectId(JNIEnv *env , jint* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Util_static_nextObjectId");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/configuration/Util", "nextObjectId", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticIntMethod(env, clazz, mid );  
   if (test_jni_error(env, "Util_nextObjectId failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t Util_addDescriptor(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "Util_addDescriptor");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/Util", "addDescriptor", "(Ljava/lang/Class;Lcom/sun/grid/jgdi/configuration/reflect/ClassDescriptor;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "Util_addDescriptor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t Util_clone(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Util_clone");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/Util", "clone", "(Ljava/lang/Object;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Util_clone failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Util_findObject(JNIEnv *env, jobject obj , const char* p0 , jobject p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Util_findObject");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/Util", "findObject", "(Ljava/lang/String;Lcom/sun/grid/jgdi/configuration/GEObject;)Lcom/sun/grid/jgdi/configuration/GEObject;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj, p1);  
   if (test_jni_error(env, "Util_findObject failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Util_getDescriptor(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Util_getDescriptor");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/Util", "getDescriptor", "(Ljava/lang/Class;)Lcom/sun/grid/jgdi/configuration/reflect/ClassDescriptor;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "Util_getDescriptor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Util_getDescriptorForCullType(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "Util_getDescriptorForCullType");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/Util", "getDescriptorForCullType", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/configuration/reflect/ClassDescriptor;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "Util_getDescriptorForCullType failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t Util_getDifferences(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jobject p2 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "Util_getDifferences");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/Util", "getDifferences", "(Lcom/sun/grid/jgdi/configuration/GEObject;Lcom/sun/grid/jgdi/configuration/GEObject;Ljava/util/List;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1, p2);  
   if (test_jni_error(env, "Util_getDifferences failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t Util_getDifferences_0(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , const char* p2 , jobject p3 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p2_obj = NULL;
   DENTER(BASIS_LAYER, "Util_getDifferences_0");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/Util", "getDifferences", "(Lcom/sun/grid/jgdi/configuration/GEObject;Lcom/sun/grid/jgdi/configuration/GEObject;Ljava/lang/String;Ljava/util/List;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p2 != NULL) {
      p2_obj = (*env)->NewStringUTF(env, p2 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0, p1, p2_obj, p3);  
   if (test_jni_error(env, "Util_getDifferences_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t Util_nextObjectId(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "Util_nextObjectId");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/Util", "nextObjectId", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "Util_nextObjectId failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/configuration/Util */



/*==== BEGIN  com/sun/grid/jgdi/configuration/JGDIAnswerImpl */

jclass JGDIAnswerImpl_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "JGDIAnswerImpl_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/configuration/JGDIAnswerImpl", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t JGDIAnswerImpl_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "JGDIAnswerImpl_init");
      
   clazz = JGDIAnswerImpl_find_class(env, alpp);
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

jgdi_result_t JGDIAnswerImpl_init_0(JNIEnv *env, jobject *obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "JGDIAnswerImpl_init_0");
      
   clazz = JGDIAnswerImpl_find_class(env, alpp);
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

jgdi_result_t JGDIAnswerImpl_dump(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JGDIAnswerImpl_dump");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/JGDIAnswerImpl", "dump", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JGDIAnswerImpl_dump failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JGDIAnswerImpl_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JGDIAnswerImpl_equals");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/JGDIAnswerImpl", "equals", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JGDIAnswerImpl_equals failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JGDIAnswerImpl_equalsCompletely(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JGDIAnswerImpl_equalsCompletely");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/JGDIAnswerImpl", "equalsCompletely", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JGDIAnswerImpl_equalsCompletely failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JGDIAnswerImpl_getQuality(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "JGDIAnswerImpl_getQuality");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/JGDIAnswerImpl", "getQuality", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "JGDIAnswerImpl_getQuality failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JGDIAnswerImpl_getStatus(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "JGDIAnswerImpl_getStatus");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/JGDIAnswerImpl", "getStatus", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "JGDIAnswerImpl_getStatus failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JGDIAnswerImpl_getText(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JGDIAnswerImpl_getText");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/JGDIAnswerImpl", "getText", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JGDIAnswerImpl_getText failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JGDIAnswerImpl_isSetQuality(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JGDIAnswerImpl_isSetQuality");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/JGDIAnswerImpl", "isSetQuality", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "JGDIAnswerImpl_isSetQuality failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JGDIAnswerImpl_isSetStatus(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JGDIAnswerImpl_isSetStatus");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/JGDIAnswerImpl", "isSetStatus", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "JGDIAnswerImpl_isSetStatus failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JGDIAnswerImpl_isSetText(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JGDIAnswerImpl_isSetText");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/JGDIAnswerImpl", "isSetText", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "JGDIAnswerImpl_isSetText failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JGDIAnswerImpl_setQuality(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JGDIAnswerImpl_setQuality");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/JGDIAnswerImpl", "setQuality", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JGDIAnswerImpl_setQuality failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JGDIAnswerImpl_setStatus(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JGDIAnswerImpl_setStatus");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/JGDIAnswerImpl", "setStatus", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JGDIAnswerImpl_setStatus failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JGDIAnswerImpl_setText(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JGDIAnswerImpl_setText");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/JGDIAnswerImpl", "setText", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JGDIAnswerImpl_setText failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JGDIAnswerImpl_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JGDIAnswerImpl_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/configuration/JGDIAnswerImpl", "toString", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JGDIAnswerImpl_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/configuration/JGDIAnswerImpl */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QHostOptions */

jclass QHostOptions_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QHostOptions_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QHostOptions", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QHostOptions_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "QHostOptions_init");
      
   clazz = QHostOptions_find_class(env, alpp);
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

jgdi_result_t QHostOptions_getHostFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QHostOptions_getHostFilter");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QHostOptions", "getHostFilter", "()Lcom/sun/grid/jgdi/monitoring/filter/HostFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QHostOptions_getHostFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QHostOptions_getResourceAttributeFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QHostOptions_getResourceAttributeFilter");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QHostOptions", "getResourceAttributeFilter", "()Lcom/sun/grid/jgdi/monitoring/filter/ResourceAttributeFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QHostOptions_getResourceAttributeFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QHostOptions_getResourceFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QHostOptions_getResourceFilter");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QHostOptions", "getResourceFilter", "()Lcom/sun/grid/jgdi/monitoring/filter/ResourceFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QHostOptions_getResourceFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QHostOptions_getUserFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QHostOptions_getUserFilter");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QHostOptions", "getUserFilter", "()Lcom/sun/grid/jgdi/monitoring/filter/UserFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QHostOptions_getUserFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QHostOptions_includeJobs(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QHostOptions_includeJobs");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QHostOptions", "includeJobs", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QHostOptions_includeJobs failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QHostOptions_includeQueue(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QHostOptions_includeQueue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QHostOptions", "includeQueue", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QHostOptions_includeQueue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QHostOptions_setHostFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QHostOptions_setHostFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QHostOptions", "setHostFilter", "(Lcom/sun/grid/jgdi/monitoring/filter/HostFilter;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QHostOptions_setHostFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QHostOptions_setIncludeJobs(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QHostOptions_setIncludeJobs");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QHostOptions", "setIncludeJobs", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QHostOptions_setIncludeJobs failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QHostOptions_setIncludeQueue(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QHostOptions_setIncludeQueue");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QHostOptions", "setIncludeQueue", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QHostOptions_setIncludeQueue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QHostOptions_setResourceAttributeFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QHostOptions_setResourceAttributeFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QHostOptions", "setResourceAttributeFilter", "(Lcom/sun/grid/jgdi/monitoring/filter/ResourceAttributeFilter;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QHostOptions_setResourceAttributeFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QHostOptions_setResourceFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QHostOptions_setResourceFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QHostOptions", "setResourceFilter", "(Lcom/sun/grid/jgdi/monitoring/filter/ResourceFilter;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QHostOptions_setResourceFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QHostOptions_setShowAsXML(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QHostOptions_setShowAsXML");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QHostOptions", "setShowAsXML", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QHostOptions_setShowAsXML failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QHostOptions_setUserFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QHostOptions_setUserFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QHostOptions", "setUserFilter", "(Lcom/sun/grid/jgdi/monitoring/filter/UserFilter;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QHostOptions_setUserFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QHostOptions_showAsXML(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QHostOptions_showAsXML");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QHostOptions", "showAsXML", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QHostOptions_showAsXML failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QHostOptions */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QHostResult */

jclass QHostResult_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QHostResult_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QHostResult", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QHostResult_getHostInfo(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QHostResult_getHostInfo");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QHostResult", "getHostInfo", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QHostResult_getHostInfo failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QHostResult_getHostInfo_0(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QHostResult_getHostInfo_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QHostResult", "getHostInfo", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/HostInfo;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QHostResult_getHostInfo_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QHostResult_getHostNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QHostResult_getHostNames");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QHostResult", "getHostNames", "()Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QHostResult_getHostNames failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QHostResult */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/BasicQueueOptions */

jclass BasicQueueOptions_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "BasicQueueOptions_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/BasicQueueOptions", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t BasicQueueOptions_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "BasicQueueOptions_init");
      
   clazz = BasicQueueOptions_find_class(env, alpp);
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

jgdi_result_t BasicQueueOptions_getQueueFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "BasicQueueOptions_getQueueFilter");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/BasicQueueOptions", "getQueueFilter", "()Lcom/sun/grid/jgdi/monitoring/filter/QueueFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "BasicQueueOptions_getQueueFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t BasicQueueOptions_getQueueStateFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "BasicQueueOptions_getQueueStateFilter");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/BasicQueueOptions", "getQueueStateFilter", "()Lcom/sun/grid/jgdi/monitoring/filter/QueueStateFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "BasicQueueOptions_getQueueStateFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t BasicQueueOptions_getQueueUserFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "BasicQueueOptions_getQueueUserFilter");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/BasicQueueOptions", "getQueueUserFilter", "()Lcom/sun/grid/jgdi/monitoring/filter/UserFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "BasicQueueOptions_getQueueUserFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t BasicQueueOptions_getResourceFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "BasicQueueOptions_getResourceFilter");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/BasicQueueOptions", "getResourceFilter", "()Lcom/sun/grid/jgdi/monitoring/filter/ResourceFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "BasicQueueOptions_getResourceFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t BasicQueueOptions_setQueueFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "BasicQueueOptions_setQueueFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/BasicQueueOptions", "setQueueFilter", "(Lcom/sun/grid/jgdi/monitoring/filter/QueueFilter;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "BasicQueueOptions_setQueueFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t BasicQueueOptions_setQueueStateFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "BasicQueueOptions_setQueueStateFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/BasicQueueOptions", "setQueueStateFilter", "(Lcom/sun/grid/jgdi/monitoring/filter/QueueStateFilter;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "BasicQueueOptions_setQueueStateFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t BasicQueueOptions_setQueueUserFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "BasicQueueOptions_setQueueUserFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/BasicQueueOptions", "setQueueUserFilter", "(Lcom/sun/grid/jgdi/monitoring/filter/UserFilter;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "BasicQueueOptions_setQueueUserFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t BasicQueueOptions_setResourceFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "BasicQueueOptions_setResourceFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/BasicQueueOptions", "setResourceFilter", "(Lcom/sun/grid/jgdi/monitoring/filter/ResourceFilter;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "BasicQueueOptions_setResourceFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t BasicQueueOptions_setShowAdditionalAttributes(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "BasicQueueOptions_setShowAdditionalAttributes");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/BasicQueueOptions", "setShowAdditionalAttributes", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "BasicQueueOptions_setShowAdditionalAttributes failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t BasicQueueOptions_showAdditionalAttributes(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "BasicQueueOptions_showAdditionalAttributes");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/BasicQueueOptions", "showAdditionalAttributes", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "BasicQueueOptions_showAdditionalAttributes failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t BasicQueueOptions_updateQueueFilter(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "BasicQueueOptions_updateQueueFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/BasicQueueOptions", "updateQueueFilter", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "BasicQueueOptions_updateQueueFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t BasicQueueOptions_updateQueueStateFilter(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "BasicQueueOptions_updateQueueStateFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/BasicQueueOptions", "updateQueueStateFilter", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "BasicQueueOptions_updateQueueStateFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t BasicQueueOptions_updateQueueUserFilter(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "BasicQueueOptions_updateQueueUserFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/BasicQueueOptions", "updateQueueUserFilter", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "BasicQueueOptions_updateQueueUserFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t BasicQueueOptions_updateResourceFilter(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "BasicQueueOptions_updateResourceFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/BasicQueueOptions", "updateResourceFilter", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "BasicQueueOptions_updateResourceFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/BasicQueueOptions */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/ClusterQueueSummary */

jclass ClusterQueueSummary_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t ClusterQueueSummary_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "ClusterQueueSummary_init");
      
   clazz = ClusterQueueSummary_find_class(env, alpp);
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

jgdi_result_t ClusterQueueSummary_getAmbiguous(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_getAmbiguous");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "getAmbiguous", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "ClusterQueueSummary_getAmbiguous failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_getAvailableSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_getAvailableSlots");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "getAvailableSlots", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "ClusterQueueSummary_getAvailableSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_getDisabledByCalendar(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_getDisabledByCalendar");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "getDisabledByCalendar", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "ClusterQueueSummary_getDisabledByCalendar failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_getDisabledManual(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_getDisabledManual");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "getDisabledManual", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "ClusterQueueSummary_getDisabledManual failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_getError(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_getError");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "getError", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "ClusterQueueSummary_getError failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_getLoad(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_getLoad");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "getLoad", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "ClusterQueueSummary_getLoad failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_getLoadAlarm(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_getLoadAlarm");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "getLoadAlarm", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "ClusterQueueSummary_getLoadAlarm failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_getManualIntervention(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_getManualIntervention");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "getManualIntervention", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "ClusterQueueSummary_getManualIntervention failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_getName(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_getName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "getName", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ClusterQueueSummary_getName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_getOrphaned(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_getOrphaned");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "getOrphaned", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "ClusterQueueSummary_getOrphaned failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_getReservedSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_getReservedSlots");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "getReservedSlots", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "ClusterQueueSummary_getReservedSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_getSuspendByCalendar(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_getSuspendByCalendar");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "getSuspendByCalendar", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "ClusterQueueSummary_getSuspendByCalendar failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_getSuspendManual(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_getSuspendManual");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "getSuspendManual", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "ClusterQueueSummary_getSuspendManual failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_getSuspendOnSubordinate(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_getSuspendOnSubordinate");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "getSuspendOnSubordinate", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "ClusterQueueSummary_getSuspendOnSubordinate failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_getSuspendThreshold(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_getSuspendThreshold");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "getSuspendThreshold", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "ClusterQueueSummary_getSuspendThreshold failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_getTempDisabled(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_getTempDisabled");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "getTempDisabled", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "ClusterQueueSummary_getTempDisabled failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_getTotalSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_getTotalSlots");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "getTotalSlots", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "ClusterQueueSummary_getTotalSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_getUnknown(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_getUnknown");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "getUnknown", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "ClusterQueueSummary_getUnknown failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_getUsedSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_getUsedSlots");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "getUsedSlots", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "ClusterQueueSummary_getUsedSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_isLoadSet(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_isLoadSet");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "isLoadSet", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "ClusterQueueSummary_isLoadSet failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_setAmbiguous(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_setAmbiguous");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "setAmbiguous", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ClusterQueueSummary_setAmbiguous failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_setAvailableSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_setAvailableSlots");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "setAvailableSlots", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ClusterQueueSummary_setAvailableSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_setDisabledByCalendar(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_setDisabledByCalendar");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "setDisabledByCalendar", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ClusterQueueSummary_setDisabledByCalendar failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_setDisabledManual(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_setDisabledManual");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "setDisabledManual", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ClusterQueueSummary_setDisabledManual failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_setError(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_setError");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "setError", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ClusterQueueSummary_setError failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_setLoad(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_setLoad");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "setLoad", "(D)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ClusterQueueSummary_setLoad failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_setLoadAlarm(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_setLoadAlarm");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "setLoadAlarm", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ClusterQueueSummary_setLoadAlarm failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_setManualIntervention(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_setManualIntervention");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "setManualIntervention", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ClusterQueueSummary_setManualIntervention failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_setName(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "ClusterQueueSummary_setName");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "setName", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ClusterQueueSummary_setName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_setOrphaned(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_setOrphaned");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "setOrphaned", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ClusterQueueSummary_setOrphaned failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_setReservedSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_setReservedSlots");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "setReservedSlots", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ClusterQueueSummary_setReservedSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_setSuspendByCalendar(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_setSuspendByCalendar");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "setSuspendByCalendar", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ClusterQueueSummary_setSuspendByCalendar failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_setSuspendManual(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_setSuspendManual");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "setSuspendManual", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ClusterQueueSummary_setSuspendManual failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_setSuspendOnSubordinate(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_setSuspendOnSubordinate");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "setSuspendOnSubordinate", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ClusterQueueSummary_setSuspendOnSubordinate failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_setSuspendThreshold(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_setSuspendThreshold");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "setSuspendThreshold", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ClusterQueueSummary_setSuspendThreshold failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_setTempDisabled(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_setTempDisabled");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "setTempDisabled", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ClusterQueueSummary_setTempDisabled failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_setTotalSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_setTotalSlots");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "setTotalSlots", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ClusterQueueSummary_setTotalSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_setUnknown(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_setUnknown");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "setUnknown", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ClusterQueueSummary_setUnknown failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ClusterQueueSummary_setUsedSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ClusterQueueSummary_setUsedSlots");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ClusterQueueSummary", "setUsedSlots", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ClusterQueueSummary_setUsedSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/ClusterQueueSummary */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions */

jclass QueueInstanceSummaryOptions_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummaryOptions_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_init");
      
   clazz = QueueInstanceSummaryOptions_find_class(env, alpp);
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

jgdi_result_t QueueInstanceSummaryOptions_getExplain(JNIEnv *env, jobject obj , jchar* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jchar temp = 0;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_getExplain");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "getExplain", "()C", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallCharMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_getExplain failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_getJobStateFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_getJobStateFilter");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "getJobStateFilter", "()Lcom/sun/grid/jgdi/monitoring/filter/JobStateFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_getJobStateFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_getJobUserFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_getJobUserFilter");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "getJobUserFilter", "()Lcom/sun/grid/jgdi/monitoring/filter/UserFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_getJobUserFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_getPeFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_getPeFilter");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "getPeFilter", "()Lcom/sun/grid/jgdi/monitoring/filter/ParallelEnvironmentFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_getPeFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_getResourceAttributeFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_getResourceAttributeFilter");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "getResourceAttributeFilter", "()Lcom/sun/grid/jgdi/monitoring/filter/ResourceAttributeFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_getResourceAttributeFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_isExplainSet(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_isExplainSet");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "isExplainSet", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_isExplainSet failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_setExplain(JNIEnv *env, jobject obj , jchar p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_setExplain");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "setExplain", "(C)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_setExplain failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_setJobStateFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_setJobStateFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "setJobStateFilter", "(Lcom/sun/grid/jgdi/monitoring/filter/JobStateFilter;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_setJobStateFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_setJobUserFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_setJobUserFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "setJobUserFilter", "(Lcom/sun/grid/jgdi/monitoring/filter/UserFilter;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_setJobUserFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_setPeFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_setPeFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "setPeFilter", "(Lcom/sun/grid/jgdi/monitoring/filter/ParallelEnvironmentFilter;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_setPeFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_setResourceAttributeFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_setResourceAttributeFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "setResourceAttributeFilter", "(Lcom/sun/grid/jgdi/monitoring/filter/ResourceAttributeFilter;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_setResourceAttributeFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_setShowArrayJobs(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_setShowArrayJobs");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "setShowArrayJobs", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_setShowArrayJobs failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_setShowEmptyQueues(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_setShowEmptyQueues");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "setShowEmptyQueues", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_setShowEmptyQueues failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_setShowExtendedSubTaskInfo(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_setShowExtendedSubTaskInfo");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "setShowExtendedSubTaskInfo", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_setShowExtendedSubTaskInfo failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_setShowFullOutput(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_setShowFullOutput");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "setShowFullOutput", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_setShowFullOutput failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_setShowJobPriorities(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_setShowJobPriorities");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "setShowJobPriorities", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_setShowJobPriorities failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_setShowJobUrgency(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_setShowJobUrgency");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "setShowJobUrgency", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_setShowJobUrgency failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_setShowPEJobs(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_setShowPEJobs");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "setShowPEJobs", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_setShowPEJobs failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_setShowRequestedResourcesForJobs(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_setShowRequestedResourcesForJobs");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "setShowRequestedResourcesForJobs", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_setShowRequestedResourcesForJobs failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_showArrayJobs(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_showArrayJobs");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "showArrayJobs", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_showArrayJobs failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_showEmptyQueues(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_showEmptyQueues");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "showEmptyQueues", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_showEmptyQueues failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_showExtendedSubTaskInfo(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_showExtendedSubTaskInfo");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "showExtendedSubTaskInfo", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_showExtendedSubTaskInfo failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_showFullOutput(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_showFullOutput");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "showFullOutput", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_showFullOutput failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_showJobPriorities(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_showJobPriorities");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "showJobPriorities", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_showJobPriorities failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_showJobUrgency(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_showJobUrgency");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "showJobUrgency", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_showJobUrgency failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_showPEJobs(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_showPEJobs");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "showPEJobs", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_showPEJobs failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_showRequestedResourcesForJobs(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_showRequestedResourcesForJobs");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "showRequestedResourcesForJobs", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_showRequestedResourcesForJobs failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_updateJobUserFilter(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_updateJobUserFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "updateJobUserFilter", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_updateJobUserFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_updatePeFilter(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_updatePeFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "updatePeFilter", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_updatePeFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryOptions_updateResourceAttributeFilter(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "QueueInstanceSummaryOptions_updateResourceAttributeFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions", "updateResourceAttributeFilter", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QueueInstanceSummaryOptions_updateResourceAttributeFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryResult */

jclass QueueInstanceSummaryResult_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryResult_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryResult", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummaryResult_getErrorJobs(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryResult_getErrorJobs");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryResult", "getErrorJobs", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryResult_getErrorJobs failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryResult_getFinishedJobs(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryResult_getFinishedJobs");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryResult", "getFinishedJobs", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryResult_getFinishedJobs failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryResult_getPendingJobs(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryResult_getPendingJobs");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryResult", "getPendingJobs", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryResult_getPendingJobs failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryResult_getQueueInstanceSummary(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryResult_getQueueInstanceSummary");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryResult", "getQueueInstanceSummary", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryResult_getQueueInstanceSummary failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryResult_getZombieJobs(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryResult_getZombieJobs");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryResult", "getZombieJobs", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryResult_getZombieJobs failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryResult */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QQuotaOptions */

jclass QQuotaOptions_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QQuotaOptions_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QQuotaOptions", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QQuotaOptions_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "QQuotaOptions_init");
      
   clazz = QQuotaOptions_find_class(env, alpp);
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

jgdi_result_t QQuotaOptions_getHostFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QQuotaOptions_getHostFilter");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QQuotaOptions", "getHostFilter", "()Lcom/sun/grid/jgdi/monitoring/filter/HostFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QQuotaOptions_getHostFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QQuotaOptions_getPeFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QQuotaOptions_getPeFilter");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QQuotaOptions", "getPeFilter", "()Lcom/sun/grid/jgdi/monitoring/filter/ParallelEnvironmentFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QQuotaOptions_getPeFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QQuotaOptions_getProjectFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QQuotaOptions_getProjectFilter");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QQuotaOptions", "getProjectFilter", "()Lcom/sun/grid/jgdi/monitoring/filter/ProjectFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QQuotaOptions_getProjectFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QQuotaOptions_getQueueFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QQuotaOptions_getQueueFilter");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QQuotaOptions", "getQueueFilter", "()Lcom/sun/grid/jgdi/monitoring/filter/QueueFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QQuotaOptions_getQueueFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QQuotaOptions_getResourceFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QQuotaOptions_getResourceFilter");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QQuotaOptions", "getResourceFilter", "()Lcom/sun/grid/jgdi/monitoring/filter/ResourceFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QQuotaOptions_getResourceFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QQuotaOptions_getUserFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QQuotaOptions_getUserFilter");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QQuotaOptions", "getUserFilter", "()Lcom/sun/grid/jgdi/monitoring/filter/UserFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QQuotaOptions_getUserFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QQuotaOptions_setHostFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QQuotaOptions_setHostFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QQuotaOptions", "setHostFilter", "(Lcom/sun/grid/jgdi/monitoring/filter/HostFilter;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QQuotaOptions_setHostFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QQuotaOptions_setPeFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QQuotaOptions_setPeFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QQuotaOptions", "setPeFilter", "(Lcom/sun/grid/jgdi/monitoring/filter/ParallelEnvironmentFilter;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QQuotaOptions_setPeFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QQuotaOptions_setProjectFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QQuotaOptions_setProjectFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QQuotaOptions", "setProjectFilter", "(Lcom/sun/grid/jgdi/monitoring/filter/ProjectFilter;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QQuotaOptions_setProjectFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QQuotaOptions_setQueueFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QQuotaOptions_setQueueFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QQuotaOptions", "setQueueFilter", "(Lcom/sun/grid/jgdi/monitoring/filter/QueueFilter;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QQuotaOptions_setQueueFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QQuotaOptions_setResourceFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QQuotaOptions_setResourceFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QQuotaOptions", "setResourceFilter", "(Lcom/sun/grid/jgdi/monitoring/filter/ResourceFilter;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QQuotaOptions_setResourceFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QQuotaOptions_setUserFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QQuotaOptions_setUserFilter");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QQuotaOptions", "setUserFilter", "(Lcom/sun/grid/jgdi/monitoring/filter/UserFilter;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QQuotaOptions_setUserFilter failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QQuotaOptions */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QQuotaResult */

jclass QQuotaResult_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QQuotaResult_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QQuotaResult", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QQuotaResult_getResourceQuotaRuleInfo(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QQuotaResult_getResourceQuotaRuleInfo");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QQuotaResult", "getResourceQuotaRuleInfo", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfo;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QQuotaResult_getResourceQuotaRuleInfo failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QQuotaResult_getResourceQuotaRuleNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QQuotaResult_getResourceQuotaRuleNames");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QQuotaResult", "getResourceQuotaRuleNames", "()Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QQuotaResult_getResourceQuotaRuleNames failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QQuotaResult_getResourceQuotaRules(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QQuotaResult_getResourceQuotaRules");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QQuotaResult", "getResourceQuotaRules", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QQuotaResult_getResourceQuotaRules failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QQuotaResult */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/HostInfo */

jclass HostInfo_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "HostInfo_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/HostInfo", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t HostInfo_getArch(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfo_getArch");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfo", "getArch", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfo_getArch failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfo_getDominanceSet(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfo_getDominanceSet");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfo", "getDominanceSet", "()Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfo_getDominanceSet failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfo_getHostValue(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfo_getHostValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfo", "getHostValue", "(Ljava/lang/String;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "HostInfo_getHostValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfo_getHostValueCount(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "HostInfo_getHostValueCount");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfo", "getHostValueCount", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfo_getHostValueCount failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfo_getHostValueKeys(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfo_getHostValueKeys");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfo", "getHostValueKeys", "()Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfo_getHostValueKeys failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfo_getHostname(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfo_getHostname");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfo", "getHostname", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfo_getHostname failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfo_getJobCount(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "HostInfo_getJobCount");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfo", "getJobCount", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfo_getJobCount failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfo_getJobList(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfo_getJobList");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfo", "getJobList", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfo_getJobList failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfo_getLoadAvg(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfo_getLoadAvg");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfo", "getLoadAvg", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfo_getLoadAvg failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfo_getMemTotal(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfo_getMemTotal");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfo", "getMemTotal", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfo_getMemTotal failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfo_getMemUsed(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfo_getMemUsed");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfo", "getMemUsed", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfo_getMemUsed failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfo_getNumberOfProcessors(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfo_getNumberOfProcessors");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfo", "getNumberOfProcessors", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfo_getNumberOfProcessors failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfo_getQueueCount(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "HostInfo_getQueueCount");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfo", "getQueueCount", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfo_getQueueCount failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfo_getQueueList(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfo_getQueueList");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfo", "getQueueList", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfo_getQueueList failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfo_getResourceValue(JNIEnv *env, jobject obj , const char* p0 , const char* p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jstring p1_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfo_getResourceValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfo", "getResourceValue", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   if (p1 != NULL) {
      p1_obj = (*env)->NewStringUTF(env, p1 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj, p1_obj);  
   if (test_jni_error(env, "HostInfo_getResourceValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfo_getResourceValueNames(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfo_getResourceValueNames");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfo", "getResourceValueNames", "(Ljava/lang/String;)Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "HostInfo_getResourceValueNames failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfo_getSwapTotal(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfo_getSwapTotal");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfo", "getSwapTotal", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfo_getSwapTotal failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfo_getSwapUsed(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfo_getSwapUsed");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfo", "getSwapUsed", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfo_getSwapUsed failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/HostInfo */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummary */

jclass QueueInstanceSummary_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummary_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummary", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummary_getArch(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummary_getArch");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummary", "getArch", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummary_getArch failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummary_getExplainMessageList(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummary_getExplainMessageList");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummary", "getExplainMessageList", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummary_getExplainMessageList failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummary_getFreeSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "QueueInstanceSummary_getFreeSlots");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummary", "getFreeSlots", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummary_getFreeSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummary_getJobList(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummary_getJobList");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummary", "getJobList", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummary_getJobList failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummary_getLoadAlarmReason(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummary_getLoadAlarmReason");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummary", "getLoadAlarmReason", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummary_getLoadAlarmReason failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummary_getLoadAvg(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "QueueInstanceSummary_getLoadAvg");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummary", "getLoadAvg", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummary_getLoadAvg failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummary_getLoadAvgStr(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummary_getLoadAvgStr");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummary", "getLoadAvgStr", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummary_getLoadAvgStr failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummary_getName(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummary_getName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummary", "getName", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummary_getName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummary_getQueueType(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummary_getQueueType");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummary", "getQueueType", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummary_getQueueType failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummary_getReservedSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "QueueInstanceSummary_getReservedSlots");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummary", "getReservedSlots", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummary_getReservedSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummary_getResourceDominanceSet(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummary_getResourceDominanceSet");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummary", "getResourceDominanceSet", "()Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummary_getResourceDominanceSet failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummary_getResourceNames(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummary_getResourceNames");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummary", "getResourceNames", "(Ljava/lang/String;)Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QueueInstanceSummary_getResourceNames failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummary_getResourceValue(JNIEnv *env, jobject obj , const char* p0 , const char* p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jstring p1_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummary_getResourceValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummary", "getResourceValue", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   if (p1 != NULL) {
      p1_obj = (*env)->NewStringUTF(env, p1 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj, p1_obj);  
   if (test_jni_error(env, "QueueInstanceSummary_getResourceValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummary_getState(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummary_getState");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummary", "getState", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummary_getState failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummary_getSuspendAlarmReason(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummary_getSuspendAlarmReason");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummary", "getSuspendAlarmReason", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummary_getSuspendAlarmReason failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummary_getTotalSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "QueueInstanceSummary_getTotalSlots");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummary", "getTotalSlots", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummary_getTotalSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummary_getUsedSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "QueueInstanceSummary_getUsedSlots");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummary", "getUsedSlots", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummary_getUsedSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummary_hasLoadValue(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueInstanceSummary_hasLoadValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummary", "hasLoadValue", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummary_hasLoadValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummary_isHasLoadValueFromObject(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueInstanceSummary_isHasLoadValueFromObject");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummary", "isHasLoadValueFromObject", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummary_isHasLoadValueFromObject failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummary */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/JobSummary */

jclass JobSummary_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "JobSummary_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/JobSummary", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t JobSummary_addSoftRequestedMasterQueue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JobSummary_addSoftRequestedMasterQueue");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "addSoftRequestedMasterQueue", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobSummary_addSoftRequestedMasterQueue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummary_addSoftRequestedQueue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JobSummary_addSoftRequestedQueue");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "addSoftRequestedQueue", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobSummary_addSoftRequestedQueue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummary_getArrayPredecessors(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummary_getArrayPredecessors");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getArrayPredecessors", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getArrayPredecessors failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getCheckpointEnv(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummary_getCheckpointEnv");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getCheckpointEnv", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getCheckpointEnv failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getCpuUsage(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "JobSummary_getCpuUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getCpuUsage", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getCpuUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getDeadline(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummary_getDeadline");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getDeadline", "()Ljava/util/Date;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getDeadline failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getDepartment(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummary_getDepartment");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getDepartment", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getDepartment failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getDlcontr(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobSummary_getDlcontr");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getDlcontr", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getDlcontr failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getFtickets(JNIEnv *env, jobject obj , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "JobSummary_getFtickets");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getFtickets", "()J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getFtickets failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getGrantedPEName(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummary_getGrantedPEName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getGrantedPEName", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getGrantedPEName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getGrantedPESlots(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "JobSummary_getGrantedPESlots");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getGrantedPESlots", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getGrantedPESlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getHardRequestNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummary_getHardRequestNames");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getHardRequestNames", "()Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getHardRequestNames failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getHardRequestValue(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummary_getHardRequestValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getHardRequestValue", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/HardRequestValue;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobSummary_getHardRequestValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getHardRequestedMasterQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummary_getHardRequestedMasterQueues");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getHardRequestedMasterQueues", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getHardRequestedMasterQueues failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getHardRequestedQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummary_getHardRequestedQueues");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getHardRequestedQueues", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getHardRequestedQueues failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getIoUsage(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobSummary_getIoUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getIoUsage", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getIoUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getMemUsage(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobSummary_getMemUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getMemUsage", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getMemUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getNormalizedPriority(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobSummary_getNormalizedPriority");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getNormalizedPriority", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getNormalizedPriority failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getNormalizedRequestedPriority(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobSummary_getNormalizedRequestedPriority");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getNormalizedRequestedPriority", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getNormalizedRequestedPriority failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getNormalizedTickets(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobSummary_getNormalizedTickets");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getNormalizedTickets", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getNormalizedTickets failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getNormalizedUrgency(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobSummary_getNormalizedUrgency");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getNormalizedUrgency", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getNormalizedUrgency failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getOtickets(JNIEnv *env, jobject obj , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "JobSummary_getOtickets");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getOtickets", "()J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getOtickets failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getOverrideTickets(JNIEnv *env, jobject obj , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "JobSummary_getOverrideTickets");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getOverrideTickets", "()J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getOverrideTickets failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getParallelEnvironmentName(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummary_getParallelEnvironmentName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getParallelEnvironmentName", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getParallelEnvironmentName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getParallelEnvironmentRange(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummary_getParallelEnvironmentRange");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getParallelEnvironmentRange", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getParallelEnvironmentRange failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getPredecessors(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummary_getPredecessors");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getPredecessors", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getPredecessors failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getProject(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummary_getProject");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getProject", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getProject failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getRequestNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummary_getRequestNames");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getRequestNames", "()Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getRequestNames failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getRequestValue(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummary_getRequestValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getRequestValue", "(Ljava/lang/String;)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobSummary_getRequestValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getRequestedArrayPredecessors(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummary_getRequestedArrayPredecessors");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getRequestedArrayPredecessors", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getRequestedArrayPredecessors failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getRequestedPredecessors(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummary_getRequestedPredecessors");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getRequestedPredecessors", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getRequestedPredecessors failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getRrcontr(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobSummary_getRrcontr");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getRrcontr", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getRrcontr failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getShare(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobSummary_getShare");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getShare", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getShare failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "JobSummary_getSlots");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getSlots", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getSoftRequestNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummary_getSoftRequestNames");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getSoftRequestNames", "()Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getSoftRequestNames failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getSoftRequestValue(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummary_getSoftRequestValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getSoftRequestValue", "(Ljava/lang/String;)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobSummary_getSoftRequestValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getSoftRequestedMasterQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummary_getSoftRequestedMasterQueues");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getSoftRequestedMasterQueues", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getSoftRequestedMasterQueues failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getSoftRequestedQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummary_getSoftRequestedQueues");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getSoftRequestedQueues", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getSoftRequestedQueues failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getStickets(JNIEnv *env, jobject obj , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "JobSummary_getStickets");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getStickets", "()J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getStickets failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getTaskCount(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "JobSummary_getTaskCount");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getTaskCount", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getTaskCount failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getTaskList(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummary_getTaskList");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getTaskList", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getTaskList failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getTickets(JNIEnv *env, jobject obj , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "JobSummary_getTickets");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getTickets", "()J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getTickets failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getUrgency(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobSummary_getUrgency");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getUrgency", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getUrgency failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_getWtcontr(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobSummary_getWtcontr");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "getWtcontr", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_getWtcontr failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_hasCpuUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JobSummary_hasCpuUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "hasCpuUsage", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_hasCpuUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_hasIoUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JobSummary_hasIoUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "hasIoUsage", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_hasIoUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_hasMemUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JobSummary_hasMemUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "hasMemUsage", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_hasMemUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_isArray(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JobSummary_isArray");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "isArray", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_isArray failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_isQueueAssigned(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JobSummary_isQueueAssigned");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "isQueueAssigned", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_isQueueAssigned failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_isRunning(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JobSummary_isRunning");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "isRunning", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_isRunning failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_isZombie(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JobSummary_isZombie");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "isZombie", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummary_isZombie failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummary_setNormalizedUrgency(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummary_setNormalizedUrgency");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "setNormalizedUrgency", "(D)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummary_setNormalizedUrgency failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummary_setSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummary_setSlots");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummary", "setSlots", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummary_setSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/JobSummary */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/JobInfo */

jclass JobInfo_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "JobInfo_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/JobInfo", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t JobInfo_getId(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "JobInfo_getId");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfo", "getId", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "JobInfo_getId failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobInfo_getMasterQueue(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobInfo_getMasterQueue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfo", "getMasterQueue", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobInfo_getMasterQueue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobInfo_getName(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobInfo_getName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfo", "getName", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobInfo_getName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobInfo_getPriority(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobInfo_getPriority");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfo", "getPriority", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobInfo_getPriority failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobInfo_getQinstanceName(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobInfo_getQinstanceName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfo", "getQinstanceName", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobInfo_getQinstanceName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobInfo_getQueue(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobInfo_getQueue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfo", "getQueue", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobInfo_getQueue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobInfo_getStartTime(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobInfo_getStartTime");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfo", "getStartTime", "()Ljava/util/Date;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobInfo_getStartTime failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobInfo_getState(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobInfo_getState");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfo", "getState", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobInfo_getState failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobInfo_getSubmitTime(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobInfo_getSubmitTime");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfo", "getSubmitTime", "()Ljava/util/Date;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobInfo_getSubmitTime failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobInfo_getTaskId(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobInfo_getTaskId");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfo", "getTaskId", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobInfo_getTaskId failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobInfo_getUser(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobInfo_getUser");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfo", "getUser", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobInfo_getUser failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/JobInfo */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfo */

jclass ResourceQuotaRuleInfo_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfo_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfo", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t ResourceQuotaRuleInfo_getHosts(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfo_getHosts");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfo", "getHosts", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfo_getHosts failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfo_getLimits(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfo_getLimits");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfo", "getLimits", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfo_getLimits failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfo_getPes(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfo_getPes");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfo", "getPes", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfo_getPes failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfo_getProjects(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfo_getProjects");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfo", "getProjects", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfo_getProjects failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfo_getQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfo_getQueues");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfo", "getQueues", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfo_getQueues failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfo_getResouceQuotaRuleName(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfo_getResouceQuotaRuleName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfo", "getResouceQuotaRuleName", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfo_getResouceQuotaRuleName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfo_getUsers(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfo_getUsers");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfo", "getUsers", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfo_getUsers failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfo_getXHosts(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfo_getXHosts");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfo", "getXHosts", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfo_getXHosts failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfo_getXPes(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfo_getXPes");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfo", "getXPes", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfo_getXPes failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfo_getXProjects(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfo_getXProjects");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfo", "getXProjects", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfo_getXProjects failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfo_getXQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfo_getXQueues");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfo", "getXQueues", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfo_getXQueues failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfo_getXUsers(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfo_getXUsers");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfo", "getXUsers", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfo_getXUsers failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfo */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInfo */

jclass QueueInfo_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInfo_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInfo", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInfo_getQname(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInfo_getQname");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInfo", "getQname", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInfo_getQname failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInfo_getQtype(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInfo_getQtype");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInfo", "getQtype", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInfo_getQtype failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInfo_getReservedSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "QueueInfo_getReservedSlots");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInfo", "getReservedSlots", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInfo_getReservedSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInfo_getState(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInfo_getState");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInfo", "getState", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInfo_getState failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInfo_getTotalSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "QueueInfo_getTotalSlots");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInfo", "getTotalSlots", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInfo_getTotalSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInfo_getUsedSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "QueueInfo_getUsedSlots");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInfo", "getUsedSlots", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInfo_getUsedSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInfo */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/HardRequestValue */

jclass HardRequestValue_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "HardRequestValue_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/HardRequestValue", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t HardRequestValue_init(JNIEnv *env, jobject *obj , const char* p0 , const char* p1 , jdouble p2 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;     jstring p0_obj = NULL;     jstring p1_obj = NULL;
   DENTER(BASIS_LAYER, "HardRequestValue_init");
      
   clazz = HardRequestValue_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(Ljava/lang/String;, Ljava/lang/String;, D)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   } 
   if ( p0 != NULL ) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   if ( p1 != NULL ) {
      p1_obj = (*env)->NewStringUTF(env, p1 ); 
   }   
   *obj = (*env)->NewObject(env, clazz, mid , p0_obj , p1_obj , p2 );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t HardRequestValue_getContribution(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "HardRequestValue_getContribution");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HardRequestValue", "getContribution", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "HardRequestValue_getContribution failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HardRequestValue_getResource(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HardRequestValue_getResource");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HardRequestValue", "getResource", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HardRequestValue_getResource failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HardRequestValue_getValue(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HardRequestValue_getValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HardRequestValue", "getValue", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HardRequestValue_getValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/HardRequestValue */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/ResourceQuota */

jclass ResourceQuota_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "ResourceQuota_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/ResourceQuota", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t ResourceQuota_getLimitValue(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuota_getLimitValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuota", "getLimitValue", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuota_getLimitValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuota_getName(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuota_getName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuota", "getName", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuota_getName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuota_getUsageValue(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuota_getUsageValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuota", "getUsageValue", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuota_getUsageValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/ResourceQuota */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QHostResultImpl */

jclass QHostResultImpl_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QHostResultImpl_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QHostResultImpl", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QHostResultImpl_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "QHostResultImpl_init");
      
   clazz = QHostResultImpl_find_class(env, alpp);
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

jgdi_result_t QHostResultImpl_addHostInfo(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QHostResultImpl_addHostInfo");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QHostResultImpl", "addHostInfo", "(Lcom/sun/grid/jgdi/monitoring/HostInfo;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QHostResultImpl_addHostInfo failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QHostResultImpl_createHostInfo(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QHostResultImpl_createHostInfo");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QHostResultImpl", "createHostInfo", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/HostInfo;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QHostResultImpl_createHostInfo failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QHostResultImpl_getHostInfo(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QHostResultImpl_getHostInfo");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QHostResultImpl", "getHostInfo", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QHostResultImpl_getHostInfo failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QHostResultImpl_getHostInfo_0(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QHostResultImpl_getHostInfo_0");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QHostResultImpl", "getHostInfo", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/HostInfo;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QHostResultImpl_getHostInfo_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QHostResultImpl_getHostNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QHostResultImpl_getHostNames");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QHostResultImpl", "getHostNames", "()Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QHostResultImpl_getHostNames failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QHostResultImpl */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryResultImpl */

jclass QueueInstanceSummaryResultImpl_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryResultImpl_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryResultImpl", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummaryResultImpl_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "QueueInstanceSummaryResultImpl_init");
      
   clazz = QueueInstanceSummaryResultImpl_find_class(env, alpp);
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

jgdi_result_t QueueInstanceSummaryResultImpl_addErrorJobs(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryResultImpl_addErrorJobs");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryResultImpl", "addErrorJobs", "(Ljava/util/List;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryResultImpl_addErrorJobs failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryResultImpl_addFinishedJobs(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryResultImpl_addFinishedJobs");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryResultImpl", "addFinishedJobs", "(Ljava/util/List;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryResultImpl_addFinishedJobs failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryResultImpl_addPendingJobs(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryResultImpl_addPendingJobs");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryResultImpl", "addPendingJobs", "(Ljava/util/List;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryResultImpl_addPendingJobs failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryResultImpl_addQueueInstanceSummary(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryResultImpl_addQueueInstanceSummary");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryResultImpl", "addQueueInstanceSummary", "(Lcom/sun/grid/jgdi/monitoring/QueueInstanceSummary;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryResultImpl_addQueueInstanceSummary failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryResultImpl_addZombieJobs(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryResultImpl_addZombieJobs");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryResultImpl", "addZombieJobs", "(Ljava/util/List;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryResultImpl_addZombieJobs failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryResultImpl_getErrorJobs(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryResultImpl_getErrorJobs");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryResultImpl", "getErrorJobs", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryResultImpl_getErrorJobs failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryResultImpl_getFinishedJobs(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryResultImpl_getFinishedJobs");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryResultImpl", "getFinishedJobs", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryResultImpl_getFinishedJobs failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryResultImpl_getPendingJobs(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryResultImpl_getPendingJobs");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryResultImpl", "getPendingJobs", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryResultImpl_getPendingJobs failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryResultImpl_getQueueInstanceSummary(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryResultImpl_getQueueInstanceSummary");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryResultImpl", "getQueueInstanceSummary", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryResultImpl_getQueueInstanceSummary failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryResultImpl_getZombieJobs(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryResultImpl_getZombieJobs");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryResultImpl", "getZombieJobs", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryResultImpl_getZombieJobs failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryResultImpl */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QQuotaResultImpl */

jclass QQuotaResultImpl_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QQuotaResultImpl_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QQuotaResultImpl", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QQuotaResultImpl_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "QQuotaResultImpl_init");
      
   clazz = QQuotaResultImpl_find_class(env, alpp);
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

jgdi_result_t QQuotaResultImpl_addResourceQuotaRuleInfo(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QQuotaResultImpl_addResourceQuotaRuleInfo");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QQuotaResultImpl", "addResourceQuotaRuleInfo", "(Lcom/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfo;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QQuotaResultImpl_addResourceQuotaRuleInfo failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QQuotaResultImpl_createResourceQuotaRuleInfo(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QQuotaResultImpl_createResourceQuotaRuleInfo");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QQuotaResultImpl", "createResourceQuotaRuleInfo", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfo;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QQuotaResultImpl_createResourceQuotaRuleInfo failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QQuotaResultImpl_getResourceQuotaRuleInfo(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QQuotaResultImpl_getResourceQuotaRuleInfo");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QQuotaResultImpl", "getResourceQuotaRuleInfo", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfo;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QQuotaResultImpl_getResourceQuotaRuleInfo failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QQuotaResultImpl_getResourceQuotaRuleNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QQuotaResultImpl_getResourceQuotaRuleNames");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QQuotaResultImpl", "getResourceQuotaRuleNames", "()Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QQuotaResultImpl_getResourceQuotaRuleNames failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QQuotaResultImpl_getResourceQuotaRules(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QQuotaResultImpl_getResourceQuotaRules");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QQuotaResultImpl", "getResourceQuotaRules", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QQuotaResultImpl_getResourceQuotaRules failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QQuotaResultImpl */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/HostInfoImpl */

jclass HostInfoImpl_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "HostInfoImpl_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/HostInfoImpl", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t HostInfoImpl_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "HostInfoImpl_init");
      
   clazz = HostInfoImpl_find_class(env, alpp);
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

jgdi_result_t HostInfoImpl_init_0(JNIEnv *env, jobject *obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;     jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "HostInfoImpl_init_0");
      
   clazz = HostInfoImpl_find_class(env, alpp);
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

jgdi_result_t HostInfoImpl_addJob(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "HostInfoImpl_addJob");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "addJob", "(Lcom/sun/grid/jgdi/monitoring/JobInfo;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "HostInfoImpl_addJob failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t HostInfoImpl_addQueue(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "HostInfoImpl_addQueue");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "addQueue", "(Lcom/sun/grid/jgdi/monitoring/QueueInfo;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "HostInfoImpl_addQueue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t HostInfoImpl_getArch(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfoImpl_getArch");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "getArch", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfoImpl_getArch failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfoImpl_getDominanceSet(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfoImpl_getDominanceSet");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "getDominanceSet", "()Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfoImpl_getDominanceSet failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfoImpl_getHostValue(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfoImpl_getHostValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "getHostValue", "(Ljava/lang/String;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "HostInfoImpl_getHostValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfoImpl_getHostValueCount(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "HostInfoImpl_getHostValueCount");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "getHostValueCount", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfoImpl_getHostValueCount failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfoImpl_getHostValueKeys(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfoImpl_getHostValueKeys");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "getHostValueKeys", "()Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfoImpl_getHostValueKeys failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfoImpl_getHostname(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfoImpl_getHostname");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "getHostname", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfoImpl_getHostname failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfoImpl_getJobCount(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "HostInfoImpl_getJobCount");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "getJobCount", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfoImpl_getJobCount failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfoImpl_getJobList(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfoImpl_getJobList");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "getJobList", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfoImpl_getJobList failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfoImpl_getLoadAvg(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfoImpl_getLoadAvg");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "getLoadAvg", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfoImpl_getLoadAvg failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfoImpl_getMemTotal(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfoImpl_getMemTotal");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "getMemTotal", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfoImpl_getMemTotal failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfoImpl_getMemUsed(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfoImpl_getMemUsed");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "getMemUsed", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfoImpl_getMemUsed failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfoImpl_getNumberOfProcessors(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfoImpl_getNumberOfProcessors");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "getNumberOfProcessors", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfoImpl_getNumberOfProcessors failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfoImpl_getQueueCount(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "HostInfoImpl_getQueueCount");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "getQueueCount", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfoImpl_getQueueCount failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfoImpl_getQueueList(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfoImpl_getQueueList");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "getQueueList", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfoImpl_getQueueList failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfoImpl_getResourceValue(JNIEnv *env, jobject obj , const char* p0 , const char* p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jstring p1_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfoImpl_getResourceValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "getResourceValue", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   if (p1 != NULL) {
      p1_obj = (*env)->NewStringUTF(env, p1 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj, p1_obj);  
   if (test_jni_error(env, "HostInfoImpl_getResourceValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfoImpl_getResourceValueNames(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfoImpl_getResourceValueNames");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "getResourceValueNames", "(Ljava/lang/String;)Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "HostInfoImpl_getResourceValueNames failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfoImpl_getSwapTotal(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfoImpl_getSwapTotal");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "getSwapTotal", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfoImpl_getSwapTotal failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfoImpl_getSwapUsed(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostInfoImpl_getSwapUsed");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "getSwapUsed", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfoImpl_getSwapUsed failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostInfoImpl_putHostValue(JNIEnv *env, jobject obj , const char* p0 , jobject p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "HostInfoImpl_putHostValue");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "putHostValue", "(Ljava/lang/String;Ljava/lang/Object;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj, p1);  
   if (test_jni_error(env, "HostInfoImpl_putHostValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t HostInfoImpl_putResourceValue(JNIEnv *env, jobject obj , const char* p0 , const char* p1 , jobject p2 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jstring p1_obj = NULL;
   DENTER(BASIS_LAYER, "HostInfoImpl_putResourceValue");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "putResourceValue", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   if (p1 != NULL) {
      p1_obj = (*env)->NewStringUTF(env, p1 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj, p1_obj, p2);  
   if (test_jni_error(env, "HostInfoImpl_putResourceValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t HostInfoImpl_removeAllHostValue(JNIEnv *env, jobject obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "HostInfoImpl_removeAllHostValue");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "removeAllHostValue", "()V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid);  
   if (test_jni_error(env, "HostInfoImpl_removeAllHostValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t HostInfoImpl_removeHostValue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "HostInfoImpl_removeHostValue");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "removeHostValue", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "HostInfoImpl_removeHostValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t HostInfoImpl_setHostname(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "HostInfoImpl_setHostname");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/HostInfoImpl", "setHostname", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "HostInfoImpl_setHostname failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/HostInfoImpl */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/JobInfoImpl */

jclass JobInfoImpl_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "JobInfoImpl_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/JobInfoImpl", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t JobInfoImpl_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "JobInfoImpl_init");
      
   clazz = JobInfoImpl_find_class(env, alpp);
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

jgdi_result_t JobInfoImpl_getId(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "JobInfoImpl_getId");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "getId", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "JobInfoImpl_getId failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobInfoImpl_getMasterQueue(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobInfoImpl_getMasterQueue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "getMasterQueue", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobInfoImpl_getMasterQueue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobInfoImpl_getName(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobInfoImpl_getName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "getName", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobInfoImpl_getName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobInfoImpl_getPriority(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobInfoImpl_getPriority");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "getPriority", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobInfoImpl_getPriority failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobInfoImpl_getQinstanceName(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobInfoImpl_getQinstanceName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "getQinstanceName", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobInfoImpl_getQinstanceName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobInfoImpl_getQueue(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobInfoImpl_getQueue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "getQueue", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobInfoImpl_getQueue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobInfoImpl_getStartTime(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobInfoImpl_getStartTime");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "getStartTime", "()Ljava/util/Date;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobInfoImpl_getStartTime failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobInfoImpl_getState(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobInfoImpl_getState");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "getState", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobInfoImpl_getState failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobInfoImpl_getSubmitTime(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobInfoImpl_getSubmitTime");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "getSubmitTime", "()Ljava/util/Date;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobInfoImpl_getSubmitTime failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobInfoImpl_getTaskId(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobInfoImpl_getTaskId");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "getTaskId", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobInfoImpl_getTaskId failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobInfoImpl_getUser(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobInfoImpl_getUser");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "getUser", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobInfoImpl_getUser failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobInfoImpl_setId(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobInfoImpl_setId");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "setId", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobInfoImpl_setId failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobInfoImpl_setMasterQueue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JobInfoImpl_setMasterQueue");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "setMasterQueue", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobInfoImpl_setMasterQueue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobInfoImpl_setName(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JobInfoImpl_setName");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "setName", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobInfoImpl_setName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobInfoImpl_setPriority(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobInfoImpl_setPriority");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "setPriority", "(D)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobInfoImpl_setPriority failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobInfoImpl_setQinstanceName(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JobInfoImpl_setQinstanceName");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "setQinstanceName", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobInfoImpl_setQinstanceName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobInfoImpl_setQueue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JobInfoImpl_setQueue");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "setQueue", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobInfoImpl_setQueue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobInfoImpl_setStartTime(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobInfoImpl_setStartTime");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "setStartTime", "(Ljava/util/Date;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobInfoImpl_setStartTime failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobInfoImpl_setStartTime_0(JNIEnv *env, jobject obj , jlong p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobInfoImpl_setStartTime_0");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "setStartTime", "(J)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobInfoImpl_setStartTime_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobInfoImpl_setState(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JobInfoImpl_setState");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "setState", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobInfoImpl_setState failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobInfoImpl_setSubmitTime(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobInfoImpl_setSubmitTime");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "setSubmitTime", "(Ljava/util/Date;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobInfoImpl_setSubmitTime failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobInfoImpl_setSubmitTime_0(JNIEnv *env, jobject obj , jlong p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobInfoImpl_setSubmitTime_0");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "setSubmitTime", "(J)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobInfoImpl_setSubmitTime_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobInfoImpl_setTaskId(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JobInfoImpl_setTaskId");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "setTaskId", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobInfoImpl_setTaskId failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobInfoImpl_setUser(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JobInfoImpl_setUser");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobInfoImpl", "setUser", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobInfoImpl_setUser failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/JobInfoImpl */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/JobSummaryImpl */

jclass JobSummaryImpl_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t JobSummaryImpl_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "JobSummaryImpl_init");
      
   clazz = JobSummaryImpl_find_class(env, alpp);
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

jgdi_result_t JobSummaryImpl_addArrayPredecessor(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_addArrayPredecessor");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "addArrayPredecessor", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_addArrayPredecessor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_addHardRequest(JNIEnv *env, jobject obj , const char* p0 , const char* p1 , jdouble p2 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jstring p1_obj = NULL;
   DENTER(BASIS_LAYER, "JobSummaryImpl_addHardRequest");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "addHardRequest", "(Ljava/lang/String;Ljava/lang/String;D)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   if (p1 != NULL) {
      p1_obj = (*env)->NewStringUTF(env, p1 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj, p1_obj, p2);  
   if (test_jni_error(env, "JobSummaryImpl_addHardRequest failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_addHardRequestedMasterQueue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JobSummaryImpl_addHardRequestedMasterQueue");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "addHardRequestedMasterQueue", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobSummaryImpl_addHardRequestedMasterQueue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_addHardRequestedQueue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JobSummaryImpl_addHardRequestedQueue");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "addHardRequestedQueue", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobSummaryImpl_addHardRequestedQueue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_addPredecessor(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_addPredecessor");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "addPredecessor", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_addPredecessor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_addRequest(JNIEnv *env, jobject obj , const char* p0 , const char* p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jstring p1_obj = NULL;
   DENTER(BASIS_LAYER, "JobSummaryImpl_addRequest");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "addRequest", "(Ljava/lang/String;Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   if (p1 != NULL) {
      p1_obj = (*env)->NewStringUTF(env, p1 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj, p1_obj);  
   if (test_jni_error(env, "JobSummaryImpl_addRequest failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_addRequestedArrayPredecessor(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JobSummaryImpl_addRequestedArrayPredecessor");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "addRequestedArrayPredecessor", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobSummaryImpl_addRequestedArrayPredecessor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_addRequestedPredecessor(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JobSummaryImpl_addRequestedPredecessor");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "addRequestedPredecessor", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobSummaryImpl_addRequestedPredecessor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_addSoftRequest(JNIEnv *env, jobject obj , const char* p0 , const char* p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jstring p1_obj = NULL;
   DENTER(BASIS_LAYER, "JobSummaryImpl_addSoftRequest");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "addSoftRequest", "(Ljava/lang/String;Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   if (p1 != NULL) {
      p1_obj = (*env)->NewStringUTF(env, p1 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj, p1_obj);  
   if (test_jni_error(env, "JobSummaryImpl_addSoftRequest failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_addSoftRequestedMasterQueue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JobSummaryImpl_addSoftRequestedMasterQueue");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "addSoftRequestedMasterQueue", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobSummaryImpl_addSoftRequestedMasterQueue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_addSoftRequestedQueue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JobSummaryImpl_addSoftRequestedQueue");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "addSoftRequestedQueue", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobSummaryImpl_addSoftRequestedQueue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_addTask(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_addTask");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "addTask", "(Lcom/sun/grid/jgdi/monitoring/TaskSummary;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_addTask failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getArrayPredecessors(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getArrayPredecessors");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getArrayPredecessors", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getArrayPredecessors failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getCheckpointEnv(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getCheckpointEnv");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getCheckpointEnv", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getCheckpointEnv failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getCpuUsage(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getCpuUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getCpuUsage", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getCpuUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getDeadline(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getDeadline");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getDeadline", "()Ljava/util/Date;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getDeadline failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getDepartment(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getDepartment");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getDepartment", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getDepartment failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getDlcontr(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getDlcontr");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getDlcontr", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getDlcontr failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getFtickets(JNIEnv *env, jobject obj , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getFtickets");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getFtickets", "()J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getFtickets failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getGrantedPEName(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getGrantedPEName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getGrantedPEName", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getGrantedPEName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getGrantedPESlots(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getGrantedPESlots");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getGrantedPESlots", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getGrantedPESlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getHardRequestNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getHardRequestNames");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getHardRequestNames", "()Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getHardRequestNames failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getHardRequestValue(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getHardRequestValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getHardRequestValue", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/HardRequestValue;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobSummaryImpl_getHardRequestValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getHardRequestedMasterQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getHardRequestedMasterQueues");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getHardRequestedMasterQueues", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getHardRequestedMasterQueues failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getHardRequestedQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getHardRequestedQueues");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getHardRequestedQueues", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getHardRequestedQueues failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getIoUsage(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getIoUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getIoUsage", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getIoUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getMemUsage(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getMemUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getMemUsage", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getMemUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getNormalizedPriority(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getNormalizedPriority");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getNormalizedPriority", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getNormalizedPriority failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getNormalizedRequestedPriority(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getNormalizedRequestedPriority");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getNormalizedRequestedPriority", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getNormalizedRequestedPriority failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getNormalizedTickets(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getNormalizedTickets");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getNormalizedTickets", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getNormalizedTickets failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getNormalizedUrgency(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getNormalizedUrgency");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getNormalizedUrgency", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getNormalizedUrgency failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getOtickets(JNIEnv *env, jobject obj , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getOtickets");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getOtickets", "()J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getOtickets failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getOverrideTickets(JNIEnv *env, jobject obj , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getOverrideTickets");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getOverrideTickets", "()J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getOverrideTickets failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getParallelEnvironmentName(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getParallelEnvironmentName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getParallelEnvironmentName", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getParallelEnvironmentName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getParallelEnvironmentRange(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getParallelEnvironmentRange");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getParallelEnvironmentRange", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getParallelEnvironmentRange failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getPredecessors(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getPredecessors");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getPredecessors", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getPredecessors failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getProject(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getProject");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getProject", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getProject failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getRequestNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getRequestNames");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getRequestNames", "()Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getRequestNames failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getRequestValue(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getRequestValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getRequestValue", "(Ljava/lang/String;)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobSummaryImpl_getRequestValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getRequestedArrayPredecessors(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getRequestedArrayPredecessors");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getRequestedArrayPredecessors", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getRequestedArrayPredecessors failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getRequestedPredecessors(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getRequestedPredecessors");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getRequestedPredecessors", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getRequestedPredecessors failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getRrcontr(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getRrcontr");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getRrcontr", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getRrcontr failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getShare(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getShare");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getShare", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getShare failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getSlots");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getSlots", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getSoftRequestNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getSoftRequestNames");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getSoftRequestNames", "()Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getSoftRequestNames failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getSoftRequestValue(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getSoftRequestValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getSoftRequestValue", "(Ljava/lang/String;)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobSummaryImpl_getSoftRequestValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getSoftRequestedMasterQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getSoftRequestedMasterQueues");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getSoftRequestedMasterQueues", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getSoftRequestedMasterQueues failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getSoftRequestedQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getSoftRequestedQueues");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getSoftRequestedQueues", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getSoftRequestedQueues failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getStickets(JNIEnv *env, jobject obj , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getStickets");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getStickets", "()J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getStickets failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getTaskCount(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getTaskCount");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getTaskCount", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getTaskCount failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getTaskList(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getTaskList");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getTaskList", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getTaskList failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getTickets(JNIEnv *env, jobject obj , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getTickets");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getTickets", "()J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getTickets failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getUrgency(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getUrgency");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getUrgency", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getUrgency failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_getWtcontr(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "JobSummaryImpl_getWtcontr");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "getWtcontr", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_getWtcontr failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_hasCpuUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JobSummaryImpl_hasCpuUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "hasCpuUsage", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_hasCpuUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_hasIoUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JobSummaryImpl_hasIoUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "hasIoUsage", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_hasIoUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_hasMemUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JobSummaryImpl_hasMemUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "hasMemUsage", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_hasMemUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_isArray(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JobSummaryImpl_isArray");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "isArray", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_isArray failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_isQueueAssigned(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JobSummaryImpl_isQueueAssigned");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "isQueueAssigned", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_isQueueAssigned failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_isRunning(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JobSummaryImpl_isRunning");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "isRunning", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_isRunning failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_isZombie(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JobSummaryImpl_isZombie");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "isZombie", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "JobSummaryImpl_isZombie failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setArray(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setArray");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setArray", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setArray failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setCheckpointEnv(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JobSummaryImpl_setCheckpointEnv");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setCheckpointEnv", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobSummaryImpl_setCheckpointEnv failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setCpuUsage(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setCpuUsage");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setCpuUsage", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setCpuUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setDeadline(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setDeadline");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setDeadline", "(Ljava/util/Date;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setDeadline failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setDeadline_0(JNIEnv *env, jobject obj , jlong p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setDeadline_0");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setDeadline", "(J)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setDeadline_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setDepartment(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JobSummaryImpl_setDepartment");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setDepartment", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobSummaryImpl_setDepartment failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setDlcontr(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setDlcontr");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setDlcontr", "(D)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setDlcontr failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setFtickets(JNIEnv *env, jobject obj , jlong p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setFtickets");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setFtickets", "(J)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setFtickets failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setGrantedPEName(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JobSummaryImpl_setGrantedPEName");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setGrantedPEName", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobSummaryImpl_setGrantedPEName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setGrantedPESlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setGrantedPESlots");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setGrantedPESlots", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setGrantedPESlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setIoUsage(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setIoUsage");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setIoUsage", "(D)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setIoUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setMemUsage(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setMemUsage");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setMemUsage", "(D)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setMemUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setNormalizedPriority(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setNormalizedPriority");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setNormalizedPriority", "(D)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setNormalizedPriority failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setNormalizedRequestedPriority(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setNormalizedRequestedPriority");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setNormalizedRequestedPriority", "(D)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setNormalizedRequestedPriority failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setNormalizedTickets(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setNormalizedTickets");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setNormalizedTickets", "(D)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setNormalizedTickets failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setNormalizedUrgency(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setNormalizedUrgency");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setNormalizedUrgency", "(D)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setNormalizedUrgency failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setOtickets(JNIEnv *env, jobject obj , jlong p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setOtickets");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setOtickets", "(J)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setOtickets failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setOverrideTickets(JNIEnv *env, jobject obj , jlong p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setOverrideTickets");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setOverrideTickets", "(J)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setOverrideTickets failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setParallelEnvironmentName(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JobSummaryImpl_setParallelEnvironmentName");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setParallelEnvironmentName", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobSummaryImpl_setParallelEnvironmentName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setParallelEnvironmentRange(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JobSummaryImpl_setParallelEnvironmentRange");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setParallelEnvironmentRange", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobSummaryImpl_setParallelEnvironmentRange failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setProject(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JobSummaryImpl_setProject");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setProject", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobSummaryImpl_setProject failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setQueueAssigned(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setQueueAssigned");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setQueueAssigned", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setQueueAssigned failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setRrcontr(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setRrcontr");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setRrcontr", "(D)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setRrcontr failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setRunning(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setRunning");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setRunning", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setRunning failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setShare(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setShare");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setShare", "(D)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setShare failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setSlots");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setSlots", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setStickets(JNIEnv *env, jobject obj , jlong p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setStickets");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setStickets", "(J)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setStickets failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setTickets(JNIEnv *env, jobject obj , jlong p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setTickets");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setTickets", "(J)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setTickets failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setUrgency(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setUrgency");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setUrgency", "(D)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setUrgency failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setWtcontr(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setWtcontr");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setWtcontr", "(D)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setWtcontr failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobSummaryImpl_setZombie(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobSummaryImpl_setZombie");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/JobSummaryImpl", "setZombie", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobSummaryImpl_setZombie failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/JobSummaryImpl */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/TaskSummary */

jclass TaskSummary_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "TaskSummary_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/TaskSummary", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t TaskSummary_getCpuUsage(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "TaskSummary_getCpuUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummary", "getCpuUsage", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "TaskSummary_getCpuUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t TaskSummary_getExitStatus(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "TaskSummary_getExitStatus");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummary", "getExitStatus", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "TaskSummary_getExitStatus failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t TaskSummary_getIoUsage(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "TaskSummary_getIoUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummary", "getIoUsage", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "TaskSummary_getIoUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t TaskSummary_getMemUsage(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "TaskSummary_getMemUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummary", "getMemUsage", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "TaskSummary_getMemUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t TaskSummary_getState(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "TaskSummary_getState");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummary", "getState", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "TaskSummary_getState failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t TaskSummary_getTaskId(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "TaskSummary_getTaskId");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummary", "getTaskId", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "TaskSummary_getTaskId failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t TaskSummary_hasCpuUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "TaskSummary_hasCpuUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummary", "hasCpuUsage", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "TaskSummary_hasCpuUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t TaskSummary_hasExitStatus(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "TaskSummary_hasExitStatus");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummary", "hasExitStatus", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "TaskSummary_hasExitStatus failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t TaskSummary_hasIoUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "TaskSummary_hasIoUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummary", "hasIoUsage", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "TaskSummary_hasIoUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t TaskSummary_hasMemUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "TaskSummary_hasMemUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummary", "hasMemUsage", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "TaskSummary_hasMemUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t TaskSummary_isRunning(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "TaskSummary_isRunning");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummary", "isRunning", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "TaskSummary_isRunning failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/TaskSummary */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInfoImpl */

jclass QueueInfoImpl_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInfoImpl_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInfoImpl", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInfoImpl_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "QueueInfoImpl_init");
      
   clazz = QueueInfoImpl_find_class(env, alpp);
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

jgdi_result_t QueueInfoImpl_getQname(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInfoImpl_getQname");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInfoImpl", "getQname", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInfoImpl_getQname failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInfoImpl_getQtype(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInfoImpl_getQtype");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInfoImpl", "getQtype", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInfoImpl_getQtype failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInfoImpl_getReservedSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "QueueInfoImpl_getReservedSlots");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInfoImpl", "getReservedSlots", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInfoImpl_getReservedSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInfoImpl_getState(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInfoImpl_getState");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInfoImpl", "getState", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInfoImpl_getState failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInfoImpl_getTotalSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "QueueInfoImpl_getTotalSlots");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInfoImpl", "getTotalSlots", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInfoImpl_getTotalSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInfoImpl_getUsedSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "QueueInfoImpl_getUsedSlots");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInfoImpl", "getUsedSlots", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInfoImpl_getUsedSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInfoImpl_setQname(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "QueueInfoImpl_setQname");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInfoImpl", "setQname", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QueueInfoImpl_setQname failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInfoImpl_setQtype(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "QueueInfoImpl_setQtype");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInfoImpl", "setQtype", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QueueInfoImpl_setQtype failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInfoImpl_setReservedSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInfoImpl_setReservedSlots");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInfoImpl", "setReservedSlots", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInfoImpl_setReservedSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInfoImpl_setState(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "QueueInfoImpl_setState");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInfoImpl", "setState", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QueueInfoImpl_setState failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInfoImpl_setTotalSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInfoImpl_setTotalSlots");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInfoImpl", "setTotalSlots", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInfoImpl_setTotalSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInfoImpl_setUsedSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInfoImpl_setUsedSlots");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInfoImpl", "setUsedSlots", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInfoImpl_setUsedSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInfoImpl */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl */

jclass QueueInstanceSummaryImpl_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummaryImpl_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_init");
      
   clazz = QueueInstanceSummaryImpl_find_class(env, alpp);
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

jgdi_result_t QueueInstanceSummaryImpl_addExplainMessage(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_addExplainMessage");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "addExplainMessage", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_addExplainMessage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_addJob(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_addJob");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "addJob", "(Lcom/sun/grid/jgdi/monitoring/JobSummary;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_addJob failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_addJobs(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_addJobs");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "addJobs", "(Ljava/util/List;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_addJobs failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_addResource(JNIEnv *env, jobject obj , const char* p0 , const char* p1 , const char* p2 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jstring p1_obj = NULL;
   jstring p2_obj = NULL;
   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_addResource");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "addResource", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   if (p1 != NULL) {
      p1_obj = (*env)->NewStringUTF(env, p1 ); 
   }
   if (p2 != NULL) {
      p2_obj = (*env)->NewStringUTF(env, p2 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj, p1_obj, p2_obj);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_addResource failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_getArch(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_getArch");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "getArch", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_getArch failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_getExplainMessageList(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_getExplainMessageList");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "getExplainMessageList", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_getExplainMessageList failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_getFreeSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_getFreeSlots");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "getFreeSlots", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_getFreeSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_getJobList(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_getJobList");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "getJobList", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_getJobList failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_getLoadAlarmReason(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_getLoadAlarmReason");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "getLoadAlarmReason", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_getLoadAlarmReason failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_getLoadAvg(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_getLoadAvg");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "getLoadAvg", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_getLoadAvg failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_getLoadAvgStr(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_getLoadAvgStr");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "getLoadAvgStr", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_getLoadAvgStr failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_getName(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_getName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "getName", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_getName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_getQueueType(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_getQueueType");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "getQueueType", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_getQueueType failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_getReservedSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_getReservedSlots");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "getReservedSlots", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_getReservedSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_getResourceDominanceSet(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_getResourceDominanceSet");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "getResourceDominanceSet", "()Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_getResourceDominanceSet failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_getResourceNames(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_getResourceNames");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "getResourceNames", "(Ljava/lang/String;)Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_getResourceNames failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_getResourceValue(JNIEnv *env, jobject obj , const char* p0 , const char* p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jstring p1_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_getResourceValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "getResourceValue", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   if (p1 != NULL) {
      p1_obj = (*env)->NewStringUTF(env, p1 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj, p1_obj);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_getResourceValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_getState(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_getState");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "getState", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_getState failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_getSuspendAlarmReason(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_getSuspendAlarmReason");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "getSuspendAlarmReason", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_getSuspendAlarmReason failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_getTotalSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_getTotalSlots");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "getTotalSlots", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_getTotalSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_getUsedSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_getUsedSlots");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "getUsedSlots", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_getUsedSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_hasLoadValue(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_hasLoadValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "hasLoadValue", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_hasLoadValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_isHasLoadValueFromObject(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_isHasLoadValueFromObject");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "isHasLoadValueFromObject", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_isHasLoadValueFromObject failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_setArch(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_setArch");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "setArch", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_setArch failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_setHasLoadValue(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_setHasLoadValue");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "setHasLoadValue", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_setHasLoadValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_setHasLoadValueFromObject(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_setHasLoadValueFromObject");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "setHasLoadValueFromObject", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_setHasLoadValueFromObject failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_setLoadAlarmReason(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_setLoadAlarmReason");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "setLoadAlarmReason", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_setLoadAlarmReason failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_setLoadAvg(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_setLoadAvg");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "setLoadAvg", "(D)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_setLoadAvg failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_setLoadAvgStr(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_setLoadAvgStr");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "setLoadAvgStr", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_setLoadAvgStr failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_setName(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_setName");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "setName", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_setName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_setQueueType(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_setQueueType");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "setQueueType", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_setQueueType failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_setReservedSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_setReservedSlots");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "setReservedSlots", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_setReservedSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_setState(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_setState");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "setState", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_setState failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_setSuspendAlarmReason(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_setSuspendAlarmReason");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "setSuspendAlarmReason", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_setSuspendAlarmReason failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_setTotalSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_setTotalSlots");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "setTotalSlots", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_setTotalSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryImpl_setUsedSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryImpl_setUsedSlots");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl", "setUsedSlots", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryImpl_setUsedSlots failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryImpl */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$1 */

jclass QueueInstanceSummaryPrinter_1_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_1_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$1", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummaryPrinter_1_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_1_getValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$1", "getValue", "(Ljava/lang/Object;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_1_getValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$1 */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$2 */

jclass QueueInstanceSummaryPrinter_2_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_2_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$2", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummaryPrinter_2_getValue(JNIEnv *env, jobject obj , jobject p0 , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_2_getValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$2", "getValue", "(Lcom/sun/grid/jgdi/monitoring/JobSummary;)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_2_getValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$2 */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$3 */

jclass QueueInstanceSummaryPrinter_3_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_3_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$3", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummaryPrinter_3_getValue(JNIEnv *env, jobject obj , jobject p0 , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_3_getValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$3", "getValue", "(Lcom/sun/grid/jgdi/monitoring/JobSummary;)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_3_getValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$3 */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$4 */

jclass QueueInstanceSummaryPrinter_4_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_4_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$4", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummaryPrinter_4_getValue(JNIEnv *env, jobject obj , jobject p0 , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_4_getValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$4", "getValue", "(Lcom/sun/grid/jgdi/monitoring/JobSummary;)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_4_getValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$4 */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$5 */

jclass QueueInstanceSummaryPrinter_5_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_5_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$5", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummaryPrinter_5_getValue(JNIEnv *env, jobject obj , jobject p0 , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_5_getValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$5", "getValue", "(Lcom/sun/grid/jgdi/monitoring/JobSummary;)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_5_getValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$5 */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$6 */

jclass QueueInstanceSummaryPrinter_6_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_6_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$6", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummaryPrinter_6_getValue(JNIEnv *env, jobject obj , jobject p0 , jlong* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jlong temp = 0;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_6_getValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$6", "getValue", "(Lcom/sun/grid/jgdi/monitoring/JobSummary;)J", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallLongMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_6_getValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$6 */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$7 */

jclass QueueInstanceSummaryPrinter_7_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_7_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$7", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummaryPrinter_7_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_7_getValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$7", "getValue", "(Ljava/lang/Object;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_7_getValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$7 */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$StatCalc */

jclass QueueInstanceSummaryPrinter_StatCalc_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_StatCalc_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$StatCalc", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummaryPrinter_StatCalc_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_StatCalc_getValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$StatCalc", "getValue", "(Ljava/lang/Object;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_StatCalc_getValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$StatCalc */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$CpuUsageCalc */

jclass QueueInstanceSummaryPrinter_CpuUsageCalc_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_CpuUsageCalc_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$CpuUsageCalc", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummaryPrinter_CpuUsageCalc_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_CpuUsageCalc_getValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$CpuUsageCalc", "getValue", "(Ljava/lang/Object;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_CpuUsageCalc_getValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$CpuUsageCalc */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$UsageCalc */

jclass QueueInstanceSummaryPrinter_UsageCalc_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_UsageCalc_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$UsageCalc", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummaryPrinter_UsageCalc_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_UsageCalc_getValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$UsageCalc", "getValue", "(Ljava/lang/Object;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_UsageCalc_getValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$UsageCalc */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$MemUsageCalc */

jclass QueueInstanceSummaryPrinter_MemUsageCalc_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_MemUsageCalc_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$MemUsageCalc", alpp);
   }
   DRETURN(clazz);
}



/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$MemUsageCalc */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$IOUsageCalc */

jclass QueueInstanceSummaryPrinter_IOUsageCalc_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_IOUsageCalc_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$IOUsageCalc", alpp);
   }
   DRETURN(clazz);
}



/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$IOUsageCalc */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$TicketCalc */

jclass QueueInstanceSummaryPrinter_TicketCalc_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_TicketCalc_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$TicketCalc", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummaryPrinter_TicketCalc_init(JNIEnv *env, jobject *obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_TicketCalc_init");
      
   clazz = QueueInstanceSummaryPrinter_TicketCalc_find_class(env, alpp);
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

jgdi_result_t QueueInstanceSummaryPrinter_TicketCalc_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_TicketCalc_getValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$TicketCalc", "getValue", "(Ljava/lang/Object;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_TicketCalc_getValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$TicketCalc */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$ShareCalc */

jclass QueueInstanceSummaryPrinter_ShareCalc_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_ShareCalc_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$ShareCalc", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummaryPrinter_ShareCalc_init(JNIEnv *env, jobject *obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_ShareCalc_init");
      
   clazz = QueueInstanceSummaryPrinter_ShareCalc_find_class(env, alpp);
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

jgdi_result_t QueueInstanceSummaryPrinter_ShareCalc_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_ShareCalc_getValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$ShareCalc", "getValue", "(Ljava/lang/Object;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_ShareCalc_getValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$ShareCalc */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$JaTaskIdCalc */

jclass QueueInstanceSummaryPrinter_JaTaskIdCalc_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_JaTaskIdCalc_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$JaTaskIdCalc", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummaryPrinter_JaTaskIdCalc_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_JaTaskIdCalc_getValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$JaTaskIdCalc", "getValue", "(Ljava/lang/Object;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_JaTaskIdCalc_getValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$JaTaskIdCalc */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$8 */

jclass QueueInstanceSummaryPrinter_8_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_8_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$8", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummaryPrinter_8_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_8_getValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$8", "getValue", "(Ljava/lang/Object;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_8_getValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$8 */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$9 */

jclass QueueInstanceSummaryPrinter_9_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_9_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$9", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummaryPrinter_9_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_9_getValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$9", "getValue", "(Ljava/lang/Object;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_9_getValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$9 */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$10 */

jclass QueueInstanceSummaryPrinter_10_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_10_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$10", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummaryPrinter_10_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_10_getValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$10", "getValue", "(Ljava/lang/Object;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_10_getValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$10 */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$11 */

jclass QueueInstanceSummaryPrinter_11_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_11_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$11", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummaryPrinter_11_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_11_getValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$11", "getValue", "(Ljava/lang/Object;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_11_getValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$11 */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$12 */

jclass QueueInstanceSummaryPrinter_12_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_12_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$12", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummaryPrinter_12_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_12_getValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$12", "getValue", "(Ljava/lang/Object;)Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_12_getValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter$12 */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter */

jclass QueueInstanceSummaryPrinter_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueInstanceSummaryPrinter_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_init");
      
   clazz = QueueInstanceSummaryPrinter_find_class(env, alpp);
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

jgdi_result_t QueueInstanceSummaryPrinter_static_createJobSummaryTable(JNIEnv *env , jobject p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_static_createJobSummaryTable");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter", "createJobSummaryTable", "(Lcom/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions;)Lcom/sun/grid/jgdi/util/OutputTable;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_createJobSummaryTable failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryPrinter_static_print(JNIEnv *env , jobject p0 , jobject p1 , jobject p2 , lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_static_print");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter", "print", "(Ljava/io/PrintWriter;Lcom/sun/grid/jgdi/monitoring/QHostResult;Lcom/sun/grid/jgdi/monitoring/QHostOptions;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }
   
 (*env)->CallStaticVoidMethod(env, clazz, mid , p0 , p1 , p2 );  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_print failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   } 
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryPrinter_static_print_0(JNIEnv *env , jobject p0 , jobject p1 , jobject p2 , lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_static_print_0");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter", "print", "(Ljava/io/PrintWriter;Lcom/sun/grid/jgdi/monitoring/QueueInstanceSummaryResult;Lcom/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }
   
 (*env)->CallStaticVoidMethod(env, clazz, mid , p0 , p1 , p2 );  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_print_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   } 
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryPrinter_createJobSummaryTable(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_createJobSummaryTable");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter", "createJobSummaryTable", "(Lcom/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions;)Lcom/sun/grid/jgdi/util/OutputTable;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_createJobSummaryTable failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryPrinter_print(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jobject p2 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_print");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter", "print", "(Ljava/io/PrintWriter;Lcom/sun/grid/jgdi/monitoring/QHostResult;Lcom/sun/grid/jgdi/monitoring/QHostOptions;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1, p2);  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_print failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueInstanceSummaryPrinter_print_0(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jobject p2 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueInstanceSummaryPrinter_print_0");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter", "print", "(Ljava/io/PrintWriter;Lcom/sun/grid/jgdi/monitoring/QueueInstanceSummaryResult;Lcom/sun/grid/jgdi/monitoring/QueueInstanceSummaryOptions;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1, p2);  
   if (test_jni_error(env, "QueueInstanceSummaryPrinter_print_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/QueueInstanceSummaryPrinter */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/ResourceQuotaImpl */

jclass ResourceQuotaImpl_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaImpl_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/ResourceQuotaImpl", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t ResourceQuotaImpl_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "ResourceQuotaImpl_init");
      
   clazz = ResourceQuotaImpl_find_class(env, alpp);
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

jgdi_result_t ResourceQuotaImpl_init_0(JNIEnv *env, jobject *obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;     jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "ResourceQuotaImpl_init_0");
      
   clazz = ResourceQuotaImpl_find_class(env, alpp);
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

jgdi_result_t ResourceQuotaImpl_init_1(JNIEnv *env, jobject *obj , const char* p0 , const char* p1 , const char* p2 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;     jstring p0_obj = NULL;     jstring p1_obj = NULL;     jstring p2_obj = NULL;
   DENTER(BASIS_LAYER, "ResourceQuotaImpl_init_1");
      
   clazz = ResourceQuotaImpl_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(Ljava/lang/String;, Ljava/lang/String;, Ljava/lang/String;)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   } 
   if ( p0 != NULL ) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   if ( p1 != NULL ) {
      p1_obj = (*env)->NewStringUTF(env, p1 ); 
   }
   if ( p2 != NULL ) {
      p2_obj = (*env)->NewStringUTF(env, p2 ); 
   }   
   *obj = (*env)->NewObject(env, clazz, mid , p0_obj , p1_obj , p2_obj );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t ResourceQuotaImpl_getLimitValue(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaImpl_getLimitValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaImpl", "getLimitValue", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaImpl_getLimitValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaImpl_getName(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaImpl_getName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaImpl", "getName", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaImpl_getName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaImpl_getUsageValue(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaImpl_getUsageValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaImpl", "getUsageValue", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaImpl_getUsageValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaImpl_setLimitValue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "ResourceQuotaImpl_setLimitValue");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaImpl", "setLimitValue", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ResourceQuotaImpl_setLimitValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ResourceQuotaImpl_setName(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "ResourceQuotaImpl_setName");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaImpl", "setName", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ResourceQuotaImpl_setName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ResourceQuotaImpl_setUsageValue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "ResourceQuotaImpl_setUsageValue");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaImpl", "setUsageValue", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ResourceQuotaImpl_setUsageValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/ResourceQuotaImpl */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl */

jclass ResourceQuotaRuleInfoImpl_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t ResourceQuotaRuleInfoImpl_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_init");
      
   clazz = ResourceQuotaRuleInfoImpl_find_class(env, alpp);
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

jgdi_result_t ResourceQuotaRuleInfoImpl_init_0(JNIEnv *env, jobject *obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;     jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_init_0");
      
   clazz = ResourceQuotaRuleInfoImpl_find_class(env, alpp);
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

jgdi_result_t ResourceQuotaRuleInfoImpl_addHost(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_addHost");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "addHost", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_addHost failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfoImpl_addLimit(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_addLimit");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "addLimit", "(Lcom/sun/grid/jgdi/monitoring/ResourceQuota;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_addLimit failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfoImpl_addPe(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_addPe");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "addPe", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_addPe failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfoImpl_addProject(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_addProject");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "addProject", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_addProject failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfoImpl_addQueue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_addQueue");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "addQueue", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_addQueue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfoImpl_addUser(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_addUser");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "addUser", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_addUser failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfoImpl_addXHost(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_addXHost");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "addXHost", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_addXHost failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfoImpl_addXPe(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_addXPe");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "addXPe", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_addXPe failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfoImpl_addXProject(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_addXProject");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "addXProject", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_addXProject failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfoImpl_addXQueue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_addXQueue");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "addXQueue", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_addXQueue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfoImpl_addXUser(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_addXUser");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "addXUser", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_addXUser failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfoImpl_getHosts(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_getHosts");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "getHosts", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_getHosts failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfoImpl_getLimits(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_getLimits");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "getLimits", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_getLimits failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfoImpl_getPes(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_getPes");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "getPes", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_getPes failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfoImpl_getProjects(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_getProjects");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "getProjects", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_getProjects failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfoImpl_getQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_getQueues");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "getQueues", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_getQueues failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfoImpl_getResouceQuotaRuleName(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_getResouceQuotaRuleName");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "getResouceQuotaRuleName", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_getResouceQuotaRuleName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfoImpl_getUsers(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_getUsers");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "getUsers", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_getUsers failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfoImpl_getXHosts(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_getXHosts");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "getXHosts", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_getXHosts failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfoImpl_getXPes(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_getXPes");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "getXPes", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_getXPes failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfoImpl_getXProjects(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_getXProjects");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "getXProjects", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_getXProjects failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfoImpl_getXQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_getXQueues");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "getXQueues", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_getXQueues failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfoImpl_getXUsers(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_getXUsers");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "getXUsers", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_getXUsers failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceQuotaRuleInfoImpl_setresourceQuotaRuleName(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "ResourceQuotaRuleInfoImpl_setresourceQuotaRuleName");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl", "setresourceQuotaRuleName", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ResourceQuotaRuleInfoImpl_setresourceQuotaRuleName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/ResourceQuotaRuleInfoImpl */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/TaskSummaryImpl */

jclass TaskSummaryImpl_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "TaskSummaryImpl_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/TaskSummaryImpl", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t TaskSummaryImpl_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "TaskSummaryImpl_init");
      
   clazz = TaskSummaryImpl_find_class(env, alpp);
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

jgdi_result_t TaskSummaryImpl_getCpuUsage(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "TaskSummaryImpl_getCpuUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummaryImpl", "getCpuUsage", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "TaskSummaryImpl_getCpuUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t TaskSummaryImpl_getExitStatus(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "TaskSummaryImpl_getExitStatus");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummaryImpl", "getExitStatus", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "TaskSummaryImpl_getExitStatus failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t TaskSummaryImpl_getIoUsage(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "TaskSummaryImpl_getIoUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummaryImpl", "getIoUsage", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "TaskSummaryImpl_getIoUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t TaskSummaryImpl_getMemUsage(JNIEnv *env, jobject obj , jdouble* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jdouble temp = 0.0;

   DENTER(BASIS_LAYER, "TaskSummaryImpl_getMemUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0.0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummaryImpl", "getMemUsage", "()D", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallDoubleMethod(env, obj, mid);  
   if (test_jni_error(env, "TaskSummaryImpl_getMemUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0.0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t TaskSummaryImpl_getState(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "TaskSummaryImpl_getState");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummaryImpl", "getState", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "TaskSummaryImpl_getState failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t TaskSummaryImpl_getTaskId(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "TaskSummaryImpl_getTaskId");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummaryImpl", "getTaskId", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "TaskSummaryImpl_getTaskId failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t TaskSummaryImpl_hasCpuUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "TaskSummaryImpl_hasCpuUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummaryImpl", "hasCpuUsage", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "TaskSummaryImpl_hasCpuUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t TaskSummaryImpl_hasExitStatus(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "TaskSummaryImpl_hasExitStatus");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummaryImpl", "hasExitStatus", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "TaskSummaryImpl_hasExitStatus failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t TaskSummaryImpl_hasIoUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "TaskSummaryImpl_hasIoUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummaryImpl", "hasIoUsage", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "TaskSummaryImpl_hasIoUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t TaskSummaryImpl_hasMemUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "TaskSummaryImpl_hasMemUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummaryImpl", "hasMemUsage", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "TaskSummaryImpl_hasMemUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t TaskSummaryImpl_isRunning(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "TaskSummaryImpl_isRunning");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummaryImpl", "isRunning", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "TaskSummaryImpl_isRunning failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t TaskSummaryImpl_setCpuUsage(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "TaskSummaryImpl_setCpuUsage");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummaryImpl", "setCpuUsage", "(D)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "TaskSummaryImpl_setCpuUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t TaskSummaryImpl_setExitStatus(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "TaskSummaryImpl_setExitStatus");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummaryImpl", "setExitStatus", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "TaskSummaryImpl_setExitStatus failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t TaskSummaryImpl_setIoUsage(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "TaskSummaryImpl_setIoUsage");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummaryImpl", "setIoUsage", "(D)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "TaskSummaryImpl_setIoUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t TaskSummaryImpl_setMemUsage(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "TaskSummaryImpl_setMemUsage");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummaryImpl", "setMemUsage", "(D)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "TaskSummaryImpl_setMemUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t TaskSummaryImpl_setRunning(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "TaskSummaryImpl_setRunning");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummaryImpl", "setRunning", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "TaskSummaryImpl_setRunning failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t TaskSummaryImpl_setState(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "TaskSummaryImpl_setState");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummaryImpl", "setState", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "TaskSummaryImpl_setState failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t TaskSummaryImpl_setTaskId(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "TaskSummaryImpl_setTaskId");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/TaskSummaryImpl", "setTaskId", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "TaskSummaryImpl_setTaskId failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/TaskSummaryImpl */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/ClusterQueueSummaryOptions */

jclass ClusterQueueSummaryOptions_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "ClusterQueueSummaryOptions_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/ClusterQueueSummaryOptions", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t ClusterQueueSummaryOptions_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "ClusterQueueSummaryOptions_init");
      
   clazz = ClusterQueueSummaryOptions_find_class(env, alpp);
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


/*==== END  com/sun/grid/jgdi/monitoring/ClusterQueueSummaryOptions */



/*==== BEGIN  com/sun/grid/jgdi/filter/DefaultSimpleFilter */

jclass DefaultSimpleFilter_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "DefaultSimpleFilter_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/filter/DefaultSimpleFilter", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t DefaultSimpleFilter_init(JNIEnv *env, jobject *obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "DefaultSimpleFilter_init");
      
   clazz = DefaultSimpleFilter_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(Lcom/sun/grid/jgdi/filter/WhereClause;)V", alpp);
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

jgdi_result_t DefaultSimpleFilter_init_0(JNIEnv *env, jobject *obj , const char* p0 , jobject p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;     jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "DefaultSimpleFilter_init_0");
      
   clazz = DefaultSimpleFilter_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(Ljava/lang/String;, Lcom/sun/grid/jgdi/filter/WhereClause;)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   } 
   if ( p0 != NULL ) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }   
   *obj = (*env)->NewObject(env, clazz, mid , p0_obj , p1 );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t DefaultSimpleFilter_getWhat(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "DefaultSimpleFilter_getWhat");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/filter/DefaultSimpleFilter", "getWhat", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "DefaultSimpleFilter_getWhat failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t DefaultSimpleFilter_getWhere(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "DefaultSimpleFilter_getWhere");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/filter/DefaultSimpleFilter", "getWhere", "()Lcom/sun/grid/jgdi/filter/WhereClause;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "DefaultSimpleFilter_getWhere failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/filter/DefaultSimpleFilter */



/*==== BEGIN  com/sun/grid/jgdi/filter/SimpleFilter */

jclass SimpleFilter_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "SimpleFilter_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/filter/SimpleFilter", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t SimpleFilter_getWhat(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "SimpleFilter_getWhat");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/filter/SimpleFilter", "getWhat", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "SimpleFilter_getWhat failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t SimpleFilter_getWhere(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "SimpleFilter_getWhere");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/filter/SimpleFilter", "getWhere", "()Lcom/sun/grid/jgdi/filter/WhereClause;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "SimpleFilter_getWhere failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/filter/SimpleFilter */



/*==== BEGIN  com/sun/grid/jgdi/filter/JGDIFilter */

jclass JGDIFilter_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "JGDIFilter_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/filter/JGDIFilter", alpp);
   }
   DRETURN(clazz);
}



/*==== END  com/sun/grid/jgdi/filter/JGDIFilter */



/*==== BEGIN  com/sun/grid/jgdi/filter/WhereClause */

jclass WhereClause_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "WhereClause_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/filter/WhereClause", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t WhereClause_init(JNIEnv *env, jobject *obj , const char* p0 , jint p1 , const char* p2 , jobject p3 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;     jstring p0_obj = NULL;     jstring p2_obj = NULL;
   DENTER(BASIS_LAYER, "WhereClause_init");
      
   clazz = WhereClause_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(Ljava/lang/String;, I, Ljava/lang/String;, Ljava/lang/Object;)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   } 
   if ( p0 != NULL ) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   if ( p2 != NULL ) {
      p2_obj = (*env)->NewStringUTF(env, p2 ); 
   }   
   *obj = (*env)->NewObject(env, clazz, mid , p0_obj , p1 , p2_obj , p3 );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t WhereClause_getField(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "WhereClause_getField");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/filter/WhereClause", "getField", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "WhereClause_getField failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t WhereClause_getPattern(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "WhereClause_getPattern");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/filter/WhereClause", "getPattern", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "WhereClause_getPattern failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t WhereClause_getType(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "WhereClause_getType");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/filter/WhereClause", "getType", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "WhereClause_getType failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/filter/WhereClause */



/*==== BEGIN  com/sun/grid/jgdi/filter/IntWhereClause */

jclass IntWhereClause_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "IntWhereClause_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/filter/IntWhereClause", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t IntWhereClause_init(JNIEnv *env, jobject *obj , const char* p0 , jint p1 , const char* p2 , jint p3 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;     jstring p0_obj = NULL;     jstring p2_obj = NULL;
   DENTER(BASIS_LAYER, "IntWhereClause_init");
      
   clazz = IntWhereClause_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(Ljava/lang/String;, I, Ljava/lang/String;, I)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   } 
   if ( p0 != NULL ) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   if ( p2 != NULL ) {
      p2_obj = (*env)->NewStringUTF(env, p2 ); 
   }   
   *obj = (*env)->NewObject(env, clazz, mid , p0_obj , p1 , p2_obj , p3 );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t IntWhereClause_getValue(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "IntWhereClause_getValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/filter/IntWhereClause", "getValue", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "IntWhereClause_getValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/filter/IntWhereClause */



/*==== BEGIN  com/sun/grid/jgdi/filter/NativeFilter */

jclass NativeFilter_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "NativeFilter_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/filter/NativeFilter", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t NativeFilter_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "NativeFilter_init");
      
   clazz = NativeFilter_find_class(env, alpp);
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

jgdi_result_t NativeFilter_getNativeFilterFunction(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "NativeFilter_getNativeFilterFunction");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/filter/NativeFilter", "getNativeFilterFunction", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "NativeFilter_getNativeFilterFunction failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/filter/NativeFilter */



/*==== BEGIN  com/sun/grid/jgdi/filter/PrimaryKeyFilter */

jclass PrimaryKeyFilter_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "PrimaryKeyFilter_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/filter/PrimaryKeyFilter", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t PrimaryKeyFilter_init(JNIEnv *env, jobject *obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;     jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "PrimaryKeyFilter_init");
      
   clazz = PrimaryKeyFilter_find_class(env, alpp);
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

jgdi_result_t PrimaryKeyFilter_exclude(JNIEnv *env, jobject obj , jint p0 , jint p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "PrimaryKeyFilter_exclude");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/filter/PrimaryKeyFilter", "exclude", "(II)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "PrimaryKeyFilter_exclude failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t PrimaryKeyFilter_exclude_0(JNIEnv *env, jobject obj , jint p0 , const char* p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p1_obj = NULL;
   DENTER(BASIS_LAYER, "PrimaryKeyFilter_exclude_0");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/filter/PrimaryKeyFilter", "exclude", "(ILjava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p1 != NULL) {
      p1_obj = (*env)->NewStringUTF(env, p1 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0, p1_obj);  
   if (test_jni_error(env, "PrimaryKeyFilter_exclude_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t PrimaryKeyFilter_getFields(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "PrimaryKeyFilter_getFields");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/filter/PrimaryKeyFilter", "getFields", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "PrimaryKeyFilter_getFields failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t PrimaryKeyFilter_getType(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "PrimaryKeyFilter_getType");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/filter/PrimaryKeyFilter", "getType", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "PrimaryKeyFilter_getType failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t PrimaryKeyFilter_include(JNIEnv *env, jobject obj , jint p0 , jint p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "PrimaryKeyFilter_include");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/filter/PrimaryKeyFilter", "include", "(II)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "PrimaryKeyFilter_include failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t PrimaryKeyFilter_include_0(JNIEnv *env, jobject obj , jint p0 , const char* p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p1_obj = NULL;
   DENTER(BASIS_LAYER, "PrimaryKeyFilter_include_0");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/filter/PrimaryKeyFilter", "include", "(ILjava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p1 != NULL) {
      p1_obj = (*env)->NewStringUTF(env, p1 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0, p1_obj);  
   if (test_jni_error(env, "PrimaryKeyFilter_include_0 failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/filter/PrimaryKeyFilter */



/*==== BEGIN  com/sun/grid/jgdi/filter/StringWhereClause */

jclass StringWhereClause_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "StringWhereClause_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/filter/StringWhereClause", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t StringWhereClause_init(JNIEnv *env, jobject *obj , const char* p0 , jint p1 , const char* p2 , const char* p3 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;     jstring p0_obj = NULL;     jstring p2_obj = NULL;     jstring p3_obj = NULL;
   DENTER(BASIS_LAYER, "StringWhereClause_init");
      
   clazz = StringWhereClause_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(Ljava/lang/String;, I, Ljava/lang/String;, Ljava/lang/String;)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   } 
   if ( p0 != NULL ) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   if ( p2 != NULL ) {
      p2_obj = (*env)->NewStringUTF(env, p2 ); 
   }
   if ( p3 != NULL ) {
      p3_obj = (*env)->NewStringUTF(env, p3 ); 
   }   
   *obj = (*env)->NewObject(env, clazz, mid , p0_obj , p1 , p2_obj , p3_obj );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t StringWhereClause_getValue(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "StringWhereClause_getValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/filter/StringWhereClause", "getValue", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "StringWhereClause_getValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/filter/StringWhereClause */



/*==== BEGIN  com/sun/grid/jgdi/security/SecurityHelper */

jclass SecurityHelper_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "SecurityHelper_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/security/SecurityHelper", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t SecurityHelper_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "SecurityHelper_init");
      
   clazz = SecurityHelper_find_class(env, alpp);
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

jgdi_result_t SecurityHelper_static_getCertificate(JNIEnv *env , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "SecurityHelper_static_getCertificate");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/security/SecurityHelper", "getCertificate", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid );  
   if (test_jni_error(env, "SecurityHelper_getCertificate failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t SecurityHelper_static_getPrivateKey(JNIEnv *env , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "SecurityHelper_static_getPrivateKey");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/security/SecurityHelper", "getPrivateKey", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid );  
   if (test_jni_error(env, "SecurityHelper_getPrivateKey failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t SecurityHelper_static_getUsername(JNIEnv *env , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "SecurityHelper_static_getUsername");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/security/SecurityHelper", "getUsername", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid );  
   if (test_jni_error(env, "SecurityHelper_getUsername failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t SecurityHelper_getCertificate(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "SecurityHelper_getCertificate");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/security/SecurityHelper", "getCertificate", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "SecurityHelper_getCertificate failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t SecurityHelper_getPrivateKey(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "SecurityHelper_getPrivateKey");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/security/SecurityHelper", "getPrivateKey", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "SecurityHelper_getPrivateKey failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t SecurityHelper_getUsername(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "SecurityHelper_getUsername");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/security/SecurityHelper", "getUsername", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "SecurityHelper_getUsername failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/security/SecurityHelper */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/filter/ResourceAttributeFilter */

jclass ResourceAttributeFilter_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "ResourceAttributeFilter_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/filter/ResourceAttributeFilter", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t ResourceAttributeFilter_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "ResourceAttributeFilter_init");
      
   clazz = ResourceAttributeFilter_find_class(env, alpp);
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

jgdi_result_t ResourceAttributeFilter_static_parse(JNIEnv *env , const char* p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceAttributeFilter_static_parse");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/monitoring/filter/ResourceAttributeFilter", "parse", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/ResourceAttributeFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "ResourceAttributeFilter_parse failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t ResourceAttributeFilter_addValueName(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "ResourceAttributeFilter_addValueName");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/ResourceAttributeFilter", "addValueName", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ResourceAttributeFilter_addValueName failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ResourceAttributeFilter_fill(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceAttributeFilter_fill");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/ResourceAttributeFilter", "fill", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/ResourceAttributeFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ResourceAttributeFilter_fill failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceAttributeFilter_getValueNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceAttributeFilter_getValueNames");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/ResourceAttributeFilter", "getValueNames", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceAttributeFilter_getValueNames failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceAttributeFilter_parse(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceAttributeFilter_parse");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/ResourceAttributeFilter", "parse", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/ResourceAttributeFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ResourceAttributeFilter_parse failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceAttributeFilter_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceAttributeFilter_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/ResourceAttributeFilter", "toString", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceAttributeFilter_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/filter/ResourceAttributeFilter */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/filter/HostFilter */

jclass HostFilter_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "HostFilter_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/filter/HostFilter", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t HostFilter_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "HostFilter_init");
      
   clazz = HostFilter_find_class(env, alpp);
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

jgdi_result_t HostFilter_static_parse(JNIEnv *env , const char* p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostFilter_static_parse");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/monitoring/filter/HostFilter", "parse", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/HostFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "HostFilter_parse failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t HostFilter_addHost(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "HostFilter_addHost");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/HostFilter", "addHost", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "HostFilter_addHost failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t HostFilter_getHosts(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostFilter_getHosts");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/HostFilter", "getHosts", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostFilter_getHosts failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostFilter_parse(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostFilter_parse");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/HostFilter", "parse", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/HostFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "HostFilter_parse failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t HostFilter_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "HostFilter_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/HostFilter", "toString", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "HostFilter_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/filter/HostFilter */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/filter/UserFilter */

jclass UserFilter_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "UserFilter_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/filter/UserFilter", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t UserFilter_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "UserFilter_init");
      
   clazz = UserFilter_find_class(env, alpp);
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

jgdi_result_t UserFilter_static_parse(JNIEnv *env , const char* p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "UserFilter_static_parse");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/monitoring/filter/UserFilter", "parse", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/UserFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "UserFilter_parse failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t UserFilter_addUser(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "UserFilter_addUser");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/UserFilter", "addUser", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "UserFilter_addUser failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t UserFilter_fill(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "UserFilter_fill");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/UserFilter", "fill", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/UserFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "UserFilter_fill failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t UserFilter_getUsers(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "UserFilter_getUsers");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/UserFilter", "getUsers", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "UserFilter_getUsers failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t UserFilter_parse(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "UserFilter_parse");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/UserFilter", "parse", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/UserFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "UserFilter_parse failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t UserFilter_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "UserFilter_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/UserFilter", "toString", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "UserFilter_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/filter/UserFilter */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/filter/ResourceFilter */

jclass ResourceFilter_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "ResourceFilter_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/filter/ResourceFilter", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t ResourceFilter_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "ResourceFilter_init");
      
   clazz = ResourceFilter_find_class(env, alpp);
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

jgdi_result_t ResourceFilter_static_parse(JNIEnv *env , const char* p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceFilter_static_parse");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/monitoring/filter/ResourceFilter", "parse", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/ResourceFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "ResourceFilter_parse failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t ResourceFilter_addResource(JNIEnv *env, jobject obj , const char* p0 , const char* p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jstring p1_obj = NULL;
   DENTER(BASIS_LAYER, "ResourceFilter_addResource");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/ResourceFilter", "addResource", "(Ljava/lang/String;Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   if (p1 != NULL) {
      p1_obj = (*env)->NewStringUTF(env, p1 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj, p1_obj);  
   if (test_jni_error(env, "ResourceFilter_addResource failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ResourceFilter_fill(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceFilter_fill");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/ResourceFilter", "fill", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/ResourceFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ResourceFilter_fill failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceFilter_getResource(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceFilter_getResource");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/ResourceFilter", "getResource", "(Ljava/lang/String;)Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ResourceFilter_getResource failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceFilter_getResourceNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceFilter_getResourceNames");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/ResourceFilter", "getResourceNames", "()Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceFilter_getResourceNames failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceFilter_getResources(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceFilter_getResources");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/ResourceFilter", "getResources", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ResourceFilter_getResources failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ResourceFilter_parse(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ResourceFilter_parse");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/ResourceFilter", "parse", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/ResourceFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ResourceFilter_parse failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/filter/ResourceFilter */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/filter/QueueFilter */

jclass QueueFilter_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueFilter_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/filter/QueueFilter", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueFilter_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "QueueFilter_init");
      
   clazz = QueueFilter_find_class(env, alpp);
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

jgdi_result_t QueueFilter_static_parse(JNIEnv *env , const char* p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueFilter_static_parse");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueFilter", "parse", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/QueueFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "QueueFilter_parse failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t QueueFilter_addQueue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "QueueFilter_addQueue");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueFilter", "addQueue", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QueueFilter_addQueue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueFilter_fill(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueFilter_fill");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueFilter", "fill", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/QueueFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QueueFilter_fill failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueFilter_getQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueFilter_getQueues");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueFilter", "getQueues", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueFilter_getQueues failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueFilter_parse(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueFilter_parse");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueFilter", "parse", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/QueueFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QueueFilter_parse failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueFilter_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueFilter_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueFilter", "toString", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueFilter_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/filter/QueueFilter */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/filter/QueueStateFilter$StateToString */

jclass QueueStateFilter_StateToString_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueStateFilter_StateToString_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter$StateToString", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueStateFilter_StateToString_init(JNIEnv *env, jobject *obj , jint p0 , jchar p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "QueueStateFilter_StateToString_init");
      
   clazz = QueueStateFilter_StateToString_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(I, C)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }    
   *obj = (*env)->NewObject(env, clazz, mid , p0 , p1 );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}


/*==== END  com/sun/grid/jgdi/monitoring/filter/QueueStateFilter$StateToString */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/filter/QueueStateFilter */

jclass QueueStateFilter_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "QueueStateFilter_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t QueueStateFilter_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "QueueStateFilter_init");
      
   clazz = QueueStateFilter_find_class(env, alpp);
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

jgdi_result_t QueueStateFilter_init_0(JNIEnv *env, jobject *obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "QueueStateFilter_init_0");
      
   clazz = QueueStateFilter_find_class(env, alpp);
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

jgdi_result_t QueueStateFilter_static_ALARM(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "QueueStateFilter_static_ALARM");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = QueueStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class com/sun/grid/jgdi/monitoring/filter/QueueStateFilter not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "ALARM", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "QueueStateFilter_static_ALARM failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t QueueStateFilter_static_CALENDAR_DISABLED(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "QueueStateFilter_static_CALENDAR_DISABLED");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = QueueStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class com/sun/grid/jgdi/monitoring/filter/QueueStateFilter not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "CALENDAR_DISABLED", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "QueueStateFilter_static_CALENDAR_DISABLED failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t QueueStateFilter_static_CALENDAR_SUSPENDED(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "QueueStateFilter_static_CALENDAR_SUSPENDED");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = QueueStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class com/sun/grid/jgdi/monitoring/filter/QueueStateFilter not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "CALENDAR_SUSPENDED", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "QueueStateFilter_static_CALENDAR_SUSPENDED failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t QueueStateFilter_static_CONFIGURATION_AMBIGUOUS(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "QueueStateFilter_static_CONFIGURATION_AMBIGUOUS");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = QueueStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class com/sun/grid/jgdi/monitoring/filter/QueueStateFilter not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "CONFIGURATION_AMBIGUOUS", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "QueueStateFilter_static_CONFIGURATION_AMBIGUOUS failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t QueueStateFilter_static_DISABLED(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "QueueStateFilter_static_DISABLED");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = QueueStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class com/sun/grid/jgdi/monitoring/filter/QueueStateFilter not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "DISABLED", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "QueueStateFilter_static_DISABLED failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t QueueStateFilter_static_ERROR(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "QueueStateFilter_static_ERROR");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = QueueStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class com/sun/grid/jgdi/monitoring/filter/QueueStateFilter not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "ERROR", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "QueueStateFilter_static_ERROR failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t QueueStateFilter_static_ORPHANED(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "QueueStateFilter_static_ORPHANED");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = QueueStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class com/sun/grid/jgdi/monitoring/filter/QueueStateFilter not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "ORPHANED", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "QueueStateFilter_static_ORPHANED failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t QueueStateFilter_static_SUBORDINATE(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "QueueStateFilter_static_SUBORDINATE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = QueueStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class com/sun/grid/jgdi/monitoring/filter/QueueStateFilter not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "SUBORDINATE", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "QueueStateFilter_static_SUBORDINATE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t QueueStateFilter_static_SUSPENDED(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "QueueStateFilter_static_SUSPENDED");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = QueueStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class com/sun/grid/jgdi/monitoring/filter/QueueStateFilter not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "SUSPENDED", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "QueueStateFilter_static_SUSPENDED failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t QueueStateFilter_static_SUSPEND_ALARM(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "QueueStateFilter_static_SUSPEND_ALARM");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = QueueStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class com/sun/grid/jgdi/monitoring/filter/QueueStateFilter not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "SUSPEND_ALARM", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "QueueStateFilter_static_SUSPEND_ALARM failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t QueueStateFilter_static_UNKNOWN(JNIEnv *env, jint *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jint field = 0;*/
   
   DENTER(BASIS_LAYER, "QueueStateFilter_static_UNKNOWN");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = QueueStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class com/sun/grid/jgdi/monitoring/filter/QueueStateFilter not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "UNKNOWN", "I", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticIntField(env, clazz, mid);
   if (test_jni_error(env, "QueueStateFilter_static_UNKNOWN failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t QueueStateFilter_static_parse(JNIEnv *env , const char* p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueStateFilter_static_parse");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "parse", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/QueueStateFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "QueueStateFilter_parse failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t QueueStateFilter_fill(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueStateFilter_fill");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "fill", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/QueueStateFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QueueStateFilter_fill failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueStateFilter_getOptions(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueStateFilter_getOptions");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "getOptions", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueStateFilter_getOptions failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueStateFilter_hasAlarm(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueStateFilter_hasAlarm");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "hasAlarm", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueStateFilter_hasAlarm failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueStateFilter_hasCalendarDisabled(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueStateFilter_hasCalendarDisabled");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "hasCalendarDisabled", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueStateFilter_hasCalendarDisabled failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueStateFilter_hasCalendarSuspend(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueStateFilter_hasCalendarSuspend");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "hasCalendarSuspend", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueStateFilter_hasCalendarSuspend failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueStateFilter_hasConfigurationAmbiguous(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueStateFilter_hasConfigurationAmbiguous");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "hasConfigurationAmbiguous", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueStateFilter_hasConfigurationAmbiguous failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueStateFilter_hasDisabled(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueStateFilter_hasDisabled");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "hasDisabled", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueStateFilter_hasDisabled failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueStateFilter_hasError(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueStateFilter_hasError");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "hasError", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueStateFilter_hasError failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueStateFilter_hasOrphaned(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueStateFilter_hasOrphaned");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "hasOrphaned", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueStateFilter_hasOrphaned failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueStateFilter_hasSubordinate(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueStateFilter_hasSubordinate");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "hasSubordinate", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueStateFilter_hasSubordinate failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueStateFilter_hasSuspendAlarm(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueStateFilter_hasSuspendAlarm");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "hasSuspendAlarm", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueStateFilter_hasSuspendAlarm failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueStateFilter_hasSuspended(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueStateFilter_hasSuspended");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "hasSuspended", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueStateFilter_hasSuspended failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueStateFilter_hasUnknown(JNIEnv *env, jobject obj , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "QueueStateFilter_hasUnknown");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "hasUnknown", "()Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueStateFilter_hasUnknown failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueStateFilter_parse(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueStateFilter_parse");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "parse", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/QueueStateFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "QueueStateFilter_parse failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t QueueStateFilter_setAlarm(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueStateFilter_setAlarm");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "setAlarm", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueStateFilter_setAlarm failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueStateFilter_setCalendarDisabled(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueStateFilter_setCalendarDisabled");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "setCalendarDisabled", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueStateFilter_setCalendarDisabled failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueStateFilter_setCalendarSuspend(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueStateFilter_setCalendarSuspend");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "setCalendarSuspend", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueStateFilter_setCalendarSuspend failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueStateFilter_setConfigurationAmbiguous(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueStateFilter_setConfigurationAmbiguous");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "setConfigurationAmbiguous", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueStateFilter_setConfigurationAmbiguous failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueStateFilter_setDisabled(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueStateFilter_setDisabled");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "setDisabled", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueStateFilter_setDisabled failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueStateFilter_setError(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueStateFilter_setError");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "setError", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueStateFilter_setError failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueStateFilter_setOrphaned(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueStateFilter_setOrphaned");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "setOrphaned", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueStateFilter_setOrphaned failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueStateFilter_setSubordinate(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueStateFilter_setSubordinate");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "setSubordinate", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueStateFilter_setSubordinate failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueStateFilter_setSuspendAlarm(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueStateFilter_setSuspendAlarm");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "setSuspendAlarm", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueStateFilter_setSuspendAlarm failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueStateFilter_setSuspended(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueStateFilter_setSuspended");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "setSuspended", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueStateFilter_setSuspended failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueStateFilter_setUnknown(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "QueueStateFilter_setUnknown");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "setUnknown", "(Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "QueueStateFilter_setUnknown failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t QueueStateFilter_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "QueueStateFilter_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/QueueStateFilter", "toString", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "QueueStateFilter_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/filter/QueueStateFilter */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/filter/ParallelEnvironmentFilter */

jclass ParallelEnvironmentFilter_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "ParallelEnvironmentFilter_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/filter/ParallelEnvironmentFilter", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t ParallelEnvironmentFilter_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "ParallelEnvironmentFilter_init");
      
   clazz = ParallelEnvironmentFilter_find_class(env, alpp);
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

jgdi_result_t ParallelEnvironmentFilter_static_parse(JNIEnv *env , const char* p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ParallelEnvironmentFilter_static_parse");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/monitoring/filter/ParallelEnvironmentFilter", "parse", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/ParallelEnvironmentFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "ParallelEnvironmentFilter_parse failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t ParallelEnvironmentFilter_addPE(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "ParallelEnvironmentFilter_addPE");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/ParallelEnvironmentFilter", "addPE", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ParallelEnvironmentFilter_addPE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ParallelEnvironmentFilter_fill(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ParallelEnvironmentFilter_fill");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/ParallelEnvironmentFilter", "fill", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/ParallelEnvironmentFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ParallelEnvironmentFilter_fill failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ParallelEnvironmentFilter_getPECount(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ParallelEnvironmentFilter_getPECount");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/ParallelEnvironmentFilter", "getPECount", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "ParallelEnvironmentFilter_getPECount failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ParallelEnvironmentFilter_getPEList(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ParallelEnvironmentFilter_getPEList");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/ParallelEnvironmentFilter", "getPEList", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ParallelEnvironmentFilter_getPEList failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ParallelEnvironmentFilter_parse(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ParallelEnvironmentFilter_parse");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/ParallelEnvironmentFilter", "parse", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/ParallelEnvironmentFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ParallelEnvironmentFilter_parse failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ParallelEnvironmentFilter_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ParallelEnvironmentFilter_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/ParallelEnvironmentFilter", "toString", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ParallelEnvironmentFilter_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/filter/ParallelEnvironmentFilter */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/filter/JobStateFilter$State */

jclass JobStateFilter_State_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "JobStateFilter_State_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/filter/JobStateFilter$State", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t JobStateFilter_State_init(JNIEnv *env, jobject *obj , jint p0 , const char* p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;     jstring p1_obj = NULL;
   DENTER(BASIS_LAYER, "JobStateFilter_State_init");
      
   clazz = JobStateFilter_State_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(I, Ljava/lang/String;)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   } 
   if ( p1 != NULL ) {
      p1_obj = (*env)->NewStringUTF(env, p1 ); 
   }   
   *obj = (*env)->NewObject(env, clazz, mid , p0 , p1_obj );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t JobStateFilter_State_getMask(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "JobStateFilter_State_getMask");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/JobStateFilter$State", "getMask", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "JobStateFilter_State_getMask failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobStateFilter_State_getValue(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobStateFilter_State_getValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/JobStateFilter$State", "getValue", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobStateFilter_State_getValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobStateFilter_State_isSet(JNIEnv *env, jobject obj , jint p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JobStateFilter_State_isSet");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/JobStateFilter$State", "isSet", "(I)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobStateFilter_State_isSet failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/filter/JobStateFilter$State */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/filter/JobStateFilter */

jclass JobStateFilter_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "JobStateFilter_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/filter/JobStateFilter", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t JobStateFilter_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "JobStateFilter_init");
      
   clazz = JobStateFilter_find_class(env, alpp);
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

jgdi_result_t JobStateFilter_init_0(JNIEnv *env, jobject *obj , jobject* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "JobStateFilter_init_0");
      
   clazz = JobStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "([Lcom/sun/grid/jgdi/monitoring/filter/JobStateFilter$State;)V", alpp);
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

jgdi_result_t JobStateFilter_static_ALL(JNIEnv *env, jobject *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jobject field = NULL;*/
   
   DENTER(BASIS_LAYER, "JobStateFilter_static_ALL");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = JobStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class com/sun/grid/jgdi/monitoring/filter/JobStateFilter not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "ALL", "Lcom/sun/grid/jgdi/monitoring/filter/JobStateFilter$State;", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticObjectField(env, clazz, mid);
   if (test_jni_error(env, "JobStateFilter_static_ALL failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t JobStateFilter_static_HOLD(JNIEnv *env, jobject *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jobject field = NULL;*/
   
   DENTER(BASIS_LAYER, "JobStateFilter_static_HOLD");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = JobStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class com/sun/grid/jgdi/monitoring/filter/JobStateFilter not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "HOLD", "Lcom/sun/grid/jgdi/monitoring/filter/JobStateFilter$State;", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticObjectField(env, clazz, mid);
   if (test_jni_error(env, "JobStateFilter_static_HOLD failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t JobStateFilter_static_HOLD_JOBARRAYHOLD(JNIEnv *env, jobject *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jobject field = NULL;*/
   
   DENTER(BASIS_LAYER, "JobStateFilter_static_HOLD_JOBARRAYHOLD");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = JobStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class com/sun/grid/jgdi/monitoring/filter/JobStateFilter not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "HOLD_JOBARRAYHOLD", "Lcom/sun/grid/jgdi/monitoring/filter/JobStateFilter$State;", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticObjectField(env, clazz, mid);
   if (test_jni_error(env, "JobStateFilter_static_HOLD_JOBARRAYHOLD failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t JobStateFilter_static_HOLD_JOBHOLD(JNIEnv *env, jobject *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jobject field = NULL;*/
   
   DENTER(BASIS_LAYER, "JobStateFilter_static_HOLD_JOBHOLD");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = JobStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class com/sun/grid/jgdi/monitoring/filter/JobStateFilter not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "HOLD_JOBHOLD", "Lcom/sun/grid/jgdi/monitoring/filter/JobStateFilter$State;", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticObjectField(env, clazz, mid);
   if (test_jni_error(env, "JobStateFilter_static_HOLD_JOBHOLD failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t JobStateFilter_static_HOLD_OPERATOR(JNIEnv *env, jobject *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jobject field = NULL;*/
   
   DENTER(BASIS_LAYER, "JobStateFilter_static_HOLD_OPERATOR");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = JobStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class com/sun/grid/jgdi/monitoring/filter/JobStateFilter not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "HOLD_OPERATOR", "Lcom/sun/grid/jgdi/monitoring/filter/JobStateFilter$State;", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticObjectField(env, clazz, mid);
   if (test_jni_error(env, "JobStateFilter_static_HOLD_OPERATOR failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t JobStateFilter_static_HOLD_STARTTIMEHOLD(JNIEnv *env, jobject *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jobject field = NULL;*/
   
   DENTER(BASIS_LAYER, "JobStateFilter_static_HOLD_STARTTIMEHOLD");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = JobStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class com/sun/grid/jgdi/monitoring/filter/JobStateFilter not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "HOLD_STARTTIMEHOLD", "Lcom/sun/grid/jgdi/monitoring/filter/JobStateFilter$State;", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticObjectField(env, clazz, mid);
   if (test_jni_error(env, "JobStateFilter_static_HOLD_STARTTIMEHOLD failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t JobStateFilter_static_HOLD_SYSTEM(JNIEnv *env, jobject *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jobject field = NULL;*/
   
   DENTER(BASIS_LAYER, "JobStateFilter_static_HOLD_SYSTEM");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = JobStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class com/sun/grid/jgdi/monitoring/filter/JobStateFilter not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "HOLD_SYSTEM", "Lcom/sun/grid/jgdi/monitoring/filter/JobStateFilter$State;", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticObjectField(env, clazz, mid);
   if (test_jni_error(env, "JobStateFilter_static_HOLD_SYSTEM failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t JobStateFilter_static_HOLD_USER(JNIEnv *env, jobject *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jobject field = NULL;*/
   
   DENTER(BASIS_LAYER, "JobStateFilter_static_HOLD_USER");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = JobStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class com/sun/grid/jgdi/monitoring/filter/JobStateFilter not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "HOLD_USER", "Lcom/sun/grid/jgdi/monitoring/filter/JobStateFilter$State;", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticObjectField(env, clazz, mid);
   if (test_jni_error(env, "JobStateFilter_static_HOLD_USER failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t JobStateFilter_static_PENDING(JNIEnv *env, jobject *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jobject field = NULL;*/
   
   DENTER(BASIS_LAYER, "JobStateFilter_static_PENDING");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = JobStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class com/sun/grid/jgdi/monitoring/filter/JobStateFilter not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "PENDING", "Lcom/sun/grid/jgdi/monitoring/filter/JobStateFilter$State;", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticObjectField(env, clazz, mid);
   if (test_jni_error(env, "JobStateFilter_static_PENDING failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t JobStateFilter_static_RUNNING(JNIEnv *env, jobject *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jobject field = NULL;*/
   
   DENTER(BASIS_LAYER, "JobStateFilter_static_RUNNING");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = JobStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class com/sun/grid/jgdi/monitoring/filter/JobStateFilter not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "RUNNING", "Lcom/sun/grid/jgdi/monitoring/filter/JobStateFilter$State;", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticObjectField(env, clazz, mid);
   if (test_jni_error(env, "JobStateFilter_static_RUNNING failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t JobStateFilter_static_SUSPENDED(JNIEnv *env, jobject *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jobject field = NULL;*/
   
   DENTER(BASIS_LAYER, "JobStateFilter_static_SUSPENDED");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = JobStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class com/sun/grid/jgdi/monitoring/filter/JobStateFilter not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "SUSPENDED", "Lcom/sun/grid/jgdi/monitoring/filter/JobStateFilter$State;", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticObjectField(env, clazz, mid);
   if (test_jni_error(env, "JobStateFilter_static_SUSPENDED failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t JobStateFilter_static_ZOMBIE(JNIEnv *env, jobject *res, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   jclass clazz = NULL;
   static jfieldID mid = NULL;
   /*static jobject field = NULL;*/
   
   DENTER(BASIS_LAYER, "JobStateFilter_static_ZOMBIE");

   if (env == NULL) {
      answer_list_add(alpp, "env is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   clazz = JobStateFilter_find_class(env, alpp);
   if (clazz == NULL) {
      answer_list_add(alpp, "class com/sun/grid/jgdi/monitoring/filter/JobStateFilter not found", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      mid = get_static_fieldid(env, clazz, "ZOMBIE", "Lcom/sun/grid/jgdi/monitoring/filter/JobStateFilter$State;", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }   
   *res = (*env)->GetStaticObjectField(env, clazz, mid);
   if (test_jni_error(env, "JobStateFilter_static_ZOMBIE failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   }      
   DRETURN(ret);
}

jgdi_result_t JobStateFilter_static_fill(JNIEnv *env , const char* p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobStateFilter_static_fill");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/monitoring/filter/JobStateFilter", "fill", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/JobStateFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "JobStateFilter_fill failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t JobStateFilter_static_parse(JNIEnv *env , const char* p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobStateFilter_static_parse");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/monitoring/filter/JobStateFilter", "parse", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/JobStateFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "JobStateFilter_parse failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t JobStateFilter_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JobStateFilter_equals");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/JobStateFilter", "equals", "(Ljava/lang/Object;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobStateFilter_equals failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobStateFilter_exclude(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobStateFilter_exclude");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/JobStateFilter", "exclude", "(Lcom/sun/grid/jgdi/monitoring/filter/JobStateFilter$State;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobStateFilter_exclude failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobStateFilter_fill(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobStateFilter_fill");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/JobStateFilter", "fill", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/JobStateFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobStateFilter_fill failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobStateFilter_getStateString(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobStateFilter_getStateString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/JobStateFilter", "getStateString", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobStateFilter_getStateString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobStateFilter_getStates(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "JobStateFilter_getStates");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/JobStateFilter", "getStates", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "JobStateFilter_getStates failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobStateFilter_hashCode(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "JobStateFilter_hashCode");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/JobStateFilter", "hashCode", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "JobStateFilter_hashCode failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobStateFilter_isSet(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jboolean temp = FALSE;

   DENTER(BASIS_LAYER, "JobStateFilter_isSet");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = FALSE;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/JobStateFilter", "isSet", "(Lcom/sun/grid/jgdi/monitoring/filter/JobStateFilter$State;)Z", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallBooleanMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobStateFilter_isSet failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = FALSE;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobStateFilter_parse(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobStateFilter_parse");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/JobStateFilter", "parse", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/JobStateFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobStateFilter_parse failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobStateFilter_set(JNIEnv *env, jobject obj , jobject p0 , jboolean p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobStateFilter_set");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/JobStateFilter", "set", "(Lcom/sun/grid/jgdi/monitoring/filter/JobStateFilter$State;Z)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "JobStateFilter_set failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobStateFilter_setStates(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobStateFilter_setStates");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/JobStateFilter", "setStates", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobStateFilter_setStates failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobStateFilter_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobStateFilter_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/JobStateFilter", "toString", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobStateFilter_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/filter/JobStateFilter */



/*==== BEGIN  com/sun/grid/jgdi/monitoring/filter/ProjectFilter */

jclass ProjectFilter_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "ProjectFilter_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/monitoring/filter/ProjectFilter", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t ProjectFilter_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "ProjectFilter_init");
      
   clazz = ProjectFilter_find_class(env, alpp);
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

jgdi_result_t ProjectFilter_static_parse(JNIEnv *env , const char* p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ProjectFilter_static_parse");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/monitoring/filter/ProjectFilter", "parse", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/ProjectFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj );  
   if (test_jni_error(env, "ProjectFilter_parse failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t ProjectFilter_addProject(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "ProjectFilter_addProject");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/ProjectFilter", "addProject", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ProjectFilter_addProject failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ProjectFilter_getProjectCount(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "ProjectFilter_getProjectCount");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/ProjectFilter", "getProjectCount", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "ProjectFilter_getProjectCount failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ProjectFilter_getProjectList(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ProjectFilter_getProjectList");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/ProjectFilter", "getProjectList", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ProjectFilter_getProjectList failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ProjectFilter_parse(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ProjectFilter_parse");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/ProjectFilter", "parse", "(Ljava/lang/String;)Lcom/sun/grid/jgdi/monitoring/filter/ProjectFilter;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "ProjectFilter_parse failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ProjectFilter_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ProjectFilter_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/monitoring/filter/ProjectFilter", "toString", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ProjectFilter_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/monitoring/filter/ProjectFilter */
