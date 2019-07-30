#include "Type.h"
#include "SwTimer.h"
#include "BT_BrcmID.h"
#include "BT_API.h"
#include "BT_BrcmInt.h"
#include "utron_utils.h"
#include "wiced_bt_rfcomm.h"
#include "wiced_bt_ble.h"
#include "bt_smart_service.h"
//#include "PStore.h"

#define THIS_DBGLVL         2       //0=OFF, 1=ERROR, 2=TRACE
#define __MODULE__          BT_Brcm
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


typedef struct{
    UINT32              uiInitKey;   ///< indicate module is initail
    BT_OPEN             Open;
}BTBRCM_CTRL;


extern int   brcm_patch_ram_length;
extern UINT8 brcm_patchram_buf[];


#define CFG_BTBRCM_INIT_KEY   MAKEFOURCC('B','T','B','M')

static BTBRCM_CTRL m_BtBrcm_Ctrl = {0};

static BTBRCM_CTRL* xBtBrcm_GetCtrl(void)
{
    return &m_BtBrcm_Ctrl;
}


ER BT_Open(BT_OPEN *pOpen)
{
    BTBRCM_CTRL* pCtrl;
    DBG_FUNC_BEGIN("\r\n");
    if (!SEM_ID_BT)
    {
        DBG_ERR("ID not installed\r\n");
        return E_SYS;
    }
    if (!pOpen)
    {
        DBG_ERR("pOpen is NULL\r\n");
        return E_PAR;
    }
    wai_sem(SEM_ID_BT);
    pCtrl = xBtBrcm_GetCtrl();
    if(pCtrl->uiInitKey==CFG_BTBRCM_INIT_KEY)
    {
        sig_sem(SEM_ID_BT);
        return E_OK;
    }
    memset(pCtrl,0,sizeof(BTBRCM_CTRL));
    pCtrl->Open = *pOpen;
    pCtrl->uiInitKey = CFG_BTBRCM_INIT_KEY;
    clr_flg(FLAG_ID_BT, 0xFFFFFFFF);
    // copy bda
    if (pOpen->BDAddress)
    {
        memcpy(bt_bda, pOpen->BDAddress, sizeof(bt_bda));
    }
    // load bt info from storage
    if (pCtrl->Open.loadInfo)
        pCtrl->Open.loadInfo((UINT32)&bt_nv_ram_store,sizeof(bt_nv_ram_store));
    #if 1
    DBG_IND("patchram len = 0x%x, data =0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\r\n",brcm_patch_ram_length,brcm_patchram_buf[0],brcm_patchram_buf[1],brcm_patchram_buf[2],brcm_patchram_buf[3],brcm_patchram_buf[4],brcm_patchram_buf[5]);
    application_start();
    #else
    test_btuart_hw();
    #endif
    sig_sem(SEM_ID_BT);
    DBG_FUNC_END("\r\n");
    return E_OK;
}

void BT_StoreInfo(void)
{
    BTBRCM_CTRL* pCtrl = xBtBrcm_GetCtrl();

    if (pCtrl->Open.saveInfo)
        pCtrl->Open.saveInfo((UINT32)&bt_nv_ram_store,sizeof(bt_nv_ram_store));
}


static void BT_TimerCb(UINT32 uiEvent)
{
    set_flg(FLAG_ID_BT, FLG_BT_BLE_OFF);
    DBG_WRN("Timeout\r\n");
}



static void BT_WaitOff(void)
{
    FLGPTN       uiFlag;
    SWTIMER_ID   timer_id;
    BOOL         isOpenTimerOK = TRUE;

    DBG_FUNC_BEGIN("\r\n");
    if (SwTimer_Open(&timer_id,BT_TimerCb)!= E_OK)
    {
        DBG_ERR("open timer fail\r\n");
        isOpenTimerOK = FALSE;
    }
    else
    {
        SwTimer_Cfg(timer_id,2000,SWTIMER_MODE_FREE_RUN);
        SwTimer_Start(timer_id);
    }
    wai_flg(&uiFlag, FLAG_ID_BT, FLG_BT_BLE_OFF, TWF_ORW);
    if (isOpenTimerOK)
    {
        SwTimer_Stop(timer_id);
        SwTimer_Close(timer_id);
    }
    DBG_FUNC_END("\r\n");
}



ER BT_Close(void)
{
    BTBRCM_CTRL* pCtrl = xBtBrcm_GetCtrl();

    DBG_FUNC_BEGIN("\r\n");
    wai_sem(SEM_ID_BT);
    if(pCtrl->uiInitKey!=CFG_BTBRCM_INIT_KEY)
    {
        sig_sem(SEM_ID_BT);
        return E_OK;
    }
    application_stop();
    // stop scan
    //wiced_bt_ble_scan(BTM_BLE_SCAN_TYPE_NONE, FALSE, NULL /*scan_result_cback*/);
    // save bt info to storage
    BT_StoreInfo();
    BT_WaitOff();
    pCtrl->uiInitKey = 0;
    sig_sem(SEM_ID_BT);
    DBG_FUNC_END("\r\n");
    return E_OK;
}



INT32 BT_Rfcomm_WriteData(UINT16 port_handle, void *p_data, UINT16 max_len, UINT16 *p_len)
{
    return wiced_bt_rfcomm_write_data(port_handle, p_data, max_len, p_len);
}


BT_RCKEY_CB  *BT_GetRCKEY_CB(void)
{
    BTBRCM_CTRL* pCtrl = xBtBrcm_GetCtrl();

    if(pCtrl->uiInitKey!=CFG_BTBRCM_INIT_KEY)
    {
        return NULL;
    }
    return pCtrl->Open.rckeyCB;
}


BT_EVENT_CB  *BT_GetEvent_CB(void)
{
    BTBRCM_CTRL* pCtrl = xBtBrcm_GetCtrl();

    if(pCtrl->uiInitKey!=CFG_BTBRCM_INIT_KEY)
    {
        return NULL;
    }
    return pCtrl->Open.eventCB;
}


BT_RFCOMM_RX_CB  *BT_GetRfcomm_Rx_CB(void)
{
    BTBRCM_CTRL* pCtrl = xBtBrcm_GetCtrl();

    if(pCtrl->uiInitKey!=CFG_BTBRCM_INIT_KEY)
    {
        return NULL;
    }
    return pCtrl->Open.rfcommRxCB;
}

BT_BLE_RCVDATA_CB  *BT_GetBleRcvData_CB(void)
{
    BTBRCM_CTRL* pCtrl = xBtBrcm_GetCtrl();

    if(pCtrl->uiInitKey!=CFG_BTBRCM_INIT_KEY)
    {
        return NULL;
    }
    return pCtrl->Open.bleRcvDataCB;
}

INT32 BT_BLE_SendNotifyData(UINT8 cmdID, UINT8* data, int length)
{
    return bt_BleSndNotify_data(cmdID, data, length);
}

void BT_BLE_SetScanOff(void)
{
    set_flg(FLAG_ID_BT, FLG_BT_BLE_OFF);
}
