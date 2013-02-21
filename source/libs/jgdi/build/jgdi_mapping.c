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
#include "sge_gdi.h"
#include "sge_gdi_ctx.h"
#include "jgdi_common.h"



lDescr* get_descr(const char* name) {

   if( strcmp(name, "JJ_Type") == 0 ) return JJ_Type;

   if( strcmp(name, "XMLS_Type") == 0 ) return XMLS_Type;

   if( strcmp(name, "EH_Type") == 0 ) return EH_Type;

   if( strcmp(name, "RUE_Type") == 0 ) return RUE_Type;

   if( strcmp(name, "FCAT_Type") == 0 ) return FCAT_Type;

   if( strcmp(name, "RDE_Type") == 0 ) return RDE_Type;

   if( strcmp(name, "CU_Type") == 0 ) return CU_Type;

   if( strcmp(name, "VA_Type") == 0 ) return VA_Type;

   if( strcmp(name, "CE_Type") == 0 ) return CE_Type;

   if( strcmp(name, "ABOOL_Type") == 0 ) return ABOOL_Type;

   if( strcmp(name, "CF_Type") == 0 ) return CF_Type;

   if( strcmp(name, "KTGT_Type") == 0 ) return KTGT_Type;

   if( strcmp(name, "SO_Type") == 0 ) return SO_Type;

   if( strcmp(name, "ACK_Type") == 0 ) return ACK_Type;

   if( strcmp(name, "UU_Type") == 0 ) return UU_Type;

   if( strcmp(name, "GR_Type") == 0 ) return GR_Type;

   if( strcmp(name, "SPTR_Type") == 0 ) return SPTR_Type;

   if( strcmp(name, "SEC_Type") == 0 ) return SEC_Type;

   if( strcmp(name, "SPR_Type") == 0 ) return SPR_Type;

   if( strcmp(name, "EVS_Type") == 0 ) return EVS_Type;

   if( strcmp(name, "ARA_Type") == 0 ) return ARA_Type;

   if( strcmp(name, "AR_Type") == 0 ) return AR_Type;

   if( strcmp(name, "XMLE_Type") == 0 ) return XMLE_Type;

   if( strcmp(name, "RQR_Type") == 0 ) return RQR_Type;

   if( strcmp(name, "AINTER_Type") == 0 ) return AINTER_Type;

   if( strcmp(name, "PGR_Type") == 0 ) return PGR_Type;

   if( strcmp(name, "RQL_Type") == 0 ) return RQL_Type;

   if( strcmp(name, "QR_Type") == 0 ) return QR_Type;

   if( strcmp(name, "AMEM_Type") == 0 ) return AMEM_Type;

   if( strcmp(name, "OQ_Type") == 0 ) return OQ_Type;

   if( strcmp(name, "ASOLIST_Type") == 0 ) return ASOLIST_Type;

   if( strcmp(name, "KRB_Type") == 0 ) return KRB_Type;

   if( strcmp(name, "US_Type") == 0 ) return US_Type;

   if( strcmp(name, "ET_Type") == 0 ) return ET_Type;

   if( strcmp(name, "MES_Type") == 0 ) return MES_Type;

   if( strcmp(name, "PRO_Type") == 0 ) return PRO_Type;

   if( strcmp(name, "SCT_Type") == 0 ) return SCT_Type;

   if( strcmp(name, "JO_Type") == 0 ) return JO_Type;

   if( strcmp(name, "QIM_Type") == 0 ) return QIM_Type;

   if( strcmp(name, "CQU_Type") == 0 ) return CQU_Type;

   if( strcmp(name, "PREF_Type") == 0 ) return PREF_Type;

   if( strcmp(name, "JP_Type") == 0 ) return JP_Type;

   if( strcmp(name, "RU_Type") == 0 ) return RU_Type;

   if( strcmp(name, "STN_Type") == 0 ) return STN_Type;

   if( strcmp(name, "BN_Type") == 0 ) return BN_Type;

   if( strcmp(name, "PACK_Type") == 0 ) return PACK_Type;

   if( strcmp(name, "JAT_Type") == 0 ) return JAT_Type;

   if( strcmp(name, "STU_Type") == 0 ) return STU_Type;

   if( strcmp(name, "ACELIST_Type") == 0 ) return ACELIST_Type;

   if( strcmp(name, "AQTLIST_Type") == 0 ) return AQTLIST_Type;

   if( strcmp(name, "JJAT_Type") == 0 ) return JJAT_Type;

   if( strcmp(name, "JSV_Type") == 0 ) return JSV_Type;

   if( strcmp(name, "TE_Type") == 0 ) return TE_Type;

   if( strcmp(name, "AULNG_Type") == 0 ) return AULNG_Type;

   if( strcmp(name, "RQRF_Type") == 0 ) return RQRF_Type;

   if( strcmp(name, "LIC_Type") == 0 ) return LIC_Type;

   if( strcmp(name, "HGRP_Type") == 0 ) return HGRP_Type;

   if( strcmp(name, "CONF_Type") == 0 ) return CONF_Type;

   if( strcmp(name, "HR_Type") == 0 ) return HR_Type;

   if( strcmp(name, "FPET_Type") == 0 ) return FPET_Type;

   if( strcmp(name, "AH_Type") == 0 ) return AH_Type;

   if( strcmp(name, "PE_Type") == 0 ) return PE_Type;

   if( strcmp(name, "CQ_Type") == 0 ) return CQ_Type;

   if( strcmp(name, "CA_Type") == 0 ) return CA_Type;

   if( strcmp(name, "ASTRLIST_Type") == 0 ) return ASTRLIST_Type;

   if( strcmp(name, "USR_Type") == 0 ) return USR_Type;

   if( strcmp(name, "SPA_Type") == 0 ) return SPA_Type;

   if( strcmp(name, "REF_Type") == 0 ) return REF_Type;

   if( strcmp(name, "CAL_Type") == 0 ) return CAL_Type;

   if( strcmp(name, "AT_Type") == 0 ) return AT_Type;

   if( strcmp(name, "JR_Type") == 0 ) return JR_Type;

   if( strcmp(name, "EV_Type") == 0 ) return EV_Type;

   if( strcmp(name, "CCT_Type") == 0 ) return CCT_Type;

   if( strcmp(name, "HL_Type") == 0 ) return HL_Type;

   if( strcmp(name, "UM_Type") == 0 ) return UM_Type;

   if( strcmp(name, "ATIME_Type") == 0 ) return ATIME_Type;

   if( strcmp(name, "PA_Type") == 0 ) return PA_Type;

   if( strcmp(name, "SPT_Type") == 0 ) return SPT_Type;

   if( strcmp(name, "APRJLIST_Type") == 0 ) return APRJLIST_Type;

   if( strcmp(name, "QETI_Type") == 0 ) return QETI_Type;

   if( strcmp(name, "CT_Type") == 0 ) return CT_Type;

   if( strcmp(name, "PN_Type") == 0 ) return PN_Type;

   if( strcmp(name, "UO_Type") == 0 ) return UO_Type;

   if( strcmp(name, "AUSRLIST_Type") == 0 ) return AUSRLIST_Type;

   if( strcmp(name, "LS_Type") == 0 ) return LS_Type;

   if( strcmp(name, "SPM_Type") == 0 ) return SPM_Type;

   if( strcmp(name, "PERM_Type") == 0 ) return PERM_Type;

   if( strcmp(name, "SPC_Type") == 0 ) return SPC_Type;

   if( strcmp(name, "MA_Type") == 0 ) return MA_Type;

   if( strcmp(name, "SC_Type") == 0 ) return SC_Type;

   if( strcmp(name, "AN_Type") == 0 ) return AN_Type;

   if( strcmp(name, "PET_Type") == 0 ) return PET_Type;

   if( strcmp(name, "SGEJ_Type") == 0 ) return SGEJ_Type;

   if( strcmp(name, "ASTRING_Type") == 0 ) return ASTRING_Type;

   if( strcmp(name, "PETR_Type") == 0 ) return PETR_Type;

   if( strcmp(name, "CK_Type") == 0 ) return CK_Type;

   if( strcmp(name, "SU_Type") == 0 ) return SU_Type;

   if( strcmp(name, "MR_Type") == 0 ) return MR_Type;

   if( strcmp(name, "RTIC_Type") == 0 ) return RTIC_Type;

   if( strcmp(name, "JRL_Type") == 0 ) return JRL_Type;

   if( strcmp(name, "JB_Type") == 0 ) return JB_Type;

   if( strcmp(name, "ID_Type") == 0 ) return ID_Type;

   if( strcmp(name, "CTI_Type") == 0 ) return CTI_Type;

   if( strcmp(name, "PR_Type") == 0 ) return PR_Type;

   if( strcmp(name, "TMR_Type") == 0 ) return TMR_Type;

   if( strcmp(name, "OR_Type") == 0 ) return OR_Type;

   if( strcmp(name, "XMLH_Type") == 0 ) return XMLH_Type;

   if( strcmp(name, "QAJ_Type") == 0 ) return QAJ_Type;

   if( strcmp(name, "HS_Type") == 0 ) return HS_Type;

   if( strcmp(name, "TM_Type") == 0 ) return TM_Type;

   if( strcmp(name, "ST_Type") == 0 ) return ST_Type;

   if( strcmp(name, "RN_Type") == 0 ) return RN_Type;

   if( strcmp(name, "RT_Type") == 0 ) return RT_Type;

   if( strcmp(name, "ULNG_Type") == 0 ) return ULNG_Type;

   if( strcmp(name, "SH_Type") == 0 ) return SH_Type;

   if( strcmp(name, "NSV_Type") == 0 ) return NSV_Type;

   if( strcmp(name, "JG_Type") == 0 ) return JG_Type;

   if( strcmp(name, "REP_Type") == 0 ) return REP_Type;

   if( strcmp(name, "JRE_Type") == 0 ) return JRE_Type;

   if( strcmp(name, "LR_Type") == 0 ) return LR_Type;

   if( strcmp(name, "FES_Type") == 0 ) return FES_Type;

   if( strcmp(name, "UPP_Type") == 0 ) return UPP_Type;

   if( strcmp(name, "JL_Type") == 0 ) return JL_Type;

   if( strcmp(name, "LDR_Type") == 0 ) return LDR_Type;

   if( strcmp(name, "JC_Type") == 0 ) return JC_Type;

   if( strcmp(name, "EVR_Type") == 0 ) return EVR_Type;

   if( strcmp(name, "UPU_Type") == 0 ) return UPU_Type;

   if( strcmp(name, "PARA_Type") == 0 ) return PARA_Type;

   if( strcmp(name, "RQRL_Type") == 0 ) return RQRL_Type;

   if( strcmp(name, "XMLA_Type") == 0 ) return XMLA_Type;

   if( strcmp(name, "ASTR_Type") == 0 ) return ASTR_Type;

   if( strcmp(name, "SME_Type") == 0 ) return SME_Type;

   if( strcmp(name, "QU_Type") == 0 ) return QU_Type;

   if( strcmp(name, "UE_Type") == 0 ) return UE_Type;

   if( strcmp(name, "UA_Type") == 0 ) return UA_Type;

   if( strcmp(name, "RQS_Type") == 0 ) return RQS_Type;

   if( strcmp(name, "QRL_Type") == 0 ) return QRL_Type;

   return NULL;
}

