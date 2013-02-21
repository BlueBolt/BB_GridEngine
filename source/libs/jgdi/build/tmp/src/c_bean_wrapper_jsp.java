public class c_bean_wrapper_jsp implements com.sun.grid.cull.template.Template {
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
  String classname = beanClass.getName();
  String fullClassname = classname.replace('.','/');
  {
      int i = classname.lastIndexOf('.');
      if (i>0) {
          classname = classname.substring(i+1);
      }
  }
  classname = classname.replace('$', '_');
  
  // skip the inner EventFactory classes
  if (classname.startsWith("EventFactory_")) {
     System.out.println("Skipping " + classname);
     return;
  }
  
  java.beans.PropertyDescriptor[] props = beanInfo.getPropertyDescriptors();
  com.sun.grid.javaconv.CWrapperHelper ch = new com.sun.grid.javaconv.CWrapperHelper(beanInfo.getBeanDescriptor().getBeanClass());
p.println("");
    p.print("/*==== BEGIN  ");
    p.print(fullClassname);p.println(" */");
    p.println("");
    p.print("jclass ");
    p.print(classname);p.println("_find_class(JNIEnv *env, lList** alpp) {");
    p.println("   static jclass clazz = NULL;");
    p.println("");
    p.print("   DENTER(BASIS_LAYER, \"");
    p.print(classname);p.println("_find_class\");");
    p.println("   if (clazz == NULL) {");
    p.print("      clazz = find_class(env, \"");
    p.print(fullClassname);p.println("\", alpp);");
    p.println("   }");
    p.println("   DRETURN(clazz);");
    p.println("}");
    p.println("");
    p.print("");
    
  // ---------------------------------------------------------------------------
  // ------------ CONSTRUCTORS -------------------------------------------------
  // ---------------------------------------------------------------------------
  for (String constructorName: ch.getConstructorNames()) { 
      java.lang.reflect.Constructor constructor = ch.getConstructor(constructorName);
      Class[] parameters = constructor.getParameterTypes();
p.println("");
    p.print("jgdi_result_t ");
    p.print(classname);p.print("_");
    p.print(constructorName);p.print("(JNIEnv *env, jobject *obj ");
    
   
      for (int i = 0; i < parameters.length; i++) {
         if (String.class.equals(parameters[i])) {
            /* For strings we want a const char* not a jstring */
          p.print(", const char* p");
    p.print(i);p.print(" ");
    
         } else {
         p.print(", ");
    p.print(ch.getCType(parameters[i]));p.print(" p");
    p.print(i);p.print(" ");
    
         }
      } // end of inner for
p.println(", lList **alpp) {");
    p.println("   jgdi_result_t ret = JGDI_SUCCESS;");
    p.println("   static jmethodID mid = NULL;");
    p.print("   jclass clazz = NULL;");
    
   // For string parameters we need a local variable
   // which hold the jstring object
   for (int i = 0; i < parameters.length; i++) {
      if (String.class.equals(parameters[i])) {
   p.print("     jstring p");
    p.print(i);p.print("_obj = NULL;");
    
      }
   } // end of inner for   
   p.println("");
    p.print("   DENTER(BASIS_LAYER, \"");
    p.print(classname);p.print("_");
    p.print(constructorName);p.println("\");");
    p.println("      ");
    p.print("   clazz = ");
    p.print(classname);p.println("_find_class(env, alpp);");
    p.println("   if (clazz == NULL) {");
    p.println("      DRETURN(JGDI_ILLEGAL_STATE);");
    p.println("   }");
    p.println("   if (mid == NULL) {");
    p.println("      /* initialize the mid */");
    p.print("      mid = get_methodid(env, clazz, \"<init>\", \"");
    p.print(ch.getSignature(constructor));p.println("\", alpp);");
    p.println("      if (mid == NULL) {");
    p.println("         DRETURN(JGDI_ILLEGAL_STATE);");
    p.println("      }");
    p.print("   } ");
    
   // All string parameters comes a const char* 
   // we have to allocate the jstring object
   for (int i = 0; i < parameters.length; i++) {
      if (String.class.equals(parameters[i])) {
p.println("");
    p.print("   if ( p");
    p.print(i);p.println(" != NULL ) {");
    p.print("      p");
    p.print(i);p.print("_obj = (*env)->NewStringUTF(env, p");
    p.print(i);p.println(" ); ");
    p.print("   }");
    
      } // end of if
  } // end of inner for
p.println("   ");
    p.print("   *obj = (*env)->NewObject(env, clazz, mid ");
    
      for (int i = 0; i < parameters.length; i++) {
         if (String.class.equals(parameters[i])) {
            // For all string parameter we pass the jstring object
            // to the method call
          p.print(", p");
    p.print(i);p.print("_obj ");
    
         } else {
         p.print(", p");
    p.print(i);p.print(" ");
    
         }
      } // end of inner for
  p.println(");");
    p.println("   if (test_jni_error(env, \"call of constructor failed\", alpp)) {");
    p.println("      ret = JGDI_ILLEGAL_STATE;");
    p.println("   } ");
    p.println("   DRETURN(ret);");
    p.println("}");
    p.print("");
       
   } // end of for
   
  // ---------------------------------------------------------------------------
  // ------------ Static Fields ------------------------------------------------
  // ---------------------------------------------------------------------------
  for (String fieldName: ch.getStaticFieldNames()) {
    java.lang.reflect.Field field = ch.getStaticField(fieldName);
p.println("");
    p.print("jgdi_result_t ");
    p.print(classname);p.print("_static_");
    p.print(fieldName);p.print("(JNIEnv *env, ");
    p.print(ch.getCType(field.getType()));p.println(" *res, lList **alpp) {");
    p.println("   jgdi_result_t ret = JGDI_SUCCESS;");
    p.println("   jclass clazz = NULL;");
    p.println("   static jfieldID mid = NULL;");
    p.print("   /*static ");
    p.print(ch.getCType(field.getType()));p.print(" field = ");
    p.print(ch.getInitializer(field.getType()));p.println(";*/");
    p.println("   ");
    p.print("   DENTER(BASIS_LAYER, \"");
    p.print(classname);p.print("_static_");
    p.print(fieldName);p.println("\");");
    p.println("");
    p.println("   if (env == NULL) {");
    p.println("      answer_list_add(alpp, \"env is NULL\", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);");
    p.println("      DRETURN(JGDI_ILLEGAL_STATE);");
    p.println("   }");
    p.print("   clazz = ");
    p.print(classname);p.println("_find_class(env, alpp);");
    p.println("   if (clazz == NULL) {");
    p.print("      answer_list_add(alpp, \"class ");
    p.print(fullClassname);p.println(" not found\", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);");
    p.println("      DRETURN(JGDI_ILLEGAL_STATE);");
    p.println("   }");
    p.println("   if (mid == NULL) {");
    p.print("      mid = get_static_fieldid(env, clazz, \"");
    p.print(field.getName());p.print("\", \"");
    p.print(ch.getSignature(field));p.println("\", alpp);");
    p.println("      if (mid == NULL) {");
    p.println("         DRETURN(JGDI_ILLEGAL_STATE);");
    p.println("      }");
    p.println("   }   ");
    p.print("   *res = (*env)->");
    p.print(ch.getStaticGetFieldMethod(field));p.println("(env, clazz, mid);");
    p.print("   if (test_jni_error(env, \"");
    p.print(classname);p.print("_static_");
    p.print(fieldName);p.println(" failed\", alpp)) {");
    p.println("      ret = JGDI_ILLEGAL_STATE;");
    p.println("   }      ");
    p.println("   DRETURN(ret);");
    p.println("}");
    p.print("");
        
 } // end of for
   
   
  // ---------------------------------------------------------------------------
  // ------------ Static METHODS -----------------------------------------------
  // ---------------------------------------------------------------------------
  for (String methodName: ch.getStaticMethodNames()) {
      java.lang.reflect.Method method = ch.getStaticMethod(methodName);
      Class[] parameters = method.getParameterTypes();
p.println("");
    p.print("jgdi_result_t ");
    p.print(classname);p.print("_static_");
    p.print(methodName);p.print("(JNIEnv *env ");
       
      for (int i = 0; i < parameters.length; i++) {         
         if (String.class.equals(parameters[i])) {
            /* For strings we want a const char* not a jstring */
          p.print(", const char* p");
    p.print(i);p.print(" ");
    
         } else {
         p.print(", ");
    p.print(ch.getCType(parameters[i]));p.print(" p");
    p.print(i);p.print(" ");
    
         }
      }
      if (!Void.TYPE.equals(method.getReturnType())) {
         // Add a pointer to the result argument
         p.print(", ");
    p.print(ch.getCType(method.getReturnType()));p.print("* result");
    
         if(method.getReturnType().isArray()) {
           p.print(", int* len");
      
         }
      }
    // And finally we need the answer list
p.println(", lList **alpp) {");
    p.println("");
    p.println("   jgdi_result_t ret = JGDI_SUCCESS;");
    p.println("   static jmethodID mid = NULL; ");
    p.println("   static jclass clazz = NULL;");
    p.print("");
    
  // For string parameters we need a local variable
  // which hold the jstring object
  for(int i = 0; i < parameters.length; i++) {
     if( String.class.equals(parameters[i])) {
p.print("   jstring p");
    p.print(i);p.print("_obj = NULL;");
    
     }
  }
  // For non void methods we temporary store the result
  // of the java method in a local variable
  if (!Void.TYPE.equals(method.getReturnType())) {
     if (method.getReturnType().isArray()) { p.println("");
    p.println("   jobject temp = NULL;    ");
    p.print("");
    } else {
p.println("");
    p.print("   ");
    p.print(ch.getCType(method.getReturnType()));p.print(" temp = ");
    p.print(ch.getInitializer(method.getReturnType()));p.println(";");
    p.print("");
    
    }
  } 
p.println("");
    p.print("   DENTER(BASIS_LAYER, \"");
    p.print(classname);p.print("_static_");
    p.print(methodName);p.println("\");");
    p.print("");
       
  if (!Void.TYPE.equals(method.getReturnType())) {
p.println("         ");
    p.println("   if (result == NULL ) {");
    p.println("      answer_list_add(alpp, \"result is NULL\", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);");
    p.println("      DRETURN(JGDI_ILLEGAL_STATE);");
    p.println("   }");
    p.println("   /* We set the result always to the default value */");
    p.print("");
       
   if(method.getReturnType().isArray()) { p.println("   *result = NULL;    ");
    p.print("");
       } else {
p.print("   *result = ");
    p.print(ch.getInitializer(method.getReturnType()));p.println(";");
    p.print("");
        }
  }  p.println("   ");
    p.println("  if (mid == NULL) {");
    p.print("     if (JGDI_SUCCESS != get_static_method_id_for_fullClassname(env, &clazz, &mid, \"");
    p.print(fullClassname);p.print("\", \"");
    p.print(method.getName());p.print("\", \"");
    p.print(ch.getSignature(method));p.println("\", alpp)) {");
    p.println("        DRETURN(JGDI_ILLEGAL_STATE);");
    p.println("     }");
    p.print("   }");
    
   
   // All string parameters comes a const char* 
   // we have to allocate the jstring object
   for (int i = 0; i < parameters.length; i++) {
      if (String.class.equals(parameters[i])) {
p.print("   if (p");
    p.print(i);p.println(" != NULL) {");
    p.print("      p");
    p.print(i);p.print("_obj = (*env)->NewStringUTF(env, p");
    p.print(i);p.println("); ");
    p.print("   }");
    
      } // end of if
  } // end of for
      
  if (!Void.TYPE.equals(method.getReturnType())) {
p.print("   temp =");
    
  } else { p.println("");
    p.print("  ");
    
  }
p.println(" ");
    p.print(" (*env)->");
    p.print(ch.getStaticCallMethod(method.getReturnType()));p.print("(env, clazz, mid ");
    
      /* Add all parameter to the method call */
      for (int i = 0; i < parameters.length; i++) {  
         if (String.class.equals(parameters[i])) {
            // For all string parameter we pass the jstring object
            // to the method call
          p.print(", p");
    p.print(i);p.print("_obj ");
    
         } else {
         p.print(", p");
    p.print(i);p.print(" ");
    
         }
      }
  p.println(");  ");
    p.print("   if (test_jni_error(env, \"");
    p.print(classname);p.print("_");
    p.print(methodName);p.println(" failed\", alpp)) {");
    p.println("      ret = JGDI_ILLEGAL_STATE;");
    p.print("");
      if (!Void.TYPE.equals(method.getReturnType())) {
       if (method.getReturnType().isArray()) {
p.println("      temp = NULL;");
    p.print("");
         } else { p.print("      temp = ");
    p.print(ch.getInitializer(method.getReturnType()));p.println(";        ");
    p.print("");
           }    
    }
p.println("      ");
    p.print("   } ");
    
  if (!Void.TYPE.equals(method.getReturnType())) {
     // for non void method store the temporary result
     // in the result pointer
  p.println(" else {");
    p.print("  ");
    
     if(method.getReturnType().isArray()) {
         
       Class realType = method.getReturnType().getComponentType();
       String realCType = ch.getCType(realType);
  p.println("");
    p.println("       if (temp == NULL) {");
    p.println("          *result = NULL;");
    p.println("          *len = 0;");
    p.println("       } else {");
    p.println("           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);");
    p.println("           if (test_jni_error(env, \"Can not get the array length of the result\", alpp)) {");
    p.println("              ret = JGDI_ILLEGAL_STATE;");
    p.println("           } if (arrayLen > 0) {");
    p.print("               ");
    p.print(realCType);p.println(" *array = NULL;");
    p.println("               int i = 0;");
    p.print("               ");
     if(realType.isPrimitive()) { p.println("");
    p.println("               jboolean isCopy = false;");
    p.print("               ");
    p.print(realCType);p.println(" *tmpArray = NULL;");
    p.println("               ");
    p.print("               tmpArray = (*env)->");
    p.print(ch.getGetArrayElementsMethod(realType));p.print("(env, (");
    p.print(realCType);p.println("Array)temp, &isCopy);");
    p.println("               if (test_jni_error(env, \"Can not get the array elements of the result\", alpp)) {");
    p.println("                  ret = JGDI_ILLEGAL_STATE;");
    p.println("               } else {");
    p.print("                  array = (");
    p.print(realCType);p.print("*)malloc(sizeof(");
    p.print(realCType);p.println(")* arrayLen);");
    p.println("                  for(i = 0; i < arrayLen; i++) {");
    p.println("                     array[i] = tmpArray[i];");
    p.println("                  }");
    p.print("                  (*env)->");
    p.print(ch.getReleaseArrayElementsMethod(realType));p.print("(env, (");
    p.print(realCType);p.println("Array)temp, tmpArray, JNI_ABORT);");
    p.println("                  if (test_jni_error(env, \"release the array elements of the result failed\", alpp)) {");
    p.println("                     free(array);");
    p.println("                  } else {");
    p.println("                     *result = array;");
    p.println("                     *len = arrayLen;");
    p.println("                  }");
    p.println("               }");
    p.print("               ");
     } else { p.println("");
    p.print("               array = (");
    p.print(realCType);p.print("*)malloc(sizeof(");
    p.print(realCType);p.println(")* arrayLen);");
    p.println("               for(i = 0; i < arrayLen; i++) {");
    p.println("                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); ");
    p.println("                  if (test_jni_error(env, \"Can not get object from array\", alpp)) {");
    p.println("                     free(array);");
    p.println("                     array = NULL;");
    p.println("                     break;");
    p.println("                  }");
    p.println("               }");
    p.println("               if(array != NULL) {");
    p.println("                  *result = array;");
    p.println("                  *len = arrayLen;");
    p.println("               }");
    p.print("               ");
     } p.println("");
    p.println("          } else {");
    p.println("             *result = NULL;");
    p.println("             *len = 0;");
    p.println("          }");
    p.println("       }");
    p.print("");
         } else { p.println("");
    p.println("      *result = temp;");
    p.print("");
       } p.println("      ");
    p.println("   }");
    p.print("");
     } p.println("");
    p.println("   DRETURN(ret);");
    p.print("}");
    
   } // end of for
   
   
  // ---------------------------------------------------------------------------
  // ------------ METHODS ------------------------------------------------------
  // ---------------------------------------------------------------------------
  for (String methodName: ch.getMethodNames()) {
      java.lang.reflect.Method method = ch.getMethod(methodName);
      Class[] parameters = method.getParameterTypes();
p.println("");
    p.print("jgdi_result_t ");
    p.print(classname);p.print("_");
    p.print(methodName);p.print("(JNIEnv *env, ");
    p.print(ch.getCType(beanClass));p.print(" obj ");
    
   
      for (int i = 0; i < parameters.length; i++) {         
         if (String.class.equals(parameters[i])) {
            /* For strings we want a const char* not a jstring */
          p.print(", const char* p");
    p.print(i);p.print(" ");
    
         } else {
         p.print(", ");
    p.print(ch.getCType(parameters[i]));p.print(" p");
    p.print(i);p.print(" ");
    
         }
      }
      if (!Void.TYPE.equals(method.getReturnType())) {
         // Add a pointer to the result argument
         p.print(", ");
    p.print(ch.getCType(method.getReturnType()));p.print("* result");
    
         if(method.getReturnType().isArray()) {
           p.print(", int* len");
      
         }
      }
    // And finally we need the answer lsit
p.println(", lList **alpp) {");
    p.println("   jgdi_result_t ret = JGDI_SUCCESS;");
    p.println("   static jmethodID mid = NULL;");
    p.print("");
       
  // For string parameters we need a local variable
  // which hold the jstring object
  for (int i = 0; i < parameters.length; i++) {
     if (String.class.equals(parameters[i])) {
p.println("");
    p.print("   jstring p");
    p.print(i);p.print("_obj = NULL;");
    
     }
  }
  // For non void methods we temporary store the result
  // of the java method in a local variable
  if (!Void.TYPE.equals(method.getReturnType())) {
     if (method.getReturnType().isArray()) { p.println("");
    p.println("   jobject temp = NULL;    ");
    p.print("");
       } else {
p.println("");
    p.print("   ");
    p.print(ch.getCType(method.getReturnType()));p.print(" temp = ");
    p.print(ch.getInitializer(method.getReturnType()));p.println(";");
    p.print("");
    
    }
  } 
p.println("");
    p.print("   DENTER(BASIS_LAYER, \"");
    p.print(classname);p.print("_");
    p.print(methodName);p.println("\");");
    p.print("");
       
  if (!Void.TYPE.equals(method.getReturnType())) {
p.println("         ");
    p.println("   if (result == NULL ) {");
    p.println("      answer_list_add(alpp, \"result is NULL\", STATUS_EUNKNOWN, ANSWER_QUALITY_ERROR);");
    p.println("      DRETURN(JGDI_ILLEGAL_STATE);");
    p.println("   }");
    p.println("   /* We set the result always to the default value */");
    p.print("");
     
   if (method.getReturnType().isArray()) { p.println("   *result = NULL;    ");
    p.print("");
       } else { 
p.print("   *result = ");
    p.print(ch.getInitializer(method.getReturnType()));p.println(";");
    p.print("");
    
    }
  }p.println("   ");
    p.println("  if (mid == NULL) {");
    p.print("     if (JGDI_SUCCESS != get_method_id_for_fullClassname(env, obj, &mid, \"");
    p.print(fullClassname);p.print("\", \"");
    p.print(method.getName());p.print("\", \"");
    p.print(ch.getSignature(method));p.println("\", alpp)) {");
    p.println("        DRETURN(JGDI_ILLEGAL_STATE);");
    p.println("     }");
    p.println("   }");
    p.print("");
    
   // All string parameters comes a const char* 
   // we have to allocate the jstring object
   for (int i = 0; i < parameters.length; i++) {
      if (String.class.equals(parameters[i])) {
p.println("");
    p.print("   if (p");
    p.print(i);p.println(" != NULL) {");
    p.print("      p");
    p.print(i);p.print("_obj = (*env)->NewStringUTF(env, p");
    p.print(i);p.println(" ); ");
    p.print("   }");
    
      } // end of if
  } // end of for
      
  if (!Void.TYPE.equals(method.getReturnType())) {
p.println("");
    p.print("   temp =");
    
  } else { p.println("");
    p.print("  ");
    
  }
p.print(" (*env)->");
    p.print(ch.getCallMethod(method.getReturnType()));p.print("(env, obj, mid");
    
      // Add all parameter to the method call 
      for (int i = 0; i < parameters.length; i++) {  
         if (String.class.equals(parameters[i])) {
            // For all string parameter we pass the jstring object
            // to the method call
          p.print(", p");
    p.print(i);p.print("_obj");
    
         } else {
         p.print(", p");
    p.print(i);p.print("");
    
         }
      }
  p.println(");  ");
    p.print("   if (test_jni_error(env, \"");
    p.print(classname);p.print("_");
    p.print(methodName);p.println(" failed\", alpp)) {");
    p.println("      ret = JGDI_ILLEGAL_STATE;");
    p.print("");
      if (!Void.TYPE.equals(method.getReturnType())) {
       if (method.getReturnType().isArray()) {
p.println("      temp = NULL;");
    p.print("");
         } else { p.print("      temp = ");
    p.print(ch.getInitializer(method.getReturnType()));p.println(";        ");
    p.print("");
           }    
    }
p.println("      ");
    p.print("   } ");
    
  if (!Void.TYPE.equals(method.getReturnType())) {
     // for non void method store the temporary result
     // in the result pointer
     if (method.getReturnType().isArray()) {
       Class realType = method.getReturnType().getComponentType();
       String realCType = ch.getCType(realType);
  p.println("");
    p.println("       if (temp == NULL) {");
    p.println("          *result = NULL;");
    p.println("          *len = 0;");
    p.println("       } else {");
    p.println("           jint arrayLen = (*env)->GetArrayLength(env, (jarray)temp);");
    p.println("           if (test_jni_error(env, \"Can not get the array length of the result\", alpp)) {");
    p.println("              ret = JGDI_ILLEGAL_STATE;");
    p.println("           } if (arrayLen > 0) {");
    p.print("               ");
    p.print(realCType);p.println(" *array = NULL;");
    p.println("               int i = 0;");
    p.print("               ");
     if(realType.isPrimitive()) { p.println("");
    p.println("               jboolean isCopy = false;");
    p.print("               ");
    p.print(realCType);p.println(" *tmpArray = NULL;");
    p.print("               tmpArray = (*env)->");
    p.print(ch.getGetArrayElementsMethod(realType));p.print("(env, (");
    p.print(realCType);p.println("Array)temp, &isCopy);");
    p.println("               if (test_jni_error(env, \"Can not get the array elements of the result\", alpp)) {");
    p.println("                  ret = JGDI_ILLEGAL_STATE;");
    p.println("               } else {");
    p.print("                  array = (");
    p.print(realCType);p.print("*)malloc(sizeof(");
    p.print(realCType);p.println(")* arrayLen);");
    p.println("                  for(i = 0; i < arrayLen; i++) {");
    p.println("                     array[i] = tmpArray[i];");
    p.println("                  }");
    p.print("                  (*env)->");
    p.print(ch.getReleaseArrayElementsMethod(realType));p.print("(env, (");
    p.print(realCType);p.println("Array)temp, tmpArray, JNI_ABORT);");
    p.println("                  if (test_jni_error(env, \"release the array elements of the result failed\", alpp)) {");
    p.println("                     free(array);");
    p.println("                  } else {");
    p.println("                     *result = array;");
    p.println("                     *len = arrayLen;");
    p.println("                  }");
    p.println("               }");
    p.print("               ");
     } else { p.println("");
    p.print("               array = (");
    p.print(realCType);p.print("*)malloc(sizeof(");
    p.print(realCType);p.println(")* arrayLen);");
    p.println("               for(i = 0; i < arrayLen; i++) {");
    p.println("                  array[i] = (*env)->GetObjectArrayElement(env, (jobjectArray)temp, i); ");
    p.println("                  if (test_jni_error(env, \"Can not get object from array\", alpp)) {");
    p.println("                     free(array);");
    p.println("                     array = NULL;");
    p.println("                     break;");
    p.println("                  }");
    p.println("               }");
    p.println("               if(array != NULL) {");
    p.println("                  *result = array;");
    p.println("                  *len = arrayLen;");
    p.println("               }");
    p.print("               ");
     } p.println("");
    p.println("          } else {");
    p.println("             *result = NULL;");
    p.println("             *len = 0;");
    p.println("          }");
    p.println("       }");
    p.print("");
         } else { p.println("");
    p.println("      *result = temp;");
    p.print("");
         } p.println("      ");
    p.print("");
     } // end of !Void.TYPE p.println("");
    p.println("   DRETURN(ret);");
    p.print("}");
    
   } // end of for
p.println("");
    p.println("");
    p.print("/*==== END  ");
    p.print(fullClassname);p.println(" */");
  } // end of print
} // end of class c_bean_wrapper_jsp
