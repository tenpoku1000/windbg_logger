
// Copyright 2016 Shin'ichi Ichikawa. Released under the GNU General Public License version 3.
// See the file COPYING3. If not see <http://www.gnu.org/licenses/>.

#include "log.h"

bool write_log_normal_packet_state_change64(PACKET* packet, DWORD bytes)
{
    if (false == write_windbg_packet_header_log(packet)){

        return false;
    }

    DBGKD_ANY_WAIT_STATE_CHANGE* p = (DBGKD_ANY_WAIT_STATE_CHANGE*)packet->data;

    if (DbgKdAlternateStateChange & p->NewState){

        p->NewState &= ~DbgKdAlternateStateChange;

        put_log("NewState: DbgKdAlternateStateChange(%x)", DbgKdAlternateStateChange);
    }

    switch (p->NewState){
    case DbgKdExceptionStateChange:
        put_log("NewState: DbgKdExceptionStateChange(%x)", p->NewState);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("NumberProcessors: %x", p->NumberProcessors);
        put_log("Thread: %x", p->Thread);
        put_log("ProgramCounter: %x", p->ProgramCounter);
        put_log("ExceptionCode: %x", p->u.Exception.ExceptionRecord.ExceptionCode);
        put_log("ExceptionFlags: %x", p->u.Exception.ExceptionRecord.ExceptionFlags);
        put_log("ExceptionRecord: %x", p->u.Exception.ExceptionRecord.ExceptionRecord);
        put_log("ExceptionAddress: %x", p->u.Exception.ExceptionRecord.ExceptionAddress);
        put_log("NumberParameters: %x", p->u.Exception.ExceptionRecord.NumberParameters);

        for (int i = 0; EXCEPTION_MAXIMUM_PARAMETERS > i; ++i) {

            put_log("ExceptionInformation[%02d]: %x", i, p->u.Exception.ExceptionRecord.ExceptionInformation[i]);
        }

        put_log("FirstChance: %x", p->u.Exception.FirstChance);
        break;
    case DbgKdLoadSymbolsStateChange:
        // Droppable packet.
        put_log("NewState: DbgKdLoadSymbolsStateChange(%x)", p->NewState);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("NumberProcessors: %x", p->NumberProcessors);
        put_log("Thread: %x", p->Thread);
        put_log("ProgramCounter: %x", p->ProgramCounter);
        put_log("PathNameLength: %x", p->u.LoadSymbols.PathNameLength);
        put_log("BaseOfDll: %x", p->u.LoadSymbols.BaseOfDll);
        put_log("ProcessId: %x", p->u.LoadSymbols.ProcessId);
        put_log("CheckSum: %x", p->u.LoadSymbols.CheckSum);
        put_log("SizeOfImage: %x", p->u.LoadSymbols.SizeOfImage);
        put_log("UnloadSymbols: %x", p->u.LoadSymbols.UnloadSymbols);
        break;
    case DbgKdCommandStringStateChange:
        put_log("NewState: DbgKdCommandStringStateChange(%x)", p->NewState);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("NumberProcessors: %x", p->NumberProcessors);
        put_log("Thread: %x", p->Thread);
        put_log("ProgramCounter: %x", p->ProgramCounter);
        put_log("Flags: %x", p->u.CommandString.Flags);
        break;
    default:
        put_log("Bad NewState: %x at %s function.", p->NewState, __func__);
        return false;
    }

    put_log("Dr6: %x", p->AnyControlReport.Amd64ControlReport.Dr6);
    put_log("Dr7: %x", p->AnyControlReport.Amd64ControlReport.Dr7);
    put_log("EFlags: %x", p->AnyControlReport.Amd64ControlReport.EFlags);
    put_log("InstructionCount: %x", p->AnyControlReport.Amd64ControlReport.InstructionCount);

    switch (p->AnyControlReport.Amd64ControlReport.ReportFlags){
    case REPORT_INCLUDES_SEGS:
        put_log("ReportFlags: REPORT_INCLUDES_SEGS(%x)", p->AnyControlReport.Amd64ControlReport.ReportFlags);
        break;
    case REPORT_STANDARD_CS:
        put_log("ReportFlags: REPORT_STANDARD_CS(%x)", p->AnyControlReport.Amd64ControlReport.ReportFlags);
        break;
    default:
        put_log("ReportFlags: %x", p->AnyControlReport.Amd64ControlReport.ReportFlags);
        break;
    }

    for (int i = 0; DBGKD_MAXSTREAM > i; ++i){

        put_log("InstructionStream[%02d]: %x", i, p->AnyControlReport.Amd64ControlReport.InstructionStream[i]);
    }

    put_log("SegCs: %x", p->AnyControlReport.Amd64ControlReport.SegCs);
    put_log("SegDs: %x", p->AnyControlReport.Amd64ControlReport.SegDs);
    put_log("SegEs: %x", p->AnyControlReport.Amd64ControlReport.SegEs);
    put_log("SegFs: %x", p->AnyControlReport.Amd64ControlReport.SegFs);

    return write_log_data_section(packet->data, bytes, sizeof(DBGKD_ANY_WAIT_STATE_CHANGE));
}