lDescr* get_descr_for_classname(const char* classname ) {


   if (strcmp(classname, "com.sun.grid.jgdi.configuration.JJ") == 0) return JJ_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.XMLS") == 0) return XMLS_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ExecHost") == 0) return EH_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ResourceUtilization") == 0) return RUE_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.FCAT") == 0) return FCAT_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ResourceDiagramEntry") == 0) return RDE_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.CU") == 0) return CU_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.VA") == 0) return VA_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ComplexEntry") == 0) return CE_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ABOOL") == 0) return ABOOL_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ConfigurationElement") == 0) return CF_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.KTGT") == 0) return KTGT_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.Subordinate") == 0) return SO_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ACK") == 0) return ACK_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.User") == 0) return UU_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.GR") == 0) return GR_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.SPTR") == 0) return SPTR_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.SEC") == 0) return SEC_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.SPR") == 0) return SPR_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.EventSubcribtion") == 0) return EVS_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ARA") == 0) return ARA_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.AdvanceReservation") == 0) return AR_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.XMLE") == 0) return XMLE_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ResourceQuotaRule") == 0) return RQR_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.AINTER") == 0) return AINTER_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.PGR") == 0) return PGR_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.RQL") == 0) return RQL_Type;

   if (strcmp(classname, "java.lang.String") == 0) return QR_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.AMEM") == 0) return AMEM_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.OQ") == 0) return OQ_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ASOLIST") == 0) return ASOLIST_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.KRB") == 0) return KRB_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.UserSet") == 0) return US_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.Event") == 0) return ET_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.JobSchedulingMessage") == 0) return MES_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.PRO") == 0) return PRO_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.SCT") == 0) return SCT_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.JO") == 0) return JO_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.QueueInstanceMessage") == 0) return QIM_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.CalendarQueueState") == 0) return CQU_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.PREF") == 0) return PREF_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.JP") == 0) return JP_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.RescheduleUnknown") == 0) return RU_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ShareTree") == 0) return STN_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.Binding") == 0) return BN_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.PACK") == 0) return PACK_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.JobTask") == 0) return JAT_Type;

   if (strcmp(classname, "java.lang.String") == 0) return STU_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ACELIST") == 0) return ACELIST_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.AQTLIST") == 0) return AQTLIST_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.JJAT") == 0) return JJAT_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.JSV") == 0) return JSV_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.TE") == 0) return TE_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.AULNG") == 0) return AULNG_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ResourceQuotaRuleFilter") == 0) return RQRF_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.LIC") == 0) return LIC_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.Hostgroup") == 0) return HGRP_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.Configuration") == 0) return CONF_Type;

   if (strcmp(classname, "java.lang.String") == 0) return HR_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.FPET") == 0) return FPET_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.AdminHost") == 0) return AH_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ParallelEnvironment") == 0) return PE_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ClusterQueue") == 0) return CQ_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ParsedCalendar") == 0) return CA_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ASTRLIST") == 0) return ASTRLIST_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.USR") == 0) return USR_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.SPA") == 0) return SPA_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.REF") == 0) return REF_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.Calendar") == 0) return CAL_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.AT") == 0) return AT_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.JR") == 0) return JR_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.EventClient") == 0) return EV_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.CCT") == 0) return CCT_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.HL") == 0) return HL_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.Manager") == 0) return UM_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ATIME") == 0) return ATIME_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.PathAlias") == 0) return PA_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.SPT") == 0) return SPT_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.APRJLIST") == 0) return APRJLIST_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.QETI") == 0) return QETI_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.CT") == 0) return CT_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.PathName") == 0) return PN_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.Operator") == 0) return UO_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.AUSRLIST") == 0) return AUSRLIST_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.LS") == 0) return LS_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.SPM") == 0) return SPM_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.PERM") == 0) return PERM_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.SPC") == 0) return SPC_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.MA") == 0) return MA_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.SchedConf") == 0) return SC_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.JGDIAnswer") == 0) return AN_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.PETask") == 0) return PET_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.SGEJ") == 0) return SGEJ_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ASTRING") == 0) return ASTRING_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.PETR") == 0) return PETR_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.Checkpoint") == 0) return CK_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.SU") == 0) return SU_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.MailReceiver") == 0) return MR_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.RTIC") == 0) return RTIC_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.JRL") == 0) return JRL_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.Job") == 0) return JB_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ID") == 0) return ID_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.CTI") == 0) return CTI_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.Project") == 0) return PR_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.TimeRange") == 0) return TMR_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.OR") == 0) return OR_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.XMLH") == 0) return XMLH_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.QAJ") == 0) return QAJ_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.HS") == 0) return HS_Type;

   if (strcmp(classname, "java.util.Calendar") == 0) return TM_Type;

   if (strcmp(classname, "java.lang.String") == 0) return ST_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.Range") == 0) return RN_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.RT") == 0) return RT_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ULNG") == 0) return ULNG_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.SubmitHost") == 0) return SH_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.NSV") == 0) return NSV_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.JG") == 0) return JG_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.REP") == 0) return REP_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.JobReference") == 0) return JRE_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.LR") == 0) return LR_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.FES") == 0) return FES_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ProjectUsage") == 0) return UPP_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.JL") == 0) return JL_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.LDR") == 0) return LDR_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.JC") == 0) return JC_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.EVR") == 0) return EVR_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.JobUsage") == 0) return UPU_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.PARA") == 0) return PARA_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ResourceQuotaRuleLimit") == 0) return RQRL_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.XMLA") == 0) return XMLA_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ASTR") == 0) return ASTR_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.JobSchedulingInfo") == 0) return SME_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.QueueInstance") == 0) return QU_Type;

   if (strcmp(classname, "java.lang.String") == 0) return UE_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.UA") == 0) return UA_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.ResourceQuotaSet") == 0) return RQS_Type;

   if (strcmp(classname, "com.sun.grid.jgdi.configuration.QRL") == 0) return QRL_Type;

   return NULL;
}




