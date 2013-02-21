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

#ifndef JGDI_WRAPPER_JAVA_H
#define JGDI_WRAPPER_JAVA_H




/* ==== ChangedObjectEvent ====================== */
   jclass ChangedObjectEvent_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t ChangedObjectEvent_init(JNIEnv *env, jobject*obj , jobject p0 , jlong p1 , jint p2 , jobject p3 , lList **alpp);
   jgdi_result_t ChangedObjectEvent_getChangedObject(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ChangedObjectEvent_getObjectType(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t ChangedObjectEvent_setChangedObject(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t ChangedObjectEvent_setPKInfo(JNIEnv *env, jobject obj , jint p0 , jint p1 , const char* p2 , const char* p3 , lList **alpp);
   jgdi_result_t ChangedObjectEvent_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== EventFactoryBase ====================== */
   jclass EventFactoryBase_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactoryBase_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t EventFactoryBase_static_createJobFinalUsageEvent(JNIEnv *env, jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactoryBase_static_createJobFinishEvent(JNIEnv *env, jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactoryBase_static_createJobPriorityModEvent(JNIEnv *env, jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactoryBase_static_createJobUsageEvent(JNIEnv *env, jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactoryBase_static_createQmasterGoesDownEvent(JNIEnv *env, jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactoryBase_static_createQueueInstanceSuspendEvent(JNIEnv *env, jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactoryBase_static_createQueueInstanceUnsuspendEvent(JNIEnv *env, jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactoryBase_static_createSchedulerRunEvent(JNIEnv *env, jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactoryBase_static_createShutdownEvent(JNIEnv *env, jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactoryBase_createJobFinalUsageEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactoryBase_createJobFinishEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactoryBase_createJobPriorityModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactoryBase_createJobUsageEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactoryBase_createQmasterGoesDownEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactoryBase_createQueueInstanceSuspendEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactoryBase_createQueueInstanceUnsuspendEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactoryBase_createSchedulerRunEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactoryBase_createShutdownEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== JobEvent ====================== */
   jclass JobEvent_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t JobEvent_init(JNIEnv *env, jobject*obj , jlong p0 , jint p1 , lList **alpp);
   jgdi_result_t JobEvent_getJobId(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t JobEvent_getPeTaskId(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobEvent_getTaskId(JNIEnv *env, jobject obj , jint* result, lList **alpp);
   jgdi_result_t JobEvent_setJobId(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t JobEvent_setPeTaskId(JNIEnv *env, jobject obj , const char* p0 , lList **alpp);
   jgdi_result_t JobEvent_setTaskId(JNIEnv *env, jobject obj , jint p0 , lList **alpp);
   jgdi_result_t JobEvent_toString(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== JobUsageEvent ====================== */
   jclass JobUsageEvent_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t JobUsageEvent_init(JNIEnv *env, jobject*obj , jlong p0 , jint p1 , lList **alpp);
   jgdi_result_t JobUsageEvent_addUsage(JNIEnv *env, jobject obj , const char* p0 , jdouble p1 , lList **alpp);
   jgdi_result_t JobUsageEvent_getLoadValue(JNIEnv *env, jobject obj , const char* p0 , jobject* result, lList **alpp);
   jgdi_result_t JobUsageEvent_getLoadValueNames(JNIEnv *env, jobject obj , jobject* result, lList **alpp);
   jgdi_result_t JobUsageEvent_getUsage(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== EventTypeMapping ====================== */
   jclass EventTypeMapping_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventTypeMapping_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t EventTypeMapping_static_getEventEnumType(JNIEnv *env, jint p0 , jobject* result, lList **alpp);
   jgdi_result_t EventTypeMapping_static_getNativeEventType(JNIEnv *env, jobject p0 , jint* result, lList **alpp);
   jgdi_result_t EventTypeMapping_static_registerEvent(JNIEnv *env, const char* p0 , jint p1 , lList **alpp);
   jgdi_result_t EventTypeMapping_getEventEnumType(JNIEnv *env, jobject obj , jint p0 , jobject* result, lList **alpp);
   jgdi_result_t EventTypeMapping_getNativeEventType(JNIEnv *env, jobject obj , jobject p0 , jint* result, lList **alpp);
   jgdi_result_t EventTypeMapping_registerEvent(JNIEnv *env, jobject obj , const char* p0 , jint p1 , lList **alpp);



/* ==== ListEvent ====================== */
   jclass ListEvent_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t ListEvent_init(JNIEnv *env, jobject*obj , jlong p0 , jint p1 , jobject p2 , lList **alpp);
   jgdi_result_t ListEvent_add(JNIEnv *env, jobject obj , jobject p0 , lList **alpp);
   jgdi_result_t ListEvent_getObjects(JNIEnv *env, jobject obj , jobject* result, lList **alpp);



/* ==== EventFactory_InternalEventFactory ====================== */
   jclass EventFactory_InternalEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_InternalEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_InternalEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_InternalEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_InternalEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_HostgroupEventFactory ====================== */
   jclass EventFactory_HostgroupEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_HostgroupEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_HostgroupEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_HostgroupEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_HostgroupEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_SubmitHostEventFactory ====================== */
   jclass EventFactory_SubmitHostEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_SubmitHostEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_SubmitHostEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_SubmitHostEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_SubmitHostEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_ExecHostEventFactory ====================== */
   jclass EventFactory_ExecHostEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_ExecHostEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ExecHostEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ExecHostEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ExecHostEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_ManagerEventFactory ====================== */
   jclass EventFactory_ManagerEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_ManagerEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ManagerEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ManagerEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ManagerEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_AdvanceReservationEventFactory ====================== */
   jclass EventFactory_AdvanceReservationEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_AdvanceReservationEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_AdvanceReservationEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_AdvanceReservationEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_AdvanceReservationEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_PETaskEventFactory ====================== */
   jclass EventFactory_PETaskEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_PETaskEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_PETaskEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_PETaskEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_PETaskEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_SchedConfEventFactory ====================== */
   jclass EventFactory_SchedConfEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_SchedConfEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_SchedConfEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_SchedConfEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_SchedConfEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_ConfigurationEventFactory ====================== */
   jclass EventFactory_ConfigurationEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_ConfigurationEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ConfigurationEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ConfigurationEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ConfigurationEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_ProjectEventFactory ====================== */
   jclass EventFactory_ProjectEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_ProjectEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ProjectEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ProjectEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ProjectEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_ShareTreeEventFactory ====================== */
   jclass EventFactory_ShareTreeEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_ShareTreeEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ShareTreeEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ShareTreeEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ShareTreeEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_AdminHostEventFactory ====================== */
   jclass EventFactory_AdminHostEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_AdminHostEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_AdminHostEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_AdminHostEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_AdminHostEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_CheckpointEventFactory ====================== */
   jclass EventFactory_CheckpointEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_CheckpointEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_CheckpointEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_CheckpointEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_CheckpointEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_ParallelEnvironmentEventFactory ====================== */
   jclass EventFactory_ParallelEnvironmentEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_ParallelEnvironmentEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ParallelEnvironmentEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ParallelEnvironmentEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ParallelEnvironmentEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_JobTaskEventFactory ====================== */
   jclass EventFactory_JobTaskEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_JobTaskEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_JobTaskEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_JobTaskEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_JobTaskEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_ComplexEntryEventFactory ====================== */
   jclass EventFactory_ComplexEntryEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_ComplexEntryEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ComplexEntryEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ComplexEntryEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ComplexEntryEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_ClusterQueueEventFactory ====================== */
   jclass EventFactory_ClusterQueueEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_ClusterQueueEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ClusterQueueEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ClusterQueueEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ClusterQueueEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_JobSchedulingInfoEventFactory ====================== */
   jclass EventFactory_JobSchedulingInfoEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_JobSchedulingInfoEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_JobSchedulingInfoEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_JobSchedulingInfoEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_JobSchedulingInfoEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_UserSetEventFactory ====================== */
   jclass EventFactory_UserSetEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_UserSetEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_UserSetEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_UserSetEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_UserSetEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_CalendarEventFactory ====================== */
   jclass EventFactory_CalendarEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_CalendarEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_CalendarEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_CalendarEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_CalendarEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_OperatorEventFactory ====================== */
   jclass EventFactory_OperatorEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_OperatorEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_OperatorEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_OperatorEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_OperatorEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_JobEventFactory ====================== */
   jclass EventFactory_JobEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_JobEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_JobEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_JobEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_JobEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_QueueInstanceEventFactory ====================== */
   jclass EventFactory_QueueInstanceEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_QueueInstanceEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_QueueInstanceEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_QueueInstanceEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_QueueInstanceEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_ResourceQuotaSetEventFactory ====================== */
   jclass EventFactory_ResourceQuotaSetEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_ResourceQuotaSetEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ResourceQuotaSetEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ResourceQuotaSetEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_ResourceQuotaSetEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory_UserEventFactory ====================== */
   jclass EventFactory_UserEventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_UserEventFactory_createAddEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_UserEventFactory_createDelEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_UserEventFactory_createListEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_UserEventFactory_createModEvent(JNIEnv *env, jobject obj , jlong p0 , jint p1 , jobject* result, lList **alpp);



/* ==== EventFactory ====================== */
   jclass EventFactory_find_class(JNIEnv *env, lList** alpp);
   jgdi_result_t EventFactory_init(JNIEnv *env, jobject*obj , lList **alpp);
   jgdi_result_t EventFactory_static_createAddEvent(JNIEnv *env, const char* p0 , jlong p1 , jint p2 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_static_createDelEvent(JNIEnv *env, const char* p0 , jlong p1 , jint p2 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_static_createListEvent(JNIEnv *env, const char* p0 , jlong p1 , jint p2 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_static_createModEvent(JNIEnv *env, const char* p0 , jlong p1 , jint p2 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_createAddEvent(JNIEnv *env, jobject obj , const char* p0 , jlong p1 , jint p2 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_createDelEvent(JNIEnv *env, jobject obj , const char* p0 , jlong p1 , jint p2 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_createListEvent(JNIEnv *env, jobject obj , const char* p0 , jlong p1 , jint p2 , jobject* result, lList **alpp);
   jgdi_result_t EventFactory_createModEvent(JNIEnv *env, jobject obj , const char* p0 , jlong p1 , jint p2 , jobject* result, lList **alpp);

#endif /* JGDI_WRAPPER_EVENT_H */
