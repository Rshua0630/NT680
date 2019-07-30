/*
    System Lock

    System Lock for SxInput.

    @file       SysLock.c
    @ingroup    mIPRJSYS

    @note       �o���ɮ׭t�d�T���

                1.�޲zAutoSleep detect�������
                UI_LockAutoSleep()
                   �Ȱ�
                UI_UnlockAutoSleep()
                   ��_
                UI_IsForceLockSleep(void)
                   �߰ݪ��A

                2.�޲zAutoPowerOff detect�������
                UI_LockAutoPWROff()
                   �Ȱ�
                UI_UnlockAutoPWROff()
                   ��_
                UI_IsForceLockAutoPWROff()
                   �߰ݪ��A

                3.�޲z�Ҧ��PUI Event������detect������� (���FTimer���~)
                UI_LockEvent()
                   �Ȱ�
                UI_UnlockEvent()
                   ��_
                UI_IsForceLock()
                   �߰ݪ��A



    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#include "UIFramework.h"
#include "UICommon.h"

//---------------------SysLockCB Debug Definition -----------------------------
//---------------------SysLockCB Global Variables -----------------------------
//---------------------SysLockCB Prototype Declaration  -----------------------
//---------------------SysLockCB Public API  ----------------------------------
//---------------------SysLockCB Private API ----------------------------------

/*
/// Force Lock of Input Dectection /////////////////////////

�`�N!!! SxJob���檺�u�@�ϥή�, �ݭn�Ȱ��Y��SxTimer��detection

��]:
�ѩ�Y��SxJob���檺�u�@�ϥή�,
�t���Y��SxTimer��detect function�P�ɷ|�o�X Ux_PostEvent()
�p���@��, SxJob/SxTimer���task�����N�|�o�ʹ`��lock�y�����
(A��B��SxJob finish, B��A��UI framework window samphore)
==> �d��: TV���ު��P��, ���UPlayback Key�y�����

����:
power_on flow (�Ĥ@��change mode�����e)
power_off flow
sleep_enter flow
sleep_leave flow
change_dispdev flow
change_dispmode flow
change_mode flow

�Ȱ�: (�Z�O�|PostEvent���W�h��, ���n�Ȱ�)
SX_TIMER_DET_PWR_ID   //Key��J
SX_TIMER_DET_KEY_ID   //Key��J
SX_TIMER_DET_MODE_ID   //Key��J
SX_TIMER_DET_USB_ID   //USB�s��
SX_TIMER_DET_TV_ID   //TV�s��
SX_TIMER_DET_BATT_ID   //Batt����
SX_TIMER_DET_SLEEP_ID   //Sleep����
SX_TIMER_DET_AUTOPOWEROFF_ID   //PowerOff����


���|��SxTimer_SetFuncActive()�Ӱ�,
�ӬO�����bdetect function���[�J����, ���ŦX����Ybypass detect
���k�i�קK��LUI flow��SxTimer_SetFuncActive()�]�w�Q�}�a

�h���W:
UI Flow code -> �Шϥγ]�wSxTimer_SetFuncActive()�ӼȰ�
SxJob Flow code -> �]�wforce lock�ӼȰ� (��O�b��C�����a���d�I)
*/
UINT32 bDetect_ForceLockStatus = FALSE; //defalt can not lock
UINT32 bDetect_ForceLock = TRUE;
BOOL    g_bPostponeTVChange = FALSE;  //#NT#TV change action should be postponed from movie play start to first video frame decoded
static UINT32 bDetect_LockSleep = FALSE;
static UINT32 bDetect_LockAutoPWROff = FALSE;

UINT32 UI_IsForceLockAutoSleep(void)
{
	return bDetect_LockSleep;
}
UINT32 UI_IsForceLockAutoPWROff(void)
{
	return bDetect_LockAutoPWROff;
}
void UI_LockAutoSleep(void)
{
	bDetect_LockSleep = TRUE;
}
void UI_LockAutoPWROff(void)
{
	bDetect_LockAutoPWROff = TRUE;
}
void UI_UnlockAutoSleep(void)
{
	bDetect_LockSleep = FALSE;
}
void UI_UnlockAutoPWROff(void)
{
	bDetect_LockAutoPWROff = FALSE;
}

UINT32 UI_IsForceLockStatus(void)
{
	return bDetect_ForceLockStatus;
}
UINT32 UI_IsForceLock(void)
{
	return bDetect_ForceLock;
}

//lock mode key,before Sx job start to change mode
void UI_LockStatusEvent()
{
	bDetect_ForceLockStatus = TRUE;
}
void UI_LockEvent(void)
{
	bDetect_ForceLock = TRUE;

	//PwrKey,NormalKey,StatusKey: �קKevent queue�̭���key event�٬O�|�Q����
	//�`�N!!! ����flush release key, �_�h�ܥi��|�y��UI���`
	// ==> �d��: TV���ު��P��, ���US1�A��}S1, UI����J�ؤ�����(���v��)
	// ==> ���LUI�]�n����T�OPress/Release����B�z���{��(�p�G�S������Press,�N���n��Release)
	//Ux_FlushEventByRange(NVTEVT_KEY_PRESS_START,NVTEVT_KEY_PRESS_END); //����flush release key
	//TV
	//USB
	//Battery
	//Sleep
	//AutoPoweroff
}
//unlock mode key,after Sx job end change mode
void UI_UnlockEvent(void)
{
	bDetect_ForceLock = FALSE;
	bDetect_ForceLockStatus = FALSE;
}