const char* get_classname_for_descr(const lDescr *descr) {

   if (descr == JJ_Type) {
      return "com/sun/grid/jgdi/configuration/JJ";
   }

   if (descr == XMLS_Type) {
      return "com/sun/grid/jgdi/configuration/XMLS";
   }

   if (descr == EH_Type) {
      return "com/sun/grid/jgdi/configuration/ExecHost";
   }

   if (descr == RUE_Type) {
      return "com/sun/grid/jgdi/configuration/ResourceUtilization";
   }

   if (descr == FCAT_Type) {
      return "com/sun/grid/jgdi/configuration/FCAT";
   }

   if (descr == RDE_Type) {
      return "com/sun/grid/jgdi/configuration/ResourceDiagramEntry";
   }

   if (descr == CU_Type) {
      return "com/sun/grid/jgdi/configuration/CU";
   }

   if (descr == VA_Type) {
      return "com/sun/grid/jgdi/configuration/VA";
   }

   if (descr == CE_Type) {
      return "com/sun/grid/jgdi/configuration/ComplexEntry";
   }

   if (descr == ABOOL_Type) {
      return "com/sun/grid/jgdi/configuration/ABOOL";
   }

   if (descr == CF_Type) {
      return "com/sun/grid/jgdi/configuration/ConfigurationElement";
   }

   if (descr == KTGT_Type) {
      return "com/sun/grid/jgdi/configuration/KTGT";
   }

   if (descr == SO_Type) {
      return "com/sun/grid/jgdi/configuration/Subordinate";
   }

   if (descr == ACK_Type) {
      return "com/sun/grid/jgdi/configuration/ACK";
   }

   if (descr == UU_Type) {
      return "com/sun/grid/jgdi/configuration/User";
   }

   if (descr == GR_Type) {
      return "com/sun/grid/jgdi/configuration/GR";
   }

   if (descr == SPTR_Type) {
      return "com/sun/grid/jgdi/configuration/SPTR";
   }

   if (descr == SEC_Type) {
      return "com/sun/grid/jgdi/configuration/SEC";
   }

   if (descr == SPR_Type) {
      return "com/sun/grid/jgdi/configuration/SPR";
   }

   if (descr == EVS_Type) {
      return "com/sun/grid/jgdi/configuration/EventSubcribtion";
   }

   if (descr == ARA_Type) {
      return "com/sun/grid/jgdi/configuration/ARA";
   }

   if (descr == AR_Type) {
      return "com/sun/grid/jgdi/configuration/AdvanceReservation";
   }

   if (descr == XMLE_Type) {
      return "com/sun/grid/jgdi/configuration/XMLE";
   }

   if (descr == RQR_Type) {
      return "com/sun/grid/jgdi/configuration/ResourceQuotaRule";
   }

   if (descr == AINTER_Type) {
      return "com/sun/grid/jgdi/configuration/AINTER";
   }

   if (descr == PGR_Type) {
      return "com/sun/grid/jgdi/configuration/PGR";
   }

   if (descr == RQL_Type) {
      return "com/sun/grid/jgdi/configuration/RQL";
   }

   if (descr == QR_Type) {
      return "java/lang/String";
   }

   if (descr == AMEM_Type) {
      return "com/sun/grid/jgdi/configuration/AMEM";
   }

   if (descr == OQ_Type) {
      return "com/sun/grid/jgdi/configuration/OQ";
   }

   if (descr == ASOLIST_Type) {
      return "com/sun/grid/jgdi/configuration/ASOLIST";
   }

   if (descr == KRB_Type) {
      return "com/sun/grid/jgdi/configuration/KRB";
   }

   if (descr == US_Type) {
      return "com/sun/grid/jgdi/configuration/UserSet";
   }

   if (descr == ET_Type) {
      return "com/sun/grid/jgdi/configuration/Event";
   }

   if (descr == MES_Type) {
      return "com/sun/grid/jgdi/configuration/JobSchedulingMessage";
   }

   if (descr == PRO_Type) {
      return "com/sun/grid/jgdi/configuration/PRO";
   }

   if (descr == SCT_Type) {
      return "com/sun/grid/jgdi/configuration/SCT";
   }

   if (descr == JO_Type) {
      return "com/sun/grid/jgdi/configuration/JO";
   }

   if (descr == QIM_Type) {
      return "com/sun/grid/jgdi/configuration/QueueInstanceMessage";
   }

   if (descr == CQU_Type) {
      return "com/sun/grid/jgdi/configuration/CalendarQueueState";
   }

   if (descr == PREF_Type) {
      return "com/sun/grid/jgdi/configuration/PREF";
   }

   if (descr == JP_Type) {
      return "com/sun/grid/jgdi/configuration/JP";
   }

   if (descr == RU_Type) {
      return "com/sun/grid/jgdi/configuration/RescheduleUnknown";
   }

   if (descr == STN_Type) {
      return "com/sun/grid/jgdi/configuration/ShareTree";
   }

   if (descr == BN_Type) {
      return "com/sun/grid/jgdi/configuration/Binding";
   }

   if (descr == PACK_Type) {
      return "com/sun/grid/jgdi/configuration/PACK";
   }

   if (descr == JAT_Type) {
      return "com/sun/grid/jgdi/configuration/JobTask";
   }

   if (descr == STU_Type) {
      return "java/lang/String";
   }

   if (descr == ACELIST_Type) {
      return "com/sun/grid/jgdi/configuration/ACELIST";
   }

   if (descr == AQTLIST_Type) {
      return "com/sun/grid/jgdi/configuration/AQTLIST";
   }

   if (descr == JJAT_Type) {
      return "com/sun/grid/jgdi/configuration/JJAT";
   }

   if (descr == JSV_Type) {
      return "com/sun/grid/jgdi/configuration/JSV";
   }

   if (descr == TE_Type) {
      return "com/sun/grid/jgdi/configuration/TE";
   }

   if (descr == AULNG_Type) {
      return "com/sun/grid/jgdi/configuration/AULNG";
   }

   if (descr == RQRF_Type) {
      return "com/sun/grid/jgdi/configuration/ResourceQuotaRuleFilter";
   }

   if (descr == LIC_Type) {
      return "com/sun/grid/jgdi/configuration/LIC";
   }

   if (descr == HGRP_Type) {
      return "com/sun/grid/jgdi/configuration/Hostgroup";
   }

   if (descr == CONF_Type) {
      return "com/sun/grid/jgdi/configuration/Configuration";
   }

   if (descr == HR_Type) {
      return "java/lang/String";
   }

   if (descr == FPET_Type) {
      return "com/sun/grid/jgdi/configuration/FPET";
   }

   if (descr == AH_Type) {
      return "com/sun/grid/jgdi/configuration/AdminHost";
   }

   if (descr == PE_Type) {
      return "com/sun/grid/jgdi/configuration/ParallelEnvironment";
   }

   if (descr == CQ_Type) {
      return "com/sun/grid/jgdi/configuration/ClusterQueue";
   }

   if (descr == CA_Type) {
      return "com/sun/grid/jgdi/configuration/ParsedCalendar";
   }

   if (descr == ASTRLIST_Type) {
      return "com/sun/grid/jgdi/configuration/ASTRLIST";
   }

   if (descr == USR_Type) {
      return "com/sun/grid/jgdi/configuration/USR";
   }

   if (descr == SPA_Type) {
      return "com/sun/grid/jgdi/configuration/SPA";
   }

   if (descr == REF_Type) {
      return "com/sun/grid/jgdi/configuration/REF";
   }

   if (descr == CAL_Type) {
      return "com/sun/grid/jgdi/configuration/Calendar";
   }

   if (descr == AT_Type) {
      return "com/sun/grid/jgdi/configuration/AT";
   }

   if (descr == JR_Type) {
      return "com/sun/grid/jgdi/configuration/JR";
   }

   if (descr == EV_Type) {
      return "com/sun/grid/jgdi/configuration/EventClient";
   }

   if (descr == CCT_Type) {
      return "com/sun/grid/jgdi/configuration/CCT";
   }

   if (descr == HL_Type) {
      return "com/sun/grid/jgdi/configuration/HL";
   }

   if (descr == UM_Type) {
      return "com/sun/grid/jgdi/configuration/Manager";
   }

   if (descr == ATIME_Type) {
      return "com/sun/grid/jgdi/configuration/ATIME";
   }

   if (descr == PA_Type) {
      return "com/sun/grid/jgdi/configuration/PathAlias";
   }

   if (descr == SPT_Type) {
      return "com/sun/grid/jgdi/configuration/SPT";
   }

   if (descr == APRJLIST_Type) {
      return "com/sun/grid/jgdi/configuration/APRJLIST";
   }

   if (descr == QETI_Type) {
      return "com/sun/grid/jgdi/configuration/QETI";
   }

   if (descr == CT_Type) {
      return "com/sun/grid/jgdi/configuration/CT";
   }

   if (descr == PN_Type) {
      return "com/sun/grid/jgdi/configuration/PathName";
   }

   if (descr == UO_Type) {
      return "com/sun/grid/jgdi/configuration/Operator";
   }

   if (descr == AUSRLIST_Type) {
      return "com/sun/grid/jgdi/configuration/AUSRLIST";
   }

   if (descr == LS_Type) {
      return "com/sun/grid/jgdi/configuration/LS";
   }

   if (descr == SPM_Type) {
      return "com/sun/grid/jgdi/configuration/SPM";
   }

   if (descr == PERM_Type) {
      return "com/sun/grid/jgdi/configuration/PERM";
   }

   if (descr == SPC_Type) {
      return "com/sun/grid/jgdi/configuration/SPC";
   }

   if (descr == MA_Type) {
      return "com/sun/grid/jgdi/configuration/MA";
   }

   if (descr == SC_Type) {
      return "com/sun/grid/jgdi/configuration/SchedConf";
   }

   if (descr == AN_Type) {
      return "com/sun/grid/jgdi/configuration/JGDIAnswer";
   }

   if (descr == PET_Type) {
      return "com/sun/grid/jgdi/configuration/PETask";
   }

   if (descr == SGEJ_Type) {
      return "com/sun/grid/jgdi/configuration/SGEJ";
   }

   if (descr == ASTRING_Type) {
      return "com/sun/grid/jgdi/configuration/ASTRING";
   }

   if (descr == PETR_Type) {
      return "com/sun/grid/jgdi/configuration/PETR";
   }

   if (descr == CK_Type) {
      return "com/sun/grid/jgdi/configuration/Checkpoint";
   }

   if (descr == SU_Type) {
      return "com/sun/grid/jgdi/configuration/SU";
   }

   if (descr == MR_Type) {
      return "com/sun/grid/jgdi/configuration/MailReceiver";
   }

   if (descr == RTIC_Type) {
      return "com/sun/grid/jgdi/configuration/RTIC";
   }

   if (descr == JRL_Type) {
      return "com/sun/grid/jgdi/configuration/JRL";
   }

   if (descr == JB_Type) {
      return "com/sun/grid/jgdi/configuration/Job";
   }

   if (descr == ID_Type) {
      return "com/sun/grid/jgdi/configuration/ID";
   }

   if (descr == CTI_Type) {
      return "com/sun/grid/jgdi/configuration/CTI";
   }

   if (descr == PR_Type) {
      return "com/sun/grid/jgdi/configuration/Project";
   }

   if (descr == TMR_Type) {
      return "com/sun/grid/jgdi/configuration/TimeRange";
   }

   if (descr == OR_Type) {
      return "com/sun/grid/jgdi/configuration/OR";
   }

   if (descr == XMLH_Type) {
      return "com/sun/grid/jgdi/configuration/XMLH";
   }

   if (descr == QAJ_Type) {
      return "com/sun/grid/jgdi/configuration/QAJ";
   }

   if (descr == HS_Type) {
      return "com/sun/grid/jgdi/configuration/HS";
   }

   if (descr == TM_Type) {
      return "java/util/Calendar";
   }

   if (descr == ST_Type) {
      return "java/lang/String";
   }

   if (descr == RN_Type) {
      return "com/sun/grid/jgdi/configuration/Range";
   }

   if (descr == RT_Type) {
      return "com/sun/grid/jgdi/configuration/RT";
   }

   if (descr == ULNG_Type) {
      return "com/sun/grid/jgdi/configuration/ULNG";
   }

   if (descr == SH_Type) {
      return "com/sun/grid/jgdi/configuration/SubmitHost";
   }

   if (descr == NSV_Type) {
      return "com/sun/grid/jgdi/configuration/NSV";
   }

   if (descr == JG_Type) {
      return "com/sun/grid/jgdi/configuration/JG";
   }

   if (descr == REP_Type) {
      return "com/sun/grid/jgdi/configuration/REP";
   }

   if (descr == JRE_Type) {
      return "com/sun/grid/jgdi/configuration/JobReference";
   }

   if (descr == LR_Type) {
      return "com/sun/grid/jgdi/configuration/LR";
   }

   if (descr == FES_Type) {
      return "com/sun/grid/jgdi/configuration/FES";
   }

   if (descr == UPP_Type) {
      return "com/sun/grid/jgdi/configuration/ProjectUsage";
   }

   if (descr == JL_Type) {
      return "com/sun/grid/jgdi/configuration/JL";
   }

   if (descr == LDR_Type) {
      return "com/sun/grid/jgdi/configuration/LDR";
   }

   if (descr == JC_Type) {
      return "com/sun/grid/jgdi/configuration/JC";
   }

   if (descr == EVR_Type) {
      return "com/sun/grid/jgdi/configuration/EVR";
   }

   if (descr == UPU_Type) {
      return "com/sun/grid/jgdi/configuration/JobUsage";
   }

   if (descr == PARA_Type) {
      return "com/sun/grid/jgdi/configuration/PARA";
   }

   if (descr == RQRL_Type) {
      return "com/sun/grid/jgdi/configuration/ResourceQuotaRuleLimit";
   }

   if (descr == XMLA_Type) {
      return "com/sun/grid/jgdi/configuration/XMLA";
   }

   if (descr == ASTR_Type) {
      return "com/sun/grid/jgdi/configuration/ASTR";
   }

   if (descr == SME_Type) {
      return "com/sun/grid/jgdi/configuration/JobSchedulingInfo";
   }

   if (descr == QU_Type) {
      return "com/sun/grid/jgdi/configuration/QueueInstance";
   }

   if (descr == UE_Type) {
      return "java/lang/String";
   }

   if (descr == UA_Type) {
      return "com/sun/grid/jgdi/configuration/UA";
   }

   if (descr == RQS_Type) {
      return "com/sun/grid/jgdi/configuration/ResourceQuotaSet";
   }

   if (descr == QRL_Type) {
      return "com/sun/grid/jgdi/configuration/QRL";
   }

   return NULL;
}

