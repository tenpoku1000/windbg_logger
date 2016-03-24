
// Copyright 2016 Shin'ichi Ichikawa. Released under the GNU General Public License version 3.
// See the file COPYING3. If not see <http://www.gnu.org/licenses/>.

#include "log.h"

bool write_log_breakin_packet(PACKET* packet, DWORD bytes)
{
	return write_windbg_packet_header_log(packet);
}

