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

#ifndef JGDI_WRAPPER_H
#define JGDI_WRAPPER_H

#include "jgdi_wrapper_java.h"
#include "jgdi_wrapper_event.h"



/* ==== ClassDescriptor ====================== */
   jclass ClassDescriptor_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t ClassDescriptor_clone(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);
   jgdi_result_t ClassDescriptor_getBeanClass(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ClassDescriptor_getCullName(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ClassDescriptor_getProperties(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ClassDescriptor_getProperty(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp);
   jgdi_result_t ClassDescriptor_getPropertyByCullFieldName(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp);
   jgdi_result_t ClassDescriptor_getPropertyCount(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t ClassDescriptor_getPropertyNames(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp);
   jgdi_result_t ClassDescriptor_getProperty_0(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t ClassDescriptor_newInstance(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ClassDescriptor_toString(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);
   jgdi_result_t ClassDescriptor_validate(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);



/* ==== PropertyDescriptor ====================== */
   jclass PropertyDescriptor_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t PropertyDescriptor_clone(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , lList **alpp);
   jgdi_result_t PropertyDescriptor_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t PropertyDescriptor_getBeanClass(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t PropertyDescriptor_getCullContentField(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t PropertyDescriptor_getCullFieldName(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t PropertyDescriptor_getCullType(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t PropertyDescriptor_getJNIPropertyType(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t PropertyDescriptor_getPropertyName(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t PropertyDescriptor_getPropertyType(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t PropertyDescriptor_hasCullWrapper(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t PropertyDescriptor_hashCode(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t PropertyDescriptor_isBrowsable(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t PropertyDescriptor_isConfigurable(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t PropertyDescriptor_isReadOnly(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t PropertyDescriptor_isSet(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t PropertyDescriptor_setBrowsable(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t PropertyDescriptor_setConfigurable(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t PropertyDescriptor_setCullContentField(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t PropertyDescriptor_setHasCullWrapper(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t PropertyDescriptor_setReadOnly(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);



/* ==== SimplePropertyDescriptor ====================== */
   jclass SimplePropertyDescriptor_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t SimplePropertyDescriptor_init(JNIEnv *env, jobject*obj , jobject p0 , const char* p1 , jobject p2 , const char* p3 , jint p4 , jboolean p5 , jboolean p6 , jboolean p7 , lList **alpp);
   jgdi_result_t SimplePropertyDescriptor_clone(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , lList **alpp);
   jgdi_result_t SimplePropertyDescriptor_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);
   jgdi_result_t SimplePropertyDescriptor_isBrowsable(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t SimplePropertyDescriptor_isRequired(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t SimplePropertyDescriptor_setValue(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , lList **alpp);
   jgdi_result_t SimplePropertyDescriptor_toString(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);



/* ==== ListPropertyDescriptor ====================== */
   jclass ListPropertyDescriptor_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t ListPropertyDescriptor_init(JNIEnv *env, jobject*obj , jobject p0 , const char* p1 , jobject p2 , const char* p3 , jint p4 , jboolean p5 , jboolean p6 , jboolean p7 , lList **alpp);
   jgdi_result_t ListPropertyDescriptor_add(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , lList **alpp);
   jgdi_result_t ListPropertyDescriptor_clone(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , lList **alpp);
   jgdi_result_t ListPropertyDescriptor_get(JNIEnv *env, jobject obj , jobject p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t ListPropertyDescriptor_getCount(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp);
   jgdi_result_t ListPropertyDescriptor_remove(JNIEnv *env, jobject obj , jobject p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t ListPropertyDescriptor_removeAll(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t ListPropertyDescriptor_remove_0(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jboolean* result, lList **alpp);
   jgdi_result_t ListPropertyDescriptor_set(JNIEnv *env, jobject obj , jobject p0 , jint p1 , jobject p2 , lList **alpp);
   jgdi_result_t ListPropertyDescriptor_toString(JNIEnv *env, jobject obj , jobject p0 , jint p1 , jobject* result, lList **alpp);



/* ==== MapPropertyDescriptor ====================== */
   jclass MapPropertyDescriptor_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t MapPropertyDescriptor_get(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jobject* result, lList **alpp);
   jgdi_result_t MapPropertyDescriptor_getCount(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp);
   jgdi_result_t MapPropertyDescriptor_getDefaultKey(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t MapPropertyDescriptor_getKeyCullFieldName(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t MapPropertyDescriptor_getKeyType(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t MapPropertyDescriptor_getKeys(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);
   jgdi_result_t MapPropertyDescriptor_getValueCullFieldName(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t MapPropertyDescriptor_put(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jobject p2 , lList **alpp);
   jgdi_result_t MapPropertyDescriptor_remove(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jobject* result, lList **alpp);
   jgdi_result_t MapPropertyDescriptor_removeAll(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t MapPropertyDescriptor_toString(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jobject* result, lList **alpp);



/* ==== MapListPropertyDescriptor ====================== */
   jclass MapListPropertyDescriptor_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t MapListPropertyDescriptor_add(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jobject p2 , lList **alpp);
   jgdi_result_t MapListPropertyDescriptor_addEmpty(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , lList **alpp);
   jgdi_result_t MapListPropertyDescriptor_get(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jint p2 , jobject* result, lList **alpp);
   jgdi_result_t MapListPropertyDescriptor_getCount(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jint* result, lList **alpp);
   jgdi_result_t MapListPropertyDescriptor_getCullListType(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t MapListPropertyDescriptor_getDefaultKey(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t MapListPropertyDescriptor_getKeyCullFieldName(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t MapListPropertyDescriptor_getKeyType(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t MapListPropertyDescriptor_getKeys(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);
   jgdi_result_t MapListPropertyDescriptor_getList(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jobject* result, lList **alpp);
   jgdi_result_t MapListPropertyDescriptor_getValueCullFieldName(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t MapListPropertyDescriptor_remove(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jint p2 , jobject* result, lList **alpp);
   jgdi_result_t MapListPropertyDescriptor_removeAll(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t MapListPropertyDescriptor_removeAll_0(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , lList **alpp);
   jgdi_result_t MapListPropertyDescriptor_remove_0(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jobject p2 , jboolean* result, lList **alpp);
   jgdi_result_t MapListPropertyDescriptor_set(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jint p2 , jobject p3 , lList **alpp);
   jgdi_result_t MapListPropertyDescriptor_toString(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jint p2 , jobject* result, lList **alpp);



/* ==== JGDIAnswer ====================== */
   jclass JGDIAnswer_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t JGDIAnswer_dump(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JGDIAnswer_getQuality(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t JGDIAnswer_getStatus(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t JGDIAnswer_getText(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JGDIAnswer_isSetQuality(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t JGDIAnswer_isSetStatus(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t JGDIAnswer_isSetText(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t JGDIAnswer_setQuality(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t JGDIAnswer_setStatus(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t JGDIAnswer_setText(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);



/* ==== Util_Difference ====================== */
   jclass Util_Difference_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t Util_Difference_init(JNIEnv *env, jobject*obj , const char* p0 , const char* p1 , lList **alpp);
   jgdi_result_t Util_Difference_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== Util ====================== */
   jclass Util_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t Util_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t Util_static_addDescriptor(JNIEnv *env, jobject p0 , jobject p1 , lList **alpp);
   jgdi_result_t Util_static_clone(JNIEnv *env, jobject p0 , jobject* result, lList **alpp);
   jgdi_result_t Util_static_findObject(JNIEnv *env, const char* p0 , jobject p1 , jobject* result, lList **alpp);
   jgdi_result_t Util_static_getDescriptor(JNIEnv *env, jobject p0 , jobject* result, lList **alpp);
   jgdi_result_t Util_static_getDescriptorForCullType(JNIEnv *env, const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t Util_static_getDifferences(JNIEnv *env, jobject p0 , jobject p1 , jobject p2 , lList **alpp);
   jgdi_result_t Util_static_getDifferences_0(JNIEnv *env, jobject p0 , jobject p1 , const char* p2 , jobject p3 , lList **alpp);
   jgdi_result_t Util_static_nextObjectId(JNIEnv *env, jint* result, lList **alpp);
   jgdi_result_t Util_addDescriptor(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , lList **alpp);
   jgdi_result_t Util_clone(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);
   jgdi_result_t Util_findObject(JNIEnv *env, jobject obj , const char* p0 , jobject p1 , jobject* result, lList **alpp);
   jgdi_result_t Util_getDescriptor(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);
   jgdi_result_t Util_getDescriptorForCullType(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t Util_getDifferences(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jobject p2 , lList **alpp);
   jgdi_result_t Util_getDifferences_0(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , const char* p2 , jobject p3 , lList **alpp);
   jgdi_result_t Util_nextObjectId(JNIEnv *env, jobject obj , jint* result, lList **alpp);



/* ==== JGDIAnswerImpl ====================== */
   jclass JGDIAnswerImpl_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t JGDIAnswerImpl_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t JGDIAnswerImpl_init_0(JNIEnv *env, jobject*obj , jboolean p0 , lList **alpp);
   jgdi_result_t JGDIAnswerImpl_dump(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JGDIAnswerImpl_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t JGDIAnswerImpl_equalsCompletely(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t JGDIAnswerImpl_getQuality(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t JGDIAnswerImpl_getStatus(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t JGDIAnswerImpl_getText(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JGDIAnswerImpl_isSetQuality(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t JGDIAnswerImpl_isSetStatus(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t JGDIAnswerImpl_isSetText(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t JGDIAnswerImpl_setQuality(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t JGDIAnswerImpl_setStatus(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t JGDIAnswerImpl_setText(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t JGDIAnswerImpl_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== QHostOptions ====================== */
   jclass QHostOptions_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QHostOptions_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t QHostOptions_getHostFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QHostOptions_getResourceAttributeFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QHostOptions_getResourceFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QHostOptions_getUserFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QHostOptions_includeJobs(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QHostOptions_includeQueue(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QHostOptions_setHostFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t QHostOptions_setIncludeJobs(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QHostOptions_setIncludeQueue(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QHostOptions_setResourceAttributeFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t QHostOptions_setResourceFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t QHostOptions_setShowAsXML(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QHostOptions_setUserFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t QHostOptions_showAsXML(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);



/* ==== QHostResult ====================== */
   jclass QHostResult_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QHostResult_getHostInfo(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QHostResult_getHostInfo_0(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t QHostResult_getHostNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== BasicQueueOptions ====================== */
   jclass BasicQueueOptions_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t BasicQueueOptions_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t BasicQueueOptions_getQueueFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t BasicQueueOptions_getQueueStateFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t BasicQueueOptions_getQueueUserFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t BasicQueueOptions_getResourceFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t BasicQueueOptions_setQueueFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t BasicQueueOptions_setQueueStateFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t BasicQueueOptions_setQueueUserFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t BasicQueueOptions_setResourceFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t BasicQueueOptions_setShowAdditionalAttributes(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t BasicQueueOptions_showAdditionalAttributes(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t BasicQueueOptions_updateQueueFilter(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t BasicQueueOptions_updateQueueStateFilter(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t BasicQueueOptions_updateQueueUserFilter(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t BasicQueueOptions_updateResourceFilter(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);



/* ==== ClusterQueueSummary ====================== */
   jclass ClusterQueueSummary_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t ClusterQueueSummary_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t ClusterQueueSummary_getAmbiguous(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t ClusterQueueSummary_getAvailableSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t ClusterQueueSummary_getDisabledByCalendar(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t ClusterQueueSummary_getDisabledManual(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t ClusterQueueSummary_getError(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t ClusterQueueSummary_getLoad(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t ClusterQueueSummary_getLoadAlarm(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t ClusterQueueSummary_getManualIntervention(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t ClusterQueueSummary_getName(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ClusterQueueSummary_getOrphaned(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t ClusterQueueSummary_getReservedSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t ClusterQueueSummary_getSuspendByCalendar(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t ClusterQueueSummary_getSuspendManual(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t ClusterQueueSummary_getSuspendOnSubordinate(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t ClusterQueueSummary_getSuspendThreshold(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t ClusterQueueSummary_getTempDisabled(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t ClusterQueueSummary_getTotalSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t ClusterQueueSummary_getUnknown(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t ClusterQueueSummary_getUsedSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t ClusterQueueSummary_isLoadSet(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t ClusterQueueSummary_setAmbiguous(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t ClusterQueueSummary_setAvailableSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t ClusterQueueSummary_setDisabledByCalendar(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t ClusterQueueSummary_setDisabledManual(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t ClusterQueueSummary_setError(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t ClusterQueueSummary_setLoad(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp);
   jgdi_result_t ClusterQueueSummary_setLoadAlarm(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t ClusterQueueSummary_setManualIntervention(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t ClusterQueueSummary_setName(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t ClusterQueueSummary_setOrphaned(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t ClusterQueueSummary_setReservedSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t ClusterQueueSummary_setSuspendByCalendar(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t ClusterQueueSummary_setSuspendManual(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t ClusterQueueSummary_setSuspendOnSubordinate(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t ClusterQueueSummary_setSuspendThreshold(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t ClusterQueueSummary_setTempDisabled(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t ClusterQueueSummary_setTotalSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t ClusterQueueSummary_setUnknown(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t ClusterQueueSummary_setUsedSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp);



/* ==== QueueInstanceSummaryOptions ====================== */
   jclass QueueInstanceSummaryOptions_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummaryOptions_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_getExplain(JNIEnv *env, jobject obj , jchar* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_getJobStateFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_getJobUserFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_getPeFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_getResourceAttributeFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_isExplainSet(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_setExplain(JNIEnv *env, jobject obj , jchar p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_setJobStateFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_setJobUserFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_setPeFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_setResourceAttributeFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_setShowArrayJobs(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_setShowEmptyQueues(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_setShowExtendedSubTaskInfo(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_setShowFullOutput(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_setShowJobPriorities(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_setShowJobUrgency(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_setShowPEJobs(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_setShowRequestedResourcesForJobs(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_showArrayJobs(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_showEmptyQueues(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_showExtendedSubTaskInfo(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_showFullOutput(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_showJobPriorities(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_showJobUrgency(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_showPEJobs(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_showRequestedResourcesForJobs(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_updateJobUserFilter(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_updatePeFilter(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryOptions_updateResourceAttributeFilter(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);



/* ==== QueueInstanceSummaryResult ====================== */
   jclass QueueInstanceSummaryResult_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummaryResult_getErrorJobs(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryResult_getFinishedJobs(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryResult_getPendingJobs(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryResult_getQueueInstanceSummary(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryResult_getZombieJobs(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== QQuotaOptions ====================== */
   jclass QQuotaOptions_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QQuotaOptions_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t QQuotaOptions_getHostFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QQuotaOptions_getPeFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QQuotaOptions_getProjectFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QQuotaOptions_getQueueFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QQuotaOptions_getResourceFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QQuotaOptions_getUserFilter(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QQuotaOptions_setHostFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t QQuotaOptions_setPeFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t QQuotaOptions_setProjectFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t QQuotaOptions_setQueueFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t QQuotaOptions_setResourceFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t QQuotaOptions_setUserFilter(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);



/* ==== QQuotaResult ====================== */
   jclass QQuotaResult_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QQuotaResult_getResourceQuotaRuleInfo(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t QQuotaResult_getResourceQuotaRuleNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QQuotaResult_getResourceQuotaRules(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== HostInfo ====================== */
   jclass HostInfo_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t HostInfo_getArch(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostInfo_getDominanceSet(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostInfo_getHostValue(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t HostInfo_getHostValueCount(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t HostInfo_getHostValueKeys(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostInfo_getHostname(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostInfo_getJobCount(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t HostInfo_getJobList(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostInfo_getLoadAvg(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostInfo_getMemTotal(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostInfo_getMemUsed(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostInfo_getNumberOfProcessors(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostInfo_getQueueCount(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t HostInfo_getQueueList(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostInfo_getResourceValue(JNIEnv *env, jobject obj , const char* p0 , const char* p1 , jobject* result, lList **alpp);
   jgdi_result_t HostInfo_getResourceValueNames(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t HostInfo_getSwapTotal(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostInfo_getSwapUsed(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== QueueInstanceSummary ====================== */
   jclass QueueInstanceSummary_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummary_getArch(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummary_getExplainMessageList(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummary_getFreeSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t QueueInstanceSummary_getJobList(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummary_getLoadAlarmReason(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummary_getLoadAvg(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t QueueInstanceSummary_getLoadAvgStr(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummary_getName(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummary_getQueueType(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummary_getReservedSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t QueueInstanceSummary_getResourceDominanceSet(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummary_getResourceNames(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummary_getResourceValue(JNIEnv *env, jobject obj , const char* p0 , const char* p1 , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummary_getState(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummary_getSuspendAlarmReason(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummary_getTotalSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t QueueInstanceSummary_getUsedSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t QueueInstanceSummary_hasLoadValue(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QueueInstanceSummary_isHasLoadValueFromObject(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);



/* ==== JobSummary ====================== */
   jclass JobSummary_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t JobSummary_addSoftRequestedMasterQueue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t JobSummary_addSoftRequestedQueue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t JobSummary_getArrayPredecessors(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummary_getCheckpointEnv(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummary_getCpuUsage(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t JobSummary_getDeadline(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummary_getDepartment(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummary_getDlcontr(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobSummary_getFtickets(JNIEnv *env, jobject obj , jlong* result, lList **alpp);
   jgdi_result_t JobSummary_getGrantedPEName(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummary_getGrantedPESlots(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t JobSummary_getHardRequestNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummary_getHardRequestValue(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t JobSummary_getHardRequestedMasterQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummary_getHardRequestedQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummary_getIoUsage(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobSummary_getMemUsage(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobSummary_getNormalizedPriority(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobSummary_getNormalizedRequestedPriority(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobSummary_getNormalizedTickets(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobSummary_getNormalizedUrgency(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobSummary_getOtickets(JNIEnv *env, jobject obj , jlong* result, lList **alpp);
   jgdi_result_t JobSummary_getOverrideTickets(JNIEnv *env, jobject obj , jlong* result, lList **alpp);
   jgdi_result_t JobSummary_getParallelEnvironmentName(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummary_getParallelEnvironmentRange(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummary_getPredecessors(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummary_getProject(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummary_getRequestNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummary_getRequestValue(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t JobSummary_getRequestedArrayPredecessors(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummary_getRequestedPredecessors(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummary_getRrcontr(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobSummary_getShare(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobSummary_getSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t JobSummary_getSoftRequestNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummary_getSoftRequestValue(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t JobSummary_getSoftRequestedMasterQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummary_getSoftRequestedQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummary_getStickets(JNIEnv *env, jobject obj , jlong* result, lList **alpp);
   jgdi_result_t JobSummary_getTaskCount(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t JobSummary_getTaskList(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummary_getTickets(JNIEnv *env, jobject obj , jlong* result, lList **alpp);
   jgdi_result_t JobSummary_getUrgency(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobSummary_getWtcontr(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobSummary_hasCpuUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t JobSummary_hasIoUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t JobSummary_hasMemUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t JobSummary_isArray(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t JobSummary_isQueueAssigned(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t JobSummary_isRunning(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t JobSummary_isZombie(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t JobSummary_setNormalizedUrgency(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp);
   jgdi_result_t JobSummary_setSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp);



/* ==== JobInfo ====================== */
   jclass JobInfo_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t JobInfo_getId(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t JobInfo_getMasterQueue(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobInfo_getName(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobInfo_getPriority(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobInfo_getQinstanceName(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobInfo_getQueue(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobInfo_getStartTime(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobInfo_getState(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobInfo_getSubmitTime(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobInfo_getTaskId(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobInfo_getUser(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== ResourceQuotaRuleInfo ====================== */
   jclass ResourceQuotaRuleInfo_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t ResourceQuotaRuleInfo_getHosts(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfo_getLimits(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfo_getPes(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfo_getProjects(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfo_getQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfo_getResouceQuotaRuleName(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfo_getUsers(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfo_getXHosts(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfo_getXPes(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfo_getXProjects(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfo_getXQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfo_getXUsers(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== QueueInfo ====================== */
   jclass QueueInfo_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInfo_getQname(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInfo_getQtype(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInfo_getReservedSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t QueueInfo_getState(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInfo_getTotalSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t QueueInfo_getUsedSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp);



/* ==== HardRequestValue ====================== */
   jclass HardRequestValue_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t HardRequestValue_init(JNIEnv *env, jobject*obj , const char* p0 , const char* p1 , jdouble p2 , lList **alpp);
   jgdi_result_t HardRequestValue_getContribution(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t HardRequestValue_getResource(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HardRequestValue_getValue(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== ResourceQuota ====================== */
   jclass ResourceQuota_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t ResourceQuota_getLimitValue(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuota_getName(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuota_getUsageValue(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== QHostResultImpl ====================== */
   jclass QHostResultImpl_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QHostResultImpl_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t QHostResultImpl_addHostInfo(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t QHostResultImpl_createHostInfo(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t QHostResultImpl_getHostInfo(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QHostResultImpl_getHostInfo_0(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t QHostResultImpl_getHostNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== QueueInstanceSummaryResultImpl ====================== */
   jclass QueueInstanceSummaryResultImpl_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummaryResultImpl_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t QueueInstanceSummaryResultImpl_addErrorJobs(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryResultImpl_addFinishedJobs(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryResultImpl_addPendingJobs(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryResultImpl_addQueueInstanceSummary(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryResultImpl_addZombieJobs(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryResultImpl_getErrorJobs(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryResultImpl_getFinishedJobs(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryResultImpl_getPendingJobs(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryResultImpl_getQueueInstanceSummary(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryResultImpl_getZombieJobs(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== QQuotaResultImpl ====================== */
   jclass QQuotaResultImpl_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QQuotaResultImpl_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t QQuotaResultImpl_addResourceQuotaRuleInfo(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t QQuotaResultImpl_createResourceQuotaRuleInfo(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t QQuotaResultImpl_getResourceQuotaRuleInfo(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t QQuotaResultImpl_getResourceQuotaRuleNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QQuotaResultImpl_getResourceQuotaRules(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== HostInfoImpl ====================== */
   jclass HostInfoImpl_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t HostInfoImpl_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t HostInfoImpl_init_0(JNIEnv *env, jobject*obj , const char* p0 , lList **alpp);
   jgdi_result_t HostInfoImpl_addJob(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t HostInfoImpl_addQueue(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t HostInfoImpl_getArch(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostInfoImpl_getDominanceSet(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostInfoImpl_getHostValue(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t HostInfoImpl_getHostValueCount(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t HostInfoImpl_getHostValueKeys(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostInfoImpl_getHostname(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostInfoImpl_getJobCount(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t HostInfoImpl_getJobList(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostInfoImpl_getLoadAvg(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostInfoImpl_getMemTotal(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostInfoImpl_getMemUsed(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostInfoImpl_getNumberOfProcessors(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostInfoImpl_getQueueCount(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t HostInfoImpl_getQueueList(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostInfoImpl_getResourceValue(JNIEnv *env, jobject obj , const char* p0 , const char* p1 , jobject* result, lList **alpp);
   jgdi_result_t HostInfoImpl_getResourceValueNames(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t HostInfoImpl_getSwapTotal(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostInfoImpl_getSwapUsed(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostInfoImpl_putHostValue(JNIEnv *env, jobject obj , const char* p0 , jobject p1 , lList **alpp);
   jgdi_result_t HostInfoImpl_putResourceValue(JNIEnv *env, jobject obj , const char* p0 , const char* p1 , jobject p2 , lList **alpp);
   jgdi_result_t HostInfoImpl_removeAllHostValue(JNIEnv *env, jobject obj , lList **alpp);
   jgdi_result_t HostInfoImpl_removeHostValue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t HostInfoImpl_setHostname(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);



/* ==== JobInfoImpl ====================== */
   jclass JobInfoImpl_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t JobInfoImpl_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t JobInfoImpl_getId(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t JobInfoImpl_getMasterQueue(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobInfoImpl_getName(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobInfoImpl_getPriority(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobInfoImpl_getQinstanceName(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobInfoImpl_getQueue(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobInfoImpl_getStartTime(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobInfoImpl_getState(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobInfoImpl_getSubmitTime(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobInfoImpl_getTaskId(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobInfoImpl_getUser(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobInfoImpl_setId(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t JobInfoImpl_setMasterQueue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t JobInfoImpl_setName(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t JobInfoImpl_setPriority(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp);
   jgdi_result_t JobInfoImpl_setQinstanceName(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t JobInfoImpl_setQueue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t JobInfoImpl_setStartTime(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t JobInfoImpl_setStartTime_0(JNIEnv *env, jobject obj , jlong p0 , lList **alpp);
   jgdi_result_t JobInfoImpl_setState(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t JobInfoImpl_setSubmitTime(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t JobInfoImpl_setSubmitTime_0(JNIEnv *env, jobject obj , jlong p0 , lList **alpp);
   jgdi_result_t JobInfoImpl_setTaskId(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t JobInfoImpl_setUser(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);



/* ==== JobSummaryImpl ====================== */
   jclass JobSummaryImpl_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t JobSummaryImpl_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t JobSummaryImpl_addArrayPredecessor(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_addHardRequest(JNIEnv *env, jobject obj , const char* p0 , const char* p1 , jdouble p2 , lList **alpp);
   jgdi_result_t JobSummaryImpl_addHardRequestedMasterQueue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_addHardRequestedQueue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_addPredecessor(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_addRequest(JNIEnv *env, jobject obj , const char* p0 , const char* p1 , lList **alpp);
   jgdi_result_t JobSummaryImpl_addRequestedArrayPredecessor(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_addRequestedPredecessor(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_addSoftRequest(JNIEnv *env, jobject obj , const char* p0 , const char* p1 , lList **alpp);
   jgdi_result_t JobSummaryImpl_addSoftRequestedMasterQueue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_addSoftRequestedQueue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_addTask(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_getArrayPredecessors(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getCheckpointEnv(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getCpuUsage(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getDeadline(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getDepartment(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getDlcontr(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getFtickets(JNIEnv *env, jobject obj , jlong* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getGrantedPEName(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getGrantedPESlots(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getHardRequestNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getHardRequestValue(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getHardRequestedMasterQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getHardRequestedQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getIoUsage(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getMemUsage(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getNormalizedPriority(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getNormalizedRequestedPriority(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getNormalizedTickets(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getNormalizedUrgency(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getOtickets(JNIEnv *env, jobject obj , jlong* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getOverrideTickets(JNIEnv *env, jobject obj , jlong* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getParallelEnvironmentName(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getParallelEnvironmentRange(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getPredecessors(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getProject(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getRequestNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getRequestValue(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getRequestedArrayPredecessors(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getRequestedPredecessors(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getRrcontr(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getShare(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getSoftRequestNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getSoftRequestValue(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getSoftRequestedMasterQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getSoftRequestedQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getStickets(JNIEnv *env, jobject obj , jlong* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getTaskCount(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getTaskList(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getTickets(JNIEnv *env, jobject obj , jlong* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getUrgency(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_getWtcontr(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_hasCpuUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_hasIoUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_hasMemUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_isArray(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_isQueueAssigned(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_isRunning(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_isZombie(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t JobSummaryImpl_setArray(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setCheckpointEnv(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setCpuUsage(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setDeadline(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setDeadline_0(JNIEnv *env, jobject obj , jlong p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setDepartment(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setDlcontr(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setFtickets(JNIEnv *env, jobject obj , jlong p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setGrantedPEName(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setGrantedPESlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setIoUsage(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setMemUsage(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setNormalizedPriority(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setNormalizedRequestedPriority(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setNormalizedTickets(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setNormalizedUrgency(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setOtickets(JNIEnv *env, jobject obj , jlong p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setOverrideTickets(JNIEnv *env, jobject obj , jlong p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setParallelEnvironmentName(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setParallelEnvironmentRange(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setProject(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setQueueAssigned(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setRrcontr(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setRunning(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setShare(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setStickets(JNIEnv *env, jobject obj , jlong p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setTickets(JNIEnv *env, jobject obj , jlong p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setUrgency(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setWtcontr(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp);
   jgdi_result_t JobSummaryImpl_setZombie(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);



/* ==== TaskSummary ====================== */
   jclass TaskSummary_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t TaskSummary_getCpuUsage(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t TaskSummary_getExitStatus(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t TaskSummary_getIoUsage(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t TaskSummary_getMemUsage(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t TaskSummary_getState(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t TaskSummary_getTaskId(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t TaskSummary_hasCpuUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t TaskSummary_hasExitStatus(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t TaskSummary_hasIoUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t TaskSummary_hasMemUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t TaskSummary_isRunning(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);



/* ==== QueueInfoImpl ====================== */
   jclass QueueInfoImpl_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInfoImpl_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t QueueInfoImpl_getQname(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInfoImpl_getQtype(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInfoImpl_getReservedSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t QueueInfoImpl_getState(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInfoImpl_getTotalSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t QueueInfoImpl_getUsedSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t QueueInfoImpl_setQname(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t QueueInfoImpl_setQtype(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t QueueInfoImpl_setReservedSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t QueueInfoImpl_setState(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t QueueInfoImpl_setTotalSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t QueueInfoImpl_setUsedSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp);



/* ==== QueueInstanceSummaryImpl ====================== */
   jclass QueueInstanceSummaryImpl_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummaryImpl_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_addExplainMessage(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_addJob(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_addJobs(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_addResource(JNIEnv *env, jobject obj , const char* p0 , const char* p1 , const char* p2 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_getArch(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_getExplainMessageList(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_getFreeSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_getJobList(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_getLoadAlarmReason(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_getLoadAvg(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_getLoadAvgStr(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_getName(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_getQueueType(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_getReservedSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_getResourceDominanceSet(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_getResourceNames(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_getResourceValue(JNIEnv *env, jobject obj , const char* p0 , const char* p1 , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_getState(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_getSuspendAlarmReason(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_getTotalSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_getUsedSlots(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_hasLoadValue(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_isHasLoadValueFromObject(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_setArch(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_setHasLoadValue(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_setHasLoadValueFromObject(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_setLoadAlarmReason(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_setLoadAvg(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_setLoadAvgStr(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_setName(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_setQueueType(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_setReservedSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_setState(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_setSuspendAlarmReason(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_setTotalSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryImpl_setUsedSlots(JNIEnv *env, jobject obj , jint p0 , lList **alpp);



/* ==== QueueInstanceSummaryPrinter_1 ====================== */
   jclass QueueInstanceSummaryPrinter_1_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_1_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);



/* ==== QueueInstanceSummaryPrinter_2 ====================== */
   jclass QueueInstanceSummaryPrinter_2_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_2_getValue(JNIEnv *env, jobject obj , jobject p0 , jlong* result, lList **alpp);



/* ==== QueueInstanceSummaryPrinter_3 ====================== */
   jclass QueueInstanceSummaryPrinter_3_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_3_getValue(JNIEnv *env, jobject obj , jobject p0 , jlong* result, lList **alpp);



/* ==== QueueInstanceSummaryPrinter_4 ====================== */
   jclass QueueInstanceSummaryPrinter_4_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_4_getValue(JNIEnv *env, jobject obj , jobject p0 , jlong* result, lList **alpp);



/* ==== QueueInstanceSummaryPrinter_5 ====================== */
   jclass QueueInstanceSummaryPrinter_5_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_5_getValue(JNIEnv *env, jobject obj , jobject p0 , jlong* result, lList **alpp);



/* ==== QueueInstanceSummaryPrinter_6 ====================== */
   jclass QueueInstanceSummaryPrinter_6_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_6_getValue(JNIEnv *env, jobject obj , jobject p0 , jlong* result, lList **alpp);



/* ==== QueueInstanceSummaryPrinter_7 ====================== */
   jclass QueueInstanceSummaryPrinter_7_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_7_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);



/* ==== QueueInstanceSummaryPrinter_StatCalc ====================== */
   jclass QueueInstanceSummaryPrinter_StatCalc_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_StatCalc_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);



/* ==== QueueInstanceSummaryPrinter_CpuUsageCalc ====================== */
   jclass QueueInstanceSummaryPrinter_CpuUsageCalc_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_CpuUsageCalc_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);



/* ==== QueueInstanceSummaryPrinter_UsageCalc ====================== */
   jclass QueueInstanceSummaryPrinter_UsageCalc_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_UsageCalc_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);



/* ==== QueueInstanceSummaryPrinter_MemUsageCalc ====================== */
   jclass QueueInstanceSummaryPrinter_MemUsageCalc_find_class(JNIEnv *env, lList** alpp);



/* ==== QueueInstanceSummaryPrinter_IOUsageCalc ====================== */
   jclass QueueInstanceSummaryPrinter_IOUsageCalc_find_class(JNIEnv *env, lList** alpp);



/* ==== QueueInstanceSummaryPrinter_TicketCalc ====================== */
   jclass QueueInstanceSummaryPrinter_TicketCalc_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_TicketCalc_init(JNIEnv *env, jobject*obj , jboolean p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_TicketCalc_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);



/* ==== QueueInstanceSummaryPrinter_ShareCalc ====================== */
   jclass QueueInstanceSummaryPrinter_ShareCalc_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_ShareCalc_init(JNIEnv *env, jobject*obj , jboolean p0 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_ShareCalc_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);



/* ==== QueueInstanceSummaryPrinter_JaTaskIdCalc ====================== */
   jclass QueueInstanceSummaryPrinter_JaTaskIdCalc_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_JaTaskIdCalc_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);



/* ==== QueueInstanceSummaryPrinter_8 ====================== */
   jclass QueueInstanceSummaryPrinter_8_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_8_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);



/* ==== QueueInstanceSummaryPrinter_9 ====================== */
   jclass QueueInstanceSummaryPrinter_9_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_9_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);



/* ==== QueueInstanceSummaryPrinter_10 ====================== */
   jclass QueueInstanceSummaryPrinter_10_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_10_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);



/* ==== QueueInstanceSummaryPrinter_11 ====================== */
   jclass QueueInstanceSummaryPrinter_11_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_11_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);



/* ==== QueueInstanceSummaryPrinter_12 ====================== */
   jclass QueueInstanceSummaryPrinter_12_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_12_getValue(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);



/* ==== QueueInstanceSummaryPrinter ====================== */
   jclass QueueInstanceSummaryPrinter_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_static_createJobSummaryTable(JNIEnv *env, jobject p0 , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_static_print(JNIEnv *env, jobject p0 , jobject p1 , jobject p2 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_static_print_0(JNIEnv *env, jobject p0 , jobject p1 , jobject p2 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_createJobSummaryTable(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_print(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jobject p2 , lList **alpp);
   jgdi_result_t QueueInstanceSummaryPrinter_print_0(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jobject p2 , lList **alpp);



/* ==== ResourceQuotaImpl ====================== */
   jclass ResourceQuotaImpl_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t ResourceQuotaImpl_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t ResourceQuotaImpl_init_0(JNIEnv *env, jobject*obj , const char* p0 , lList **alpp);
   jgdi_result_t ResourceQuotaImpl_init_1(JNIEnv *env, jobject*obj , const char* p0 , const char* p1 , const char* p2 , lList **alpp);
   jgdi_result_t ResourceQuotaImpl_getLimitValue(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaImpl_getName(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaImpl_getUsageValue(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaImpl_setLimitValue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t ResourceQuotaImpl_setName(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t ResourceQuotaImpl_setUsageValue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);



/* ==== ResourceQuotaRuleInfoImpl ====================== */
   jclass ResourceQuotaRuleInfoImpl_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_init_0(JNIEnv *env, jobject*obj , const char* p0 , lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_addHost(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_addLimit(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_addPe(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_addProject(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_addQueue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_addUser(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_addXHost(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_addXPe(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_addXProject(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_addXQueue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_addXUser(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_getHosts(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_getLimits(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_getPes(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_getProjects(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_getQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_getResouceQuotaRuleName(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_getUsers(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_getXHosts(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_getXPes(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_getXProjects(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_getXQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_getXUsers(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceQuotaRuleInfoImpl_setresourceQuotaRuleName(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);



/* ==== TaskSummaryImpl ====================== */
   jclass TaskSummaryImpl_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t TaskSummaryImpl_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t TaskSummaryImpl_getCpuUsage(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t TaskSummaryImpl_getExitStatus(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t TaskSummaryImpl_getIoUsage(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t TaskSummaryImpl_getMemUsage(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t TaskSummaryImpl_getState(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t TaskSummaryImpl_getTaskId(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t TaskSummaryImpl_hasCpuUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t TaskSummaryImpl_hasExitStatus(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t TaskSummaryImpl_hasIoUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t TaskSummaryImpl_hasMemUsage(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t TaskSummaryImpl_isRunning(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t TaskSummaryImpl_setCpuUsage(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp);
   jgdi_result_t TaskSummaryImpl_setExitStatus(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t TaskSummaryImpl_setIoUsage(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp);
   jgdi_result_t TaskSummaryImpl_setMemUsage(JNIEnv *env, jobject obj , jdouble p0 , lList **alpp);
   jgdi_result_t TaskSummaryImpl_setRunning(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t TaskSummaryImpl_setState(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t TaskSummaryImpl_setTaskId(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);



/* ==== ClusterQueueSummaryOptions ====================== */
   jclass ClusterQueueSummaryOptions_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t ClusterQueueSummaryOptions_init(JNIEnv *env, jobject*obj , lList **alpp);



/* ==== DefaultSimpleFilter ====================== */
   jclass DefaultSimpleFilter_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t DefaultSimpleFilter_init(JNIEnv *env, jobject*obj , jobject p0 , lList **alpp);
   jgdi_result_t DefaultSimpleFilter_init_0(JNIEnv *env, jobject*obj , const char* p0 , jobject p1 , lList **alpp);
   jgdi_result_t DefaultSimpleFilter_getWhat(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t DefaultSimpleFilter_getWhere(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== SimpleFilter ====================== */
   jclass SimpleFilter_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t SimpleFilter_getWhat(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t SimpleFilter_getWhere(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== JGDIFilter ====================== */
   jclass JGDIFilter_find_class(JNIEnv *env, lList** alpp);



/* ==== WhereClause ====================== */
   jclass WhereClause_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t WhereClause_init(JNIEnv *env, jobject*obj , const char* p0 , jint p1 , const char* p2 , jobject p3 , lList **alpp);
   jgdi_result_t WhereClause_getField(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t WhereClause_getPattern(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t WhereClause_getType(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== IntWhereClause ====================== */
   jclass IntWhereClause_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t IntWhereClause_init(JNIEnv *env, jobject*obj , const char* p0 , jint p1 , const char* p2 , jint p3 , lList **alpp);
   jgdi_result_t IntWhereClause_getValue(JNIEnv *env, jobject obj , jint* result, lList **alpp);



/* ==== NativeFilter ====================== */
   jclass NativeFilter_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t NativeFilter_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t NativeFilter_getNativeFilterFunction(JNIEnv *env, jobject obj , jint* result, lList **alpp);



/* ==== PrimaryKeyFilter ====================== */
   jclass PrimaryKeyFilter_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t PrimaryKeyFilter_init(JNIEnv *env, jobject*obj , const char* p0 , lList **alpp);
   jgdi_result_t PrimaryKeyFilter_exclude(JNIEnv *env, jobject obj , jint p0 , jint p1 , lList **alpp);
   jgdi_result_t PrimaryKeyFilter_exclude_0(JNIEnv *env, jobject obj , jint p0 , const char* p1 , lList **alpp);
   jgdi_result_t PrimaryKeyFilter_getFields(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t PrimaryKeyFilter_getType(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t PrimaryKeyFilter_include(JNIEnv *env, jobject obj , jint p0 , jint p1 , lList **alpp);
   jgdi_result_t PrimaryKeyFilter_include_0(JNIEnv *env, jobject obj , jint p0 , const char* p1 , lList **alpp);



/* ==== StringWhereClause ====================== */
   jclass StringWhereClause_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t StringWhereClause_init(JNIEnv *env, jobject*obj , const char* p0 , jint p1 , const char* p2 , const char* p3 , lList **alpp);
   jgdi_result_t StringWhereClause_getValue(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== SecurityHelper ====================== */
   jclass SecurityHelper_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t SecurityHelper_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t SecurityHelper_static_getCertificate(JNIEnv *env, jobject* result, lList **alpp);
   jgdi_result_t SecurityHelper_static_getPrivateKey(JNIEnv *env, jobject* result, lList **alpp);
   jgdi_result_t SecurityHelper_static_getUsername(JNIEnv *env, jobject* result, lList **alpp);
   jgdi_result_t SecurityHelper_getCertificate(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t SecurityHelper_getPrivateKey(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t SecurityHelper_getUsername(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== ResourceAttributeFilter ====================== */
   jclass ResourceAttributeFilter_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t ResourceAttributeFilter_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t ResourceAttributeFilter_static_parse(JNIEnv *env, const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t ResourceAttributeFilter_addValueName(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t ResourceAttributeFilter_fill(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t ResourceAttributeFilter_getValueNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceAttributeFilter_parse(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t ResourceAttributeFilter_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== HostFilter ====================== */
   jclass HostFilter_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t HostFilter_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t HostFilter_static_parse(JNIEnv *env, const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t HostFilter_addHost(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t HostFilter_getHosts(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t HostFilter_parse(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t HostFilter_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== UserFilter ====================== */
   jclass UserFilter_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t UserFilter_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t UserFilter_static_parse(JNIEnv *env, const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t UserFilter_addUser(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t UserFilter_fill(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t UserFilter_getUsers(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t UserFilter_parse(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t UserFilter_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== ResourceFilter ====================== */
   jclass ResourceFilter_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t ResourceFilter_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t ResourceFilter_static_parse(JNIEnv *env, const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t ResourceFilter_addResource(JNIEnv *env, jobject obj , const char* p0 , const char* p1 , lList **alpp);
   jgdi_result_t ResourceFilter_fill(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t ResourceFilter_getResource(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t ResourceFilter_getResourceNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceFilter_getResources(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ResourceFilter_parse(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);



/* ==== QueueFilter ====================== */
   jclass QueueFilter_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueFilter_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t QueueFilter_static_parse(JNIEnv *env, const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t QueueFilter_addQueue(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t QueueFilter_fill(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t QueueFilter_getQueues(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueFilter_parse(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t QueueFilter_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== QueueStateFilter_StateToString ====================== */
   jclass QueueStateFilter_StateToString_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueStateFilter_StateToString_init(JNIEnv *env, jobject*obj , jint p0 , jchar p1 , lList **alpp);



/* ==== QueueStateFilter ====================== */
   jclass QueueStateFilter_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t QueueStateFilter_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t QueueStateFilter_init_0(JNIEnv *env, jobject*obj , jint p0 , lList **alpp);
   jgdi_result_t QueueStateFilter_static_ALARM(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t QueueStateFilter_static_CALENDAR_DISABLED(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t QueueStateFilter_static_CALENDAR_SUSPENDED(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t QueueStateFilter_static_CONFIGURATION_AMBIGUOUS(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t QueueStateFilter_static_DISABLED(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t QueueStateFilter_static_ERROR(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t QueueStateFilter_static_ORPHANED(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t QueueStateFilter_static_SUBORDINATE(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t QueueStateFilter_static_SUSPENDED(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t QueueStateFilter_static_SUSPEND_ALARM(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t QueueStateFilter_static_UNKNOWN(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t QueueStateFilter_static_parse(JNIEnv *env, const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t QueueStateFilter_fill(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t QueueStateFilter_getOptions(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t QueueStateFilter_hasAlarm(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QueueStateFilter_hasCalendarDisabled(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QueueStateFilter_hasCalendarSuspend(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QueueStateFilter_hasConfigurationAmbiguous(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QueueStateFilter_hasDisabled(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QueueStateFilter_hasError(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QueueStateFilter_hasOrphaned(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QueueStateFilter_hasSubordinate(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QueueStateFilter_hasSuspendAlarm(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QueueStateFilter_hasSuspended(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QueueStateFilter_hasUnknown(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t QueueStateFilter_parse(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t QueueStateFilter_setAlarm(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QueueStateFilter_setCalendarDisabled(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QueueStateFilter_setCalendarSuspend(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QueueStateFilter_setConfigurationAmbiguous(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QueueStateFilter_setDisabled(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QueueStateFilter_setError(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QueueStateFilter_setOrphaned(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QueueStateFilter_setSubordinate(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QueueStateFilter_setSuspendAlarm(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QueueStateFilter_setSuspended(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QueueStateFilter_setUnknown(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t QueueStateFilter_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== ParallelEnvironmentFilter ====================== */
   jclass ParallelEnvironmentFilter_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t ParallelEnvironmentFilter_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t ParallelEnvironmentFilter_static_parse(JNIEnv *env, const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t ParallelEnvironmentFilter_addPE(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t ParallelEnvironmentFilter_fill(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t ParallelEnvironmentFilter_getPECount(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t ParallelEnvironmentFilter_getPEList(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ParallelEnvironmentFilter_parse(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t ParallelEnvironmentFilter_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== JobStateFilter_State ====================== */
   jclass JobStateFilter_State_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t JobStateFilter_State_init(JNIEnv *env, jobject*obj , jint p0 , const char* p1 , lList **alpp);
   jgdi_result_t JobStateFilter_State_getMask(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t JobStateFilter_State_getValue(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobStateFilter_State_isSet(JNIEnv *env, jobject obj , jint p0 , jboolean* result, lList **alpp);



/* ==== JobStateFilter ====================== */
   jclass JobStateFilter_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t JobStateFilter_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t JobStateFilter_init_0(JNIEnv *env, jobject*obj , jobject* p0 , lList **alpp);
   jgdi_result_t JobStateFilter_static_ALL(JNIEnv *env, jobject *res, lList **alpp);
   jgdi_result_t JobStateFilter_static_HOLD(JNIEnv *env, jobject *res, lList **alpp);
   jgdi_result_t JobStateFilter_static_HOLD_JOBARRAYHOLD(JNIEnv *env, jobject *res, lList **alpp);
   jgdi_result_t JobStateFilter_static_HOLD_JOBHOLD(JNIEnv *env, jobject *res, lList **alpp);
   jgdi_result_t JobStateFilter_static_HOLD_OPERATOR(JNIEnv *env, jobject *res, lList **alpp);
   jgdi_result_t JobStateFilter_static_HOLD_STARTTIMEHOLD(JNIEnv *env, jobject *res, lList **alpp);
   jgdi_result_t JobStateFilter_static_HOLD_SYSTEM(JNIEnv *env, jobject *res, lList **alpp);
   jgdi_result_t JobStateFilter_static_HOLD_USER(JNIEnv *env, jobject *res, lList **alpp);
   jgdi_result_t JobStateFilter_static_PENDING(JNIEnv *env, jobject *res, lList **alpp);
   jgdi_result_t JobStateFilter_static_RUNNING(JNIEnv *env, jobject *res, lList **alpp);
   jgdi_result_t JobStateFilter_static_SUSPENDED(JNIEnv *env, jobject *res, lList **alpp);
   jgdi_result_t JobStateFilter_static_ZOMBIE(JNIEnv *env, jobject *res, lList **alpp);
   jgdi_result_t JobStateFilter_static_fill(JNIEnv *env, const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t JobStateFilter_static_parse(JNIEnv *env, const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t JobStateFilter_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t JobStateFilter_exclude(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t JobStateFilter_fill(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t JobStateFilter_getStateString(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobStateFilter_getStates(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t JobStateFilter_hashCode(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t JobStateFilter_isSet(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t JobStateFilter_parse(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t JobStateFilter_set(JNIEnv *env, jobject obj , jobject p0 , jboolean p1 , lList **alpp);
   jgdi_result_t JobStateFilter_setStates(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t JobStateFilter_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== ProjectFilter ====================== */
   jclass ProjectFilter_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t ProjectFilter_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t ProjectFilter_static_parse(JNIEnv *env, const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t ProjectFilter_addProject(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t ProjectFilter_getProjectCount(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t ProjectFilter_getProjectList(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ProjectFilter_parse(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t ProjectFilter_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp);

#endif /* JGDI_WRAPPER_H */
