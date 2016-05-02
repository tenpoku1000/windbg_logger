
// Copyright 2016 Shin'ichi Ichikawa. Released under the GNU General Public License version 3.
// See the file COPYING3. If not see <http://www.gnu.org/licenses/>.

#include "UART.h"
#include "utils.h"
#include "windbgkd.h"

static bool is_valid_port(COMMPROP* prop);
static bool is_comm_error(DWORD errors);

bool create_UART(HANDLE* UART, wchar_t* port_name)
{
    HANDLE handle = CreateFile(
        port_name,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if (INVALID_HANDLE_VALUE == handle){

        get_last_error(__func__, __LINE__);

        return false;
    }

    *UART = handle;

    COMMPROP tmp_prop;
    SecureZeroMemory(&tmp_prop, sizeof(tmp_prop));
    tmp_prop.wPacketLength = sizeof(tmp_prop);

    if (FALSE == GetCommProperties(handle, &tmp_prop)){

        get_last_error(__func__, __LINE__);

        return false;
    }

    WORD size = tmp_prop.wPacketLength;

    COMMPROP* prop = (COMMPROP*)malloc(size);
    SecureZeroMemory(prop, size);
    prop->wPacketLength = size;

    if (FALSE == GetCommProperties(handle, prop)){

        get_last_error(__func__, __LINE__);

        free(prop);
        prop = NULL;

        return false;
    }

    if (false == is_valid_port(prop)){

        free(prop);
        prop = NULL;

        return false;
    }

    free(prop);
    prop = NULL;

    if (FALSE == SetupComm(handle, PACKET_MAX_SIZE, PACKET_MAX_SIZE)){

        get_last_error(__func__, __LINE__);

        return false;
    }

    COMMTIMEOUTS comm_timeouts;
    SecureZeroMemory(&comm_timeouts, sizeof(comm_timeouts));

    // ReadTotalTimeout(msec) = ReadTotalTimeoutMultiplier * bytes + ReadTotalTimeoutConstant
    comm_timeouts.ReadTotalTimeoutMultiplier = 5;
    comm_timeouts.ReadTotalTimeoutConstant = 50;

    // WriteTotalTimeout(msec) = WriteTotalTimeoutMultiplier * bytes + WriteTotalTimeoutConstant
    comm_timeouts.WriteTotalTimeoutMultiplier = 5;
    comm_timeouts.WriteTotalTimeoutConstant = 50;

    if (FALSE == SetCommTimeouts(handle, &comm_timeouts)){

        get_last_error(__func__, __LINE__);

        return false;
    }

    DCB dcb;
    SecureZeroMemory(&dcb, sizeof(dcb));

    dcb.DCBlength = sizeof(dcb);

    if (FALSE == GetCommState(handle, &dcb)){

        get_last_error(__func__, __LINE__);

        return false;
    }

    dcb.BaudRate = CBR_115200;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;

    dcb.fOutxCtsFlow = TRUE;
    dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;

    dcb.fAbortOnError = TRUE;

    if (FALSE == SetCommState(handle, &dcb)){

        get_last_error(__func__, __LINE__);

        return false;
    }

    return true;
}

static bool is_valid_port(COMMPROP* prop)
{
    if ( ! (prop->dwProvSubType & PST_RS232)){

        put_log("PST_RS232 is not support. dwProvSubType = (%d)", prop->dwProvSubType);

        return false;
    }

    if ( ! (prop->dwProvCapabilities & PCF_TOTALTIMEOUTS)){

        put_log("PCF_TOTALTIMEOUTS is not support. dwProvCapabilities = (%d)", prop->dwProvCapabilities);

        return false;
    }

    if ( ! (prop->dwProvCapabilities & PCF_RTSCTS)){

        put_log("PCF_RTSCTS is not support. dwProvCapabilities = (%d)", prop->dwProvCapabilities);

        return false;
    }

    if ( ! (prop->dwSettableParams & SP_HANDSHAKING)){

        put_log("SP_HANDSHAKING is not support. dwSettableParams = (%d)", prop->dwSettableParams);

        return false;
    }

    if ( ! (prop->dwSettableParams & SP_BAUD)){

        put_log("SP_BAUD is not support. dwSettableParams = (%d)", prop->dwSettableParams);

        return false;
    }

    if ( ! (prop->dwSettableParams & SP_DATABITS)){

        put_log("SP_DATABITS is not support. dwSettableParams = (%d)", prop->dwSettableParams);

        return false;
    }

    if ( ! (prop->dwSettableParams & SP_PARITY_CHECK)){

        put_log("SP_PARITY_CHECK is not support. dwSettableParams = (%d)", prop->dwSettableParams);

        return false;
    }

    if ( ! (prop->dwSettableParams & SP_STOPBITS)){

        put_log("SP_STOPBITS is not support. dwSettableParams = (%d)", prop->dwSettableParams);

        return false;
    }

    if ( ! (prop->dwSettableBaud & BAUD_115200)){

        put_log("BAUD_115200 is not support. dwSettableParams = (%d)", prop->dwSettableParams);

        return false;
    }

    if ( ! (prop->wSettableData & DATABITS_8)){

        put_log("DATABITS_8 is not support. dwSettableParams = (%d)", prop->dwSettableParams);

        return false;
    }

    if ( ! (prop->wSettableStopParity & STOPBITS_10)){

        put_log("STOPBITS_10 is not support. dwSettableParams = (%d)", prop->dwSettableParams);

        return false;
    }

    if ( ! (prop->wSettableStopParity & PARITY_NONE)){

        put_log("PARITY_NONE is not support. dwSettableParams = (%d)", prop->dwSettableParams);

        return false;
    }

    return true;
}

bool read_UART(HANDLE UART, LPVOID buffer, DWORD read_size, LPDWORD read_bytes)
{
    DWORD errors = 0;
    COMSTAT stat;

    for (;;){

        errors = 0;

        SecureZeroMemory(&stat, sizeof(stat));

        if (FALSE == ClearCommError(UART, &errors, &stat)){

            get_last_error(__func__, __LINE__);

            return false;
        }

        if (is_comm_error(errors)){

            continue;
        }

        break;
    }

    if (0 == stat.cbInQue){

        *read_bytes = 0;

        return true;
    }

    if (read_size > stat.cbInQue){

        read_size = stat.cbInQue;
    }

    if (FALSE == ReadFile(UART, buffer, read_size, read_bytes, NULL)){

        get_last_error(__func__, __LINE__);

        errors = 0;

        SecureZeroMemory(&stat, sizeof(stat));

        if (FALSE == ClearCommError(UART, &errors, &stat)){

            get_last_error(__func__, __LINE__);
        }

        return false;
    }

    return true;
}

bool write_UART(HANDLE UART, LPVOID buffer, DWORD write_size, LPDWORD written_size)
{
    DWORD errors = 0;
    COMSTAT stat;

    DWORD out_que = 0;

    for (;;){

        errors = 0;

        SecureZeroMemory(&stat, sizeof(stat));

        if (FALSE == ClearCommError(UART, &errors, &stat)){

            get_last_error(__func__, __LINE__);

            return false;
        }

        if (is_comm_error(errors)){

            continue;
        }

        if (0 == stat.cbOutQue){

            break;
        }

        if ((out_que == stat.cbOutQue) && stat.fCtsHold){

            put_log("Target PC is impossible reception state.");

            return false;
        }

        out_que = stat.cbOutQue;

        Sleep(500);
    }

    for (int i = 0; 10 > i; ++i) {

        if (FALSE == WriteFile(UART, buffer, write_size, written_size, NULL)) {

            get_last_error(__func__, __LINE__);

            errors = 0;

            SecureZeroMemory(&stat, sizeof(stat));

            if (FALSE == ClearCommError(UART, &errors, &stat)) {

                get_last_error(__func__, __LINE__);
            }

            Sleep(100);

            continue;
        }

        return true;
    }

    return false;
}

static bool is_comm_error(DWORD errors)
{
    bool ret_val = false;

    if (errors & CE_BREAK){

        put_log("CE_BREAK: The hardware detected a break condition.");

        ret_val = true;
    }

    if (errors & CE_FRAME){

        put_log("CE_FRAME: The hardware detected a framing error.");

        ret_val = true;
    }

    if (errors & CE_OVERRUN){

        put_log("CE_OVERRUN: A character-buffer overrun has occurred. The next character is lost.");

        ret_val = true;
    }

    if (errors & CE_RXOVER){

        put_log("CE_RXOVER: An input buffer overflow has occurred.");

        ret_val = true;
    }

    if (errors & CE_RXPARITY){

        put_log("CE_RXPARITY: The hardware detected a parity error.");

        ret_val = true;
    }

    return ret_val;
}

bool close_UART(HANDLE* UART)
{
    if (FALSE == CloseHandle(*UART)){

        get_last_error(__func__, __LINE__);

        *UART = NULL;

        return false;
    }

    *UART = NULL;

    return true;
}

