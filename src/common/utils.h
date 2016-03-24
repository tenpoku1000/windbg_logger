
// Copyright 2016 Shin'ichi Ichikawa. Released under the GNU General Public License version 3.
// See the file COPYING3. If not see <http://www.gnu.org/licenses/>.

#if ! defined(UTILS_H_)
#define UTILS_H_

#include <stdio.h>
#include <stdbool.h>

#define CP_SHIFT_JIS 932

#define NAMED_PIPE_NAME_LENGTH 257

typedef struct ini_values_{
	wchar_t named_pipe[NAMED_PIPE_NAME_LENGTH];
	bool test_mode;
	wchar_t UART[7];
	wchar_t test_send_UART[7];
	wchar_t test_recv_UART[7];
	FILE* stream;
}ini_values;

void set_stream(FILE* fp);
void close_stream(void);
void put_log(char* fmt, ...);
void put_log_2(bool to_file, char* fmt, ...);
void put_log_no_crlf(char* fmt, ...);
void get_last_error(char* func_name, int line);
void get_ntstatus(NTSTATUS status);
bool conv_wide2mb(UINT code_page, char** to, wchar_t* from);
bool get_ini_values(ini_values* values);

#endif

