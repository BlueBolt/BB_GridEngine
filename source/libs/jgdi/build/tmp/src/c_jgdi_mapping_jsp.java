public class c_jgdi_mapping_jsp implements com.sun.grid.cull.template.Template {
  public void print(final com.sun.grid.cull.Printer p, final java.util.Map params) {
    p.println("/*___INFO__MARK_BEGIN__*/");
    p.println("/*************************************************************************");
    p.println(" *");
    p.println(" *  The Contents of this file are made available subject to the terms of");
    p.println(" *  the Sun Industry Standards Source License Version 1.2");
    p.println(" *");
    p.println(" *  Sun Microsystems Inc., March, 2001");
    p.println(" *");
    p.println(" *");
    p.println(" *  Sun Industry Standards Source License Version 1.2");
    p.println(" *  =================================================");
    p.println(" *  The contents of this file are subject to the Sun Industry Standards");
    p.println(" *  Source License Version 1.2 (the \"License\"); You may not use this file");
    p.println(" *  except in compliance with the License. You may obtain a copy of the");
    p.println(" *  License at http://gridengine.sunsource.net/Gridengine_SISSL_license.html");
    p.println(" *");
    p.println(" *  Software provided under this License is provided on an \"AS IS\" basis,");
    p.println(" *  WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING,");
    p.println(" *  WITHOUT LIMITATION, WARRANTIES THAT THE SOFTWARE IS FREE OF DEFECTS,");
    p.println(" *  MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE, OR NON-INFRINGING.");
    p.println(" *  See the License for the specific provisions governing your rights and");
    p.println(" *  obligations concerning the Software.");
    p.println(" *");
    p.println(" *   The Initial Developer of the Original Code is: Sun Microsystems, Inc.");
    p.println(" *");
    p.println(" *   Copyright: 2001 by Sun Microsystems, Inc.");
    p.println(" *");
    p.println(" *   All Rights Reserved.");
    p.println(" *");
    p.println(" ************************************************************************/");
    p.println("/*___INFO__MARK_END__*/");
    p.print("");
    
   com.sun.grid.cull.JavaHelper jh = (com.sun.grid.cull.JavaHelper)params.get("javaHelper");
   com.sun.grid.cull.CullDefinition cullDef = (com.sun.grid.cull.CullDefinition)params.get("cullDef");

   if( cullDef == null ) {
     throw new IllegalStateException("param cullDef not found");
   }
p.println("");
    p.println("#include <ctype.h>");
    p.println("#include <string.h>");
    p.println("#include <jni.h>");
    p.println("#include \"basis_types.h\"");
    p.println("#include \"cull.h\"");
    p.println("#include \"commlib.h\"");
    p.println("#include \"sgermon.h\"");
    p.println("#include \"sge_all_listsL.h\"");
    p.println("#include \"sge_answer.h\"");
    p.println("#include \"sge_prog.h\"");
    p.println("#include \"sge_bootstrap.h\"");
    p.println("#include \"sge_gdi.h\"");
    p.println("#include \"sge_gdi_ctx.h\"");
    p.println("#include \"jgdi_common.h\"");
    p.println("");
    p.println("");
    p.println("");
    p.println("lDescr* get_descr(const char* name) {");
    p.print("");
    
   for (String name : cullDef.getObjectNames()) {
     com.sun.grid.cull.CullObject cullObj = cullDef.getCullObject(name);
p.println("");
    p.print("   if( strcmp(name, \"");
    p.print(name);p.print("\") == 0 ) return ");
    p.print(name);p.println(";");
    p.print("");
       
  } // end of for
p.println("");
    p.println("   return NULL;");
    p.println("}");
    p.println("");
    p.println("lDescr* get_descr_for_classname(const char* classname ) {");
    p.println("");
    p.print("");
    
   for (String name : cullDef.getObjectNames()) {
     com.sun.grid.cull.CullObject cullObj = cullDef.getCullObject(name);
     String classname = jh.getFullClassName(cullObj);
     if (cullObj.getParentName() != null) {
        name = cullObj.getParentName();
     }
p.println("");
    p.print("   if (strcmp(classname, \"");
    p.print(classname);p.print("\") == 0) return ");
    p.print(name);p.println(";");
    p.print("");
       
  } // end of for
p.println("");
    p.println("   return NULL;");
    p.println("}");
    p.println("");
    p.println("");
    p.print("");
    
   for (String name : cullDef.getEnumNames()) {
      com.sun.grid.cull.CullEnum aEnum = cullDef.getEnum(name);
p.println("");
    p.print("int get_");
    p.print(name);p.print("(const char* a_");
    p.print(name);p.println("_name) {");
    p.print("");
    
    for (String elemName : aEnum.getElems()) {
p.println("");
    p.print("    if (strcmp(\"");
    p.print(elemName);p.print("\", a_");
    p.print(name);p.print("_name) == 0) return ");
    p.print(elemName);p.println(";");
    p.print("");
    
    } // end of inner for
  } // end of for
p.println("");
    p.println("");
    p.println("const char* get_classname_for_descr(const lDescr *descr) {");
    p.print("");
    
   for (String name : cullDef.getObjectNames()) {
     com.sun.grid.cull.CullObject cullObj = cullDef.getCullObject(name);
     String classname = jh.getFullClassName(cullObj);
     classname = classname.replace('.', '/');
     if(cullObj.getParentName() != null) {
        name = cullObj.getParentName();
     }
p.println("");
    p.print("   if (descr == ");
    p.print(name);p.println(") {");
    p.print("      return \"");
    p.print(classname);p.println("\";");
    p.println("   }");
    p.print("");
       
  } // end of for
p.println("");
    p.println("   return NULL;");
    p.println("}");
    p.println("");
    p.println("int get_master_list_for_classname(const char* classname) {");
    p.print("");
    
   for (String name : cullDef.getObjectNames()) {  
     com.sun.grid.cull.CullObject cullObj = cullDef.getCullObject(name);
     String classname = jh.getFullClassName(cullObj);
p.println("");
    p.print("   if( strcmp(classname, \"");
    p.print(classname);p.println("\") == 0 ) {");
    p.print("");
    
     if(cullObj.isRootObject()) {
p.println("");
    p.print("     return ");
    p.print(cullObj.getListName());p.println(";");
    p.print("");
       } else { p.println("     ");
    p.println("     return -1;");
    p.print("");
       } p.println("      ");
    p.println("   }");
    p.print("");
       
  } // end of for
p.println("");
    p.println("   return -1;");
    p.println("}");
  } // end of print
} // end of class c_jgdi_mapping_jsp
