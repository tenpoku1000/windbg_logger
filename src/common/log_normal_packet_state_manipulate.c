
// Copyright 2016 Shin'ichi Ichikawa. Released under the GNU General Public License version 3.
// See the file COPYING3. If not see <http://www.gnu.org/licenses/>.

#include "log.h"

bool write_log_normal_packet_state_manipulate(PACKET* packet, DWORD bytes)
{
    if (false == write_windbg_packet_header_log(packet)){

        return false;
    }

    DBGKD_MANIPULATE_STATE64* p = (DBGKD_MANIPULATE_STATE64*)packet->data;

    switch (p->ApiNumber){
    case DbgKdReadVirtualMemoryApi:
        put_log("ApiNumber: DbgKdReadVirtualMemoryApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("TargetBaseAddress: %x", p->u.ReadMemory.TargetBaseAddress);
        put_log("TransferCount: %x", p->u.ReadMemory.TransferCount);
        put_log("ActualBytesRead: %x", p->u.ReadMemory.ActualBytesRead);
        break;
    case DbgKdWriteVirtualMemoryApi:
        put_log("ApiNumber: DbgKdWriteVirtualMemoryApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("TargetBaseAddress: %x", p->u.WriteMemory.TargetBaseAddress);
        put_log("TransferCount: %x", p->u.WriteMemory.TransferCount);
        put_log("ActualBytesWritten: %x", p->u.WriteMemory.ActualBytesWritten);
        break;
    case DbgKdGetContextApi:
        put_log("ApiNumber: DbgKdGetContextApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
//        typedef struct _DBGKD_GET_CONTEXT
//        {
//            ULONG Unused;
//        } DBGKD_GET_CONTEXT, *PDBGKD_GET_CONTEXT;
        break;
    case DbgKdSetContextApi:
        put_log("ApiNumber: DbgKdSetContextApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("ContextFlags: %x", p->u.SetContext.ContextFlags);
        break;
    case DbgKdWriteBreakPointApi:
        put_log("ApiNumber: DbgKdWriteBreakPointApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("BreakPointAddress: %x", p->u.WriteBreakPoint.BreakPointAddress);
        put_log("BreakPointHandle: %x", p->u.WriteBreakPoint.BreakPointHandle);
        break;
    case DbgKdRestoreBreakPointApi:
        put_log("ApiNumber: DbgKdRestoreBreakPointApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("BreakPointHandle: %x", p->u.RestoreBreakPoint.BreakPointHandle);
        break;
    case DbgKdContinueApi:
        put_log("ApiNumber: DbgKdContinueApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("ContinueStatus: %x", p->u.Continue.ContinueStatus);
        get_ntstatus(p->u.Continue.ContinueStatus);
        break;
    case DbgKdReadControlSpaceApi:
        put_log("ApiNumber: DbgKdReadControlSpaceApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        break;
    case DbgKdWriteControlSpaceApi:
        put_log("ApiNumber: DbgKdWriteControlSpaceApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        break;
    case DbgKdReadIoSpaceApi:
        put_log("ApiNumber: DbgKdReadIoSpaceApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("IoAddress: %x", p->u.ReadWriteIo.IoAddress);
        put_log("DataSize: %x", p->u.ReadWriteIo.DataSize);
        put_log("DataValue: %x", p->u.ReadWriteIo.DataValue);
        break;
    case DbgKdWriteIoSpaceApi:
        put_log("ApiNumber: DbgKdWriteIoSpaceApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("IoAddress: %x", p->u.ReadWriteIo.IoAddress);
        put_log("DataSize: %x", p->u.ReadWriteIo.DataSize);
        put_log("DataValue: %x", p->u.ReadWriteIo.DataValue);
        break;
    case DbgKdRebootApi:
        put_log("ApiNumber: DbgKdRebootApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        break;
    case DbgKdContinueApi2:
        put_log("ApiNumber: DbgKdContinueApi2(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("ContinueStatus: %x", p->u.Continue2.ContinueStatus);
        get_ntstatus(p->u.Continue2.ContinueStatus);
        put_log("TraceFlag: %x", p->u.Continue2.AnyControlSet.Amd64ControlSet.TraceFlag);
        put_log("Dr7: %x", p->u.Continue2.AnyControlSet.Amd64ControlSet.Dr7);
        put_log("CurrentSymbolStart: %x", p->u.Continue2.AnyControlSet.Amd64ControlSet.CurrentSymbolStart);
        put_log("CurrentSymbolEnd: %x", p->u.Continue2.AnyControlSet.Amd64ControlSet.CurrentSymbolEnd);
        break;
    case DbgKdReadPhysicalMemoryApi:
        put_log("ApiNumber: DbgKdReadPhysicalMemoryApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        break;
    case DbgKdWritePhysicalMemoryApi:
        put_log("ApiNumber: DbgKdWritePhysicalMemoryApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        break;
    case DbgKdQuerySpecialCallsApi:
        put_log("ApiNumber: DbgKdQuerySpecialCallsApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("NumberOfSpecialCalls: %x", p->u.QuerySpecialCalls.NumberOfSpecialCalls);
        break;
    case DbgKdSetSpecialCallApi:
        put_log("ApiNumber: DbgKdSetSpecialCallApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("SpecialCall: %x", p->u.SetSpecialCall.SpecialCall);
        break;
    case DbgKdClearSpecialCallsApi:
        put_log("ApiNumber: DbgKdClearSpecialCallsApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        break;
    case DbgKdSetInternalBreakPointApi:
        put_log("ApiNumber: DbgKdSetInternalBreakPointApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("BreakpointAddress: %x", p->u.SetInternalBreakpoint.BreakpointAddress);

        ULONG sbp_flags = p->u.SetInternalBreakpoint.Flags;

        switch (sbp_flags){
        case DBGKD_INTERNAL_BP_FLAG_COUNTONLY:
            put_log("Flags: DBGKD_INTERNAL_BP_FLAG_COUNTONLY(%x)", sbp_flags);
            break;
        case DBGKD_INTERNAL_BP_FLAG_INVALID:
            put_log("Flags: DBGKD_INTERNAL_BP_FLAG_INVALID(%x)", sbp_flags);
            break;
        case DBGKD_INTERNAL_BP_FLAG_SUSPENDED:
            put_log("Flags: DBGKD_INTERNAL_BP_FLAG_SUSPENDED(%x)", sbp_flags);
            break;
        case DBGKD_INTERNAL_BP_FLAG_DYING:
            put_log("Flags: DBGKD_INTERNAL_BP_FLAG_DYING(%x)", sbp_flags);
            break;
        default:
            put_log("Flags: %x", sbp_flags);
            break;
        }
        break;
    case DbgKdGetInternalBreakPointApi:
        put_log("ApiNumber: DbgKdGetInternalBreakPointApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("BreakpointAddress: %x", p->u.GetInternalBreakpoint.BreakpointAddress);

        ULONG gbp_flags = p->u.GetInternalBreakpoint.Flags;

        switch (gbp_flags){
        case DBGKD_INTERNAL_BP_FLAG_COUNTONLY:
            put_log("Flags: DBGKD_INTERNAL_BP_FLAG_COUNTONLY(%x)", gbp_flags);
            break;
        case DBGKD_INTERNAL_BP_FLAG_INVALID:
            put_log("Flags: DBGKD_INTERNAL_BP_FLAG_INVALID(%x)", gbp_flags);
            break;
        case DBGKD_INTERNAL_BP_FLAG_SUSPENDED:
            put_log("Flags: DBGKD_INTERNAL_BP_FLAG_SUSPENDED(%x)", gbp_flags);
            break;
        case DBGKD_INTERNAL_BP_FLAG_DYING:
            put_log("Flags: DBGKD_INTERNAL_BP_FLAG_DYING(%x)", gbp_flags);
            break;
        default:
            put_log("Flags: %x", gbp_flags);
            break;
        }

        put_log("Calls: %x", p->u.GetInternalBreakpoint.Calls);
        put_log("MaxCallsPerPeriod: %x", p->u.GetInternalBreakpoint.MaxCallsPerPeriod);
        put_log("MinInstructions: %x", p->u.GetInternalBreakpoint.MinInstructions);
        put_log("MaxInstructions: %x", p->u.GetInternalBreakpoint.MaxInstructions);
        put_log("TotalInstructions: %x", p->u.GetInternalBreakpoint.TotalInstructions);
        break;
    case DbgKdReadIoSpaceExtendedApi:
        put_log("ApiNumber: DbgKdReadIoSpaceExtendedApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("DataSize: %x", p->u.ReadWriteIoExtended.DataSize);
        put_log("InterfaceType: %x", p->u.ReadWriteIoExtended.InterfaceType);
        put_log("BusNumber: %x", p->u.ReadWriteIoExtended.BusNumber);
        put_log("AddressSpace: %x", p->u.ReadWriteIoExtended.AddressSpace);
        put_log("IoAddress: %x", p->u.ReadWriteIoExtended.IoAddress);
        put_log("DataValue: %x", p->u.ReadWriteIoExtended.DataValue);
        break;
    case DbgKdWriteIoSpaceExtendedApi:
        put_log("ApiNumber: DbgKdWriteIoSpaceExtendedApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("DataSize: %x", p->u.ReadWriteIoExtended.DataSize);
        put_log("InterfaceType: %x", p->u.ReadWriteIoExtended.InterfaceType);
        put_log("BusNumber: %x", p->u.ReadWriteIoExtended.BusNumber);
        put_log("AddressSpace: %x", p->u.ReadWriteIoExtended.AddressSpace);
        put_log("IoAddress: %x", p->u.ReadWriteIoExtended.IoAddress);
        put_log("DataValue: %x", p->u.ReadWriteIoExtended.DataValue);
        break;
    case DbgKdGetVersionApi:
        put_log("ApiNumber: DbgKdGetVersionApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("MajorVersion: %x", p->u.GetVersion64.MajorVersion);
        put_log("MinorVersion: %x", p->u.GetVersion64.MinorVersion);

        UCHAR protocol_version = p->u.GetVersion64.ProtocolVersion;

        switch (protocol_version){
        case DBGKD_64BIT_PROTOCOL_VERSION1:
            put_log("ProtocolVersion: DBGKD_64BIT_PROTOCOL_VERSION1(%x)", protocol_version);
            break;
        case DBGKD_64BIT_PROTOCOL_VERSION2:
            put_log("ProtocolVersion: DBGKD_64BIT_PROTOCOL_VERSION2(%x)", protocol_version);
            break;
        default:
            put_log("ProtocolVersion: %x", protocol_version);
            break;
        }

        put_log("KdSecondaryVersion: %x", p->u.GetVersion64.KdSecondaryVersion);
        put_log("Flags: %x", p->u.GetVersion64.Flags);
        put_log("MachineType: %x", p->u.GetVersion64.MachineType);
        put_log("MaxPacketType: %x", p->u.GetVersion64.MaxPacketType);
        put_log("MaxStateChange: %x", p->u.GetVersion64.MaxStateChange);
        put_log("MaxManipulate: %x", p->u.GetVersion64.MaxManipulate);
        put_log("Simulation: %x", p->u.GetVersion64.Simulation);
        put_log("KernBase: %x", p->u.GetVersion64.KernBase);
        put_log("PsLoadedModuleList: %x", p->u.GetVersion64.PsLoadedModuleList);
        put_log("DebuggerDataList: %x", p->u.GetVersion64.DebuggerDataList);
        break;
    case DbgKdWriteBreakPointExApi:
        put_log("ApiNumber: DbgKdWriteBreakPointExApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("BreakPointCount: %x", p->u.BreakPointEx.BreakPointCount);
        put_log("ContinueStatus: %x", p->u.BreakPointEx.ContinueStatus);
        get_ntstatus(p->u.BreakPointEx.ContinueStatus);
        break;
    case DbgKdRestoreBreakPointExApi:
        put_log("ApiNumber: DbgKdRestoreBreakPointExApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("BreakPointCount: %x", p->u.BreakPointEx.BreakPointCount);
        put_log("ContinueStatus: %x", p->u.BreakPointEx.ContinueStatus);
        get_ntstatus(p->u.BreakPointEx.ContinueStatus);
        break;
    case DbgKdCauseBugCheckApi:
        put_log("ApiNumber: DbgKdCauseBugCheckApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        break;
    case DbgKdSwitchProcessor:
        put_log("ApiNumber: DbgKdSwitchProcessor(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        break;
    case DbgKdPageInApi:
        put_log("ApiNumber: DbgKdPageInApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        break;
    case DbgKdReadMachineSpecificRegister:
        put_log("ApiNumber: DbgKdReadMachineSpecificRegister(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("Msr: %x", p->u.ReadWriteMsr.Msr);
        put_log("DataValueLow: %x", p->u.ReadWriteMsr.DataValueLow);
        put_log("DataValueHigh: %x", p->u.ReadWriteMsr.DataValueHigh);
        break;
    case DbgKdWriteMachineSpecificRegister:
        put_log("ApiNumber: DbgKdWriteMachineSpecificRegister(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("Msr: %x", p->u.ReadWriteMsr.Msr);
        put_log("DataValueLow: %x", p->u.ReadWriteMsr.DataValueLow);
        put_log("DataValueHigh: %x", p->u.ReadWriteMsr.DataValueHigh);
        break;
    case OldVlm1:
        put_log("ApiNumber: OldVlm1(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        break;
    case OldVlm2:
        put_log("ApiNumber: OldVlm2(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        break;
    case DbgKdSearchMemoryApi:
        put_log("ApiNumber: DbgKdSearchMemoryApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        // SearchAddress == FoundAddress
        put_log("SearchAddress: %x", p->u.SearchMemory.SearchAddress);
        put_log("FoundAddress: %x", p->u.SearchMemory.FoundAddress);
        put_log("SearchLength: %x", p->u.SearchMemory.SearchLength);
        put_log("PatternLength: %x", p->u.SearchMemory.PatternLength);
        break;
    case DbgKdGetBusDataApi:
        put_log("ApiNumber: DbgKdGetBusDataApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("BusDataType: %x", p->u.GetSetBusData.BusDataType);
        put_log("BusNumber: %x", p->u.GetSetBusData.BusNumber);
        put_log("SlotNumber: %x", p->u.GetSetBusData.SlotNumber);
        put_log("Offset: %x", p->u.GetSetBusData.Offset);
        put_log("Length: %x", p->u.GetSetBusData.Length);
        break;
    case DbgKdSetBusDataApi:
        put_log("ApiNumber: DbgKdSetBusDataApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("BusDataType: %x", p->u.GetSetBusData.BusDataType);
        put_log("BusNumber: %x", p->u.GetSetBusData.BusNumber);
        put_log("SlotNumber: %x", p->u.GetSetBusData.SlotNumber);
        put_log("Offset: %x", p->u.GetSetBusData.Offset);
        put_log("Length: %x", p->u.GetSetBusData.Length);
        break;
    case DbgKdCheckLowMemoryApi:
        put_log("ApiNumber: DbgKdCheckLowMemoryApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        break;
    case DbgKdClearAllInternalBreakpointsApi:
        put_log("ApiNumber: DbgKdClearAllInternalBreakpointsApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        break;
    case DbgKdFillMemoryApi:
        put_log("ApiNumber: DbgKdFillMemoryApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("Address: %x", p->u.FillMemory.Address);

        USHORT fm_flags = p->u.FillMemory.Flags;

        switch (fm_flags){
        case DBGKD_FILL_MEMORY_VIRTUAL:
            put_log("Flags: DBGKD_FILL_MEMORY_VIRTUAL(%x)", fm_flags);
            break;
        case DBGKD_FILL_MEMORY_PHYSICAL:
            put_log("Flags: DBGKD_FILL_MEMORY_PHYSICAL(%x)", fm_flags);
            break;
        default:
            put_log("Flags: %x", fm_flags);
            break;
        }

        put_log("Length: %x", p->u.FillMemory.Length);
        put_log("PatternLength: %x", p->u.FillMemory.PatternLength);
        break;
    case DbgKdQueryMemoryApi:
        put_log("ApiNumber: DbgKdQueryMemoryApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);

        put_log("Address: %x", p->u.QueryMemory.Address);

        ULONG address_space = p->u.QueryMemory.AddressSpace;

        switch (address_space){
        case DBGKD_QUERY_MEMORY_VIRTUAL:
            put_log("AddressSpace: DBGKD_QUERY_MEMORY_VIRTUAL(%x)", address_space);
            break;
        case DBGKD_QUERY_MEMORY_SESSION:
            put_log("AddressSpace: DBGKD_QUERY_MEMORY_SESSION(%x)", address_space);
            break;
        case DBGKD_QUERY_MEMORY_KERNEL:
            put_log("AddressSpace: DBGKD_QUERY_MEMORY_KERNEL(%x)", address_space);
            break;
        default:
            put_log("AddressSpace: %x", address_space);
            break;
        }

        ULONG qm_flags = p->u.QueryMemory.Flags;

        switch (qm_flags){
        case DBGKD_QUERY_MEMORY_READ:
            put_log("Flags: DBGKD_QUERY_MEMORY_READ(%x)", qm_flags);
            break;
        case DBGKD_QUERY_MEMORY_WRITE:
            put_log("Flags: DBGKD_QUERY_MEMORY_WRITE(%x)", qm_flags);
            break;
        case DBGKD_QUERY_MEMORY_EXECUTE:
            put_log("Flags: DBGKD_QUERY_MEMORY_EXECUTE(%x)", qm_flags);
            break;
        case DBGKD_QUERY_MEMORY_FIXED:
            put_log("Flags: DBGKD_QUERY_MEMORY_FIXED(%x)", qm_flags);
            break;
        default:
            put_log("Flags: %x", qm_flags);
            break;
        }
        break;
    case DbgKdSwitchPartition:
        put_log("ApiNumber: DbgKdSwitchPartition(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);

        ULONG partition = p->u.SwitchPartition.Partition;

        switch (partition){
        case DBGKD_PARTITION_DEFAULT:
            put_log("Partition: DBGKD_PARTITION_DEFAULT(%x)", partition);
            break;
        case DBGKD_PARTITION_ALTERNATE:
            put_log("Partition: DBGKD_PARTITION_ALTERNATE(%x)", partition);
            break;
        default:
            put_log("Partition: %x", partition);
            break;
        }
        break;
    case DbgKdWriteCustomBreakpointApi:
        put_log("ApiNumber: DbgKdWriteCustomBreakpointApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("BreakPointAddress: %x", p->u.WriteCustomBreakpoint.BreakPointAddress);
        put_log("BreakPointInstruction: %x", p->u.WriteCustomBreakpoint.BreakPointInstruction);
        put_log("BreakPointHandle: %x", p->u.WriteCustomBreakpoint.BreakPointHandle);
        put_log("BreakPointInstructionSize: %x", p->u.WriteCustomBreakpoint.BreakPointInstructionSize);
        put_log("BreakPointInstructionAlignment: %x", p->u.WriteCustomBreakpoint.BreakPointInstructionAlignment);
        break;
    case DbgKdGetContextExApi:
        put_log("ApiNumber: DbgKdGetContextExApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("Offset: %x", p->u.ContextEx.Offset);
        put_log("ByteCount: %x", p->u.ContextEx.ByteCount);
        put_log("BytesCopied: %x", p->u.ContextEx.BytesCopied);
        break;
    case DbgKdSetContextExApi:
        put_log("ApiNumber: DbgKdSetContextExApi(%x)", p->ApiNumber);
        put_log("ProcessorLevel: %x", p->ProcessorLevel);
        put_log("Processor: %x", p->Processor);
        put_log("ReturnStatus: %x", p->ReturnStatus);
        get_ntstatus(p->ReturnStatus);
        put_log("Offset: %x", p->u.ContextEx.Offset);
        put_log("ByteCount: %x", p->u.ContextEx.ByteCount);
        put_log("BytesCopied: %x", p->u.ContextEx.BytesCopied);
        break;
    default:
        put_log("Bad ApiNumber: %x at %s function.", p->ApiNumber, __func__);
        return false;
    }

    return write_log_data_section(packet->data, bytes, sizeof(DBGKD_MANIPULATE_STATE64));
}

