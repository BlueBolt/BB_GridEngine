



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
#include <jni.h>
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
#include "jgdi.h"

#define JGDI_DEBUG

           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    getHostgroupList
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_fillHostgroupListWithAnswer(JNIEnv *env, jobject jgdi, jobject list, jobject filter, jobject answers) {
   
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_fillHostgroupList");
   jgdi_fill(env, jgdi, list, filter, "com/sun/grid/jgdi/configuration/Hostgroup", SGE_HGRP_LIST, HGRP_Type, answers);
   DEXIT;
}

/* -------------- ADD ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addHostgroup
 * Signature: (Lcom/sun/grid/jgdi/configuration/Hostgroup;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addHostgroup(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addHostgroup");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Hostgroup", SGE_HGRP_LIST, HGRP_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addHostgroupWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Hostgroup;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addHostgroupWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addHostgroupWithAnswer");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Hostgroup", SGE_HGRP_LIST, HGRP_Type, answers);
   DEXIT;
}

/* -------------- Delete ------------------------------------------------------- */           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteHostgroup
 * Signature: (Lcom/sun/grid/jgdi/configuration/Hostgroup;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteHostgroup(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteHostgroup");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Hostgroup", SGE_HGRP_LIST, HGRP_Type, false, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteHostgroupWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Hostgroup;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteHostgroupWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteHostgroupWithAnswer");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Hostgroup", SGE_HGRP_LIST, HGRP_Type, false, answers);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteHostgroupsWithAnswer
 * Signature: ([Ljava/lang/Object;ZL/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteHostgroupsWithAnswer(JNIEnv *env, jobject jgdi, jobjectArray jobj_array, jboolean forced, jobject options, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteHostgroupsWithAnswer");
   jgdi_delete_array(env, jgdi, jobj_array, "com/sun/grid/jgdi/configuration/Hostgroup", SGE_HGRP_LIST, HGRP_Type, forced, options, answers);
   DEXIT;
}


/* -------------- Update ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateHostgroup
 * Signature: (Lcom/sun/grid/jgdi/configuration/Hostgroup;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateHostgroup(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateHostgroup");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Hostgroup", SGE_HGRP_LIST, HGRP_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateHostgroupWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Hostgroup;, L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateHostgroupWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateHostgroup");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Hostgroup", SGE_HGRP_LIST, HGRP_Type, answers);
   DEXIT;
}







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
#include <jni.h>
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
#include "jgdi.h"

#define JGDI_DEBUG

           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    getSubmitHostList
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_fillSubmitHostListWithAnswer(JNIEnv *env, jobject jgdi, jobject list, jobject filter, jobject answers) {
   
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_fillSubmitHostList");
   jgdi_fill(env, jgdi, list, filter, "com/sun/grid/jgdi/configuration/SubmitHost", SGE_SH_LIST, SH_Type, answers);
   DEXIT;
}

/* -------------- ADD ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addSubmitHost
 * Signature: (Lcom/sun/grid/jgdi/configuration/SubmitHost;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addSubmitHost(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addSubmitHost");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/SubmitHost", SGE_SH_LIST, SH_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addSubmitHostWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/SubmitHost;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addSubmitHostWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addSubmitHostWithAnswer");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/SubmitHost", SGE_SH_LIST, SH_Type, answers);
   DEXIT;
}

/* -------------- Delete ------------------------------------------------------- */           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteSubmitHost
 * Signature: (Lcom/sun/grid/jgdi/configuration/SubmitHost;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteSubmitHost(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteSubmitHost");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/SubmitHost", SGE_SH_LIST, SH_Type, false, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteSubmitHostWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/SubmitHost;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteSubmitHostWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteSubmitHostWithAnswer");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/SubmitHost", SGE_SH_LIST, SH_Type, false, answers);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteSubmitHostsWithAnswer
 * Signature: ([Ljava/lang/Object;ZL/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteSubmitHostsWithAnswer(JNIEnv *env, jobject jgdi, jobjectArray jobj_array, jboolean forced, jobject options, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteSubmitHostsWithAnswer");
   jgdi_delete_array(env, jgdi, jobj_array, "com/sun/grid/jgdi/configuration/SubmitHost", SGE_SH_LIST, SH_Type, forced, options, answers);
   DEXIT;
}


/* -------------- Update ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateSubmitHost
 * Signature: (Lcom/sun/grid/jgdi/configuration/SubmitHost;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateSubmitHost(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateSubmitHost");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/SubmitHost", SGE_SH_LIST, SH_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateSubmitHostWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/SubmitHost;, L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateSubmitHostWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateSubmitHost");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/SubmitHost", SGE_SH_LIST, SH_Type, answers);
   DEXIT;
}







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
#include <jni.h>
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
#include "jgdi.h"

#define JGDI_DEBUG

           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    getExecHostList
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_fillExecHostListWithAnswer(JNIEnv *env, jobject jgdi, jobject list, jobject filter, jobject answers) {
   
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_fillExecHostList");
   jgdi_fill(env, jgdi, list, filter, "com/sun/grid/jgdi/configuration/ExecHost", SGE_EH_LIST, EH_Type, answers);
   DEXIT;
}

/* -------------- ADD ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addExecHost
 * Signature: (Lcom/sun/grid/jgdi/configuration/ExecHost;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addExecHost(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addExecHost");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ExecHost", SGE_EH_LIST, EH_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addExecHostWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/ExecHost;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addExecHostWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addExecHostWithAnswer");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ExecHost", SGE_EH_LIST, EH_Type, answers);
   DEXIT;
}

/* -------------- Delete ------------------------------------------------------- */           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteExecHost
 * Signature: (Lcom/sun/grid/jgdi/configuration/ExecHost;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteExecHost(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteExecHost");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ExecHost", SGE_EH_LIST, EH_Type, false, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteExecHostWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/ExecHost;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteExecHostWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteExecHostWithAnswer");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ExecHost", SGE_EH_LIST, EH_Type, false, answers);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteExecHostsWithAnswer
 * Signature: ([Ljava/lang/Object;ZL/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteExecHostsWithAnswer(JNIEnv *env, jobject jgdi, jobjectArray jobj_array, jboolean forced, jobject options, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteExecHostsWithAnswer");
   jgdi_delete_array(env, jgdi, jobj_array, "com/sun/grid/jgdi/configuration/ExecHost", SGE_EH_LIST, EH_Type, forced, options, answers);
   DEXIT;
}


/* -------------- Update ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateExecHost
 * Signature: (Lcom/sun/grid/jgdi/configuration/ExecHost;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateExecHost(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateExecHost");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ExecHost", SGE_EH_LIST, EH_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateExecHostWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/ExecHost;, L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateExecHostWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateExecHost");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ExecHost", SGE_EH_LIST, EH_Type, answers);
   DEXIT;
}







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
#include <jni.h>
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
#include "jgdi.h"

#define JGDI_DEBUG

           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    getManagerList
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_fillManagerListWithAnswer(JNIEnv *env, jobject jgdi, jobject list, jobject filter, jobject answers) {
   
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_fillManagerList");
   jgdi_fill(env, jgdi, list, filter, "com/sun/grid/jgdi/configuration/Manager", SGE_UM_LIST, UM_Type, answers);
   DEXIT;
}

/* -------------- ADD ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addManager
 * Signature: (Lcom/sun/grid/jgdi/configuration/Manager;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addManager(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addManager");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Manager", SGE_UM_LIST, UM_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addManagerWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Manager;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addManagerWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addManagerWithAnswer");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Manager", SGE_UM_LIST, UM_Type, answers);
   DEXIT;
}

/* -------------- Delete ------------------------------------------------------- */           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteManager
 * Signature: (Lcom/sun/grid/jgdi/configuration/Manager;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteManager(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteManager");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Manager", SGE_UM_LIST, UM_Type, false, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteManagerWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Manager;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteManagerWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteManagerWithAnswer");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Manager", SGE_UM_LIST, UM_Type, false, answers);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteManagersWithAnswer
 * Signature: ([Ljava/lang/Object;ZL/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteManagersWithAnswer(JNIEnv *env, jobject jgdi, jobjectArray jobj_array, jboolean forced, jobject options, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteManagersWithAnswer");
   jgdi_delete_array(env, jgdi, jobj_array, "com/sun/grid/jgdi/configuration/Manager", SGE_UM_LIST, UM_Type, forced, options, answers);
   DEXIT;
}


/* -------------- Update ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateManager
 * Signature: (Lcom/sun/grid/jgdi/configuration/Manager;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateManager(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateManager");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Manager", SGE_UM_LIST, UM_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateManagerWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Manager;, L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateManagerWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateManager");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Manager", SGE_UM_LIST, UM_Type, answers);
   DEXIT;
}







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
#include <jni.h>
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
#include "jgdi.h"

#define JGDI_DEBUG

           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    getAdvanceReservationList
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_fillAdvanceReservationListWithAnswer(JNIEnv *env, jobject jgdi, jobject list, jobject filter, jobject answers) {
   
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_fillAdvanceReservationList");
   jgdi_fill(env, jgdi, list, filter, "com/sun/grid/jgdi/configuration/AdvanceReservation", SGE_AR_LIST, AR_Type, answers);
   DEXIT;
}

/* -------------- ADD ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addAdvanceReservation
 * Signature: (Lcom/sun/grid/jgdi/configuration/AdvanceReservation;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addAdvanceReservation(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addAdvanceReservation");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/AdvanceReservation", SGE_AR_LIST, AR_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addAdvanceReservationWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/AdvanceReservation;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addAdvanceReservationWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addAdvanceReservationWithAnswer");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/AdvanceReservation", SGE_AR_LIST, AR_Type, answers);
   DEXIT;
}

/* -------------- Delete ------------------------------------------------------- */           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteAdvanceReservation
 * Signature: (Lcom/sun/grid/jgdi/configuration/AdvanceReservation;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteAdvanceReservation(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteAdvanceReservation");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/AdvanceReservation", SGE_AR_LIST, AR_Type, false, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteAdvanceReservationWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/AdvanceReservation;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteAdvanceReservationWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteAdvanceReservationWithAnswer");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/AdvanceReservation", SGE_AR_LIST, AR_Type, false, answers);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteAdvanceReservationsWithAnswer
 * Signature: ([Ljava/lang/Object;ZL/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteAdvanceReservationsWithAnswer(JNIEnv *env, jobject jgdi, jobjectArray jobj_array, jboolean forced, jobject options, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteAdvanceReservationsWithAnswer");
   jgdi_delete_array(env, jgdi, jobj_array, "com/sun/grid/jgdi/configuration/AdvanceReservation", SGE_AR_LIST, AR_Type, forced, options, answers);
   DEXIT;
}


/* -------------- Update ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateAdvanceReservation
 * Signature: (Lcom/sun/grid/jgdi/configuration/AdvanceReservation;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateAdvanceReservation(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateAdvanceReservation");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/AdvanceReservation", SGE_AR_LIST, AR_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateAdvanceReservationWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/AdvanceReservation;, L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateAdvanceReservationWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateAdvanceReservation");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/AdvanceReservation", SGE_AR_LIST, AR_Type, answers);
   DEXIT;
}







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
#include <jni.h>
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
#include "jgdi.h"

#define JGDI_DEBUG

           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    getSchedConfList
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_fillSchedConfListWithAnswer(JNIEnv *env, jobject jgdi, jobject list, jobject filter, jobject answers) {
   
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_fillSchedConfList");
   jgdi_fill(env, jgdi, list, filter, "com/sun/grid/jgdi/configuration/SchedConf", SGE_SC_LIST, SC_Type, answers);
   DEXIT;
}

/* -------------- ADD ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addSchedConf
 * Signature: (Lcom/sun/grid/jgdi/configuration/SchedConf;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addSchedConf(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addSchedConf");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/SchedConf", SGE_SC_LIST, SC_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addSchedConfWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/SchedConf;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addSchedConfWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addSchedConfWithAnswer");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/SchedConf", SGE_SC_LIST, SC_Type, answers);
   DEXIT;
}

/* -------------- Delete ------------------------------------------------------- */           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteSchedConf
 * Signature: (Lcom/sun/grid/jgdi/configuration/SchedConf;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteSchedConf(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteSchedConf");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/SchedConf", SGE_SC_LIST, SC_Type, false, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteSchedConfWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/SchedConf;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteSchedConfWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteSchedConfWithAnswer");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/SchedConf", SGE_SC_LIST, SC_Type, false, answers);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteSchedConfsWithAnswer
 * Signature: ([Ljava/lang/Object;ZL/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteSchedConfsWithAnswer(JNIEnv *env, jobject jgdi, jobjectArray jobj_array, jboolean forced, jobject options, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteSchedConfsWithAnswer");
   jgdi_delete_array(env, jgdi, jobj_array, "com/sun/grid/jgdi/configuration/SchedConf", SGE_SC_LIST, SC_Type, forced, options, answers);
   DEXIT;
}


/* -------------- Update ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateSchedConf
 * Signature: (Lcom/sun/grid/jgdi/configuration/SchedConf;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateSchedConf(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateSchedConf");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/SchedConf", SGE_SC_LIST, SC_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateSchedConfWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/SchedConf;, L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateSchedConfWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateSchedConf");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/SchedConf", SGE_SC_LIST, SC_Type, answers);
   DEXIT;
}







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
#include <jni.h>
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
#include "jgdi.h"

#define JGDI_DEBUG

           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    getConfigurationList
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_fillConfigurationListWithAnswer(JNIEnv *env, jobject jgdi, jobject list, jobject filter, jobject answers) {
   
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_fillConfigurationList");
   jgdi_fill(env, jgdi, list, filter, "com/sun/grid/jgdi/configuration/Configuration", SGE_CONF_LIST, CONF_Type, answers);
   DEXIT;
}

/* -------------- ADD ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addConfiguration
 * Signature: (Lcom/sun/grid/jgdi/configuration/Configuration;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addConfiguration(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addConfiguration");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Configuration", SGE_CONF_LIST, CONF_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addConfigurationWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Configuration;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addConfigurationWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addConfigurationWithAnswer");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Configuration", SGE_CONF_LIST, CONF_Type, answers);
   DEXIT;
}

/* -------------- Delete ------------------------------------------------------- */           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteConfiguration
 * Signature: (Lcom/sun/grid/jgdi/configuration/Configuration;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteConfiguration(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteConfiguration");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Configuration", SGE_CONF_LIST, CONF_Type, false, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteConfigurationWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Configuration;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteConfigurationWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteConfigurationWithAnswer");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Configuration", SGE_CONF_LIST, CONF_Type, false, answers);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteConfigurationsWithAnswer
 * Signature: ([Ljava/lang/Object;ZL/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteConfigurationsWithAnswer(JNIEnv *env, jobject jgdi, jobjectArray jobj_array, jboolean forced, jobject options, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteConfigurationsWithAnswer");
   jgdi_delete_array(env, jgdi, jobj_array, "com/sun/grid/jgdi/configuration/Configuration", SGE_CONF_LIST, CONF_Type, forced, options, answers);
   DEXIT;
}


/* -------------- Update ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateConfiguration
 * Signature: (Lcom/sun/grid/jgdi/configuration/Configuration;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateConfiguration(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateConfiguration");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Configuration", SGE_CONF_LIST, CONF_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateConfigurationWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Configuration;, L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateConfigurationWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateConfiguration");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Configuration", SGE_CONF_LIST, CONF_Type, answers);
   DEXIT;
}







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
#include <jni.h>
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
#include "jgdi.h"

#define JGDI_DEBUG

           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    getProjectList
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_fillProjectListWithAnswer(JNIEnv *env, jobject jgdi, jobject list, jobject filter, jobject answers) {
   
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_fillProjectList");
   jgdi_fill(env, jgdi, list, filter, "com/sun/grid/jgdi/configuration/Project", SGE_PR_LIST, PR_Type, answers);
   DEXIT;
}

/* -------------- ADD ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addProject
 * Signature: (Lcom/sun/grid/jgdi/configuration/Project;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addProject(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addProject");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Project", SGE_PR_LIST, PR_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addProjectWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Project;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addProjectWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addProjectWithAnswer");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Project", SGE_PR_LIST, PR_Type, answers);
   DEXIT;
}

/* -------------- Delete ------------------------------------------------------- */           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteProject
 * Signature: (Lcom/sun/grid/jgdi/configuration/Project;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteProject(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteProject");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Project", SGE_PR_LIST, PR_Type, false, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteProjectWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Project;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteProjectWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteProjectWithAnswer");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Project", SGE_PR_LIST, PR_Type, false, answers);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteProjectsWithAnswer
 * Signature: ([Ljava/lang/Object;ZL/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteProjectsWithAnswer(JNIEnv *env, jobject jgdi, jobjectArray jobj_array, jboolean forced, jobject options, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteProjectsWithAnswer");
   jgdi_delete_array(env, jgdi, jobj_array, "com/sun/grid/jgdi/configuration/Project", SGE_PR_LIST, PR_Type, forced, options, answers);
   DEXIT;
}


/* -------------- Update ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateProject
 * Signature: (Lcom/sun/grid/jgdi/configuration/Project;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateProject(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateProject");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Project", SGE_PR_LIST, PR_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateProjectWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Project;, L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateProjectWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateProject");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Project", SGE_PR_LIST, PR_Type, answers);
   DEXIT;
}







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
#include <jni.h>
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
#include "jgdi.h"

#define JGDI_DEBUG

           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    getShareTreeList
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_fillShareTreeListWithAnswer(JNIEnv *env, jobject jgdi, jobject list, jobject filter, jobject answers) {
   
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_fillShareTreeList");
   jgdi_fill(env, jgdi, list, filter, "com/sun/grid/jgdi/configuration/ShareTree", SGE_STN_LIST, STN_Type, answers);
   DEXIT;
}

/* -------------- ADD ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addShareTree
 * Signature: (Lcom/sun/grid/jgdi/configuration/ShareTree;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addShareTree(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addShareTree");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ShareTree", SGE_STN_LIST, STN_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addShareTreeWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/ShareTree;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addShareTreeWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addShareTreeWithAnswer");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ShareTree", SGE_STN_LIST, STN_Type, answers);
   DEXIT;
}

/* -------------- Delete ------------------------------------------------------- */           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteShareTree
 * Signature: (Lcom/sun/grid/jgdi/configuration/ShareTree;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteShareTree(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteShareTree");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ShareTree", SGE_STN_LIST, STN_Type, false, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteShareTreeWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/ShareTree;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteShareTreeWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteShareTreeWithAnswer");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ShareTree", SGE_STN_LIST, STN_Type, false, answers);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteShareTreesWithAnswer
 * Signature: ([Ljava/lang/Object;ZL/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteShareTreesWithAnswer(JNIEnv *env, jobject jgdi, jobjectArray jobj_array, jboolean forced, jobject options, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteShareTreesWithAnswer");
   jgdi_delete_array(env, jgdi, jobj_array, "com/sun/grid/jgdi/configuration/ShareTree", SGE_STN_LIST, STN_Type, forced, options, answers);
   DEXIT;
}


/* -------------- Update ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateShareTree
 * Signature: (Lcom/sun/grid/jgdi/configuration/ShareTree;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateShareTree(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateShareTree");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ShareTree", SGE_STN_LIST, STN_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateShareTreeWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/ShareTree;, L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateShareTreeWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateShareTree");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ShareTree", SGE_STN_LIST, STN_Type, answers);
   DEXIT;
}







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
#include <jni.h>
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
#include "jgdi.h"

#define JGDI_DEBUG

           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    getAdminHostList
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_fillAdminHostListWithAnswer(JNIEnv *env, jobject jgdi, jobject list, jobject filter, jobject answers) {
   
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_fillAdminHostList");
   jgdi_fill(env, jgdi, list, filter, "com/sun/grid/jgdi/configuration/AdminHost", SGE_AH_LIST, AH_Type, answers);
   DEXIT;
}

/* -------------- ADD ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addAdminHost
 * Signature: (Lcom/sun/grid/jgdi/configuration/AdminHost;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addAdminHost(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addAdminHost");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/AdminHost", SGE_AH_LIST, AH_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addAdminHostWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/AdminHost;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addAdminHostWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addAdminHostWithAnswer");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/AdminHost", SGE_AH_LIST, AH_Type, answers);
   DEXIT;
}

/* -------------- Delete ------------------------------------------------------- */           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteAdminHost
 * Signature: (Lcom/sun/grid/jgdi/configuration/AdminHost;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteAdminHost(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteAdminHost");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/AdminHost", SGE_AH_LIST, AH_Type, false, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteAdminHostWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/AdminHost;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteAdminHostWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteAdminHostWithAnswer");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/AdminHost", SGE_AH_LIST, AH_Type, false, answers);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteAdminHostsWithAnswer
 * Signature: ([Ljava/lang/Object;ZL/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteAdminHostsWithAnswer(JNIEnv *env, jobject jgdi, jobjectArray jobj_array, jboolean forced, jobject options, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteAdminHostsWithAnswer");
   jgdi_delete_array(env, jgdi, jobj_array, "com/sun/grid/jgdi/configuration/AdminHost", SGE_AH_LIST, AH_Type, forced, options, answers);
   DEXIT;
}


/* -------------- Update ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateAdminHost
 * Signature: (Lcom/sun/grid/jgdi/configuration/AdminHost;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateAdminHost(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateAdminHost");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/AdminHost", SGE_AH_LIST, AH_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateAdminHostWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/AdminHost;, L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateAdminHostWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateAdminHost");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/AdminHost", SGE_AH_LIST, AH_Type, answers);
   DEXIT;
}







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
#include <jni.h>
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
#include "jgdi.h"

#define JGDI_DEBUG

           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    getCheckpointList
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_fillCheckpointListWithAnswer(JNIEnv *env, jobject jgdi, jobject list, jobject filter, jobject answers) {
   
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_fillCheckpointList");
   jgdi_fill(env, jgdi, list, filter, "com/sun/grid/jgdi/configuration/Checkpoint", SGE_CK_LIST, CK_Type, answers);
   DEXIT;
}

/* -------------- ADD ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addCheckpoint
 * Signature: (Lcom/sun/grid/jgdi/configuration/Checkpoint;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addCheckpoint(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addCheckpoint");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Checkpoint", SGE_CK_LIST, CK_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addCheckpointWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Checkpoint;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addCheckpointWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addCheckpointWithAnswer");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Checkpoint", SGE_CK_LIST, CK_Type, answers);
   DEXIT;
}

/* -------------- Delete ------------------------------------------------------- */           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteCheckpoint
 * Signature: (Lcom/sun/grid/jgdi/configuration/Checkpoint;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteCheckpoint(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteCheckpoint");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Checkpoint", SGE_CK_LIST, CK_Type, false, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteCheckpointWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Checkpoint;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteCheckpointWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteCheckpointWithAnswer");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Checkpoint", SGE_CK_LIST, CK_Type, false, answers);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteCheckpointsWithAnswer
 * Signature: ([Ljava/lang/Object;ZL/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteCheckpointsWithAnswer(JNIEnv *env, jobject jgdi, jobjectArray jobj_array, jboolean forced, jobject options, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteCheckpointsWithAnswer");
   jgdi_delete_array(env, jgdi, jobj_array, "com/sun/grid/jgdi/configuration/Checkpoint", SGE_CK_LIST, CK_Type, forced, options, answers);
   DEXIT;
}


/* -------------- Update ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateCheckpoint
 * Signature: (Lcom/sun/grid/jgdi/configuration/Checkpoint;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateCheckpoint(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateCheckpoint");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Checkpoint", SGE_CK_LIST, CK_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateCheckpointWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Checkpoint;, L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateCheckpointWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateCheckpoint");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Checkpoint", SGE_CK_LIST, CK_Type, answers);
   DEXIT;
}







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
#include <jni.h>
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
#include "jgdi.h"

#define JGDI_DEBUG

           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    getParallelEnvironmentList
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_fillParallelEnvironmentListWithAnswer(JNIEnv *env, jobject jgdi, jobject list, jobject filter, jobject answers) {
   
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_fillParallelEnvironmentList");
   jgdi_fill(env, jgdi, list, filter, "com/sun/grid/jgdi/configuration/ParallelEnvironment", SGE_PE_LIST, PE_Type, answers);
   DEXIT;
}

/* -------------- ADD ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addParallelEnvironment
 * Signature: (Lcom/sun/grid/jgdi/configuration/ParallelEnvironment;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addParallelEnvironment(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addParallelEnvironment");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ParallelEnvironment", SGE_PE_LIST, PE_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addParallelEnvironmentWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/ParallelEnvironment;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addParallelEnvironmentWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addParallelEnvironmentWithAnswer");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ParallelEnvironment", SGE_PE_LIST, PE_Type, answers);
   DEXIT;
}

/* -------------- Delete ------------------------------------------------------- */           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteParallelEnvironment
 * Signature: (Lcom/sun/grid/jgdi/configuration/ParallelEnvironment;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteParallelEnvironment(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteParallelEnvironment");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ParallelEnvironment", SGE_PE_LIST, PE_Type, false, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteParallelEnvironmentWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/ParallelEnvironment;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteParallelEnvironmentWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteParallelEnvironmentWithAnswer");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ParallelEnvironment", SGE_PE_LIST, PE_Type, false, answers);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteParallelEnvironmentsWithAnswer
 * Signature: ([Ljava/lang/Object;ZL/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteParallelEnvironmentsWithAnswer(JNIEnv *env, jobject jgdi, jobjectArray jobj_array, jboolean forced, jobject options, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteParallelEnvironmentsWithAnswer");
   jgdi_delete_array(env, jgdi, jobj_array, "com/sun/grid/jgdi/configuration/ParallelEnvironment", SGE_PE_LIST, PE_Type, forced, options, answers);
   DEXIT;
}


/* -------------- Update ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateParallelEnvironment
 * Signature: (Lcom/sun/grid/jgdi/configuration/ParallelEnvironment;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateParallelEnvironment(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateParallelEnvironment");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ParallelEnvironment", SGE_PE_LIST, PE_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateParallelEnvironmentWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/ParallelEnvironment;, L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateParallelEnvironmentWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateParallelEnvironment");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ParallelEnvironment", SGE_PE_LIST, PE_Type, answers);
   DEXIT;
}







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
#include <jni.h>
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
#include "jgdi.h"

#define JGDI_DEBUG

           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    getComplexEntryList
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_fillComplexEntryListWithAnswer(JNIEnv *env, jobject jgdi, jobject list, jobject filter, jobject answers) {
   
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_fillComplexEntryList");
   jgdi_fill(env, jgdi, list, filter, "com/sun/grid/jgdi/configuration/ComplexEntry", SGE_CE_LIST, CE_Type, answers);
   DEXIT;
}

/* -------------- ADD ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addComplexEntry
 * Signature: (Lcom/sun/grid/jgdi/configuration/ComplexEntry;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addComplexEntry(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addComplexEntry");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ComplexEntry", SGE_CE_LIST, CE_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addComplexEntryWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/ComplexEntry;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addComplexEntryWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addComplexEntryWithAnswer");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ComplexEntry", SGE_CE_LIST, CE_Type, answers);
   DEXIT;
}

/* -------------- Delete ------------------------------------------------------- */           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteComplexEntry
 * Signature: (Lcom/sun/grid/jgdi/configuration/ComplexEntry;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteComplexEntry(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteComplexEntry");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ComplexEntry", SGE_CE_LIST, CE_Type, false, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteComplexEntryWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/ComplexEntry;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteComplexEntryWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteComplexEntryWithAnswer");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ComplexEntry", SGE_CE_LIST, CE_Type, false, answers);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteComplexEntrysWithAnswer
 * Signature: ([Ljava/lang/Object;ZL/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteComplexEntrysWithAnswer(JNIEnv *env, jobject jgdi, jobjectArray jobj_array, jboolean forced, jobject options, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteComplexEntrysWithAnswer");
   jgdi_delete_array(env, jgdi, jobj_array, "com/sun/grid/jgdi/configuration/ComplexEntry", SGE_CE_LIST, CE_Type, forced, options, answers);
   DEXIT;
}


/* -------------- Update ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateComplexEntry
 * Signature: (Lcom/sun/grid/jgdi/configuration/ComplexEntry;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateComplexEntry(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateComplexEntry");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ComplexEntry", SGE_CE_LIST, CE_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateComplexEntryWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/ComplexEntry;, L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateComplexEntryWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateComplexEntry");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ComplexEntry", SGE_CE_LIST, CE_Type, answers);
   DEXIT;
}







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
#include <jni.h>
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
#include "jgdi.h"

#define JGDI_DEBUG

           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    getClusterQueueList
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_fillClusterQueueListWithAnswer(JNIEnv *env, jobject jgdi, jobject list, jobject filter, jobject answers) {
   
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_fillClusterQueueList");
   jgdi_fill(env, jgdi, list, filter, "com/sun/grid/jgdi/configuration/ClusterQueue", SGE_CQ_LIST, CQ_Type, answers);
   DEXIT;
}

/* -------------- ADD ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addClusterQueue
 * Signature: (Lcom/sun/grid/jgdi/configuration/ClusterQueue;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addClusterQueue(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addClusterQueue");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ClusterQueue", SGE_CQ_LIST, CQ_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addClusterQueueWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/ClusterQueue;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addClusterQueueWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addClusterQueueWithAnswer");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ClusterQueue", SGE_CQ_LIST, CQ_Type, answers);
   DEXIT;
}

/* -------------- Delete ------------------------------------------------------- */           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteClusterQueue
 * Signature: (Lcom/sun/grid/jgdi/configuration/ClusterQueue;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteClusterQueue(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteClusterQueue");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ClusterQueue", SGE_CQ_LIST, CQ_Type, false, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteClusterQueueWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/ClusterQueue;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteClusterQueueWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteClusterQueueWithAnswer");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ClusterQueue", SGE_CQ_LIST, CQ_Type, false, answers);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteClusterQueuesWithAnswer
 * Signature: ([Ljava/lang/Object;ZL/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteClusterQueuesWithAnswer(JNIEnv *env, jobject jgdi, jobjectArray jobj_array, jboolean forced, jobject options, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteClusterQueuesWithAnswer");
   jgdi_delete_array(env, jgdi, jobj_array, "com/sun/grid/jgdi/configuration/ClusterQueue", SGE_CQ_LIST, CQ_Type, forced, options, answers);
   DEXIT;
}


/* -------------- Update ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateClusterQueue
 * Signature: (Lcom/sun/grid/jgdi/configuration/ClusterQueue;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateClusterQueue(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateClusterQueue");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ClusterQueue", SGE_CQ_LIST, CQ_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateClusterQueueWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/ClusterQueue;, L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateClusterQueueWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateClusterQueue");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ClusterQueue", SGE_CQ_LIST, CQ_Type, answers);
   DEXIT;
}







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
#include <jni.h>
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
#include "jgdi.h"

#define JGDI_DEBUG

           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    getJobSchedulingInfoList
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_fillJobSchedulingInfoListWithAnswer(JNIEnv *env, jobject jgdi, jobject list, jobject filter, jobject answers) {
   
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_fillJobSchedulingInfoList");
   jgdi_fill(env, jgdi, list, filter, "com/sun/grid/jgdi/configuration/JobSchedulingInfo", SGE_SME_LIST, SME_Type, answers);
   DEXIT;
}

/* -------------- ADD ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addJobSchedulingInfo
 * Signature: (Lcom/sun/grid/jgdi/configuration/JobSchedulingInfo;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addJobSchedulingInfo(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addJobSchedulingInfo");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/JobSchedulingInfo", SGE_SME_LIST, SME_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addJobSchedulingInfoWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/JobSchedulingInfo;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addJobSchedulingInfoWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addJobSchedulingInfoWithAnswer");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/JobSchedulingInfo", SGE_SME_LIST, SME_Type, answers);
   DEXIT;
}

/* -------------- Delete ------------------------------------------------------- */           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteJobSchedulingInfo
 * Signature: (Lcom/sun/grid/jgdi/configuration/JobSchedulingInfo;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteJobSchedulingInfo(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteJobSchedulingInfo");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/JobSchedulingInfo", SGE_SME_LIST, SME_Type, false, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteJobSchedulingInfoWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/JobSchedulingInfo;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteJobSchedulingInfoWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteJobSchedulingInfoWithAnswer");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/JobSchedulingInfo", SGE_SME_LIST, SME_Type, false, answers);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteJobSchedulingInfosWithAnswer
 * Signature: ([Ljava/lang/Object;ZL/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteJobSchedulingInfosWithAnswer(JNIEnv *env, jobject jgdi, jobjectArray jobj_array, jboolean forced, jobject options, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteJobSchedulingInfosWithAnswer");
   jgdi_delete_array(env, jgdi, jobj_array, "com/sun/grid/jgdi/configuration/JobSchedulingInfo", SGE_SME_LIST, SME_Type, forced, options, answers);
   DEXIT;
}


/* -------------- Update ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateJobSchedulingInfo
 * Signature: (Lcom/sun/grid/jgdi/configuration/JobSchedulingInfo;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateJobSchedulingInfo(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateJobSchedulingInfo");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/JobSchedulingInfo", SGE_SME_LIST, SME_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateJobSchedulingInfoWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/JobSchedulingInfo;, L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateJobSchedulingInfoWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateJobSchedulingInfo");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/JobSchedulingInfo", SGE_SME_LIST, SME_Type, answers);
   DEXIT;
}







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
#include <jni.h>
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
#include "jgdi.h"

#define JGDI_DEBUG

           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    getUserSetList
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_fillUserSetListWithAnswer(JNIEnv *env, jobject jgdi, jobject list, jobject filter, jobject answers) {
   
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_fillUserSetList");
   jgdi_fill(env, jgdi, list, filter, "com/sun/grid/jgdi/configuration/UserSet", SGE_US_LIST, US_Type, answers);
   DEXIT;
}

/* -------------- ADD ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addUserSet
 * Signature: (Lcom/sun/grid/jgdi/configuration/UserSet;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addUserSet(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addUserSet");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/UserSet", SGE_US_LIST, US_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addUserSetWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/UserSet;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addUserSetWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addUserSetWithAnswer");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/UserSet", SGE_US_LIST, US_Type, answers);
   DEXIT;
}

/* -------------- Delete ------------------------------------------------------- */           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteUserSet
 * Signature: (Lcom/sun/grid/jgdi/configuration/UserSet;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteUserSet(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteUserSet");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/UserSet", SGE_US_LIST, US_Type, false, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteUserSetWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/UserSet;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteUserSetWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteUserSetWithAnswer");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/UserSet", SGE_US_LIST, US_Type, false, answers);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteUserSetsWithAnswer
 * Signature: ([Ljava/lang/Object;ZL/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteUserSetsWithAnswer(JNIEnv *env, jobject jgdi, jobjectArray jobj_array, jboolean forced, jobject options, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteUserSetsWithAnswer");
   jgdi_delete_array(env, jgdi, jobj_array, "com/sun/grid/jgdi/configuration/UserSet", SGE_US_LIST, US_Type, forced, options, answers);
   DEXIT;
}


/* -------------- Update ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateUserSet
 * Signature: (Lcom/sun/grid/jgdi/configuration/UserSet;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateUserSet(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateUserSet");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/UserSet", SGE_US_LIST, US_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateUserSetWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/UserSet;, L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateUserSetWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateUserSet");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/UserSet", SGE_US_LIST, US_Type, answers);
   DEXIT;
}







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
#include <jni.h>
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
#include "jgdi.h"

#define JGDI_DEBUG

           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    getCalendarList
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_fillCalendarListWithAnswer(JNIEnv *env, jobject jgdi, jobject list, jobject filter, jobject answers) {
   
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_fillCalendarList");
   jgdi_fill(env, jgdi, list, filter, "com/sun/grid/jgdi/configuration/Calendar", SGE_CAL_LIST, CAL_Type, answers);
   DEXIT;
}

/* -------------- ADD ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addCalendar
 * Signature: (Lcom/sun/grid/jgdi/configuration/Calendar;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addCalendar(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addCalendar");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Calendar", SGE_CAL_LIST, CAL_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addCalendarWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Calendar;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addCalendarWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addCalendarWithAnswer");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Calendar", SGE_CAL_LIST, CAL_Type, answers);
   DEXIT;
}

/* -------------- Delete ------------------------------------------------------- */           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteCalendar
 * Signature: (Lcom/sun/grid/jgdi/configuration/Calendar;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteCalendar(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteCalendar");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Calendar", SGE_CAL_LIST, CAL_Type, false, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteCalendarWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Calendar;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteCalendarWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteCalendarWithAnswer");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Calendar", SGE_CAL_LIST, CAL_Type, false, answers);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteCalendarsWithAnswer
 * Signature: ([Ljava/lang/Object;ZL/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteCalendarsWithAnswer(JNIEnv *env, jobject jgdi, jobjectArray jobj_array, jboolean forced, jobject options, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteCalendarsWithAnswer");
   jgdi_delete_array(env, jgdi, jobj_array, "com/sun/grid/jgdi/configuration/Calendar", SGE_CAL_LIST, CAL_Type, forced, options, answers);
   DEXIT;
}


/* -------------- Update ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateCalendar
 * Signature: (Lcom/sun/grid/jgdi/configuration/Calendar;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateCalendar(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateCalendar");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Calendar", SGE_CAL_LIST, CAL_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateCalendarWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Calendar;, L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateCalendarWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateCalendar");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Calendar", SGE_CAL_LIST, CAL_Type, answers);
   DEXIT;
}







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
#include <jni.h>
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
#include "jgdi.h"

#define JGDI_DEBUG

           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    getOperatorList
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_fillOperatorListWithAnswer(JNIEnv *env, jobject jgdi, jobject list, jobject filter, jobject answers) {
   
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_fillOperatorList");
   jgdi_fill(env, jgdi, list, filter, "com/sun/grid/jgdi/configuration/Operator", SGE_UO_LIST, UO_Type, answers);
   DEXIT;
}

/* -------------- ADD ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addOperator
 * Signature: (Lcom/sun/grid/jgdi/configuration/Operator;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addOperator(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addOperator");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Operator", SGE_UO_LIST, UO_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addOperatorWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Operator;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addOperatorWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addOperatorWithAnswer");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Operator", SGE_UO_LIST, UO_Type, answers);
   DEXIT;
}

/* -------------- Delete ------------------------------------------------------- */           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteOperator
 * Signature: (Lcom/sun/grid/jgdi/configuration/Operator;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteOperator(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteOperator");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Operator", SGE_UO_LIST, UO_Type, false, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteOperatorWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Operator;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteOperatorWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteOperatorWithAnswer");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Operator", SGE_UO_LIST, UO_Type, false, answers);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteOperatorsWithAnswer
 * Signature: ([Ljava/lang/Object;ZL/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteOperatorsWithAnswer(JNIEnv *env, jobject jgdi, jobjectArray jobj_array, jboolean forced, jobject options, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteOperatorsWithAnswer");
   jgdi_delete_array(env, jgdi, jobj_array, "com/sun/grid/jgdi/configuration/Operator", SGE_UO_LIST, UO_Type, forced, options, answers);
   DEXIT;
}


/* -------------- Update ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateOperator
 * Signature: (Lcom/sun/grid/jgdi/configuration/Operator;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateOperator(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateOperator");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Operator", SGE_UO_LIST, UO_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateOperatorWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Operator;, L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateOperatorWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateOperator");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Operator", SGE_UO_LIST, UO_Type, answers);
   DEXIT;
}







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
#include <jni.h>
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
#include "jgdi.h"

#define JGDI_DEBUG

           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    getJobList
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_fillJobListWithAnswer(JNIEnv *env, jobject jgdi, jobject list, jobject filter, jobject answers) {
   
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_fillJobList");
   jgdi_fill(env, jgdi, list, filter, "com/sun/grid/jgdi/configuration/Job", SGE_JB_LIST, JB_Type, answers);
   DEXIT;
}

/* -------------- ADD ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addJob
 * Signature: (Lcom/sun/grid/jgdi/configuration/Job;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addJob(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addJob");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Job", SGE_JB_LIST, JB_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addJobWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Job;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addJobWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addJobWithAnswer");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Job", SGE_JB_LIST, JB_Type, answers);
   DEXIT;
}

/* -------------- Delete ------------------------------------------------------- */           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteJob
 * Signature: (Lcom/sun/grid/jgdi/configuration/Job;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteJob(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteJob");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Job", SGE_JB_LIST, JB_Type, false, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteJobWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Job;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteJobWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteJobWithAnswer");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Job", SGE_JB_LIST, JB_Type, false, answers);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteJobsWithAnswer
 * Signature: ([Ljava/lang/Object;ZL/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteJobsWithAnswer(JNIEnv *env, jobject jgdi, jobjectArray jobj_array, jboolean forced, jobject options, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteJobsWithAnswer");
   jgdi_delete_array(env, jgdi, jobj_array, "com/sun/grid/jgdi/configuration/Job", SGE_JB_LIST, JB_Type, forced, options, answers);
   DEXIT;
}


/* -------------- Update ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateJob
 * Signature: (Lcom/sun/grid/jgdi/configuration/Job;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateJob(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateJob");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Job", SGE_JB_LIST, JB_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateJobWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/Job;, L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateJobWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateJob");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/Job", SGE_JB_LIST, JB_Type, answers);
   DEXIT;
}







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
#include <jni.h>
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
#include "jgdi.h"

#define JGDI_DEBUG

           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    getResourceQuotaSetList
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_fillResourceQuotaSetListWithAnswer(JNIEnv *env, jobject jgdi, jobject list, jobject filter, jobject answers) {
   
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_fillResourceQuotaSetList");
   jgdi_fill(env, jgdi, list, filter, "com/sun/grid/jgdi/configuration/ResourceQuotaSet", SGE_RQS_LIST, RQS_Type, answers);
   DEXIT;
}

/* -------------- ADD ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addResourceQuotaSet
 * Signature: (Lcom/sun/grid/jgdi/configuration/ResourceQuotaSet;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addResourceQuotaSet(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addResourceQuotaSet");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ResourceQuotaSet", SGE_RQS_LIST, RQS_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addResourceQuotaSetWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/ResourceQuotaSet;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addResourceQuotaSetWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addResourceQuotaSetWithAnswer");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ResourceQuotaSet", SGE_RQS_LIST, RQS_Type, answers);
   DEXIT;
}

/* -------------- Delete ------------------------------------------------------- */           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteResourceQuotaSet
 * Signature: (Lcom/sun/grid/jgdi/configuration/ResourceQuotaSet;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteResourceQuotaSet(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteResourceQuotaSet");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ResourceQuotaSet", SGE_RQS_LIST, RQS_Type, false, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteResourceQuotaSetWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/ResourceQuotaSet;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteResourceQuotaSetWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteResourceQuotaSetWithAnswer");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ResourceQuotaSet", SGE_RQS_LIST, RQS_Type, false, answers);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteResourceQuotaSetsWithAnswer
 * Signature: ([Ljava/lang/Object;ZL/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteResourceQuotaSetsWithAnswer(JNIEnv *env, jobject jgdi, jobjectArray jobj_array, jboolean forced, jobject options, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteResourceQuotaSetsWithAnswer");
   jgdi_delete_array(env, jgdi, jobj_array, "com/sun/grid/jgdi/configuration/ResourceQuotaSet", SGE_RQS_LIST, RQS_Type, forced, options, answers);
   DEXIT;
}


/* -------------- Update ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateResourceQuotaSet
 * Signature: (Lcom/sun/grid/jgdi/configuration/ResourceQuotaSet;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateResourceQuotaSet(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateResourceQuotaSet");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ResourceQuotaSet", SGE_RQS_LIST, RQS_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateResourceQuotaSetWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/ResourceQuotaSet;, L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateResourceQuotaSetWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateResourceQuotaSet");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/ResourceQuotaSet", SGE_RQS_LIST, RQS_Type, answers);
   DEXIT;
}







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
#include <jni.h>
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
#include "jgdi.h"

#define JGDI_DEBUG

           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    getEventClientList
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_fillEventClientListWithAnswer(JNIEnv *env, jobject jgdi, jobject list, jobject filter, jobject answers) {
   
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_fillEventClientList");
   jgdi_fill(env, jgdi, list, filter, "com/sun/grid/jgdi/configuration/EventClient", SGE_EV_LIST, EV_Type, answers);
   DEXIT;
}

/* -------------- ADD ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addEventClient
 * Signature: (Lcom/sun/grid/jgdi/configuration/EventClient;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addEventClient(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addEventClient");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/EventClient", SGE_EV_LIST, EV_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addEventClientWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/EventClient;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addEventClientWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addEventClientWithAnswer");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/EventClient", SGE_EV_LIST, EV_Type, answers);
   DEXIT;
}

/* -------------- Delete ------------------------------------------------------- */           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteEventClient
 * Signature: (Lcom/sun/grid/jgdi/configuration/EventClient;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteEventClient(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteEventClient");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/EventClient", SGE_EV_LIST, EV_Type, false, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteEventClientWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/EventClient;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteEventClientWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteEventClientWithAnswer");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/EventClient", SGE_EV_LIST, EV_Type, false, answers);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteEventClientsWithAnswer
 * Signature: ([Ljava/lang/Object;ZL/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteEventClientsWithAnswer(JNIEnv *env, jobject jgdi, jobjectArray jobj_array, jboolean forced, jobject options, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteEventClientsWithAnswer");
   jgdi_delete_array(env, jgdi, jobj_array, "com/sun/grid/jgdi/configuration/EventClient", SGE_EV_LIST, EV_Type, forced, options, answers);
   DEXIT;
}


/* -------------- Update ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateEventClient
 * Signature: (Lcom/sun/grid/jgdi/configuration/EventClient;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateEventClient(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateEventClient");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/EventClient", SGE_EV_LIST, EV_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateEventClientWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/EventClient;, L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateEventClientWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateEventClient");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/EventClient", SGE_EV_LIST, EV_Type, answers);
   DEXIT;
}







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
#include <jni.h>
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
#include "jgdi.h"

#define JGDI_DEBUG

           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    getUserList
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_fillUserListWithAnswer(JNIEnv *env, jobject jgdi, jobject list, jobject filter, jobject answers) {
   
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_fillUserList");
   jgdi_fill(env, jgdi, list, filter, "com/sun/grid/jgdi/configuration/User", SGE_UU_LIST, UU_Type, answers);
   DEXIT;
}

/* -------------- ADD ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addUser
 * Signature: (Lcom/sun/grid/jgdi/configuration/User;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addUser(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addUser");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/User", SGE_UU_LIST, UU_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    addUserWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/User;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_addUserWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_addUserWithAnswer");
   jgdi_add(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/User", SGE_UU_LIST, UU_Type, answers);
   DEXIT;
}

/* -------------- Delete ------------------------------------------------------- */           
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteUser
 * Signature: (Lcom/sun/grid/jgdi/configuration/User;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteUser(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteUser");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/User", SGE_UU_LIST, UU_Type, false, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteUserWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/User;L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteUserWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteUserWithAnswer");
   jgdi_delete(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/User", SGE_UU_LIST, UU_Type, false, answers);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    deleteUsersWithAnswer
 * Signature: ([Ljava/lang/Object;ZL/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteUsersWithAnswer(JNIEnv *env, jobject jgdi, jobjectArray jobj_array, jboolean forced, jobject options, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_deleteUsersWithAnswer");
   jgdi_delete_array(env, jgdi, jobj_array, "com/sun/grid/jgdi/configuration/User", SGE_UU_LIST, UU_Type, forced, options, answers);
   DEXIT;
}


/* -------------- Update ------------------------------------------------------- */
/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateUser
 * Signature: (Lcom/sun/grid/jgdi/configuration/User;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateUser(JNIEnv *env, jobject jgdi, jobject jobj)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateUser");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/User", SGE_UU_LIST, UU_Type, NULL);
   DEXIT;
}

/*
 * Class:     com_sun_grid_jgdi_jni_JGDIImpl
 * Method:    updateUserWithAnswer
 * Signature: (Lcom/sun/grid/jgdi/configuration/User;, L/java/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_sun_grid_jgdi_jni_JGDIImpl_updateUserWithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)
{
   DENTER(TOP_LAYER, "Java_com_sun_grid_jgdi_jni_JGDIImpl_updateUser");
   jgdi_update(env, jgdi, jobj, "com/sun/grid/jgdi/configuration/User", SGE_UU_LIST, UU_Type, answers);
   DEXIT;
}



