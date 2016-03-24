
// Copyright 2016 Shin'ichi Ichikawa. Released under the GNU General Public License version 3.
// See the file COPYING3. If not see <http://www.gnu.org/licenses/>.

#include "log.h"

bool write_log_etc_packet_file_io(PACKET* packet, DWORD bytes)
{
	if (false == write_windbg_packet_header_log(packet)){

		return false;
	}

	DBGKD_FILE_IO* p = (DBGKD_FILE_IO*)packet->data;

	switch (p->ApiNumber){
	case DbgKdCreateFileApi:
		// Droppable packet.
		put_log("ApiNumber: DbgKdCreateFileApi(%x)", p->ApiNumber);
		put_log("Status: %x", p->Status);
		put_log("DesiredAccess: %x", p->u.CreateFile.DesiredAccess);
		put_log("FileAttributes: %x", p->u.CreateFile.FileAttributes);
		put_log("ShareAccess: %x", p->u.CreateFile.ShareAccess);
		put_log("CreateDisposition: %x", p->u.CreateFile.CreateDisposition);
		put_log("CreateOptions: %x", p->u.CreateFile.CreateOptions);
		put_log("Handle: %x", p->u.CreateFile.Handle);
		put_log("Length: %x", p->u.CreateFile.Length);
		break;
	case DbgKdReadFileApi:
		put_log("ApiNumber: DbgKdReadFileApi(%x)", p->ApiNumber);
		put_log("Status: %x", p->Status);
		put_log("Handle: %x", p->u.ReadFile.Handle);
		put_log("Offset: %x", p->u.ReadFile.Offset);
		put_log("Length: %x", p->u.ReadFile.Length);
		break;
	case DbgKdWriteFileApi:
		put_log("ApiNumber: DbgKdWriteFileApi(%x)", p->ApiNumber);
		put_log("Status: %x", p->Status);
		put_log("Handle: %x", p->u.WriteFile.Handle);
		put_log("Offset: %x", p->u.WriteFile.Offset);
		put_log("Length: %x", p->u.WriteFile.Length);
		break;
	case DbgKdCloseFileApi:
		put_log("ApiNumber: DbgKdCloseFileApi(%x)", p->ApiNumber);
		put_log("Status: %x", p->Status);
		put_log("Handle: %x", p->u.CloseFile.Handle);
		break;
	default:
		put_log("Bad ApiNumber: %x at %s function.", p->ApiNumber, __func__);
		return false;
	}

	return write_log_data_section(packet->data, bytes, sizeof(DBGKD_FILE_IO));
}

