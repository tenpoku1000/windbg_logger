
// Copyright 2016 Shin'ichi Ichikawa. Released under the GNU General Public License version 3.
// See the file COPYING3. If not see <http://www.gnu.org/licenses/>.

#if ! defined(THREAD_H_)
#define THREAD_H_

#include <windows.h>
#include <process.h>
#include <stdbool.h>

typedef unsigned __stdcall thread_func(void* arg);

bool create_thread(thread_func func, void* arg, HANDLE* handle);
bool resume_thread(HANDLE handle);
bool close_thread(HANDLE* handle);

#endif

