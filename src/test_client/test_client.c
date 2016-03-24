
// Copyright 2016 Shin'ichi Ichikawa. Released under the GNU General Public License version 3.
// See the file COPYING3. If not see <http://www.gnu.org/licenses/>.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "thread.h"
#include "named_pipe.h"
#include "utils.h"
#include "log.h"
#include "windbgkd.h"

typedef struct handles_{
    HANDLE named_pipe;
}handles;

static bool init(handles* p);
static bool do_test(HANDLE pipe);
static bool end(handles* p);

static BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)
{
    exit(EXIT_SUCCESS);

    return FALSE;
}

int main()
{
    handles arg;
    SecureZeroMemory(&arg, sizeof(arg));

    if (false == init(&arg)){

        goto fail;
    }

    if (false == do_test(arg.named_pipe)){

        (void)end(&arg);

        goto fail;
    }

    put_log("SUCCESS: To exit, please press enter key.");

    getchar();

    (void)end(&arg);

    return EXIT_SUCCESS;

fail:
    close_stream();

    put_log("FAILURE: To exit, please press enter key.");

    getchar();

    return EXIT_FAILURE;
}

static bool init(handles* p)
{
    SetConsoleCtrlHandler(HandlerRoutine, TRUE);

    RemoveMenu(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_BYCOMMAND);

    ini_values values;

    if (false == get_ini_values(&values)){

        return false;
    }

    set_stream(values.stream);

    if (false == create_file_named_pipe(values.named_pipe, &(p->named_pipe))){

        return false;
    }

    return true;
}

