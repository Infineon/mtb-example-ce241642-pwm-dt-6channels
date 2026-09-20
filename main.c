/*******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for TCPWM 6 channel to generate
*              complementary PWM - edge aligned with dead time.
*
* Related Document: See README.md
*
*
********************************************************************************
* (c) 2026, Infineon Technologies AG, or an affiliate of Infineon
* Technologies AG. All rights reserved.
* This software, associated documentation and materials ("Software") is
* owned by Infineon Technologies AG or one of its affiliates ("Infineon")
* and is protected by and subject to worldwide patent protection, worldwide
* copyright laws, and international treaty provisions. Therefore, you may use
* this Software only as provided in the license agreement accompanying the
* software package from which you obtained this Software. If no license
* agreement applies, then any use, reproduction, modification, translation, or
* compilation of this Software is prohibited without the express written
* permission of Infineon.
*
* Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE
* IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF
* THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A
* SPECIFIC USE/PURPOSE OR MERCHANTABILITY.
* Infineon reserves the right to make changes to the Software without notice.
* You are responsible for properly designing, programming, and testing the
* functionality and safety of your intended application of the Software, as
* well as complying with any legal requirements related to its use. Infineon
* does not guarantee that the Software will be free from intrusion, data theft
* or loss, or other breaches ("Security Breaches"), and Infineon shall have
* no liability arising out of any Security Breaches. Unless otherwise
* explicitly approved by Infineon, the Software may not be used in any
* application where a failure of the Product or any consequences of the use
* thereof can reasonably be expected to result in personal injury.
*******************************************************************************/


/*******************************************************************************
* Header Files
*******************************************************************************/
#include "cy_pdl.h"
#include "cybsp.h"

/*******************************************************************************
* Macros
*******************************************************************************/


/*******************************************************************************
* Global Variables
*******************************************************************************/


/*******************************************************************************
* Function Prototypes
*******************************************************************************/


/*******************************************************************************
* Function Definitions
*******************************************************************************/

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function for the CPU. It configures Configure TCPWM 6 channel
* to generate complementary PWM - edge aligned with dead time.
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    Cy_PPCA_Enable(CNFG_PPCA_INOUT_HW);

    Cy_PPCA_CNFG_PPCA_Output_Selector(CNFG_PPCA_INOUTCNFG_HW, &CNFG_PPCA_INOUT_ppcaOutConfig);

    /* EPU configuration */
    Cy_PPCA_EPU_EnableExclusiveAccess(EPU_BLK_HW, true);
    Cy_PPCA_EPU_Enable(EPU_BLK_HW);

    Cy_PPCA_EPU_PU_T2_Configure(put2_0_HW, put2_0_INDEX, &put2_0_put2_config);
    Cy_PPCA_EPU_PU_T2_Enable(put2_0_HW, put2_0_INDEX, put2_0_ENABLE_MODE);

    Cy_PPCA_EPU_Combo_Configure(combiner12_HW, combiner12_INDEX, &combiner12_combo_config);

    /*Initialize and start PWM_U*/
    if (CY_TCPWM_SUCCESS != Cy_TCPWM_PWM_Init(PWM_U_HW, PWM_U_NUM, &PWM_U_config))
    {
        CY_ASSERT(0);
    }

    /* Enable the initialized PWM_U */
    Cy_TCPWM_PWM_Enable(PWM_U_HW, PWM_U_NUM);

    /*Initialize and start PWM_V*/
    if (CY_TCPWM_SUCCESS != Cy_TCPWM_PWM_Init(PWM_V_HW, PWM_V_NUM, &PWM_V_config))
    {
        CY_ASSERT(0);
    }

    /* Enable the initialized PWM_V */
    Cy_TCPWM_PWM_Enable(PWM_V_HW, PWM_V_NUM);

    /*Initialize and start PWM_W*/
    if (CY_TCPWM_SUCCESS != Cy_TCPWM_PWM_Init(PWM_W_HW, PWM_W_NUM, &PWM_W_config))
    {
        CY_ASSERT(0);
    }

    /* Enable the initialized PWM_W */
    Cy_TCPWM_PWM_Enable(PWM_W_HW, PWM_W_NUM);

    /*Initialize the timer, Configure as Trigger signal*/
    if (CY_TCPWM_SUCCESS != Cy_TCPWM_Counter_Init(PWM_Trigger_HW, PWM_Trigger_NUM, &PWM_Trigger_config))
    {
        CY_ASSERT(0);
    }
    Cy_TCPWM_Counter_Enable(PWM_Trigger_HW, PWM_Trigger_NUM);

    /*SWAP event: constant 1*/
    Cy_TCPWM_InputTriggerSetup(PWM_U_HW,PWM_U_NUM,CY_TCPWM_INPUT_TR_INDEX_OR_SWAP ,CY_TCPWM_INPUT_LEVEL,CY_TCPWM_INPUT_1);
    Cy_TCPWM_InputTriggerSetup(PWM_V_HW,PWM_V_NUM,CY_TCPWM_INPUT_TR_INDEX_OR_SWAP ,CY_TCPWM_INPUT_LEVEL,CY_TCPWM_INPUT_1);
    Cy_TCPWM_InputTriggerSetup(PWM_W_HW,PWM_W_NUM,CY_TCPWM_INPUT_TR_INDEX_OR_SWAP ,CY_TCPWM_INPUT_LEVEL,CY_TCPWM_INPUT_1);

    /* Enable global interrupts */
    __enable_irq();

    /*Start the trigger timer*/
    Cy_TCPWM_TriggerStart_Single(PWM_Trigger_HW, PWM_Trigger_NUM);

    for (;;)
    {
    }
}

/* [] END OF FILE */
