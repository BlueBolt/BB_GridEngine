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
/**
 *  Generated from javamodel.jsp
 *  !!! DO NOT EDIT THIS FILE !!!
 */

package com.sun.grid.jgdi.configuration;

import com.sun.grid.jgdi.configuration.GEObject;
import java.util.List;
import java.util.Set;

/**
 *  Java representation of the cull object RUE_Type
 *  defined in sge_all_listsL.template
 *
 *  <p><strong>Warning:</strong>  The  ResourceUtilization class will 
 *  not be compatible with future Sun&trade; Grid Engine releases.</p>
 *
 */
public interface ResourceUtilization extends 
    GEObject  { 

    // Attribute name ------------------------------------------------

       
    /**
     *   Set the name attribute.
     *
     *   @param aName  the new value for the  name attribute
     */
    public void setName(java.lang.String aName);
  
    /**
     *  Get the value of the name attribute.
     *  @return the value of the name attribute
     */
    public java.lang.String getName();
   

 
    /**
     *  Determine if any name attribute is set
     *  @return <code>true</code> if the name attribute is set
     */
    public boolean isSetName();


    // Attribute utilizedNow ------------------------------------------------

       
    /**
     *   Set the utilizedNow attribute.
     *
     *   @param aUtilizedNow  the new value for the  utilizedNow attribute
     */
    public void setUtilizedNow(double aUtilizedNow);
  
    /**
     *  Get the value of the utilizedNow attribute.
     *  @return the value of the utilizedNow attribute
     */
    public double getUtilizedNow();
   

 
    /**
     *  Determine if any utilizedNow attribute is set
     *  @return <code>true</code> if the utilizedNow attribute is set
     */
    public boolean isSetUtilizedNow();


    // Attribute utilized ------------------------------------------------


    /**
     *   Get a unmodifiable list of all com.sun.grid.jgdi.configuration.ResourceDiagramEntry attributes.
     *
     *   @return Unmodifiable list with all com.sun.grid.jgdi.configuration.ResourceDiagramEntry attributes
     */
    public List< com.sun.grid.jgdi.configuration.ResourceDiagramEntry >  getUtilizedList();

    /**
     *  Get the number of com.sun.grid.jgdi.configuration.ResourceDiagramEntry attributes.
     *
     *  @return The number of com.sun.grid.jgdi.configuration.ResourceDiagramEntry attributes.
     */
    public int getUtilizedCount();
 
    /**
     *  Get a com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute at an index.
     *  @param index  the index of the com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute which should be
     *                removed
     *  @return the com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute
     */
    public com.sun.grid.jgdi.configuration.ResourceDiagramEntry getUtilized(int index);
   

    /**
     *   Add a com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute.
     *   @param autilized  the new com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute
     */
    public void addUtilized(com.sun.grid.jgdi.configuration.ResourceDiagramEntry autilized);
   
    /**
     *   Set a com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute.
     *   @param index   index of the com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute
     *   @param autilized  the new com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute
     */
    public void setUtilized(int index, com.sun.grid.jgdi.configuration.ResourceDiagramEntry autilized);

    /**
     *  Remove all com.sun.grid.jgdi.configuration.ResourceDiagramEntry attributes.
     */
    public void removeAllUtilized();
   
    /**
     *  Remote a com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute at an index.
     *  @param index  the index of the com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute which should be
     *                removed
     *  @return the removed com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute or <code>null</code> if
     *          no attribute at this index is stored
     */
    public com.sun.grid.jgdi.configuration.ResourceDiagramEntry removeUtilized(int index);

    /**
     *  Remote a specific com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute.
     *  @param autilized  the com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute which should be
     *                         removed
     *  @return <code>true</code> if the com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute has been removed
     */
    public boolean removeUtilized(com.sun.grid.jgdi.configuration.ResourceDiagramEntry autilized);
   
       
 
    /**
     *  Determine if any utilized attribute is set
     *  @return <code>true</code> if the utilized attribute is set
     */
    public boolean isSetUtilized();


    // Attribute utilizedNowNonexclusive ------------------------------------------------

       
    /**
     *   Set the utilizedNowNonexclusive attribute.
     *
     *   @param aUtilizedNowNonexclusive  the new value for the  utilizedNowNonexclusive attribute
     */
    public void setUtilizedNowNonexclusive(double aUtilizedNowNonexclusive);
  
    /**
     *  Get the value of the utilizedNowNonexclusive attribute.
     *  @return the value of the utilizedNowNonexclusive attribute
     */
    public double getUtilizedNowNonexclusive();
   

 
    /**
     *  Determine if any utilizedNowNonexclusive attribute is set
     *  @return <code>true</code> if the utilizedNowNonexclusive attribute is set
     */
    public boolean isSetUtilizedNowNonexclusive();


    // Attribute utilizedNonexclusive ------------------------------------------------


    /**
     *   Get a unmodifiable list of all com.sun.grid.jgdi.configuration.ResourceDiagramEntry attributes.
     *
     *   @return Unmodifiable list with all com.sun.grid.jgdi.configuration.ResourceDiagramEntry attributes
     */
    public List< com.sun.grid.jgdi.configuration.ResourceDiagramEntry >  getUtilizedNonexclusiveList();

    /**
     *  Get the number of com.sun.grid.jgdi.configuration.ResourceDiagramEntry attributes.
     *
     *  @return The number of com.sun.grid.jgdi.configuration.ResourceDiagramEntry attributes.
     */
    public int getUtilizedNonexclusiveCount();
 
    /**
     *  Get a com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute at an index.
     *  @param index  the index of the com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute which should be
     *                removed
     *  @return the com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute
     */
    public com.sun.grid.jgdi.configuration.ResourceDiagramEntry getUtilizedNonexclusive(int index);
   

    /**
     *   Add a com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute.
     *   @param autilizedNonexclusive  the new com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute
     */
    public void addUtilizedNonexclusive(com.sun.grid.jgdi.configuration.ResourceDiagramEntry autilizedNonexclusive);
   
    /**
     *   Set a com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute.
     *   @param index   index of the com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute
     *   @param autilizedNonexclusive  the new com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute
     */
    public void setUtilizedNonexclusive(int index, com.sun.grid.jgdi.configuration.ResourceDiagramEntry autilizedNonexclusive);

    /**
     *  Remove all com.sun.grid.jgdi.configuration.ResourceDiagramEntry attributes.
     */
    public void removeAllUtilizedNonexclusive();
   
    /**
     *  Remote a com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute at an index.
     *  @param index  the index of the com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute which should be
     *                removed
     *  @return the removed com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute or <code>null</code> if
     *          no attribute at this index is stored
     */
    public com.sun.grid.jgdi.configuration.ResourceDiagramEntry removeUtilizedNonexclusive(int index);

    /**
     *  Remote a specific com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute.
     *  @param autilizedNonexclusive  the com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute which should be
     *                         removed
     *  @return <code>true</code> if the com.sun.grid.jgdi.configuration.ResourceDiagramEntry attribute has been removed
     */
    public boolean removeUtilizedNonexclusive(com.sun.grid.jgdi.configuration.ResourceDiagramEntry autilizedNonexclusive);
   
       
 
    /**
     *  Determine if any utilizedNonexclusive attribute is set
     *  @return <code>true</code> if the utilizedNonexclusive attribute is set
     */
    public boolean isSetUtilizedNonexclusive();



    /*
     * Dump the object
     * @return <code>String</code> the dump string
     */
     public String dump();
     

}