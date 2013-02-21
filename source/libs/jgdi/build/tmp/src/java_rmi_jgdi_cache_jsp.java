public class java_rmi_jgdi_cache_jsp implements com.sun.grid.cull.template.Template {
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
    p.println("/**");
    p.println(" *  Generated from java_rmi_jgdi_cache.jsp");
    p.println(" *  !!! DO NOT EDIT THIS FILE !!!");
    p.println(" */");
    p.print("");
    
  final com.sun.grid.cull.JavaHelper jh = (com.sun.grid.cull.JavaHelper)params.get("javaHelper");
  final com.sun.grid.cull.CullDefinition cullDef = (com.sun.grid.cull.CullDefinition)params.get("cullDef");
  class JGDIRMICacheGenerator extends com.sun.grid.cull.AbstractGDIGenerator {
     
     public JGDIRMICacheGenerator(com.sun.grid.cull.CullObject cullObject) {
        super(cullObject.getIdlName(),  jh.getClassName(cullObject), cullObject);
        addPrimaryKeys(cullObject, jh);
     }
     
     public void genImport() {
        if(!(cullObject.getType() == cullObject.TYPE_PRIMITIVE || 
             cullObject.getType() == cullObject.TYPE_MAPPED)) {
p.print("import com.sun.grid.jgdi.configuration.");
    p.print(classname);p.println(";        ");
    p.print("");
            
        }
     } // end of genImport
     
     public void genUpdateMethod() {
p.println("");
    p.println("");
    p.println("   /**");
    p.print("    *  Update ");
    p.print(getPrimaryKeyCount() == 0 ? "the" : "a");p.print(" ");
    p.print(name);p.println(" object");
    p.println("    *");
    p.print("    *  @param obj  the ");
    p.print(name);p.println(" object with the new values");
    p.println("    *  @throws RemoteException on any error");
    p.println("    */");
    p.print("   public void update");
    p.print(name);p.print("(");
    p.print(classname);p.println(" obj) throws RemoteException {");
    p.println("      try {");
    p.print("         jgdi.update");
    p.print(name);p.println("(obj);");
    p.println("      } catch( RemoteException re ) {");
    p.println("         handleError(re);");
    p.println("         throw re;");
    p.println("      }");
    p.print("");
    
      if (cullObject.hasGetListOperation()) {
p.println("     ");
    p.println("       finally {      ");
    p.print("         ");
    p.print(name);p.println("Cache = null;");
    p.println("       }");
    p.print("");
    
      }
p.println("      ");
    p.println("   }");
    p.print("");
    
    } // end of genUpdateMethod
     
    public void genGetMethod() {
p.println("      ");
    p.print("   private ");
    p.print(classname);p.print(" ");
    p.print(name);p.println("Cache = null;");
    p.println("   /**");
    p.print("    *   Get the <code>");
    p.print(name);p.println("</code> object.");
    p.print("    *   @return the <code>");
    p.print(name);p.println("</code> object");
    p.println("    *   @throws RemoteException on any error");
    p.println("    */");
    p.print("   public ");
    p.print(classname);p.print(" get");
    p.print(name);p.println("() throws RemoteException {");
    p.println("   ");
    p.println("      if ( jgdi == null ) {");
    p.println("         return null;");
    p.println("      }");
    p.print("      if (");
    p.print(name);p.println("Cache == null ) {");
    p.println("         try {");
    p.print("             ");
    p.print(name);p.print("Cache = jgdi.get");
    p.print(name);p.println("();");
    p.println("         } catch( RemoteException re ) {");
    p.println("            handleError(re);");
    p.println("            return null;");
    p.println("         }");
    p.println("      }");
    p.print("      return ");
    p.print(name);p.println("Cache;");
    p.println("   }");
    p.println("   ");
    p.print("");
     } // end of genGetMethod
    
   public void genGetListMethod() {
p.println("");
    p.print("   private List ");
    p.print(name);p.println("Cache = null;");
    p.println("   /**");
    p.print("    *   Get all <code>");
    p.print(name);p.println("</code> objects.");
    p.println("    *");
    p.print("    *   @return a list of <code>");
    p.print(name);p.println("</code> objects");
    p.println("    *   @throws RemoteException on any error");
    p.println("    */");
    p.print("   public List get");
    p.print(name);p.println("List() throws RemoteException {");
    p.println("   ");
    p.println("      if ( jgdi == null ) {");
    p.println("         return Collections.EMPTY_LIST;");
    p.println("      }");
    p.print("      if (");
    p.print(name);p.println("Cache == null ) {");
    p.println("         try {");
    p.print("             ");
    p.print(name);p.print("Cache = jgdi.get");
    p.print(name);p.println("List();");
    p.println("         } catch( RemoteException re ) {");
    p.println("            handleError(re);");
    p.println("            return Collections.EMPTY_LIST;");
    p.println("         }");
    p.println("      }");
    p.print("      return ");
    p.print(name);p.println("Cache;");
    p.println("   }");
    p.print("");
    
   } // end of genGetListMethod
   
   public void genAddMethod() {
p.println("");
    p.println("   /**");
    p.print("    *   Add a new <code>");
    p.print(name);p.println("</code> object.");
    p.println("    *");
    p.print("    *   @param obj the new <code>");
    p.print(name);p.println("</code> object");
    p.println("    *   @throws RemoteException on any error");
    p.println("    */");
    p.print("   public void add");
    p.print(name);p.print("(");
    p.print(classname);p.println(" obj) throws RemoteException {");
    p.println("     try {");
    p.print("        jgdi.add");
    p.print(name);p.println("(obj);");
    p.println("     } catch( RemoteException re ) {");
    p.println("        handleError(re);");
    p.println("        throw re;");
    p.print("     } ");
    
       
      if (cullObject.hasGetListOperation()) {
p.println(" finally {      ");
    p.print("         ");
    p.print(name);p.println("Cache = null;");
    p.println("       }");
    p.print("");
    
      }
p.println("      ");
    p.println("   }");
    p.println("   ");
    p.print("");
    
   } // end of genAddMethod
   
   public void genDeleteMethod() {
p.println("  ");
    p.println("   ");
    p.println("   /**");
    p.print("    *   Add a delete <code>");
    p.print(name);p.println("</code> object.");
    p.println("    *");
    p.print("    *   @param obj the <code>");
    p.print(name);p.println("</code> object with the primary key information");
    p.println("    *   @throws RemoteException on any error");
    p.println("    */");
    p.print("   public void delete");
    p.print(name);p.print("(");
    p.print(classname);p.println(" obj) throws RemoteException {");
    p.println("     try {");
    p.print("        jgdi.delete");
    p.print(name);p.println("(obj);");
    p.println("     } catch( RemoteException re ) {");
    p.println("        handleError(re);");
    p.println("        throw re;");
    p.print("     } ");
    
       
      if (cullObject.hasGetListOperation()) {
p.println(" finally {      ");
    p.print("         ");
    p.print(name);p.println("Cache = null;");
    p.println("       }");
    p.print("");
    
      }
p.println("");
    p.println("   }");
    p.print("");
    
  } // end of genDeleteMethod
  public void genDeleteByPrimaryKeyMethod() {
p.println("  ");
    p.println("   /**");
    p.print("    *   Delete a a <code>");
    p.print(name);p.println("</code> object by its primary key");
    p.println("    *");
    p.print("");
    
   boolean first = true;
   for (java.util.Map.Entry<String, String> entry: primaryKeys.entrySet()) {
      String pkName = entry.getKey();
p.print("    *   @param ");
    p.print(pkName);p.print("  the ");
    p.print(pkName);p.print(" of the <code>");
    p.print(name);p.print("</code> object ");
          
   }
p.println("      ");
    p.println("    *   @throws RemoteException on any error");
    p.println("    */");
    p.print("   public void delete");
    p.print(name);p.print("(");
    
   first = true;
   for (java.util.Map.Entry<String, String> entry: primaryKeys.entrySet()) {
      String pkName = entry.getKey();
      String pkType = entry.getValue();
      if(first) {
         first = false;
      } else {
         p.print(", ");
    
      }
      p.print(" ");
    p.print(pkType);p.print(" ");
    p.print(pkName);p.print("");
    
   }
   p.println(") throws RemoteException {");
    p.println("   ");
    p.println("      try {");
    p.print("          jgdi.delete");
    p.print(name);p.print("(");
    
   first = true;
   for (java.util.Map.Entry<String, String> entry: primaryKeys.entrySet()) {
      String pkName = entry.getKey();
      if(first) {
         first = false;
      } else {
         p.print(", ");
    
      }
      p.print("");
    p.print(pkName);p.print("");
    
   }             
              p.println(");");
    p.println("       } catch( RemoteException re ) {");
    p.println("          handleError(re);");
    p.println("       }            ");
    p.println("   }");
    p.print("");
            
     } // end of genDeleteByPrimaryKeyMethod
  public void genGetByPrimaryKeyMethod() {
p.println("  ");
    p.println("   /**");
    p.print("    *   Get a a <code>");
    p.print(name);p.println("</code> object by its primary key");
    p.println("    *");
    p.print("");
    
   boolean first = true;
   for (java.util.Map.Entry<String, String> entry: primaryKeys.entrySet()) {
      String pkName = entry.getKey();
p.print("    *   @param ");
    p.print(pkName);p.print("  the ");
    p.print(pkName);p.print(" of the <code>");
    p.print(name);p.print("</code> object ");
          
   }
p.println("      ");
    p.print("    *   @return the <code>");
    p.print(name);p.println("</code> object or <code>null</code>");
    p.println("    *   @throws RemoteException on any error");
    p.println("    */");
    p.print("   public ");
    p.print(classname);p.print(" get");
    p.print(name);p.print("(");
    
   first = true;
   for (java.util.Map.Entry<String, String> entry: primaryKeys.entrySet()) {
      String pkName = entry.getKey();
      String pkType = entry.getValue();
      if(first) {
         first = false;
      } else {
         p.print(", ");
    
      }
      p.print(" ");
    p.print(pkType);p.print(" ");
    p.print(pkName);p.print("");
    
   }
   p.println(") throws RemoteException {");
    p.println("   ");
    p.println("      try {");
    p.print("          return jgdi.get");
    p.print(name);p.print("(");
    
   first = true;
   for (java.util.Map.Entry<String, String> entry: primaryKeys.entrySet()) {
      String pkName = entry.getKey();
      if(first) {
         first = false;
      } else {
         p.print(", ");
    
      }
      p.print("");
    p.print(pkName);p.print("");
    
   }             
              p.println(");");
    p.println("       } catch( RemoteException re ) {");
    p.println("          handleError(re);");
    p.println("          return null;");
    p.println("       }            ");
    p.println("   }");
    p.print("");
            
     } // end of genGetByPrimaryKeyMethod
  } // end of class JGDIRMICacheGenerator
  
  // ---------------------------------------------------------------------------
  // Build Generator instances
  // ---------------------------------------------------------------------------
  java.util.List<JGDIRMICacheGenerator> generators = new java.util.ArrayList<JGDIRMICacheGenerator>();
  com.sun.grid.cull.CullObject cullObj = null;
  for (String name : cullDef.getObjectNames()) {
    cullObj = cullDef.getCullObject(name);
    generators.add(new JGDIRMICacheGenerator(cullObj));
  }
p.println("");
    p.println("package com.sun.grid.jgdi.rmi;");
    p.println("");
    p.println("");
    p.println("import java.rmi.RemoteException; ");
    p.println("import java.rmi.Remote;");
    p.println("import java.util.List;");
    p.println("import java.util.Collections;");
    p.println("import com.sun.grid.jgdi.JGDI;");
    p.println("import com.sun.grid.jgdi.JGDIException;");
    p.println("");
    p.print("");
     // Import all cull object names;
   for (JGDIRMICacheGenerator gen : generators) {
       gen.genImport();
    } // end of for
p.println("");
    p.println("    ");
    p.println("    ");
    p.println("/**");
    p.println(" *  This class implements a cache for the {@link JGDIRemote} interface.");
    p.println(" */");
    p.println("public class JGDIRemoteCache extends JGDIRemoteCacheBase implements JGDIRemote {");
    p.println("");
    p.println("   /** Creates a new instance of JGDIRemoteCache */");
    p.println("   public JGDIRemoteCache() {");
    p.println("   }");
    p.println("   ");
    p.println("   /**");
    p.println("    *  Create a new instanceof of JGDIRemoteCache");
    p.println("    *  @param jgdi  the jgdi remote service");
    p.println("    */");
    p.println("   public JGDIRemoteCache(JGDIRemote jgdi) {");
    p.println("      super(jgdi);");
    p.println("   }");
    p.println("   ");
    p.print("");
    
   for (JGDIRMICacheGenerator gen : generators) {
       gen.genMethods();
   } // end of for
p.println("");
    p.println("}");
  } // end of print
} // end of class java_rmi_jgdi_cache_jsp