static bool do_test(HANDLE pipe)
{
    static char buffer[PACKET_MAX_SIZE];

    SecureZeroMemory(buffer, sizeof(buffer));

    static DBGKD_MANIPULATE_STATE64* state64 = NULL;
    static DBGKD_DEBUG_IO* debug_io = NULL;
    static DBGKD_ANY_WAIT_STATE_CHANGE* state_change = NULL;
    static DBGKD_TRACE_IO* trace_io = NULL;
    static DBGKD_CONTROL_REQUEST* control_request = NULL;
    static DBGKD_FILE_IO* file_io = NULL;

#define STATE_MANIPULATE(x) \
    PACKET_LEADER, PACKET_TYPE_KD_STATE_MANIPULATE, sizeof(DBGKD_MANIPULATE_STATE64), \
    offsetof(DBGKD_MANIPULATE_STATE64, ApiNumber), sizeof(state64->ApiNumber), (x)

    static struct tbl_ {
        bool valid;
        ULONG leader;
        USHORT type;
        size_t size;
        size_t api_offset;
        size_t api_size;
        ULONG api;
    } tbl[] = {
//      { true, CONTROL_PACKET_LEADER, PACKET_TYPE_UNUSED, 0, 0, 0, 0 },
//      { true, BREAKIN_PACKET, PACKET_TYPE_KD_POLL_BREAKIN, 0, 0, 0, 0 },
        { true, CONTROL_PACKET_LEADER, PACKET_TYPE_KD_ACKNOWLEDGE, 0, 0, 0, 0 },
        { true, CONTROL_PACKET_LEADER, PACKET_TYPE_KD_RESEND, 0, 0, 0, 0 },
        { true, CONTROL_PACKET_LEADER, PACKET_TYPE_KD_RESET, 0, 0, 0, 0 },
        { true, STATE_MANIPULATE(DbgKdReadVirtualMemoryApi) },
        { true, STATE_MANIPULATE(DbgKdWriteVirtualMemoryApi) },
        { true, STATE_MANIPULATE(DbgKdGetContextApi) },
        { true, STATE_MANIPULATE(DbgKdSetContextApi) },
        { true, STATE_MANIPULATE(DbgKdWriteBreakPointApi) },
        { true, STATE_MANIPULATE(DbgKdRestoreBreakPointApi) },
        { true, STATE_MANIPULATE(DbgKdContinueApi) },
        { true, STATE_MANIPULATE(DbgKdReadControlSpaceApi) },
        { true, STATE_MANIPULATE(DbgKdWriteControlSpaceApi) },
        { true, STATE_MANIPULATE(DbgKdReadIoSpaceApi) },
        { true, STATE_MANIPULATE(DbgKdWriteIoSpaceApi) },
        { true, STATE_MANIPULATE(DbgKdRebootApi) },
        { true, STATE_MANIPULATE(DbgKdContinueApi2) },
        { true, STATE_MANIPULATE(DbgKdReadPhysicalMemoryApi) },
        { true, STATE_MANIPULATE(DbgKdWritePhysicalMemoryApi) },
        { true, STATE_MANIPULATE(DbgKdQuerySpecialCallsApi) },
        { true, STATE_MANIPULATE(DbgKdSetSpecialCallApi) },
        { true, STATE_MANIPULATE(DbgKdClearSpecialCallsApi) },
        { true, STATE_MANIPULATE(DbgKdQuerySpecialCallsApi) },
        { true, STATE_MANIPULATE(DbgKdSetInternalBreakPointApi) },
        { true, STATE_MANIPULATE(DbgKdGetInternalBreakPointApi) },
        { true, STATE_MANIPULATE(DbgKdReadIoSpaceExtendedApi) },
        { true, STATE_MANIPULATE(DbgKdWriteIoSpaceExtendedApi) },
        { true, STATE_MANIPULATE(DbgKdGetVersionApi) },
        { true, STATE_MANIPULATE(DbgKdWriteBreakPointExApi) },
        { true, STATE_MANIPULATE(DbgKdRestoreBreakPointExApi) },
        { true, STATE_MANIPULATE(DbgKdCauseBugCheckApi) },
        { true, STATE_MANIPULATE(DbgKdSwitchProcessor) },
        { true, STATE_MANIPULATE(DbgKdPageInApi) },
        { true, STATE_MANIPULATE(DbgKdReadMachineSpecificRegister) },
        { true, STATE_MANIPULATE(DbgKdWriteMachineSpecificRegister) },
        { true, STATE_MANIPULATE(OldVlm1) },
        { true, STATE_MANIPULATE(OldVlm2) },
        { true, STATE_MANIPULATE(DbgKdSearchMemoryApi) },
        { true, STATE_MANIPULATE(DbgKdGetBusDataApi) },
        { true, STATE_MANIPULATE(DbgKdSetBusDataApi) },
        { true, STATE_MANIPULATE(DbgKdCheckLowMemoryApi) },
        { true, STATE_MANIPULATE(DbgKdClearAllInternalBreakpointsApi) },
        { true, STATE_MANIPULATE(DbgKdFillMemoryApi) },
        { true, STATE_MANIPULATE(DbgKdQueryMemoryApi) },
        { true, STATE_MANIPULATE(DbgKdSwitchPartition) },
        { true, STATE_MANIPULATE(DbgKdWriteCustomBreakpointApi) },
        { true, STATE_MANIPULATE(DbgKdGetContextExApi) },
        { true, STATE_MANIPULATE(DbgKdSetContextExApi) },
        { true, PACKET_LEADER, PACKET_TYPE_KD_DEBUG_IO, sizeof(DBGKD_DEBUG_IO),
            offsetof(DBGKD_DEBUG_IO, ApiNumber), sizeof(debug_io->ApiNumber), DbgKdPrintStringApi },
        { true, PACKET_LEADER, PACKET_TYPE_KD_DEBUG_IO, sizeof(DBGKD_DEBUG_IO),
            offsetof(DBGKD_DEBUG_IO, ApiNumber), sizeof(debug_io->ApiNumber), DbgKdGetStringApi },
        { true, PACKET_LEADER, PACKET_TYPE_KD_STATE_CHANGE64, sizeof(DBGKD_ANY_WAIT_STATE_CHANGE),
            offsetof(DBGKD_ANY_WAIT_STATE_CHANGE, NewState), sizeof(state_change->NewState), DbgKdExceptionStateChange },
        { true, PACKET_LEADER, PACKET_TYPE_KD_STATE_CHANGE64, sizeof(DBGKD_ANY_WAIT_STATE_CHANGE),
            offsetof(DBGKD_ANY_WAIT_STATE_CHANGE, NewState), sizeof(state_change->NewState), DbgKdLoadSymbolsStateChange },
        { true, PACKET_LEADER, PACKET_TYPE_KD_STATE_CHANGE64, sizeof(DBGKD_ANY_WAIT_STATE_CHANGE),
            offsetof(DBGKD_ANY_WAIT_STATE_CHANGE, NewState), sizeof(state_change->NewState), DbgKdCommandStringStateChange },
        { true, PACKET_LEADER, PACKET_TYPE_KD_TRACE_IO, sizeof(DBGKD_TRACE_IO),
            offsetof(DBGKD_TRACE_IO, ApiNumber), sizeof(trace_io->ApiNumber), DbgKdPrintTraceApi },
        { true, PACKET_LEADER, PACKET_TYPE_KD_CONTROL_REQUEST, sizeof(DBGKD_CONTROL_REQUEST),
            offsetof(DBGKD_CONTROL_REQUEST, ApiNumber), sizeof(control_request->ApiNumber), DbgKdRequestHardwareBp },
        { true, PACKET_LEADER, PACKET_TYPE_KD_CONTROL_REQUEST, sizeof(DBGKD_CONTROL_REQUEST),
            offsetof(DBGKD_CONTROL_REQUEST, ApiNumber), sizeof(control_request->ApiNumber), DbgKdReleaseHardwareBp },
        { true, PACKET_LEADER, PACKET_TYPE_KD_FILE_IO, sizeof(DBGKD_FILE_IO),
            offsetof(DBGKD_FILE_IO, ApiNumber), sizeof(file_io->ApiNumber), DbgKdCreateFileApi },
        { true, PACKET_LEADER, PACKET_TYPE_KD_FILE_IO, sizeof(DBGKD_FILE_IO),
            offsetof(DBGKD_FILE_IO, ApiNumber), sizeof(file_io->ApiNumber), DbgKdReadFileApi },
        { true, PACKET_LEADER, PACKET_TYPE_KD_FILE_IO, sizeof(DBGKD_FILE_IO),
            offsetof(DBGKD_FILE_IO, ApiNumber), sizeof(file_io->ApiNumber), DbgKdWriteFileApi },
        { true, PACKET_LEADER, PACKET_TYPE_KD_FILE_IO, sizeof(DBGKD_FILE_IO),
            offsetof(DBGKD_FILE_IO, ApiNumber), sizeof(file_io->ApiNumber), DbgKdCloseFileApi },
        { false, 0, 0, 0, 0, 0 },
    };

    SecureZeroMemory(buffer, sizeof(buffer));

    PACKET* packet = (PACKET*)buffer;

    for (int i = 0; tbl[i].valid; ++i){

        packet->header.PacketLeader = tbl[i].leader;

        packet->header.PacketType = tbl[i].type;

        if (0 == tbl[i].size){

            packet->header.ByteCount = 0;
        }else{

            const int DATA_SECTION_SIZE = 20;

            packet->header.ByteCount = (USHORT)(tbl[i].size + DATA_SECTION_SIZE);

            memset(packet->data + tbl[i].size, '0', DATA_SECTION_SIZE);

            if (sizeof(ULONG) == tbl[i].api_size){

                memcpy(&(packet->data[tbl[i].api_offset]), &(tbl[i].api), sizeof(ULONG));
            }

            packet->data[tbl[i].size + DATA_SECTION_SIZE] = PACKET_TRAILING_BYTE;
        }

        DWORD total_bytes = 0;

        if (0 < packet->header.ByteCount){

            total_bytes = sizeof(PACKET) + packet->header.ByteCount + 1;
        }else{

            total_bytes = sizeof(PACKET);
        }

        DWORD bytes = 0;

        if (false == write_named_pipe(pipe, buffer, total_bytes, &bytes)){

            return false;
        }

        if (total_bytes != bytes){

            put_log("\n --- write_named_pipe: total_bytes != bytes --- \n");
        }

        put_log("\n --- Send to windbg_logger. --- \n");

        if (false == write_windbg_packet_log(buffer, total_bytes)){

            exit(EXIT_FAILURE);
        }

        if (false == recv_named_pipe(pipe, buffer, &total_bytes)){

            exit(EXIT_FAILURE);
        }

        put_log("\n --- Recv from windbg_logger. --- \n");

        if (false == write_windbg_packet_log(buffer, total_bytes)){

            exit(EXIT_FAILURE);
        }

        SecureZeroMemory(buffer, sizeof(buffer));
    }

    return true;
}

static bool end(handles* p)
{
    close_stream();

    if (false == close_named_pipe(&(p->named_pipe))){

        return false;
    }

    return true;
}

