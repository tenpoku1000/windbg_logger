
// Copyright 2016 Shin'ichi Ichikawa. Released under the GNU General Public License version 3.
// See the file COPYING3. If not see <http://www.gnu.org/licenses/>.

#include "log.h"

bool write_log_etc_packet_control_request(PACKET* packet, DWORD bytes)
{
	if (false == write_windbg_packet_header_log(packet)){

		return false;
	}

	DBGKD_CONTROL_REQUEST* p = (DBGKD_CONTROL_REQUEST*)packet->data;

	switch (p->ApiNumber){
	case DbgKdRequestHardwareBp:
		put_log("ApiNumber: DbgKdRequestHardwareBp(%x)", p->ApiNumber);
		put_log("HardwareBreakPointNumber: %x", p->u.RequestBreakpoint.HardwareBreakPointNumber);
		put_log("Available: %x", p->u.RequestBreakpoint.Available);
		break;
	case DbgKdReleaseHardwareBp:
		put_log("ApiNumber: DbgKdReleaseHardwareBp(%x)", p->ApiNumber);
		put_log("HardwareBreakPointNumber: %x", p->u.ReleaseBreakpoint.HardwareBreakPointNumber);
		put_log("Released: %x", p->u.ReleaseBreakpoint.Released);
		break;
	default:
		put_log("Bad ApiNumber: %x at %s function.", p->ApiNumber, __func__);
		return false;
	}

	return write_log_data_section(packet->data, bytes, sizeof(DBGKD_CONTROL_REQUEST));
}

