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
 *  Java representation of the cull object EV_Type
 *  defined in sge_all_listsL.template
 *
 *  <p><strong>Warning:</strong>  The  EventClient class will 
 *  not be compatible with future Sun&trade; Grid Engine releases.</p>
 *
 */
public interface EventClient extends 
    GEObject  { 

    // Attribute id ------------------------------------------------

       
    /**
     *   Set the id attribute.
     *
     *   @param aId  the new value for the  id attribute
     */
    public void setId(int aId);
  
    /**
     *  Get the value of the id attribute.
     *  @return the value of the id attribute
     */
    public int getId();
   

 
    /**
     *  Determine if any id attribute is set
     *  @return <code>true</code> if the id attribute is set
     */
    public boolean isSetId();


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


    // Attribute host ------------------------------------------------

       
    /**
     *   Set the host attribute.
     *
     *   @param aHost  the new value for the  host attribute
     */
    public void setHost(java.lang.String aHost);
  
    /**
     *  Get the value of the host attribute.
     *  @return the value of the host attribute
     */
    public java.lang.String getHost();
   

 
    /**
     *  Determine if any host attribute is set
     *  @return <code>true</code> if the host attribute is set
     */
    public boolean isSetHost();


    // Attribute commproc ------------------------------------------------

       
    /**
     *   Set the commproc attribute.
     *
     *   @param aCommproc  the new value for the  commproc attribute
     */
    public void setCommproc(java.lang.String aCommproc);
  
    /**
     *  Get the value of the commproc attribute.
     *  @return the value of the commproc attribute
     */
    public java.lang.String getCommproc();
   

 
    /**
     *  Determine if any commproc attribute is set
     *  @return <code>true</code> if the commproc attribute is set
     */
    public boolean isSetCommproc();


    // Attribute commid ------------------------------------------------

       
    /**
     *   Set the commid attribute.
     *
     *   @param aCommid  the new value for the  commid attribute
     */
    public void setCommid(int aCommid);
  
    /**
     *  Get the value of the commid attribute.
     *  @return the value of the commid attribute
     */
    public int getCommid();
   

 
    /**
     *  Determine if any commid attribute is set
     *  @return <code>true</code> if the commid attribute is set
     */
    public boolean isSetCommid();


    // Attribute uid ------------------------------------------------

       
    /**
     *   Set the uid attribute.
     *
     *   @param aUid  the new value for the  uid attribute
     */
    public void setUid(int aUid);
  
    /**
     *  Get the value of the uid attribute.
     *  @return the value of the uid attribute
     */
    public int getUid();
   

 
    /**
     *  Determine if any uid attribute is set
     *  @return <code>true</code> if the uid attribute is set
     */
    public boolean isSetUid();


    // Attribute dTime ------------------------------------------------

       
    /**
     *   Set the dTime attribute.
     *
     *   @param aDTime  the new value for the  dTime attribute
     */
    public void setDTime(int aDTime);
  
    /**
     *  Get the value of the dTime attribute.
     *  @return the value of the dTime attribute
     */
    public int getDTime();
   

 
    /**
     *  Determine if any dTime attribute is set
     *  @return <code>true</code> if the dTime attribute is set
     */
    public boolean isSetDTime();


    // Attribute flushDelay ------------------------------------------------

       
    /**
     *   Set the flushDelay attribute.
     *
     *   @param aFlushDelay  the new value for the  flushDelay attribute
     */
    public void setFlushDelay(int aFlushDelay);
  
    /**
     *  Get the value of the flushDelay attribute.
     *  @return the value of the flushDelay attribute
     */
    public int getFlushDelay();
   

 
    /**
     *  Determine if any flushDelay attribute is set
     *  @return <code>true</code> if the flushDelay attribute is set
     */
    public boolean isSetFlushDelay();


    // Attribute subscribed ------------------------------------------------


    /**
     *   Get a unmodifiable list of all com.sun.grid.jgdi.configuration.EventSubcribtion attributes.
     *
     *   @return Unmodifiable list with all com.sun.grid.jgdi.configuration.EventSubcribtion attributes
     */
    public List< com.sun.grid.jgdi.configuration.EventSubcribtion >  getSubscribedList();

    /**
     *  Get the number of com.sun.grid.jgdi.configuration.EventSubcribtion attributes.
     *
     *  @return The number of com.sun.grid.jgdi.configuration.EventSubcribtion attributes.
     */
    public int getSubscribedCount();
 
    /**
     *  Get a com.sun.grid.jgdi.configuration.EventSubcribtion attribute at an index.
     *  @param index  the index of the com.sun.grid.jgdi.configuration.EventSubcribtion attribute which should be
     *                removed
     *  @return the com.sun.grid.jgdi.configuration.EventSubcribtion attribute
     */
    public com.sun.grid.jgdi.configuration.EventSubcribtion getSubscribed(int index);
   

    /**
     *   Add a com.sun.grid.jgdi.configuration.EventSubcribtion attribute.
     *   @param asubscribed  the new com.sun.grid.jgdi.configuration.EventSubcribtion attribute
     */
    public void addSubscribed(com.sun.grid.jgdi.configuration.EventSubcribtion asubscribed);
   
    /**
     *   Set a com.sun.grid.jgdi.configuration.EventSubcribtion attribute.
     *   @param index   index of the com.sun.grid.jgdi.configuration.EventSubcribtion attribute
     *   @param asubscribed  the new com.sun.grid.jgdi.configuration.EventSubcribtion attribute
     */
    public void setSubscribed(int index, com.sun.grid.jgdi.configuration.EventSubcribtion asubscribed);

    /**
     *  Remove all com.sun.grid.jgdi.configuration.EventSubcribtion attributes.
     */
    public void removeAllSubscribed();
   
    /**
     *  Remote a com.sun.grid.jgdi.configuration.EventSubcribtion attribute at an index.
     *  @param index  the index of the com.sun.grid.jgdi.configuration.EventSubcribtion attribute which should be
     *                removed
     *  @return the removed com.sun.grid.jgdi.configuration.EventSubcribtion attribute or <code>null</code> if
     *          no attribute at this index is stored
     */
    public com.sun.grid.jgdi.configuration.EventSubcribtion removeSubscribed(int index);

    /**
     *  Remote a specific com.sun.grid.jgdi.configuration.EventSubcribtion attribute.
     *  @param asubscribed  the com.sun.grid.jgdi.configuration.EventSubcribtion attribute which should be
     *                         removed
     *  @return <code>true</code> if the com.sun.grid.jgdi.configuration.EventSubcribtion attribute has been removed
     */
    public boolean removeSubscribed(com.sun.grid.jgdi.configuration.EventSubcribtion asubscribed);
   
       
 
    /**
     *  Determine if any subscribed attribute is set
     *  @return <code>true</code> if the subscribed attribute is set
     */
    public boolean isSetSubscribed();


    // Attribute changed ------------------------------------------------

       
    /**
     *   Set the changed attribute.
     *
     *   @param aChanged  the new value for the  changed attribute
     */
    public void setChanged(boolean aChanged);
  
    /**
     *  Get the value of the changed attribute.
     *  @return the value of the changed attribute
     */
    public boolean isChanged();
   

 
    /**
     *  Determine if any changed attribute is set
     *  @return <code>true</code> if the changed attribute is set
     */
    public boolean isSetChanged();


    // Attribute busyHandling ------------------------------------------------

       
    /**
     *   Set the busyHandling attribute.
     *
     *   @param aBusyHandling  the new value for the  busyHandling attribute
     */
    public void setBusyHandling(int aBusyHandling);
  
    /**
     *  Get the value of the busyHandling attribute.
     *  @return the value of the busyHandling attribute
     */
    public int getBusyHandling();
   

 
    /**
     *  Determine if any busyHandling attribute is set
     *  @return <code>true</code> if the busyHandling attribute is set
     */
    public boolean isSetBusyHandling();


    // Attribute session ------------------------------------------------

       
    /**
     *   Set the session attribute.
     *
     *   @param aSession  the new value for the  session attribute
     */
    public void setSession(java.lang.String aSession);
  
    /**
     *  Get the value of the session attribute.
     *  @return the value of the session attribute
     */
    public java.lang.String getSession();
   

 
    /**
     *  Determine if any session attribute is set
     *  @return <code>true</code> if the session attribute is set
     */
    public boolean isSetSession();


    // Attribute lastHeardFrom ------------------------------------------------

       
    /**
     *   Set the lastHeardFrom attribute.
     *
     *   @param aLastHeardFrom  the new value for the  lastHeardFrom attribute
     */
    public void setLastHeardFrom(int aLastHeardFrom);
  
    /**
     *  Get the value of the lastHeardFrom attribute.
     *  @return the value of the lastHeardFrom attribute
     */
    public int getLastHeardFrom();
   

 
    /**
     *  Determine if any lastHeardFrom attribute is set
     *  @return <code>true</code> if the lastHeardFrom attribute is set
     */
    public boolean isSetLastHeardFrom();


    // Attribute lastSendTime ------------------------------------------------

       
    /**
     *   Set the lastSendTime attribute.
     *
     *   @param aLastSendTime  the new value for the  lastSendTime attribute
     */
    public void setLastSendTime(int aLastSendTime);
  
    /**
     *  Get the value of the lastSendTime attribute.
     *  @return the value of the lastSendTime attribute
     */
    public int getLastSendTime();
   

 
    /**
     *  Determine if any lastSendTime attribute is set
     *  @return <code>true</code> if the lastSendTime attribute is set
     */
    public boolean isSetLastSendTime();


    // Attribute nextSendTime ------------------------------------------------

       
    /**
     *   Set the nextSendTime attribute.
     *
     *   @param aNextSendTime  the new value for the  nextSendTime attribute
     */
    public void setNextSendTime(int aNextSendTime);
  
    /**
     *  Get the value of the nextSendTime attribute.
     *  @return the value of the nextSendTime attribute
     */
    public int getNextSendTime();
   

 
    /**
     *  Determine if any nextSendTime attribute is set
     *  @return <code>true</code> if the nextSendTime attribute is set
     */
    public boolean isSetNextSendTime();


    // Attribute nextNumber ------------------------------------------------

       
    /**
     *   Set the nextNumber attribute.
     *
     *   @param aNextNumber  the new value for the  nextNumber attribute
     */
    public void setNextNumber(int aNextNumber);
  
    /**
     *  Get the value of the nextNumber attribute.
     *  @return the value of the nextNumber attribute
     */
    public int getNextNumber();
   

 
    /**
     *  Determine if any nextNumber attribute is set
     *  @return <code>true</code> if the nextNumber attribute is set
     */
    public boolean isSetNextNumber();


    // Attribute busy ------------------------------------------------

       
    /**
     *   Set the busy attribute.
     *
     *   @param aBusy  the new value for the  busy attribute
     */
    public void setBusy(int aBusy);
  
    /**
     *  Get the value of the busy attribute.
     *  @return the value of the busy attribute
     */
    public int getBusy();
   

 
    /**
     *  Determine if any busy attribute is set
     *  @return <code>true</code> if the busy attribute is set
     */
    public boolean isSetBusy();


    // Attribute events ------------------------------------------------


    /**
     *   Get a unmodifiable list of all com.sun.grid.jgdi.configuration.Event attributes.
     *
     *   @return Unmodifiable list with all com.sun.grid.jgdi.configuration.Event attributes
     */
    public List< com.sun.grid.jgdi.configuration.Event >  getEventsList();

    /**
     *  Get the number of com.sun.grid.jgdi.configuration.Event attributes.
     *
     *  @return The number of com.sun.grid.jgdi.configuration.Event attributes.
     */
    public int getEventsCount();
 
    /**
     *  Get a com.sun.grid.jgdi.configuration.Event attribute at an index.
     *  @param index  the index of the com.sun.grid.jgdi.configuration.Event attribute which should be
     *                removed
     *  @return the com.sun.grid.jgdi.configuration.Event attribute
     */
    public com.sun.grid.jgdi.configuration.Event getEvents(int index);
   

    /**
     *   Add a com.sun.grid.jgdi.configuration.Event attribute.
     *   @param aevents  the new com.sun.grid.jgdi.configuration.Event attribute
     */
    public void addEvents(com.sun.grid.jgdi.configuration.Event aevents);
   
    /**
     *   Set a com.sun.grid.jgdi.configuration.Event attribute.
     *   @param index   index of the com.sun.grid.jgdi.configuration.Event attribute
     *   @param aevents  the new com.sun.grid.jgdi.configuration.Event attribute
     */
    public void setEvents(int index, com.sun.grid.jgdi.configuration.Event aevents);

    /**
     *  Remove all com.sun.grid.jgdi.configuration.Event attributes.
     */
    public void removeAllEvents();
   
    /**
     *  Remote a com.sun.grid.jgdi.configuration.Event attribute at an index.
     *  @param index  the index of the com.sun.grid.jgdi.configuration.Event attribute which should be
     *                removed
     *  @return the removed com.sun.grid.jgdi.configuration.Event attribute or <code>null</code> if
     *          no attribute at this index is stored
     */
    public com.sun.grid.jgdi.configuration.Event removeEvents(int index);

    /**
     *  Remote a specific com.sun.grid.jgdi.configuration.Event attribute.
     *  @param aevents  the com.sun.grid.jgdi.configuration.Event attribute which should be
     *                         removed
     *  @return <code>true</code> if the com.sun.grid.jgdi.configuration.Event attribute has been removed
     */
    public boolean removeEvents(com.sun.grid.jgdi.configuration.Event aevents);
   
       
 
    /**
     *  Determine if any events attribute is set
     *  @return <code>true</code> if the events attribute is set
     */
    public boolean isSetEvents();


    // Attribute state ------------------------------------------------

       
    /**
     *   Set the state attribute.
     *
     *   @param aState  the new value for the  state attribute
     */
    public void setState(int aState);
  
    /**
     *  Get the value of the state attribute.
     *  @return the value of the state attribute
     */
    public int getState();
   

 
    /**
     *  Determine if any state attribute is set
     *  @return <code>true</code> if the state attribute is set
     */
    public boolean isSetState();



    /*
     * Dump the object
     * @return <code>String</code> the dump string
     */
     public String dump();
     

}