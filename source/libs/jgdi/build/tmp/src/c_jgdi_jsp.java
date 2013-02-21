public class c_jgdi_jsp implements com.sun.grid.cull.template.Template {
  public void print(final com.sun.grid.cull.Printer p, final java.util.Map params) {
    p.print("");
    
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
p.println("");
    p.println("");
    p.print("");
    
   com.sun.grid.cull.JavaHelper jh = (com.sun.grid.cull.JavaHelper)params.get("javaHelper");
   com.sun.grid.cull.CullDefinition cullDef = (com.sun.grid.cull.CullDefinition)params.get("cullDef");
   com.sun.grid.cull.CullObject cullObj = (com.sun.grid.cull.CullObject)params.get("cullObj");
   
   class CGDIGenerator {
       
       String fullClassname;
       String classname;
       String listname;
       String cullname;
       
       public CGDIGenerator(String fullClassname, String classname, String listname, String cullname) {
           this.fullClassname = fullClassname.replace('.','/');
           this.classname = classname;
           this.listname = listname;
           this.cullname = cullname;
       } 
       
       
       public void genListMethod() {
           
           String methodName = "Java_com_sun_grid_jgdi_jni_JGDIImpl_fill" + classname + "List";
p.println("           ");
    p.println("/*");
    p.println(" * Class:     com_sun_grid_jgdi_jni_JGDIImpl");
    p.print(" * Method:    get");
    p.print(classname);p.println("List");
    p.println(" */");
    p.print("JNIEXPORT void JNICALL ");
    p.print(methodName);p.println("WithAnswer(JNIEnv *env, jobject jgdi, jobject list, jobject filter, jobject answers) {");
    p.println("   ");
    p.print("   DENTER(TOP_LAYER, \"");
    p.print(methodName);p.println("\");");
    p.print("   jgdi_fill(env, jgdi, list, filter, \"");
    p.print(fullClassname);p.print("\", ");
    p.print(listname);p.print(", ");
    p.print(cullname);p.println(", answers);");
    p.println("   DEXIT;");
    p.println("}");
    p.print("");
               
       } // end of genListMethod
       
       
       public void genAddMethod() {
         String methodName = "Java_com_sun_grid_jgdi_jni_JGDIImpl_add" + classname;
p.println("");
    p.println("/* -------------- ADD ------------------------------------------------------- */");
    p.println("/*");
    p.println(" * Class:     com_sun_grid_jgdi_jni_JGDIImpl");
    p.print(" * Method:    add");
    p.print(classname);p.println("");
    p.print(" * Signature: (L");
    p.print(fullClassname);p.println(";)V");
    p.println(" */");
    p.print("JNIEXPORT void JNICALL ");
    p.print(methodName);p.println("(JNIEnv *env, jobject jgdi, jobject jobj)");
    p.println("{");
    p.print("   DENTER(TOP_LAYER, \"");
    p.print(methodName);p.println("\");");
    p.print("   jgdi_add(env, jgdi, jobj, \"");
    p.print(fullClassname);p.print("\", ");
    p.print(listname);p.print(", ");
    p.print(cullname);p.println(", NULL);");
    p.println("   DEXIT;");
    p.println("}");
    p.println("");
    p.println("/*");
    p.println(" * Class:     com_sun_grid_jgdi_jni_JGDIImpl");
    p.print(" * Method:    add");
    p.print(classname);p.println("WithAnswer");
    p.print(" * Signature: (L");
    p.print(fullClassname);p.println(";L/java/util/List;)V");
    p.println(" */");
    p.print("JNIEXPORT void JNICALL ");
    p.print(methodName);p.println("WithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)");
    p.println("{");
    p.print("   DENTER(TOP_LAYER, \"");
    p.print(methodName);p.println("WithAnswer\");");
    p.print("   jgdi_add(env, jgdi, jobj, \"");
    p.print(fullClassname);p.print("\", ");
    p.print(listname);p.print(", ");
    p.print(cullname);p.println(", answers);");
    p.println("   DEXIT;");
    p.println("}");
    p.print("");
    
       } // end of genAddMethod     

       public void genDeleteMethod() {
         String methodName = "Java_com_sun_grid_jgdi_jni_JGDIImpl_delete" + classname;
p.println("");
    p.println("/* -------------- Delete ------------------------------------------------------- */           ");
    p.println("/*");
    p.println(" * Class:     com_sun_grid_jgdi_jni_JGDIImpl");
    p.print(" * Method:    delete");
    p.print(classname);p.println("");
    p.print(" * Signature: (L");
    p.print(fullClassname);p.println(";)V");
    p.println(" */");
    p.print("JNIEXPORT void JNICALL ");
    p.print(methodName);p.println("(JNIEnv *env, jobject jgdi, jobject jobj)");
    p.println("{");
    p.print("   DENTER(TOP_LAYER, \"");
    p.print(methodName);p.println("\");");
    p.print("   jgdi_delete(env, jgdi, jobj, \"");
    p.print(fullClassname);p.print("\", ");
    p.print(listname);p.print(", ");
    p.print(cullname);p.println(", false, NULL);");
    p.println("   DEXIT;");
    p.println("}");
    p.println("");
    p.println("/*");
    p.println(" * Class:     com_sun_grid_jgdi_jni_JGDIImpl");
    p.print(" * Method:    delete");
    p.print(classname);p.println("WithAnswer");
    p.print(" * Signature: (L");
    p.print(fullClassname);p.println(";L/java/util/List;)V");
    p.println(" */");
    p.print("JNIEXPORT void JNICALL ");
    p.print(methodName);p.println("WithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)");
    p.println("{");
    p.print("   DENTER(TOP_LAYER, \"");
    p.print(methodName);p.println("WithAnswer\");");
    p.print("   jgdi_delete(env, jgdi, jobj, \"");
    p.print(fullClassname);p.print("\", ");
    p.print(listname);p.print(", ");
    p.print(cullname);p.println(", false, answers);");
    p.println("   DEXIT;");
    p.println("}");
    p.println("");
    p.println("/*");
    p.println(" * Class:     com_sun_grid_jgdi_jni_JGDIImpl");
    p.print(" * Method:    delete");
    p.print(classname);p.println("sWithAnswer");
    p.println(" * Signature: ([Ljava/lang/Object;ZL/java/util/List;)V");
    p.println(" */");
    p.print("JNIEXPORT void JNICALL ");
    p.print(methodName);p.println("sWithAnswer(JNIEnv *env, jobject jgdi, jobjectArray jobj_array, jboolean forced, jobject options, jobject answers)");
    p.println("{");
    p.print("   DENTER(TOP_LAYER, \"");
    p.print(methodName);p.println("sWithAnswer\");");
    p.print("   jgdi_delete_array(env, jgdi, jobj_array, \"");
    p.print(fullClassname);p.print("\", ");
    p.print(listname);p.print(", ");
    p.print(cullname);p.println(", forced, options, answers);");
    p.println("   DEXIT;");
    p.println("}");
    p.println("");
    p.print("");
                          
       } // end of genDeleteMethod
       
       public void genUpdateMethod() {
           
          String methodName = "Java_com_sun_grid_jgdi_jni_JGDIImpl_update" + classname;
p.println("");
    p.println("/* -------------- Update ------------------------------------------------------- */");
    p.println("/*");
    p.println(" * Class:     com_sun_grid_jgdi_jni_JGDIImpl");
    p.print(" * Method:    update");
    p.print(classname);p.println("");
    p.print(" * Signature: (L");
    p.print(fullClassname);p.println(";)V");
    p.println(" */");
    p.print("JNIEXPORT void JNICALL ");
    p.print(methodName);p.println("(JNIEnv *env, jobject jgdi, jobject jobj)");
    p.println("{");
    p.print("   DENTER(TOP_LAYER, \"");
    p.print(methodName);p.println("\");");
    p.print("   jgdi_update(env, jgdi, jobj, \"");
    p.print(fullClassname);p.print("\", ");
    p.print(listname);p.print(", ");
    p.print(cullname);p.println(", NULL);");
    p.println("   DEXIT;");
    p.println("}");
    p.println("");
    p.println("/*");
    p.println(" * Class:     com_sun_grid_jgdi_jni_JGDIImpl");
    p.print(" * Method:    update");
    p.print(classname);p.println("WithAnswer");
    p.print(" * Signature: (L");
    p.print(fullClassname);p.println(";, L/java/util/List;)V");
    p.println(" */");
    p.print("JNIEXPORT void JNICALL ");
    p.print(methodName);p.println("WithAnswer(JNIEnv *env, jobject jgdi, jobject jobj, jobject answers)");
    p.println("{");
    p.print("   DENTER(TOP_LAYER, \"");
    p.print(methodName);p.println("\");");
    p.print("   jgdi_update(env, jgdi, jobj, \"");
    p.print(fullClassname);p.print("\", ");
    p.print(listname);p.print(", ");
    p.print(cullname);p.println(", answers);");
    p.println("   DEXIT;");
    p.println("}");
    p.print("");
               
       } // end of genUpdateMethod
   } // end of class CGDIGenerator
   
   // -------------------------------------------------------------------------
   // Build the generators 
   // -------------------------------------------------------------------------

   java.util.List<CGDIGenerator> generators = new java.util.ArrayList<CGDIGenerator>();
   
   if (cullObj == null ) {
     throw new IllegalStateException("param cullObj not found");
   }
   if (cullObj.getIdlName() == null ) {
     throw new IllegalStateException("cullObj " + cullObj.getName() + " is has no idl name");
   }
   if (cullDef == null ) {
     throw new IllegalStateException("param cullDef not found");
   }
   if (!cullObj.isRootObject()) {
      return;
   }
   String listname = cullObj.getListName();

   if (listname == null) {
     // we not a ILISTDEF, return
     return;
   }
   {
   CGDIGenerator gen = null;
   
   if (cullObj.getParentName() != null) {      
      gen = new CGDIGenerator(jh.getFullClassName(cullObj), cullObj.getIdlName(), 
                              listname, cullObj.getParentName());
   } else {
      gen = new CGDIGenerator(jh.getFullClassName(cullObj), cullObj.getIdlName(), 
                              listname, cullObj.getName());
   }
   generators.add(gen);
   }
p.println("");
    p.print("");
    
   boolean first = true;
   for (CGDIGenerator gen : generators) {
      if (first) {
         first = false;
p.println("");
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
    p.println("#include \"sge_gdi_ctx.h\"");
    p.println("#include \"cl_errors.h\"");
    p.println("#include \"sge_log.h\"");
    p.println("#include \"sge_error_class.h\"");
    p.println("#include \"jgdi_common.h\"");
    p.println("#include \"jgdi.h\"");
    p.println("");
    p.println("#define JGDI_DEBUG");
    p.println("");
    p.print("");
    }
      gen.genListMethod();
      gen.genAddMethod();
      gen.genDeleteMethod();
      gen.genUpdateMethod();
   }
   
p.println("");
    p.println("");
    p.println("");
  } // end of print
} // end of class c_jgdi_jsp
