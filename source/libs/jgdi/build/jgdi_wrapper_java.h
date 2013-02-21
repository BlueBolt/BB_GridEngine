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

#ifndef JGDI_WRAPPER_EVENT_H
#define JGDI_WRAPPER_EVENT_H




/* ==== Iterator ====================== */
   jclass Iterator_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t Iterator_hasNext(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t Iterator_next(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Iterator_remove(JNIEnv *env, jobject obj , lList **alpp);



/* ==== Set ====================== */
   jclass Set_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t Set_add(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t Set_addAll(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t Set_clear(JNIEnv *env, jobject obj , lList **alpp);
   jgdi_result_t Set_contains(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t Set_containsAll(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t Set_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t Set_hashCode(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t Set_isEmpty(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t Set_iterator(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Set_remove(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t Set_removeAll(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t Set_retainAll(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t Set_size(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t Set_toArray(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp);
   jgdi_result_t Set_toArray_0(JNIEnv *env, jobject obj , jobject* p0 , jobject** result, int* len, lList **alpp);



/* ==== List ====================== */
   jclass List_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t List_add(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t List_addAll(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t List_addAll_0(JNIEnv *env, jobject obj , jint p0 , jobject p1 , jboolean* result, lList **alpp);
   jgdi_result_t List_add_0(JNIEnv *env, jobject obj , jint p0 , jobject p1 , lList **alpp);
   jgdi_result_t List_clear(JNIEnv *env, jobject obj , lList **alpp);
   jgdi_result_t List_contains(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t List_containsAll(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t List_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t List_get(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp);
   jgdi_result_t List_hashCode(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t List_indexOf(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp);
   jgdi_result_t List_isEmpty(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t List_iterator(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t List_lastIndexOf(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp);
   jgdi_result_t List_listIterator(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t List_listIterator_0(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp);
   jgdi_result_t List_remove(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp);
   jgdi_result_t List_removeAll(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t List_remove_0(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t List_retainAll(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t List_set(JNIEnv *env, jobject obj , jint p0 , jobject p1 , jobject* result, lList **alpp);
   jgdi_result_t List_size(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t List_subList(JNIEnv *env, jobject obj , jint p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t List_toArray(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp);
   jgdi_result_t List_toArray_0(JNIEnv *env, jobject obj , jobject* p0 , jobject** result, int* len, lList **alpp);



/* ==== ArrayList ====================== */
   jclass ArrayList_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t ArrayList_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t ArrayList_init_0(JNIEnv *env, jobject*obj , jint p0 , lList **alpp);
   jgdi_result_t ArrayList_init_1(JNIEnv *env, jobject*obj , jobject p0 , lList **alpp);
   jgdi_result_t ArrayList_add(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t ArrayList_addAll(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t ArrayList_addAll_0(JNIEnv *env, jobject obj , jint p0 , jobject p1 , jboolean* result, lList **alpp);
   jgdi_result_t ArrayList_add_0(JNIEnv *env, jobject obj , jint p0 , jobject p1 , lList **alpp);
   jgdi_result_t ArrayList_clear(JNIEnv *env, jobject obj , lList **alpp);
   jgdi_result_t ArrayList_clone(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ArrayList_contains(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t ArrayList_ensureCapacity(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t ArrayList_get(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp);
   jgdi_result_t ArrayList_indexOf(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp);
   jgdi_result_t ArrayList_isEmpty(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t ArrayList_lastIndexOf(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp);
   jgdi_result_t ArrayList_remove(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp);
   jgdi_result_t ArrayList_remove_0(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t ArrayList_set(JNIEnv *env, jobject obj , jint p0 , jobject p1 , jobject* result, lList **alpp);
   jgdi_result_t ArrayList_size(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t ArrayList_toArray(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp);
   jgdi_result_t ArrayList_toArray_0(JNIEnv *env, jobject obj , jobject* p0 , jobject** result, int* len, lList **alpp);
   jgdi_result_t ArrayList_trimToSize(JNIEnv *env, jobject obj , lList **alpp);



/* ==== Calendar ====================== */
   jclass Calendar_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t Calendar_static_ALL_STYLES(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_AM(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_AM_PM(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_APRIL(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_AUGUST(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_DATE(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_DAY_OF_MONTH(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_DAY_OF_WEEK(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_DAY_OF_WEEK_IN_MONTH(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_DAY_OF_YEAR(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_DECEMBER(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_DST_OFFSET(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_ERA(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_FEBRUARY(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_FIELD_COUNT(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_FRIDAY(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_HOUR(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_HOUR_OF_DAY(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_JANUARY(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_JULY(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_JUNE(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_LONG(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_MARCH(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_MAY(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_MILLISECOND(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_MINUTE(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_MONDAY(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_MONTH(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_NOVEMBER(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_OCTOBER(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_PM(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_SATURDAY(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_SECOND(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_SEPTEMBER(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_SHORT(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_SUNDAY(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_THURSDAY(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_TUESDAY(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_UNDECIMBER(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_WEDNESDAY(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_WEEK_OF_MONTH(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_WEEK_OF_YEAR(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_YEAR(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_ZONE_OFFSET(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Calendar_static_getAvailableLocales(JNIEnv *env, jobject** result, int* len, lList **alpp);
   jgdi_result_t Calendar_static_getInstance(JNIEnv *env, jobject* result, lList **alpp);
   jgdi_result_t Calendar_static_getInstance_0(JNIEnv *env, jobject p0 , jobject* result, lList **alpp);
   jgdi_result_t Calendar_static_getInstance_1(JNIEnv *env, jobject p0 , jobject* result, lList **alpp);
   jgdi_result_t Calendar_static_getInstance_2(JNIEnv *env, jobject p0 , jobject p1 , jobject* result, lList **alpp);
   jgdi_result_t Calendar_add(JNIEnv *env, jobject obj , jint p0 , jint p1 , lList **alpp);
   jgdi_result_t Calendar_after(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t Calendar_before(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t Calendar_clear(JNIEnv *env, jobject obj , lList **alpp);
   jgdi_result_t Calendar_clear_0(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t Calendar_clone(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Calendar_compareTo(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp);
   jgdi_result_t Calendar_compareTo_0(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp);
   jgdi_result_t Calendar_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t Calendar_get(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp);
   jgdi_result_t Calendar_getActualMaximum(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp);
   jgdi_result_t Calendar_getActualMinimum(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp);
   jgdi_result_t Calendar_getAvailableLocales(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp);
   jgdi_result_t Calendar_getDisplayName(JNIEnv *env, jobject obj , jint p0 , jint p1 , jobject p2 , jobject* result, lList **alpp);
   jgdi_result_t Calendar_getDisplayNames(JNIEnv *env, jobject obj , jint p0 , jint p1 , jobject p2 , jobject* result, lList **alpp);
   jgdi_result_t Calendar_getFirstDayOfWeek(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t Calendar_getGreatestMinimum(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp);
   jgdi_result_t Calendar_getInstance(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Calendar_getInstance_0(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);
   jgdi_result_t Calendar_getInstance_1(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);
   jgdi_result_t Calendar_getInstance_2(JNIEnv *env, jobject obj , jobject p0 , jobject p1 , jobject* result, lList **alpp);
   jgdi_result_t Calendar_getLeastMaximum(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp);
   jgdi_result_t Calendar_getMaximum(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp);
   jgdi_result_t Calendar_getMinimalDaysInFirstWeek(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t Calendar_getMinimum(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp);
   jgdi_result_t Calendar_getTime(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Calendar_getTimeInMillis(JNIEnv *env, jobject obj , jlong* result, lList **alpp);
   jgdi_result_t Calendar_getTimeZone(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Calendar_hashCode(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t Calendar_isLenient(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t Calendar_isSet(JNIEnv *env, jobject obj , jint p0 , jboolean* result, lList **alpp);
   jgdi_result_t Calendar_roll(JNIEnv *env, jobject obj , jint p0 , jboolean p1 , lList **alpp);
   jgdi_result_t Calendar_roll_0(JNIEnv *env, jobject obj , jint p0 , jint p1 , lList **alpp);
   jgdi_result_t Calendar_set(JNIEnv *env, jobject obj , jint p0 , jint p1 , lList **alpp);
   jgdi_result_t Calendar_setFirstDayOfWeek(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t Calendar_setLenient(JNIEnv *env, jobject obj , jboolean p0 , lList **alpp);
   jgdi_result_t Calendar_setMinimalDaysInFirstWeek(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t Calendar_setTime(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t Calendar_setTimeInMillis(JNIEnv *env, jobject obj , jlong p0 , lList **alpp);
   jgdi_result_t Calendar_setTimeZone(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t Calendar_set_0(JNIEnv *env, jobject obj , jint p0 , jint p1 , jint p2 , lList **alpp);
   jgdi_result_t Calendar_set_1(JNIEnv *env, jobject obj , jint p0 , jint p1 , jint p2 , jint p3 , jint p4 , lList **alpp);
   jgdi_result_t Calendar_set_2(JNIEnv *env, jobject obj , jint p0 , jint p1 , jint p2 , jint p3 , jint p4 , jint p5 , lList **alpp);
   jgdi_result_t Calendar_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== Class ====================== */
   jclass Class_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t Class_static_forName(JNIEnv *env, const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t Class_static_forName_0(JNIEnv *env, const char* p0 , jboolean p1 , jobject p2 , jobject* result, lList **alpp);
   jgdi_result_t Class_asSubclass(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);
   jgdi_result_t Class_cast(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);
   jgdi_result_t Class_desiredAssertionStatus(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t Class_forName(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t Class_forName_0(JNIEnv *env, jobject obj , const char* p0 , jboolean p1 , jobject p2 , jobject* result, lList **alpp);
   jgdi_result_t Class_getAnnotation(JNIEnv *env, jobject obj , jobject p0 , jobject* result, lList **alpp);
   jgdi_result_t Class_getAnnotations(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp);
   jgdi_result_t Class_getCanonicalName(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Class_getClassLoader(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Class_getClasses(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp);
   jgdi_result_t Class_getComponentType(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Class_getConstructor(JNIEnv *env, jobject obj , jobject* p0 , jobject* result, lList **alpp);
   jgdi_result_t Class_getConstructors(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp);
   jgdi_result_t Class_getDeclaredAnnotations(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp);
   jgdi_result_t Class_getDeclaredClasses(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp);
   jgdi_result_t Class_getDeclaredConstructor(JNIEnv *env, jobject obj , jobject* p0 , jobject* result, lList **alpp);
   jgdi_result_t Class_getDeclaredConstructors(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp);
   jgdi_result_t Class_getDeclaredField(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t Class_getDeclaredFields(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp);
   jgdi_result_t Class_getDeclaredMethod(JNIEnv *env, jobject obj , const char* p0 , jobject* p1 , jobject* result, lList **alpp);
   jgdi_result_t Class_getDeclaredMethods(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp);
   jgdi_result_t Class_getDeclaringClass(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Class_getEnclosingClass(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Class_getEnclosingConstructor(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Class_getEnclosingMethod(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Class_getEnumConstants(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp);
   jgdi_result_t Class_getField(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t Class_getFields(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp);
   jgdi_result_t Class_getGenericInterfaces(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp);
   jgdi_result_t Class_getGenericSuperclass(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Class_getInterfaces(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp);
   jgdi_result_t Class_getMethod(JNIEnv *env, jobject obj , const char* p0 , jobject* p1 , jobject* result, lList **alpp);
   jgdi_result_t Class_getMethods(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp);
   jgdi_result_t Class_getModifiers(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t Class_getName(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Class_getPackage(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Class_getProtectionDomain(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Class_getResource(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t Class_getResourceAsStream(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t Class_getSigners(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp);
   jgdi_result_t Class_getSimpleName(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Class_getSuperclass(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Class_getTypeParameters(JNIEnv *env, jobject obj , jobject** result, int* len, lList **alpp);
   jgdi_result_t Class_isAnnotation(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t Class_isAnnotationPresent(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t Class_isAnonymousClass(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t Class_isArray(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t Class_isAssignableFrom(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t Class_isEnum(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t Class_isInstance(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t Class_isInterface(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t Class_isLocalClass(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t Class_isMemberClass(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t Class_isPrimitive(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t Class_isSynthetic(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t Class_newInstance(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Class_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== Object ====================== */
   jclass Object_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t Object_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t Object_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t Object_getClass(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Object_hashCode(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t Object_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== Long ====================== */
   jclass Long_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t Long_init(JNIEnv *env, jobject*obj , const char* p0 , lList **alpp);
   jgdi_result_t Long_init_0(JNIEnv *env, jobject*obj , jlong p0 , lList **alpp);
   jgdi_result_t Long_static_MAX_VALUE(JNIEnv *env, jlong *res, lList **alpp);
   jgdi_result_t Long_static_MIN_VALUE(JNIEnv *env, jlong *res, lList **alpp);
   jgdi_result_t Long_static_SIZE(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Long_static_TYPE(JNIEnv *env, jobject *res, lList **alpp);
   jgdi_result_t Long_static_bitCount(JNIEnv *env, jlong p0 , jint* result, lList **alpp);
   jgdi_result_t Long_static_decode(JNIEnv *env, const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t Long_static_getLong(JNIEnv *env, const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t Long_static_getLong_0(JNIEnv *env, const char* p0 , jobject p1 , jobject* result, lList **alpp);
   jgdi_result_t Long_static_getLong_1(JNIEnv *env, const char* p0 , jlong p1 , jobject* result, lList **alpp);
   jgdi_result_t Long_static_highestOneBit(JNIEnv *env, jlong p0 , jlong* result, lList **alpp);
   jgdi_result_t Long_static_lowestOneBit(JNIEnv *env, jlong p0 , jlong* result, lList **alpp);
   jgdi_result_t Long_static_numberOfLeadingZeros(JNIEnv *env, jlong p0 , jint* result, lList **alpp);
   jgdi_result_t Long_static_numberOfTrailingZeros(JNIEnv *env, jlong p0 , jint* result, lList **alpp);
   jgdi_result_t Long_static_parseLong(JNIEnv *env, const char* p0 , jlong* result, lList **alpp);
   jgdi_result_t Long_static_parseLong_0(JNIEnv *env, const char* p0 , jint p1 , jlong* result, lList **alpp);
   jgdi_result_t Long_static_reverse(JNIEnv *env, jlong p0 , jlong* result, lList **alpp);
   jgdi_result_t Long_static_reverseBytes(JNIEnv *env, jlong p0 , jlong* result, lList **alpp);
   jgdi_result_t Long_static_rotateLeft(JNIEnv *env, jlong p0 , jint p1 , jlong* result, lList **alpp);
   jgdi_result_t Long_static_rotateRight(JNIEnv *env, jlong p0 , jint p1 , jlong* result, lList **alpp);
   jgdi_result_t Long_static_signum(JNIEnv *env, jlong p0 , jint* result, lList **alpp);
   jgdi_result_t Long_static_toBinaryString(JNIEnv *env, jlong p0 , jobject* result, lList **alpp);
   jgdi_result_t Long_static_toHexString(JNIEnv *env, jlong p0 , jobject* result, lList **alpp);
   jgdi_result_t Long_static_toOctalString(JNIEnv *env, jlong p0 , jobject* result, lList **alpp);
   jgdi_result_t Long_static_toString(JNIEnv *env, jlong p0 , jobject* result, lList **alpp);
   jgdi_result_t Long_static_toString_0(JNIEnv *env, jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t Long_static_valueOf(JNIEnv *env, const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t Long_static_valueOf_0(JNIEnv *env, jlong p0 , jobject* result, lList **alpp);
   jgdi_result_t Long_static_valueOf_1(JNIEnv *env, const char* p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t Long_bitCount(JNIEnv *env, jobject obj , jlong p0 , jint* result, lList **alpp);
   jgdi_result_t Long_byteValue(JNIEnv *env, jobject obj , jbyte* result, lList **alpp);
   jgdi_result_t Long_compareTo(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp);
   jgdi_result_t Long_compareTo_0(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp);
   jgdi_result_t Long_decode(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t Long_doubleValue(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t Long_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t Long_floatValue(JNIEnv *env, jobject obj , jfloat* result, lList **alpp);
   jgdi_result_t Long_getLong(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t Long_getLong_0(JNIEnv *env, jobject obj , const char* p0 , jobject p1 , jobject* result, lList **alpp);
   jgdi_result_t Long_getLong_1(JNIEnv *env, jobject obj , const char* p0 , jlong p1 , jobject* result, lList **alpp);
   jgdi_result_t Long_hashCode(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t Long_highestOneBit(JNIEnv *env, jobject obj , jlong p0 , jlong* result, lList **alpp);
   jgdi_result_t Long_intValue(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t Long_longValue(JNIEnv *env, jobject obj , jlong* result, lList **alpp);
   jgdi_result_t Long_lowestOneBit(JNIEnv *env, jobject obj , jlong p0 , jlong* result, lList **alpp);
   jgdi_result_t Long_numberOfLeadingZeros(JNIEnv *env, jobject obj , jlong p0 , jint* result, lList **alpp);
   jgdi_result_t Long_numberOfTrailingZeros(JNIEnv *env, jobject obj , jlong p0 , jint* result, lList **alpp);
   jgdi_result_t Long_parseLong(JNIEnv *env, jobject obj , const char* p0 , jlong* result, lList **alpp);
   jgdi_result_t Long_parseLong_0(JNIEnv *env, jobject obj , const char* p0 , jint p1 , jlong* result, lList **alpp);
   jgdi_result_t Long_reverse(JNIEnv *env, jobject obj , jlong p0 , jlong* result, lList **alpp);
   jgdi_result_t Long_reverseBytes(JNIEnv *env, jobject obj , jlong p0 , jlong* result, lList **alpp);
   jgdi_result_t Long_rotateLeft(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jlong* result, lList **alpp);
   jgdi_result_t Long_rotateRight(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jlong* result, lList **alpp);
   jgdi_result_t Long_shortValue(JNIEnv *env, jobject obj , jshort* result, lList **alpp);
   jgdi_result_t Long_signum(JNIEnv *env, jobject obj , jlong p0 , jint* result, lList **alpp);
   jgdi_result_t Long_toBinaryString(JNIEnv *env, jobject obj , jlong p0 , jobject* result, lList **alpp);
   jgdi_result_t Long_toHexString(JNIEnv *env, jobject obj , jlong p0 , jobject* result, lList **alpp);
   jgdi_result_t Long_toOctalString(JNIEnv *env, jobject obj , jlong p0 , jobject* result, lList **alpp);
   jgdi_result_t Long_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Long_toString_0(JNIEnv *env, jobject obj , jlong p0 , jobject* result, lList **alpp);
   jgdi_result_t Long_toString_1(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t Long_valueOf(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t Long_valueOf_0(JNIEnv *env, jobject obj , jlong p0 , jobject* result, lList **alpp);
   jgdi_result_t Long_valueOf_1(JNIEnv *env, jobject obj , const char* p0 , jint p1 , jobject* result, lList **alpp);



/* ==== Integer ====================== */
   jclass Integer_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t Integer_init(JNIEnv *env, jobject*obj , jint p0 , lList **alpp);
   jgdi_result_t Integer_init_0(JNIEnv *env, jobject*obj , const char* p0 , lList **alpp);
   jgdi_result_t Integer_static_MAX_VALUE(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Integer_static_MIN_VALUE(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Integer_static_SIZE(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Integer_static_TYPE(JNIEnv *env, jobject *res, lList **alpp);
   jgdi_result_t Integer_static_bitCount(JNIEnv *env, jint p0 , jint* result, lList **alpp);
   jgdi_result_t Integer_static_decode(JNIEnv *env, const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t Integer_static_getInteger(JNIEnv *env, const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t Integer_static_getInteger_0(JNIEnv *env, const char* p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t Integer_static_getInteger_1(JNIEnv *env, const char* p0 , jobject p1 , jobject* result, lList **alpp);
   jgdi_result_t Integer_static_highestOneBit(JNIEnv *env, jint p0 , jint* result, lList **alpp);
   jgdi_result_t Integer_static_lowestOneBit(JNIEnv *env, jint p0 , jint* result, lList **alpp);
   jgdi_result_t Integer_static_numberOfLeadingZeros(JNIEnv *env, jint p0 , jint* result, lList **alpp);
   jgdi_result_t Integer_static_numberOfTrailingZeros(JNIEnv *env, jint p0 , jint* result, lList **alpp);
   jgdi_result_t Integer_static_parseInt(JNIEnv *env, const char* p0 , jint* result, lList **alpp);
   jgdi_result_t Integer_static_parseInt_0(JNIEnv *env, const char* p0 , jint p1 , jint* result, lList **alpp);
   jgdi_result_t Integer_static_reverse(JNIEnv *env, jint p0 , jint* result, lList **alpp);
   jgdi_result_t Integer_static_reverseBytes(JNIEnv *env, jint p0 , jint* result, lList **alpp);
   jgdi_result_t Integer_static_rotateLeft(JNIEnv *env, jint p0 , jint p1 , jint* result, lList **alpp);
   jgdi_result_t Integer_static_rotateRight(JNIEnv *env, jint p0 , jint p1 , jint* result, lList **alpp);
   jgdi_result_t Integer_static_signum(JNIEnv *env, jint p0 , jint* result, lList **alpp);
   jgdi_result_t Integer_static_toBinaryString(JNIEnv *env, jint p0 , jobject* result, lList **alpp);
   jgdi_result_t Integer_static_toHexString(JNIEnv *env, jint p0 , jobject* result, lList **alpp);
   jgdi_result_t Integer_static_toOctalString(JNIEnv *env, jint p0 , jobject* result, lList **alpp);
   jgdi_result_t Integer_static_toString(JNIEnv *env, jint p0 , jobject* result, lList **alpp);
   jgdi_result_t Integer_static_toString_0(JNIEnv *env, jint p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t Integer_static_valueOf(JNIEnv *env, jint p0 , jobject* result, lList **alpp);
   jgdi_result_t Integer_static_valueOf_0(JNIEnv *env, const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t Integer_static_valueOf_1(JNIEnv *env, const char* p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t Integer_bitCount(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp);
   jgdi_result_t Integer_byteValue(JNIEnv *env, jobject obj , jbyte* result, lList **alpp);
   jgdi_result_t Integer_compareTo(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp);
   jgdi_result_t Integer_compareTo_0(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp);
   jgdi_result_t Integer_decode(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t Integer_doubleValue(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t Integer_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t Integer_floatValue(JNIEnv *env, jobject obj , jfloat* result, lList **alpp);
   jgdi_result_t Integer_getInteger(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t Integer_getInteger_0(JNIEnv *env, jobject obj , const char* p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t Integer_getInteger_1(JNIEnv *env, jobject obj , const char* p0 , jobject p1 , jobject* result, lList **alpp);
   jgdi_result_t Integer_hashCode(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t Integer_highestOneBit(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp);
   jgdi_result_t Integer_intValue(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t Integer_longValue(JNIEnv *env, jobject obj , jlong* result, lList **alpp);
   jgdi_result_t Integer_lowestOneBit(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp);
   jgdi_result_t Integer_numberOfLeadingZeros(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp);
   jgdi_result_t Integer_numberOfTrailingZeros(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp);
   jgdi_result_t Integer_parseInt(JNIEnv *env, jobject obj , const char* p0 , jint* result, lList **alpp);
   jgdi_result_t Integer_parseInt_0(JNIEnv *env, jobject obj , const char* p0 , jint p1 , jint* result, lList **alpp);
   jgdi_result_t Integer_reverse(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp);
   jgdi_result_t Integer_reverseBytes(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp);
   jgdi_result_t Integer_rotateLeft(JNIEnv *env, jobject obj , jint p0 , jint p1 , jint* result, lList **alpp);
   jgdi_result_t Integer_rotateRight(JNIEnv *env, jobject obj , jint p0 , jint p1 , jint* result, lList **alpp);
   jgdi_result_t Integer_shortValue(JNIEnv *env, jobject obj , jshort* result, lList **alpp);
   jgdi_result_t Integer_signum(JNIEnv *env, jobject obj , jint p0 , jint* result, lList **alpp);
   jgdi_result_t Integer_toBinaryString(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp);
   jgdi_result_t Integer_toHexString(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp);
   jgdi_result_t Integer_toOctalString(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp);
   jgdi_result_t Integer_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Integer_toString_0(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp);
   jgdi_result_t Integer_toString_1(JNIEnv *env, jobject obj , jint p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t Integer_valueOf(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp);
   jgdi_result_t Integer_valueOf_0(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t Integer_valueOf_1(JNIEnv *env, jobject obj , const char* p0 , jint p1 , jobject* result, lList **alpp);



/* ==== Boolean ====================== */
   jclass Boolean_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t Boolean_init(JNIEnv *env, jobject*obj , jboolean p0 , lList **alpp);
   jgdi_result_t Boolean_init_0(JNIEnv *env, jobject*obj , const char* p0 , lList **alpp);
   jgdi_result_t Boolean_static_FALSE(JNIEnv *env, jobject *res, lList **alpp);
   jgdi_result_t Boolean_static_TRUE(JNIEnv *env, jobject *res, lList **alpp);
   jgdi_result_t Boolean_static_TYPE(JNIEnv *env, jobject *res, lList **alpp);
   jgdi_result_t Boolean_static_getBoolean(JNIEnv *env, const char* p0 , jboolean* result, lList **alpp);
   jgdi_result_t Boolean_static_parseBoolean(JNIEnv *env, const char* p0 , jboolean* result, lList **alpp);
   jgdi_result_t Boolean_static_toString(JNIEnv *env, jboolean p0 , jobject* result, lList **alpp);
   jgdi_result_t Boolean_static_valueOf(JNIEnv *env, jboolean p0 , jobject* result, lList **alpp);
   jgdi_result_t Boolean_static_valueOf_0(JNIEnv *env, const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t Boolean_booleanValue(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t Boolean_compareTo(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp);
   jgdi_result_t Boolean_compareTo_0(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp);
   jgdi_result_t Boolean_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t Boolean_getBoolean(JNIEnv *env, jobject obj , const char* p0 , jboolean* result, lList **alpp);
   jgdi_result_t Boolean_hashCode(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t Boolean_parseBoolean(JNIEnv *env, jobject obj , const char* p0 , jboolean* result, lList **alpp);
   jgdi_result_t Boolean_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Boolean_toString_0(JNIEnv *env, jobject obj , jboolean p0 , jobject* result, lList **alpp);
   jgdi_result_t Boolean_valueOf(JNIEnv *env, jobject obj , jboolean p0 , jobject* result, lList **alpp);
   jgdi_result_t Boolean_valueOf_0(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);



/* ==== Double ====================== */
   jclass Double_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t Double_init(JNIEnv *env, jobject*obj , jdouble p0 , lList **alpp);
   jgdi_result_t Double_init_0(JNIEnv *env, jobject*obj , const char* p0 , lList **alpp);
   jgdi_result_t Double_static_MAX_EXPONENT(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Double_static_MAX_VALUE(JNIEnv *env, jdouble *res, lList **alpp);
   jgdi_result_t Double_static_MIN_EXPONENT(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Double_static_MIN_NORMAL(JNIEnv *env, jdouble *res, lList **alpp);
   jgdi_result_t Double_static_MIN_VALUE(JNIEnv *env, jdouble *res, lList **alpp);
   jgdi_result_t Double_static_NEGATIVE_INFINITY(JNIEnv *env, jdouble *res, lList **alpp);
   jgdi_result_t Double_static_NaN(JNIEnv *env, jdouble *res, lList **alpp);
   jgdi_result_t Double_static_POSITIVE_INFINITY(JNIEnv *env, jdouble *res, lList **alpp);
   jgdi_result_t Double_static_SIZE(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Double_static_TYPE(JNIEnv *env, jobject *res, lList **alpp);
   jgdi_result_t Double_static_compare(JNIEnv *env, jdouble p0 , jdouble p1 , jint* result, lList **alpp);
   jgdi_result_t Double_static_doubleToLongBits(JNIEnv *env, jdouble p0 , jlong* result, lList **alpp);
   jgdi_result_t Double_static_doubleToRawLongBits(JNIEnv *env, jdouble p0 , jlong* result, lList **alpp);
   jgdi_result_t Double_static_isInfinite(JNIEnv *env, jdouble p0 , jboolean* result, lList **alpp);
   jgdi_result_t Double_static_isNaN(JNIEnv *env, jdouble p0 , jboolean* result, lList **alpp);
   jgdi_result_t Double_static_longBitsToDouble(JNIEnv *env, jlong p0 , jdouble* result, lList **alpp);
   jgdi_result_t Double_static_parseDouble(JNIEnv *env, const char* p0 , jdouble* result, lList **alpp);
   jgdi_result_t Double_static_toHexString(JNIEnv *env, jdouble p0 , jobject* result, lList **alpp);
   jgdi_result_t Double_static_toString(JNIEnv *env, jdouble p0 , jobject* result, lList **alpp);
   jgdi_result_t Double_static_valueOf(JNIEnv *env, jdouble p0 , jobject* result, lList **alpp);
   jgdi_result_t Double_static_valueOf_0(JNIEnv *env, const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t Double_byteValue(JNIEnv *env, jobject obj , jbyte* result, lList **alpp);
   jgdi_result_t Double_compare(JNIEnv *env, jobject obj , jdouble p0 , jdouble p1 , jint* result, lList **alpp);
   jgdi_result_t Double_compareTo(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp);
   jgdi_result_t Double_compareTo_0(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp);
   jgdi_result_t Double_doubleToLongBits(JNIEnv *env, jobject obj , jdouble p0 , jlong* result, lList **alpp);
   jgdi_result_t Double_doubleToRawLongBits(JNIEnv *env, jobject obj , jdouble p0 , jlong* result, lList **alpp);
   jgdi_result_t Double_doubleValue(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t Double_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t Double_floatValue(JNIEnv *env, jobject obj , jfloat* result, lList **alpp);
   jgdi_result_t Double_hashCode(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t Double_intValue(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t Double_isInfinite(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t Double_isInfinite_0(JNIEnv *env, jobject obj , jdouble p0 , jboolean* result, lList **alpp);
   jgdi_result_t Double_isNaN(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t Double_isNaN_0(JNIEnv *env, jobject obj , jdouble p0 , jboolean* result, lList **alpp);
   jgdi_result_t Double_longBitsToDouble(JNIEnv *env, jobject obj , jlong p0 , jdouble* result, lList **alpp);
   jgdi_result_t Double_longValue(JNIEnv *env, jobject obj , jlong* result, lList **alpp);
   jgdi_result_t Double_parseDouble(JNIEnv *env, jobject obj , const char* p0 , jdouble* result, lList **alpp);
   jgdi_result_t Double_shortValue(JNIEnv *env, jobject obj , jshort* result, lList **alpp);
   jgdi_result_t Double_toHexString(JNIEnv *env, jobject obj , jdouble p0 , jobject* result, lList **alpp);
   jgdi_result_t Double_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Double_toString_0(JNIEnv *env, jobject obj , jdouble p0 , jobject* result, lList **alpp);
   jgdi_result_t Double_valueOf(JNIEnv *env, jobject obj , jdouble p0 , jobject* result, lList **alpp);
   jgdi_result_t Double_valueOf_0(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);



/* ==== Float ====================== */
   jclass Float_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t Float_init(JNIEnv *env, jobject*obj , jdouble p0 , lList **alpp);
   jgdi_result_t Float_init_0(JNIEnv *env, jobject*obj , jfloat p0 , lList **alpp);
   jgdi_result_t Float_init_1(JNIEnv *env, jobject*obj , const char* p0 , lList **alpp);
   jgdi_result_t Float_static_MAX_EXPONENT(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Float_static_MAX_VALUE(JNIEnv *env, jfloat *res, lList **alpp);
   jgdi_result_t Float_static_MIN_EXPONENT(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Float_static_MIN_NORMAL(JNIEnv *env, jfloat *res, lList **alpp);
   jgdi_result_t Float_static_MIN_VALUE(JNIEnv *env, jfloat *res, lList **alpp);
   jgdi_result_t Float_static_NEGATIVE_INFINITY(JNIEnv *env, jfloat *res, lList **alpp);
   jgdi_result_t Float_static_NaN(JNIEnv *env, jfloat *res, lList **alpp);
   jgdi_result_t Float_static_POSITIVE_INFINITY(JNIEnv *env, jfloat *res, lList **alpp);
   jgdi_result_t Float_static_SIZE(JNIEnv *env, jint *res, lList **alpp);
   jgdi_result_t Float_static_TYPE(JNIEnv *env, jobject *res, lList **alpp);
   jgdi_result_t Float_static_compare(JNIEnv *env, jfloat p0 , jfloat p1 , jint* result, lList **alpp);
   jgdi_result_t Float_static_floatToIntBits(JNIEnv *env, jfloat p0 , jint* result, lList **alpp);
   jgdi_result_t Float_static_floatToRawIntBits(JNIEnv *env, jfloat p0 , jint* result, lList **alpp);
   jgdi_result_t Float_static_intBitsToFloat(JNIEnv *env, jint p0 , jfloat* result, lList **alpp);
   jgdi_result_t Float_static_isInfinite(JNIEnv *env, jfloat p0 , jboolean* result, lList **alpp);
   jgdi_result_t Float_static_isNaN(JNIEnv *env, jfloat p0 , jboolean* result, lList **alpp);
   jgdi_result_t Float_static_parseFloat(JNIEnv *env, const char* p0 , jfloat* result, lList **alpp);
   jgdi_result_t Float_static_toHexString(JNIEnv *env, jfloat p0 , jobject* result, lList **alpp);
   jgdi_result_t Float_static_toString(JNIEnv *env, jfloat p0 , jobject* result, lList **alpp);
   jgdi_result_t Float_static_valueOf(JNIEnv *env, jfloat p0 , jobject* result, lList **alpp);
   jgdi_result_t Float_static_valueOf_0(JNIEnv *env, const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t Float_byteValue(JNIEnv *env, jobject obj , jbyte* result, lList **alpp);
   jgdi_result_t Float_compare(JNIEnv *env, jobject obj , jfloat p0 , jfloat p1 , jint* result, lList **alpp);
   jgdi_result_t Float_compareTo(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp);
   jgdi_result_t Float_compareTo_0(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp);
   jgdi_result_t Float_doubleValue(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t Float_equals(JNIEnv *env, jobject obj , jobject p0 , jboolean* result, lList **alpp);
   jgdi_result_t Float_floatToIntBits(JNIEnv *env, jobject obj , jfloat p0 , jint* result, lList **alpp);
   jgdi_result_t Float_floatToRawIntBits(JNIEnv *env, jobject obj , jfloat p0 , jint* result, lList **alpp);
   jgdi_result_t Float_floatValue(JNIEnv *env, jobject obj , jfloat* result, lList **alpp);
   jgdi_result_t Float_hashCode(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t Float_intBitsToFloat(JNIEnv *env, jobject obj , jint p0 , jfloat* result, lList **alpp);
   jgdi_result_t Float_intValue(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t Float_isInfinite(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t Float_isInfinite_0(JNIEnv *env, jobject obj , jfloat p0 , jboolean* result, lList **alpp);
   jgdi_result_t Float_isNaN(JNIEnv *env, jobject obj , jboolean* result, lList **alpp);
   jgdi_result_t Float_isNaN_0(JNIEnv *env, jobject obj , jfloat p0 , jboolean* result, lList **alpp);
   jgdi_result_t Float_longValue(JNIEnv *env, jobject obj , jlong* result, lList **alpp);
   jgdi_result_t Float_parseFloat(JNIEnv *env, jobject obj , const char* p0 , jfloat* result, lList **alpp);
   jgdi_result_t Float_shortValue(JNIEnv *env, jobject obj , jshort* result, lList **alpp);
   jgdi_result_t Float_toHexString(JNIEnv *env, jobject obj , jfloat p0 , jobject* result, lList **alpp);
   jgdi_result_t Float_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t Float_toString_0(JNIEnv *env, jobject obj , jfloat p0 , jobject* result, lList **alpp);
   jgdi_result_t Float_valueOf(JNIEnv *env, jobject obj , jfloat p0 , jobject* result, lList **alpp);
   jgdi_result_t Float_valueOf_0(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);



/* ==== Number ====================== */
   jclass Number_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t Number_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t Number_byteValue(JNIEnv *env, jobject obj , jbyte* result, lList **alpp);
   jgdi_result_t Number_doubleValue(JNIEnv *env, jobject obj , jdouble* result, lList **alpp);
   jgdi_result_t Number_floatValue(JNIEnv *env, jobject obj , jfloat* result, lList **alpp);
   jgdi_result_t Number_intValue(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t Number_longValue(JNIEnv *env, jobject obj , jlong* result, lList **alpp);
   jgdi_result_t Number_shortValue(JNIEnv *env, jobject obj , jshort* result, lList **alpp);

#endif /* JGDI_WRAPPER_JAVA_H */
