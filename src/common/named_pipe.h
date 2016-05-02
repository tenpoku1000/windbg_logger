
// Copyright 2016 Shin'ichi Ichikawa. Released under the GNU General Public License version 3.
// See the file COPYING3. If not see <http://www.gnu.org/licenses/>.

#if ! defined(NAMED_PIPE_H_)
#define NAMED_PIPE_H_

#include <windows.h>
#include <stdbool.h>

typedef enum READ_PACKET_STATE_{
    READ_PACKET_STATE_FIRST,
    READ_PACKET_STATE_HEADER,
    READ_PACKET_STATE_BODY,
}READ_PACKET_STATE;

bool recv_named_pipe(HANDLE pipe, char* buffer, DWORD* total_bytes);
bool create_named_pipe(wchar_t* name, HANDLE* pipe);
bool connect_named_pipe(HANDLE pipe);
bool create_file_named_pipe(wchar_t* name, HANDLE* pipe);
bool read_named_pipe(HANDLE pipe, LPVOID buffer, DWORD read_size, LPDWORD read_bytes);
bool write_named_pipe(HANDLE pipe, LPVOID buffer, DWORD write_size, LPDWORD written_size);
bool close_named_pipe(HANDLE* pipe);

#endif