int get_master_list_for_classname(const char* classname) {

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.JJ") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.XMLS") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ExecHost") == 0 ) {

     return SGE_EH_LIST;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ResourceUtilization") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.FCAT") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ResourceDiagramEntry") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.CU") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.VA") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ComplexEntry") == 0 ) {

     return SGE_CE_LIST;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ABOOL") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ConfigurationElement") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.KTGT") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.Subordinate") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ACK") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.User") == 0 ) {

     return SGE_UU_LIST;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.GR") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.SPTR") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.SEC") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.SPR") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.EventSubcribtion") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ARA") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.AdvanceReservation") == 0 ) {

     return SGE_AR_LIST;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.XMLE") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ResourceQuotaRule") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.AINTER") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.PGR") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.RQL") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "java.lang.String") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.AMEM") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.OQ") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ASOLIST") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.KRB") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.UserSet") == 0 ) {

     return SGE_US_LIST;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.Event") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.JobSchedulingMessage") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.PRO") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.SCT") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.JO") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.QueueInstanceMessage") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.CalendarQueueState") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.PREF") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.JP") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.RescheduleUnknown") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ShareTree") == 0 ) {

     return SGE_STN_LIST;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.Binding") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.PACK") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.JobTask") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "java.lang.String") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ACELIST") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.AQTLIST") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.JJAT") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.JSV") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.TE") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.AULNG") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ResourceQuotaRuleFilter") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.LIC") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.Hostgroup") == 0 ) {

     return SGE_HGRP_LIST;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.Configuration") == 0 ) {

     return SGE_CONF_LIST;
      
   }

   if( strcmp(classname, "java.lang.String") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.FPET") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.AdminHost") == 0 ) {

     return SGE_AH_LIST;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ParallelEnvironment") == 0 ) {

     return SGE_PE_LIST;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ClusterQueue") == 0 ) {

     return SGE_CQ_LIST;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ParsedCalendar") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ASTRLIST") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.USR") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.SPA") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.REF") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.Calendar") == 0 ) {

     return SGE_CAL_LIST;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.AT") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.JR") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.EventClient") == 0 ) {

     return SGE_EV_LIST;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.CCT") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.HL") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.Manager") == 0 ) {

     return SGE_UM_LIST;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ATIME") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.PathAlias") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.SPT") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.APRJLIST") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.QETI") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.CT") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.PathName") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.Operator") == 0 ) {

     return SGE_UO_LIST;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.AUSRLIST") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.LS") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.SPM") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.PERM") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.SPC") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.MA") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.SchedConf") == 0 ) {

     return SGE_SC_LIST;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.JGDIAnswer") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.PETask") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.SGEJ") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ASTRING") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.PETR") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.Checkpoint") == 0 ) {

     return SGE_CK_LIST;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.SU") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.MailReceiver") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.RTIC") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.JRL") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.Job") == 0 ) {

     return SGE_JB_LIST;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ID") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.CTI") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.Project") == 0 ) {

     return SGE_PR_LIST;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.TimeRange") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.OR") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.XMLH") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.QAJ") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.HS") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "java.util.Calendar") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "java.lang.String") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.Range") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.RT") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ULNG") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.SubmitHost") == 0 ) {

     return SGE_SH_LIST;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.NSV") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.JG") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.REP") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.JobReference") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.LR") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.FES") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ProjectUsage") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.JL") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.LDR") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.JC") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.EVR") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.JobUsage") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.PARA") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ResourceQuotaRuleLimit") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.XMLA") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ASTR") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.JobSchedulingInfo") == 0 ) {

     return SGE_SME_LIST;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.QueueInstance") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "java.lang.String") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.UA") == 0 ) {
     
     return -1;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.ResourceQuotaSet") == 0 ) {

     return SGE_RQS_LIST;
      
   }

   if( strcmp(classname, "com.sun.grid.jgdi.configuration.QRL") == 0 ) {
     
     return -1;
      
   }

   return -1;
}
