
// Copyright 2016 Shin'ichi Ichikawa. Released under the GNU General Public License version 3.
// See the file COPYING3. If not see <http://www.gnu.org/licenses/>.

#include "log.h"

bool write_log_etc_packet_trace_io(PACKET* packet, DWORD bytes)
{
    if (false == write_windbg_packet_header_log(packet)){

        return false;
    }

    DBGKD_TRACE_IO* p = (DBGKD_TRACE_IO*)packet->data;

    if (DbgKdPrintTraceApi == p->ApiNumber){

        put_log("ApiNumber: DbgKdPrintTraceApi(%x)", p->ApiNumber);
    }else{

        put_log("Bad ApiNumber: %x at %s function.", p->ApiNumber, __func__);

        return false;
    }

    put_log("ProcessorLevel: %x", p->ProcessorLevel);
    put_log("Processor: %x", p->Processor);
    put_log("LengthOfData: %x", p->u.PrintTrace.LengthOfData);

    return write_log_data_section(packet->data, bytes, sizeof(DBGKD_TRACE_IO));
}

