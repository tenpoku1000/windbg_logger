
// Copyright 2016 Shin'ichi Ichikawa. Released under the GNU General Public License version 3.
// See the file COPYING3. If not see <http://www.gnu.org/licenses/>.

#if ! defined(LOG_H_)
#define LOG_H_

#include <windows.h>
#include <stdbool.h>
#include "windbgkd.h"
#include "utils.h"

typedef struct PACKET_{
    KD_PACKET header;
    UINT8 data[];
}PACKET;

bool write_windbg_packet_log(LPVOID buffer, DWORD bytes);
bool write_windbg_packet_header_log(PACKET* packet);
bool write_log_data_section(LPVOID buffer, DWORD bytes, DWORD header_section_size);

#endif

