
// Copyright 2016 Shin'ichi Ichikawa. Released under the GNU General Public License version 3.
// See the file COPYING3. If not see <http://www.gnu.org/licenses/>.

#include "named_pipe.h"
#include "windbgkd.h"
#include "utils.h"
#include "log.h"

bool recv_named_pipe(HANDLE pipe, char* buffer, DWORD* total_bytes)
{
	UCHAR* packet_buffer = (UCHAR*)buffer;

	READ_PACKET_STATE state = READ_PACKET_STATE_FIRST;

	DWORD read_bytes = 1;

	DWORD recv_bytes = 0;

	for (;;){

		DWORD bytes = 0;

		if (false == read_named_pipe(pipe, packet_buffer, read_bytes, &bytes)){

			return false;
		}

		if (0 == bytes){

			Sleep(5);

			continue;
		}

		if (READ_PACKET_STATE_FIRST == state){

			if ((BREAKIN_PACKET_BYTE != buffer[0]) &&
				(PACKET_LEADER_BYTE != buffer[0]) &&
				(CONTROL_PACKET_LEADER_BYTE != buffer[0])){

				continue;
			}

			state = READ_PACKET_STATE_HEADER;

			if (BREAKIN_PACKET_BYTE == buffer[0]){

				recv_bytes = 1;

				break;
			}
		}

		recv_bytes += bytes;

		packet_buffer += bytes;

		if (READ_PACKET_STATE_HEADER == state){

			if (sizeof(PACKET) > recv_bytes){

				read_bytes = sizeof(PACKET) - recv_bytes;

				continue;
			}
		}

		state = READ_PACKET_STATE_BODY;

		PACKET* packet = (PACKET*)buffer;

		if (0 < packet->header.ByteCount){

			DWORD packet_size = sizeof(PACKET) + packet->header.ByteCount + 1;

			if (packet_size > recv_bytes){

				read_bytes = packet_size - recv_bytes;

				continue;
			}
		}

		break;
	}

	*total_bytes = recv_bytes;

	return true;
}

bool create_named_pipe(wchar_t* name, HANDLE* pipe)
{
	*pipe = CreateNamedPipe(
		name,
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_WAIT,
		1,
		PACKET_MAX_SIZE,
		PACKET_MAX_SIZE,
		0,
		NULL
	);

	if (INVALID_HANDLE_VALUE == *pipe){

		get_last_error(__func__, __LINE__);

		return false;
	}

	return true;
}

bool connect_named_pipe(HANDLE pipe)
{
	if (FALSE == ConnectNamedPipe(pipe, NULL)){

		if (ERROR_PIPE_CONNECTED == GetLastError()){

			return true;
		}

		get_last_error(__func__, __LINE__);

		return false;
	}

	return true;
}

bool create_file_named_pipe(wchar_t* name, HANDLE* pipe)
{
	*pipe = CreateFile(name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (INVALID_HANDLE_VALUE == *pipe){

		get_last_error(__func__, __LINE__);

		return false;
	}

	return true;
}

bool read_named_pipe(HANDLE pipe, LPVOID buffer, DWORD read_size, LPDWORD read_bytes)
{
	DWORD avail_bytes = 0;

	if (FALSE == PeekNamedPipe(pipe, NULL, 0, NULL, &avail_bytes, NULL)){

		get_last_error(__func__, __LINE__);

		return false;
	}

	if (0 == avail_bytes){

		*read_bytes = 0;

		return true;
	}

	if (FALSE == ReadFile(pipe, buffer, read_size, read_bytes, NULL)){

		get_last_error(__func__, __LINE__);

		return false;
	}

	return true;
}

bool write_named_pipe(HANDLE pipe, LPVOID buffer, DWORD write_size, LPDWORD written_size)
{
	if (FALSE == WriteFile(pipe, buffer, write_size, written_size, NULL)){

		get_last_error(__func__, __LINE__);

		return false;
	}

	return true;
}

bool close_named_pipe(HANDLE* pipe)
{
	if (FALSE == CloseHandle(*pipe)){

		get_last_error(__func__, __LINE__);

		*pipe = NULL;

		return false;
	}

	*pipe = NULL;

	return true;
}

