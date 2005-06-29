
/******************************************************************************
 * 
 * Module Name: hwmode - Functions for ACPI<->Legacy mode switching logic
 *
 *****************************************************************************/

/******************************************************************************
 *
 * 1. Copyright Notice
 *
 * Some or all of this work - Copyright (c) 1999, Intel Corp.  All rights
 * reserved.
 *
 * 2. License
 *
 * 2.1. This is your license from Intel Corp. under its intellectual property
 * rights.  You may have additional license terms from the party that provided
 * you this software, covering your right to use that party's intellectual
 * property rights.
 *
 * 2.2. Intel grants, free of charge, to any person ("Licensee") obtaining a
 * copy of the source code appearing in this file ("Covered Code") an
 * irrevocable, perpetual, worldwide license under Intel's copyrights in the
 * base code distributed originally by Intel ("Original Intel Code") to copy,
 * make derivatives, distribute, use and display any portion of the Covered
 * Code in any form, with the right to sublicense such rights; and
 *
 * 2.3. Intel grants Licensee a non-exclusive and non-transferable patent
 * license (with the right to sublicense), under only those claims of Intel
 * patents that are infringed by the Original Intel Code, to make, use, sell,
 * offer to sell, and import the Covered Code and derivative works thereof
 * solely to the minimum extent necessary to exercise the above copyright
 * license, and in no event shall the patent license extend to any additions
 * to or modifications of the Original Intel Code.  No other license or right
 * is granted directly or by implication, estoppel or otherwise;
 *
 * The above copyright and patent license is granted only if the following
 * conditions are met:
 *
 * 3. Conditions 
 *
 * 3.1. Redistribution of Source with Rights to Further Distribute Source.  
 * Redistribution of source code of any substantial portion of the Covered
 * Code or modification with rights to further distribute source must include
 * the above Copyright Notice, the above License, this list of Conditions,
 * and the following Disclaimer and Export Compliance provision.  In addition,
 * Licensee must cause all Covered Code to which Licensee contributes to
 * contain a file documenting the changes Licensee made to create that Covered
 * Code and the date of any change.  Licensee must include in that file the
 * documentation of any changes made by any predecessor Licensee.  Licensee 
 * must include a prominent statement that the modification is derived,
 * directly or indirectly, from Original Intel Code.
 *
 * 3.2. Redistribution of Source with no Rights to Further Distribute Source.  
 * Redistribution of source code of any substantial portion of the Covered
 * Code or modification without rights to further distribute source must
 * include the following Disclaimer and Export Compliance provision in the
 * documentation and/or other materials provided with distribution.  In
 * addition, Licensee may not authorize further sublicense of source of any
 * portion of the Covered Code, and must include terms to the effect that the
 * license from Licensee to its licensee is limited to the intellectual
 * property embodied in the software Licensee provides to its licensee, and
 * not to intellectual property embodied in modifications its licensee may
 * make.
 *
 * 3.3. Redistribution of Executable. Redistribution in executable form of any
 * substantial portion of the Covered Code or modification must reproduce the
 * above Copyright Notice, and the following Disclaimer and Export Compliance
 * provision in the documentation and/or other materials provided with the
 * distribution.
 *
 * 3.4. Intel retains all right, title, and interest in and to the Original
 * Intel Code.
 *
 * 3.5. Neither the name Intel nor any other trademark owned or controlled by
 * Intel shall be used in advertising or otherwise to promote the sale, use or
 * other dealings in products derived from or relating to the Covered Code
 * without prior written authorization from Intel.
 *
 * 4. Disclaimer and Export Compliance
 *
 * 4.1. INTEL MAKES NO WARRANTY OF ANY KIND REGARDING ANY SOFTWARE PROVIDED
 * HERE.  ANY SOFTWARE ORIGINATING FROM INTEL OR DERIVED FROM INTEL SOFTWARE
 * IS PROVIDED "AS IS," AND INTEL WILL NOT PROVIDE ANY SUPPORT,  ASSISTANCE,
 * INSTALLATION, TRAINING OR OTHER SERVICES.  INTEL WILL NOT PROVIDE ANY
 * UPDATES, ENHANCEMENTS OR EXTENSIONS.  INTEL SPECIFICALLY DISCLAIMS ANY
 * IMPLIED WARRANTIES OF MERCHANTABILITY, NONINFRINGEMENT AND FITNESS FOR A
 * PARTICULAR PURPOSE. 
 *
 * 4.2. IN NO EVENT SHALL INTEL HAVE ANY LIABILITY TO LICENSEE, ITS LICENSEES
 * OR ANY OTHER THIRD PARTY, FOR ANY LOST PROFITS, LOST DATA, LOSS OF USE OR
 * COSTS OF PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES, OR FOR ANY INDIRECT,
 * SPECIAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THIS AGREEMENT, UNDER ANY
 * CAUSE OF ACTION OR THEORY OF LIABILITY, AND IRRESPECTIVE OF WHETHER INTEL
 * HAS ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES.  THESE LIMITATIONS
 * SHALL APPLY NOTWITHSTANDING THE FAILURE OF THE ESSENTIAL PURPOSE OF ANY
 * LIMITED REMEDY.
 *
 * 4.3. Licensee shall not export, either directly or indirectly, any of this
 * software or system incorporating such software without first obtaining any
 * required license or other approval from the U. S. Department of Commerce or
 * any other agency or department of the United States Government.  In the
 * event Licensee exports any such software from the United States or
 * re-exports any such software from a foreign destination, Licensee shall
 * ensure that the distribution and export/re-export of the software is in
 * compliance with all laws, regulations, orders, or other restrictions of the
 * U.S. Export Administration Regulations. Licensee agrees that neither it nor
 * any of its subsidiaries will export/re-export any technical data, process,
 * software, or service, directly or indirectly, to any country for which the
 * United States government or any agency thereof requires an export license,
 * other governmental approval, or letter of assurance, without first obtaining
 * such license, approval or letter.
 *
 *****************************************************************************/

