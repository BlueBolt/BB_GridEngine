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



/*==== BEGIN  com/sun/grid/jgdi/event/ChangedObjectEvent */

jclass ChangedObjectEvent_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "ChangedObjectEvent_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/event/ChangedObjectEvent", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t ChangedObjectEvent_init(JNIEnv *env, jobject *obj , jobject p0 , jlong p1 , jint p2 , jobject p3 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "ChangedObjectEvent_init");
      
   clazz = ChangedObjectEvent_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(Lcom/sun/grid/jgdi/event/EventType;, J, I, Ljava/lang/Class;)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }    
   *obj = (*env)->NewObject(env, clazz, mid , p0 , p1 , p2 , p3 );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t ChangedObjectEvent_getChangedObject(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ChangedObjectEvent_getChangedObject");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/ChangedObjectEvent", "getChangedObject", "()Ljava/lang/Object;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ChangedObjectEvent_getChangedObject failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ChangedObjectEvent_getObjectType(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ChangedObjectEvent_getObjectType");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/ChangedObjectEvent", "getObjectType", "()Ljava/lang/Class;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ChangedObjectEvent_getObjectType failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t ChangedObjectEvent_setChangedObject(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ChangedObjectEvent_setChangedObject");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/ChangedObjectEvent", "setChangedObject", "(Ljava/lang/Object;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ChangedObjectEvent_setChangedObject failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ChangedObjectEvent_setPKInfo(JNIEnv *env, jobject obj , jint p0 , jint p1 , const char* p2 , const char* p3 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p2_obj = NULL;
   jstring p3_obj = NULL;
   DENTER(BASIS_LAYER, "ChangedObjectEvent_setPKInfo");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/ChangedObjectEvent", "setPKInfo", "(IILjava/lang/String;Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p2 != NULL) {
      p2_obj = (*env)->NewStringUTF(env, p2 ); 
   }
   if (p3 != NULL) {
      p3_obj = (*env)->NewStringUTF(env, p3 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0, p1, p2_obj, p3_obj);  
   if (test_jni_error(env, "ChangedObjectEvent_setPKInfo failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ChangedObjectEvent_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ChangedObjectEvent_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/ChangedObjectEvent", "toString", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ChangedObjectEvent_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/event/ChangedObjectEvent */



/*==== BEGIN  com/sun/grid/jgdi/event/EventFactoryBase */

jclass EventFactoryBase_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "EventFactoryBase_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/event/EventFactoryBase", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t EventFactoryBase_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "EventFactoryBase_init");
      
   clazz = EventFactoryBase_find_class(env, alpp);
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

jgdi_result_t EventFactoryBase_static_createJobFinalUsageEvent(JNIEnv *env , jlong p0 , jint p1 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactoryBase_static_createJobFinalUsageEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/event/EventFactoryBase", "createJobFinalUsageEvent", "(JI)Lcom/sun/grid/jgdi/event/JobFinalUsageEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 , p1 );  
   if (test_jni_error(env, "EventFactoryBase_createJobFinalUsageEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t EventFactoryBase_static_createJobFinishEvent(JNIEnv *env , jlong p0 , jint p1 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactoryBase_static_createJobFinishEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/event/EventFactoryBase", "createJobFinishEvent", "(JI)Lcom/sun/grid/jgdi/event/JobFinishEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 , p1 );  
   if (test_jni_error(env, "EventFactoryBase_createJobFinishEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t EventFactoryBase_static_createJobPriorityModEvent(JNIEnv *env , jlong p0 , jint p1 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactoryBase_static_createJobPriorityModEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/event/EventFactoryBase", "createJobPriorityModEvent", "(JI)Lcom/sun/grid/jgdi/event/JobPriorityModEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 , p1 );  
   if (test_jni_error(env, "EventFactoryBase_createJobPriorityModEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t EventFactoryBase_static_createJobUsageEvent(JNIEnv *env , jlong p0 , jint p1 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactoryBase_static_createJobUsageEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/event/EventFactoryBase", "createJobUsageEvent", "(JI)Lcom/sun/grid/jgdi/event/JobUsageEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 , p1 );  
   if (test_jni_error(env, "EventFactoryBase_createJobUsageEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t EventFactoryBase_static_createQmasterGoesDownEvent(JNIEnv *env , jlong p0 , jint p1 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactoryBase_static_createQmasterGoesDownEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/event/EventFactoryBase", "createQmasterGoesDownEvent", "(JI)Lcom/sun/grid/jgdi/event/QmasterGoesDownEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 , p1 );  
   if (test_jni_error(env, "EventFactoryBase_createQmasterGoesDownEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t EventFactoryBase_static_createQueueInstanceSuspendEvent(JNIEnv *env , jlong p0 , jint p1 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactoryBase_static_createQueueInstanceSuspendEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/event/EventFactoryBase", "createQueueInstanceSuspendEvent", "(JI)Lcom/sun/grid/jgdi/event/QueueInstanceSuspendEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 , p1 );  
   if (test_jni_error(env, "EventFactoryBase_createQueueInstanceSuspendEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t EventFactoryBase_static_createQueueInstanceUnsuspendEvent(JNIEnv *env , jlong p0 , jint p1 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactoryBase_static_createQueueInstanceUnsuspendEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/event/EventFactoryBase", "createQueueInstanceUnsuspendEvent", "(JI)Lcom/sun/grid/jgdi/event/QueueInstanceUnsuspendEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 , p1 );  
   if (test_jni_error(env, "EventFactoryBase_createQueueInstanceUnsuspendEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t EventFactoryBase_static_createSchedulerRunEvent(JNIEnv *env , jlong p0 , jint p1 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactoryBase_static_createSchedulerRunEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/event/EventFactoryBase", "createSchedulerRunEvent", "(JI)Lcom/sun/grid/jgdi/event/SchedulerRunEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 , p1 );  
   if (test_jni_error(env, "EventFactoryBase_createSchedulerRunEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t EventFactoryBase_static_createShutdownEvent(JNIEnv *env , jlong p0 , jint p1 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactoryBase_static_createShutdownEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/event/EventFactoryBase", "createShutdownEvent", "(JI)Lcom/sun/grid/jgdi/event/ShutdownEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 , p1 );  
   if (test_jni_error(env, "EventFactoryBase_createShutdownEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t EventFactoryBase_createJobFinalUsageEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactoryBase_createJobFinalUsageEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/EventFactoryBase", "createJobFinalUsageEvent", "(JI)Lcom/sun/grid/jgdi/event/JobFinalUsageEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "EventFactoryBase_createJobFinalUsageEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t EventFactoryBase_createJobFinishEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactoryBase_createJobFinishEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/EventFactoryBase", "createJobFinishEvent", "(JI)Lcom/sun/grid/jgdi/event/JobFinishEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "EventFactoryBase_createJobFinishEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t EventFactoryBase_createJobPriorityModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactoryBase_createJobPriorityModEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/EventFactoryBase", "createJobPriorityModEvent", "(JI)Lcom/sun/grid/jgdi/event/JobPriorityModEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "EventFactoryBase_createJobPriorityModEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t EventFactoryBase_createJobUsageEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactoryBase_createJobUsageEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/EventFactoryBase", "createJobUsageEvent", "(JI)Lcom/sun/grid/jgdi/event/JobUsageEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "EventFactoryBase_createJobUsageEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t EventFactoryBase_createQmasterGoesDownEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactoryBase_createQmasterGoesDownEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/EventFactoryBase", "createQmasterGoesDownEvent", "(JI)Lcom/sun/grid/jgdi/event/QmasterGoesDownEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "EventFactoryBase_createQmasterGoesDownEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t EventFactoryBase_createQueueInstanceSuspendEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactoryBase_createQueueInstanceSuspendEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/EventFactoryBase", "createQueueInstanceSuspendEvent", "(JI)Lcom/sun/grid/jgdi/event/QueueInstanceSuspendEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "EventFactoryBase_createQueueInstanceSuspendEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t EventFactoryBase_createQueueInstanceUnsuspendEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactoryBase_createQueueInstanceUnsuspendEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/EventFactoryBase", "createQueueInstanceUnsuspendEvent", "(JI)Lcom/sun/grid/jgdi/event/QueueInstanceUnsuspendEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "EventFactoryBase_createQueueInstanceUnsuspendEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t EventFactoryBase_createSchedulerRunEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactoryBase_createSchedulerRunEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/EventFactoryBase", "createSchedulerRunEvent", "(JI)Lcom/sun/grid/jgdi/event/SchedulerRunEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "EventFactoryBase_createSchedulerRunEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t EventFactoryBase_createShutdownEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactoryBase_createShutdownEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/EventFactoryBase", "createShutdownEvent", "(JI)Lcom/sun/grid/jgdi/event/ShutdownEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0, p1);  
   if (test_jni_error(env, "EventFactoryBase_createShutdownEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/event/EventFactoryBase */



/*==== BEGIN  com/sun/grid/jgdi/event/JobEvent */

jclass JobEvent_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "JobEvent_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/event/JobEvent", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t JobEvent_init(JNIEnv *env, jobject *obj , jlong p0 , jint p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "JobEvent_init");
      
   clazz = JobEvent_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(J, I)V", alpp);
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

jgdi_result_t JobEvent_getJobId(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "JobEvent_getJobId");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/JobEvent", "getJobId", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "JobEvent_getJobId failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobEvent_getPeTaskId(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobEvent_getPeTaskId");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/JobEvent", "getPeTaskId", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobEvent_getPeTaskId failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobEvent_getTaskId(JNIEnv *env, jobject obj , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "JobEvent_getTaskId");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/JobEvent", "getTaskId", "()I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid);  
   if (test_jni_error(env, "JobEvent_getTaskId failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobEvent_setJobId(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobEvent_setJobId");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/JobEvent", "setJobId", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobEvent_setJobId failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobEvent_setPeTaskId(JNIEnv *env, jobject obj , const char* p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JobEvent_setPeTaskId");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/JobEvent", "setPeTaskId", "(Ljava/lang/String;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobEvent_setPeTaskId failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobEvent_setTaskId(JNIEnv *env, jobject obj , jint p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "JobEvent_setTaskId");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/JobEvent", "setTaskId", "(I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "JobEvent_setTaskId failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobEvent_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobEvent_toString");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/JobEvent", "toString", "()Ljava/lang/String;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobEvent_toString failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/event/JobEvent */



/*==== BEGIN  com/sun/grid/jgdi/event/JobUsageEvent */

jclass JobUsageEvent_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "JobUsageEvent_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/event/JobUsageEvent", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t JobUsageEvent_init(JNIEnv *env, jobject *obj , jlong p0 , jint p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "JobUsageEvent_init");
      
   clazz = JobUsageEvent_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(J, I)V", alpp);
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

jgdi_result_t JobUsageEvent_addUsage(JNIEnv *env, jobject obj , const char* p0 , jdouble p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "JobUsageEvent_addUsage");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/JobUsageEvent", "addUsage", "(Ljava/lang/String;D)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj, p1);  
   if (test_jni_error(env, "JobUsageEvent_addUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t JobUsageEvent_getLoadValue(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobUsageEvent_getLoadValue");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/JobUsageEvent", "getLoadValue", "(Ljava/lang/String;)Ljava/lang/Double;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj);  
   if (test_jni_error(env, "JobUsageEvent_getLoadValue failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobUsageEvent_getLoadValueNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobUsageEvent_getLoadValueNames");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/JobUsageEvent", "getLoadValueNames", "()Ljava/util/Set;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobUsageEvent_getLoadValueNames failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t JobUsageEvent_getUsage(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "JobUsageEvent_getUsage");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/JobUsageEvent", "getUsage", "()Ljava/util/Map;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "JobUsageEvent_getUsage failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/event/JobUsageEvent */



/*==== BEGIN  com/sun/grid/jgdi/event/EventTypeMapping */

jclass EventTypeMapping_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "EventTypeMapping_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/event/EventTypeMapping", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t EventTypeMapping_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "EventTypeMapping_init");
      
   clazz = EventTypeMapping_find_class(env, alpp);
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

jgdi_result_t EventTypeMapping_static_getEventEnumType(JNIEnv *env , jint p0 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventTypeMapping_static_getEventEnumType");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/event/EventTypeMapping", "getEventEnumType", "(I)Lcom/sun/grid/jgdi/event/EventTypeEnum;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "EventTypeMapping_getEventEnumType failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t EventTypeMapping_static_getNativeEventType(JNIEnv *env , jobject p0 , jint* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "EventTypeMapping_static_getNativeEventType");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/event/EventTypeMapping", "getNativeEventType", "(Lcom/sun/grid/jgdi/event/EventTypeEnum;)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   temp = 
 (*env)->CallStaticIntMethod(env, clazz, mid , p0 );  
   if (test_jni_error(env, "EventTypeMapping_getNativeEventType failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t EventTypeMapping_static_registerEvent(JNIEnv *env , const char* p0 , jint p1 , lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "EventTypeMapping_static_registerEvent");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/event/EventTypeMapping", "registerEvent", "(Ljava/lang/String;I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }
   
 (*env)->CallStaticVoidMethod(env, clazz, mid , p0_obj , p1 );  
   if (test_jni_error(env, "EventTypeMapping_registerEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   } 
   DRETURN(ret);
}
jgdi_result_t EventTypeMapping_getEventEnumType(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventTypeMapping_getEventEnumType");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/EventTypeMapping", "getEventEnumType", "(I)Lcom/sun/grid/jgdi/event/EventTypeEnum;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "EventTypeMapping_getEventEnumType failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t EventTypeMapping_getNativeEventType(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jint temp = 0;

   DENTER(BASIS_LAYER, "EventTypeMapping_getNativeEventType");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = 0;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/EventTypeMapping", "getNativeEventType", "(Lcom/sun/grid/jgdi/event/EventTypeEnum;)I", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallIntMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "EventTypeMapping_getNativeEventType failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = 0;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t EventTypeMapping_registerEvent(JNIEnv *env, jobject obj , const char* p0 , jint p1 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   DENTER(BASIS_LAYER, "EventTypeMapping_registerEvent");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/EventTypeMapping", "registerEvent", "(Ljava/lang/String;I)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   (*env)->CallVoidMethod(env, obj, mid, p0_obj, p1);  
   if (test_jni_error(env, "EventTypeMapping_registerEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/event/EventTypeMapping */



/*==== BEGIN  com/sun/grid/jgdi/event/ListEvent */

jclass ListEvent_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "ListEvent_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/event/ListEvent", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t ListEvent_init(JNIEnv *env, jobject *obj , jlong p0 , jint p1 , jobject p2 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "ListEvent_init");
      
   clazz = ListEvent_find_class(env, alpp);
   if (clazz == NULL) {
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   if (mid == NULL) {
      /* initialize the mid */
      mid = get_methodid(env, clazz, "<init>", "(J, I, Ljava/lang/Class;)V", alpp);
      if (mid == NULL) {
         DRETURN(JGDI_ILLEGAL_STATE);
      }
   }    
   *obj = (*env)->NewObject(env, clazz, mid , p0 , p1 , p2 );
   if (test_jni_error(env, "call of constructor failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
   } 
   DRETURN(ret);
}

jgdi_result_t ListEvent_add(JNIEnv *env, jobject obj , jobject p0 , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   DENTER(BASIS_LAYER, "ListEvent_add");
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/ListEvent", "add", "(Ljava/lang/Object;)V", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   (*env)->CallVoidMethod(env, obj, mid, p0);  
   if (test_jni_error(env, "ListEvent_add failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      
   }    DRETURN(ret);
}
jgdi_result_t ListEvent_getObjects(JNIEnv *env, jobject obj , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jobject temp = NULL;

   DENTER(BASIS_LAYER, "ListEvent_getObjects");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/ListEvent", "getObjects", "()Ljava/util/List;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   temp = (*env)->CallObjectMethod(env, obj, mid);  
   if (test_jni_error(env, "ListEvent_getObjects failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/event/ListEvent */





















































/*==== BEGIN  com/sun/grid/jgdi/event/EventFactory */

jclass EventFactory_find_class(JNIEnv *env, lList** alpp) {
   static jclass clazz = NULL;

   DENTER(BASIS_LAYER, "EventFactory_find_class");
   if (clazz == NULL) {
      clazz = find_class(env, "com/sun/grid/jgdi/event/EventFactory", alpp);
   }
   DRETURN(clazz);
}


jgdi_result_t EventFactory_init(JNIEnv *env, jobject *obj , lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;
   jclass clazz = NULL;
   DENTER(BASIS_LAYER, "EventFactory_init");
      
   clazz = EventFactory_find_class(env, alpp);
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

jgdi_result_t EventFactory_static_createAddEvent(JNIEnv *env , const char* p0 , jlong p1 , jint p2 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactory_static_createAddEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/event/EventFactory", "createAddEvent", "(Ljava/lang/String;JI)Lcom/sun/grid/jgdi/event/AddEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj , p1 , p2 );  
   if (test_jni_error(env, "EventFactory_createAddEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t EventFactory_static_createDelEvent(JNIEnv *env , const char* p0 , jlong p1 , jint p2 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactory_static_createDelEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/event/EventFactory", "createDelEvent", "(Ljava/lang/String;JI)Lcom/sun/grid/jgdi/event/DelEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj , p1 , p2 );  
   if (test_jni_error(env, "EventFactory_createDelEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t EventFactory_static_createListEvent(JNIEnv *env , const char* p0 , jlong p1 , jint p2 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactory_static_createListEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/event/EventFactory", "createListEvent", "(Ljava/lang/String;JI)Lcom/sun/grid/jgdi/event/ListEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj , p1 , p2 );  
   if (test_jni_error(env, "EventFactory_createListEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t EventFactory_static_createModEvent(JNIEnv *env , const char* p0 , jlong p1 , jint p2 , jobject* result, lList **alpp) {

   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL; 
   static jclass clazz = NULL;
   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactory_static_createModEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, "com/sun/grid/jgdi/event/EventFactory", "createModEvent", "(Ljava/lang/String;JI)Lcom/sun/grid/jgdi/event/ModEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0); 
   }   temp = 
 (*env)->CallStaticObjectMethod(env, clazz, mid , p0_obj , p1 , p2 );  
   if (test_jni_error(env, "EventFactory_createModEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   }  else {
  
      *result = temp;
      
   }

   DRETURN(ret);
}
jgdi_result_t EventFactory_createAddEvent(JNIEnv *env, jobject obj , const char* p0 , jlong p1 , jint p2 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactory_createAddEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/EventFactory", "createAddEvent", "(Ljava/lang/String;JI)Lcom/sun/grid/jgdi/event/AddEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj, p1, p2);  
   if (test_jni_error(env, "EventFactory_createAddEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t EventFactory_createDelEvent(JNIEnv *env, jobject obj , const char* p0 , jlong p1 , jint p2 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactory_createDelEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/EventFactory", "createDelEvent", "(Ljava/lang/String;JI)Lcom/sun/grid/jgdi/event/DelEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj, p1, p2);  
   if (test_jni_error(env, "EventFactory_createDelEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t EventFactory_createListEvent(JNIEnv *env, jobject obj , const char* p0 , jlong p1 , jint p2 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactory_createListEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/EventFactory", "createListEvent", "(Ljava/lang/String;JI)Lcom/sun/grid/jgdi/event/ListEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj, p1, p2);  
   if (test_jni_error(env, "EventFactory_createListEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}
jgdi_result_t EventFactory_createModEvent(JNIEnv *env, jobject obj , const char* p0 , jlong p1 , jint p2 , jobject* result, lList **alpp) {
   jgdi_result_t ret = JGDI_SUCCESS;
   static jmethodID mid = NULL;

   jstring p0_obj = NULL;
   jobject temp = NULL;

   DENTER(BASIS_LAYER, "EventFactory_createModEvent");
         
   if (result == NULL ) {
      answer_list_add(alpp, "result is NULL", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);
      DRETURN(JGDI_ILLEGAL_STATE);
   }
   /* We set the result always to the default value */
   *result = NULL;
   
  if (mid == NULL) {
     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, "com/sun/grid/jgdi/event/EventFactory", "createModEvent", "(Ljava/lang/String;JI)Lcom/sun/grid/jgdi/event/ModEvent;", alpp)) {
        DRETURN(JGDI_ILLEGAL_STATE);
     }
   }

   if (p0 != NULL) {
      p0_obj = (*env)->NewStringUTF(env, p0 ); 
   }
   temp = (*env)->CallObjectMethod(env, obj, mid, p0_obj, p1, p2);  
   if (test_jni_error(env, "EventFactory_createModEvent failed", alpp)) {
      ret = JGDI_ILLEGAL_STATE;
      temp = NULL;        
      
   } 
      *result = temp;
      
   DRETURN(ret);
}

/*==== END  com/sun/grid/jgdi/event/EventFactory */
