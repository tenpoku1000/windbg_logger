
// Copyright 2016 Shin'ichi Ichikawa. Released under the GNU General Public License version 3.
// See the file COPYING3. If not see <http://www.gnu.org/licenses/>.

#if ! defined(UART_H_)
#define UART_H_

#include <windows.h>
#include <stdbool.h>

bool create_UART(HANDLE* UART, wchar_t* port_name);
bool read_UART(HANDLE UART, LPVOID buffer, DWORD read_size, LPDWORD read_bytes);
bool write_UART(HANDLE UART, LPVOID buffer, DWORD write_size, LPDWORD written_size);
bool close_UART(HANDLE* UART);

#endif

