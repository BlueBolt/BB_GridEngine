public class c_bean_wrapper_h_jsp implements com.sun.grid.cull.template.Template {
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
    
  java.beans.BeanInfo beanInfo = (java.beans.BeanInfo)params.get("beanInfo");
  Class beanClass = beanInfo.getBeanDescriptor().getBeanClass();
  String classname = beanInfo.getBeanDescriptor().getBeanClass().getName();
  
  {
      int i = classname.lastIndexOf('.');
      if(i>0) {
          classname = classname.substring(i+1);
      }
  }  
  classname = classname.replace('$', '_');
  java.beans.PropertyDescriptor [] props = beanInfo.getPropertyDescriptors();
  com.sun.grid.javaconv.CWrapperHelper ch = new com.sun.grid.javaconv.CWrapperHelper(beanInfo.getBeanDescriptor().getBeanClass());
p.println("");
    p.print("/* ==== ");
    p.print(classname);p.println(" ====================== */");
    p.print("   jclass ");
    p.print(classname);p.println("_find_class(JNIEnv *env, lList** alpp);");
    p.print("");
      
  // ---------------------------------------------------------------------------
  // ------------ CONSTRUCTORS -------------------------------------------------
  // ---------------------------------------------------------------------------
  for (String constructorName : ch.getConstructorNames()) {
      java.lang.reflect.Constructor constructor = ch.getConstructor(constructorName);
p.print("   jgdi_result_t ");
    p.print(classname);p.print("_");
    p.print(constructorName);p.print("(JNIEnv *env, jobject*obj ");
    
      Class [] parameters = constructor.getParameterTypes();
      for(int i = 0; i < parameters.length; i++) {
         if( String.class.equals(parameters[i])) {
            /* For strings we want a const char* not a jstring */
          p.print(", const char* p");
    p.print(i);p.print(" ");
    
         } else {
         p.print(", ");
    p.print(ch.getCType(parameters[i]));p.print(" p");
    p.print(i);p.print(" ");
    
         }
      }
p.println(", lList **alpp);");
    p.print("");
       } // end of for
  // ---------------------------------------------------------------------------
  // ------------ Static Fields ------------------------------------------------
  // ---------------------------------------------------------------------------
  for (String fieldName : ch.getStaticFieldNames()) {
    java.lang.reflect.Field field = ch.getStaticField(fieldName);
p.print("   jgdi_result_t ");
    p.print(classname);p.print("_static_");
    p.print(fieldName);p.print("(JNIEnv *env, ");
    p.print(ch.getCType(field.getType()));p.println(" *res, lList **alpp);");
    p.print("");
        
 } // end of for
  // ---------------------------------------------------------------------------
  // ------------ Static METHODS -----------------------------------------------
  // ---------------------------------------------------------------------------
  for (String methodName : ch.getStaticMethodNames()) { 
      java.lang.reflect.Method method = ch.getStaticMethod(methodName);
p.print("   jgdi_result_t ");
    p.print(classname);p.print("_static_");
    p.print(methodName);p.print("(JNIEnv *env");
    
      Class [] parameters = method.getParameterTypes();      
      for(int i = 0; i < parameters.length; i++) {
         if( String.class.equals(parameters[i])) {
          p.print(", const char* p");
    p.print(i);p.print(" ");
    
         } else {
         p.print(", ");
    p.print(ch.getCType(parameters[i]));p.print(" p");
    p.print(i);p.print(" ");
    
         }
      }
      if (!Void.TYPE.equals(method.getReturnType())) {
         p.print(", ");
    p.print(ch.getCType(method.getReturnType()));p.print("");
    
           p.print("* result");
    
         if(method.getReturnType().isArray()) {
           p.print(", int* len");
      
         }
      }
p.println(", lList **alpp);");
    p.print("");
       } // end of for
  // ---------------------------------------------------------------------------
  // ------------ METHODS ------------------------------------------------------
  // ---------------------------------------------------------------------------
  for (String methodName : ch.getMethodNames()) {
     java.lang.reflect.Method method = ch.getMethod(methodName);
p.print("   jgdi_result_t ");
    p.print(classname);p.print("_");
    p.print(methodName);p.print("(JNIEnv *env, ");
    p.print(ch.getCType(beanClass));p.print(" obj ");
    
      Class [] parameters = method.getParameterTypes();
      for(int i = 0; i < parameters.length; i++) {
         if( String.class.equals(parameters[i])) {
          p.print(", const char* p");
    p.print(i);p.print(" ");
    
         } else {
         p.print(", ");
    p.print(ch.getCType(parameters[i]));p.print(" p");
    p.print(i);p.print(" ");
    
         }
      }
      if (!Void.TYPE.equals(method.getReturnType())) {
         p.print(", ");
    p.print(ch.getCType(method.getReturnType()));p.print("");
    
           p.print("* result");
    
         if(method.getReturnType().isArray()) {
           p.print(", int* len");
      
         }
      }
p.println(", lList **alpp);");
    p.print("");
       } // end of for
} // end of print
} // end of class c_bean_wrapper_h_jsp
