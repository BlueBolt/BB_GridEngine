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
package com.sun.grid.jgdi.examples;

import com.sun.grid.jgdi.EventClient;
import com.sun.grid.jgdi.JGDI;
import com.sun.grid.jgdi.JGDIException;
import com.sun.grid.jgdi.JGDIFactory;
import com.sun.grid.jgdi.configuration.Checkpoint;
import com.sun.grid.jgdi.configuration.ConfigurationFactory;
import com.sun.grid.jgdi.event.Event;
import com.sun.grid.jgdi.event.EventListener;
import com.sun.grid.jgdi.event.EventTypeEnum;

/**
 * Simple example which demonstrates how to receive events from the
 * Sun&trade; Grid Engine
 *
 */
public class EventExample {

    public static void main(String[] args) {

        try {

            String url = "bootstrap:///opt/sge@default:1026";

            if (args.length == 1) {
                url = args[0];
            }

            JGDI jgdi = JGDIFactory.newInstance(url);

            try {
                System.out.println("Successfully connected to " + url);

                EventClient evc = JGDIFactory.createEventClient(url, 0);

                evc.subscribe(EventTypeEnum.CheckpointAdd);
                evc.subscribe(EventTypeEnum.CheckpointMod);
                evc.subscribe(EventTypeEnum.CheckpointDel);

                evc.commit();

                evc.addEventListener(new EventListener() {

                    public void eventOccured(Event evt) {
                        System.out.println("got event " + evt);
                    }
                });


                try {

                    // Create a new checkpoint object which intialized with default values
                    Checkpoint ckpt = ConfigurationFactory.createCheckpointWithDefaults();
                    ckpt.setName("sample");
                    ckpt.setCkptCommand("/usr/bin/ckpt");
                    ckpt.setCkptDir("/tmp");

                    jgdi.addCheckpoint(ckpt);
                    try {
                        System.out.println("Checkpoint " + ckpt.getName() + " successfully added");
                        ckpt = jgdi.getCheckpoint(ckpt.getName());
                        ckpt.setRestCommand("/tmp/blubber");
                        jgdi.updateCheckpoint(ckpt);
                        System.out.println("Checkpoint " + ckpt.getName() + " successfully modified");
                    } finally {
                        jgdi.deleteCheckpoint(ckpt);
                        System.out.println("Checkpoint " + ckpt.getName() + " successfully deleted");
                    }

                    Thread.sleep(60000);
                } finally {
                    evc.close();
                }
            } finally {
                jgdi.close();
            }
        } catch (JGDIException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
        // Ignore
        }
    }
}