#define __HWMODE_C__

#include <acpi.h>
#include <hardware.h>


#define _COMPONENT          DEVICE_MANAGER
        MODULE_NAME         ("hwmode");


/******************************************************************************
 *
 * FUNCTION:    HwSetMode
 *
 * PARAMETERS:  Mode            - SYS_MODE_ACPI or SYS_MODE_LEGACY
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Transitions the system into the requested mode or does nothing
 *              if the system is already in that mode.
 *
 ******************************************************************************/

ACPI_STATUS
HwSetMode (
    UINT32                  Mode)
{

    ACPI_STATUS             Status = AE_ERROR;

    FUNCTION_TRACE ("HwSetMode");

    
    if (Mode == SYS_MODE_ACPI)
    {
        /* BIOS should have disabled ALL fixed and GP events */
        
        OsdOut8 (Gbl_FACP->SmiCmd, Gbl_FACP->AcpiEnable);
        DEBUG_PRINT (ACPI_INFO, ("Attempting to enable ACPI mode\n"));
    }

    else if (Mode == SYS_MODE_LEGACY)
    {
        /* 
         * BIOS should clear all fixed status bits and restore fixed event
         * enable bits to default
         */

        OsdOut8 (Gbl_FACP->SmiCmd, Gbl_FACP->AcpiDisable);
        DEBUG_PRINT (ACPI_INFO, ("Attempting to enable Legacy (non-ACPI) mode\n"));
    }

    if (HwGetMode () == Mode)
    {
        DEBUG_PRINT (ACPI_INFO, ("Mode %d successfully enabled\n", Mode));
        Status = AE_OK;
    }
        
    return_ACPI_STATUS (Status);
}


/******************************************************************************
 *
 * FUNCTION:    Hw
 
 *
 * PARAMETERS:  none
 *
 * RETURN:      SYS_MODE_ACPI or SYS_MODE_LEGACY
 *
 * DESCRIPTION: Return current operating state of system.  Determined by
 *              querying the SCI_EN bit.
 *
 ******************************************************************************/

UINT32
HwGetMode (void)
{

    FUNCTION_TRACE ("HwGetMode");

    
    if (READ_ACPI_REGISTER (SCI_EN))
    {
        return_VALUE (SYS_MODE_ACPI);
    }
    else
    {
        return_VALUE (SYS_MODE_LEGACY);
    }
}

/******************************************************************************
 *
 * FUNCTION:    HwGetModeCapabilities
 *
 * PARAMETERS:  none
 *
 * RETURN:      logical OR of SYS_MODE_ACPI and SYS_MODE_LEGACY determined at initial
 *              system state.
 *
 * DESCRIPTION: Returns capablities of system
 *
 ******************************************************************************/

UINT32
HwGetModeCapabilities (void)
{

    FUNCTION_TRACE ("HwGetModeCapabilities");

    
    if (!(Gbl_SystemFlags & SYS_MODES_MASK))
    {
        if (HwGetMode () == SYS_MODE_LEGACY)
        {   
            /* 
             * Assume that if this call is being made, AcpiInit has been called and
             * ACPI support has been established by the presence of the tables.
             * Therefore since we're in SYS_MODE_LEGACY, the system must support both
             * modes 
             */
            
            Gbl_SystemFlags |= (SYS_MODE_ACPI | SYS_MODE_LEGACY);
        }
        
        else
        {   
            /* TBD!!! this may be unsafe... */

            /* 
             * system is is ACPI mode, so try to switch back to LEGACY to see if
             * it is supported 
             */
            HwSetMode (SYS_MODE_LEGACY);
            
            if (HwGetMode () == SYS_MODE_LEGACY)
            {   
                /* Now in SYS_MODE_LEGACY, so both are supported */
                
                Gbl_SystemFlags |= (SYS_MODE_ACPI | SYS_MODE_LEGACY);
                HwSetMode (SYS_MODE_ACPI);
            }
            
            else
            {   
                /* Still in SYS_MODE_ACPI so this must be an ACPI only system */
                
                Gbl_SystemFlags |= SYS_MODE_ACPI;
            }
        }
    }
    
    return_VALUE (Gbl_SystemFlags & SYS_MODES_MASK);
}
