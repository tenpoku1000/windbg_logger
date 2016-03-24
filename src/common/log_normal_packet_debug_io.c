
// Copyright 2016 Shin'ichi Ichikawa. Released under the GNU General Public License version 3.
// See the file COPYING3. If not see <http://www.gnu.org/licenses/>.

#include "log.h"

bool write_log_normal_packet_debug_io(PACKET* packet, DWORD bytes)
{
	if (false == write_windbg_packet_header_log(packet)){

		return false;
	}

	DBGKD_DEBUG_IO* p = (DBGKD_DEBUG_IO*)packet->data;

	switch (p->ApiNumber){
	case DbgKdPrintStringApi:
		// Droppable packet.
		put_log("ApiNumber: DbgKdPrintStringApi(%x)", p->ApiNumber);
		put_log("ProcessorLevel: %x", p->ProcessorLevel);
		put_log("Processor: %x", p->Processor);
		put_log("LengthOfString: %x", p->u.PrintString.LengthOfString);
		break;
	case DbgKdGetStringApi:
		put_log("ApiNumber: DbgKdGetStringApi(%x)", p->ApiNumber);
		put_log("ProcessorLevel: %x", p->ProcessorLevel);
		put_log("Processor: %x", p->Processor);
		put_log("LengthOfPromptString: %x", p->u.GetString.LengthOfPromptString);
		put_log("LengthOfStringRead: %x", p->u.GetString.LengthOfStringRead);
		break;
	default:
		put_log("Bad ApiNumber: %x at %s function.", p->ApiNumber, __func__);
		return false;
	}

	return write_log_data_section(packet->data, bytes, sizeof(DBGKD_DEBUG_IO));
}

