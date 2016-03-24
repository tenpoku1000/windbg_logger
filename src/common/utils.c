
// Copyright 2016 Shin'ichi Ichikawa. Released under the GNU General Public License version 3.
// See the file COPYING3. If not see <http://www.gnu.org/licenses/>.

#include <windows.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <share.h>
#include "utils.h"

static FILE* stream = NULL;
static wchar_t ini_path[_MAX_PATH] = {0};
static wchar_t log_path[_MAX_PATH] = { 0 };
static CRITICAL_SECTION critical_section = { 0 };

static bool get_ini_path(void);
static bool get_log_path(void);

void set_stream(FILE* fp)
{
	stream = fp;
}

void close_stream(void)
{
	if (stream){

		fclose(stream);
		stream = NULL;
	}
}

void put_log(char* fmt, ...)
{
	EnterCriticalSection(&critical_section);

	va_list arg;

	va_start(arg, fmt);

	if (stream){

		vfprintf(stream, fmt, arg);
		fprintf(stream, "\n");
	}

	vfprintf(stderr, fmt, arg);
	fprintf(stderr, "\n");

	va_end(arg);

	LeaveCriticalSection(&critical_section);
}

void put_log_2(bool to_file, char* fmt, ...)
{
	EnterCriticalSection(&critical_section);

	va_list arg;

	va_start(arg, fmt);

	if (to_file){

		if (stream){

			vfprintf(stream, fmt, arg);
			fprintf(stream, "\n");
		}
	}else{

		vfprintf(stderr, fmt, arg);
		fprintf(stderr, "\n");
	}

	va_end(arg);

	LeaveCriticalSection(&critical_section);
}

void put_log_no_crlf(char* fmt, ...)
{
	EnterCriticalSection(&critical_section);

	va_list arg;

	va_start(arg, fmt);

	if (stream){

		vfprintf(stream, fmt, arg);
	}

	vfprintf(stderr, fmt, arg);

	va_end(arg);

	LeaveCriticalSection(&critical_section);
}

void get_last_error(char* func_name, int line)
{
    LPVOID msg = NULL;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&msg,
        0,
        NULL
    );

	if (msg){

		char* sjis = NULL;

		if (conv_wide2mb(CP_SHIFT_JIS, &sjis, msg)){

			put_log_2(false, "%s : %s function(%d).", sjis, func_name, line);

			free(sjis);
			sjis = NULL;
		}

		char* utf8 = NULL;

		if (conv_wide2mb(CP_UTF8, &utf8, msg)){

			put_log_2(true, "%s : %s function(%d).", utf8, func_name, line);

			free(utf8);
			utf8 = NULL;
		}

		LocalFree(msg);
		msg = NULL;
	}else{

		put_log("%s", msg);
	}

	SetLastError(0);
}

void get_ntstatus(NTSTATUS status)
{
	HMODULE handle = LoadLibrary(L"NTDLL.DLL");

	if (handle){

		LPVOID msg = NULL;

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			handle,
			status,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPWSTR)&msg,
			0,
			NULL
		);

		if (msg){

			char* sjis = NULL;

			if (conv_wide2mb(CP_SHIFT_JIS, &sjis, msg)){

				put_log_2(false, "%s", sjis);

				free(sjis);
				sjis = NULL;
			}

			char* utf8 = NULL;

			if (conv_wide2mb(CP_UTF8, &utf8, msg)){

				put_log_2(true, "%s", utf8);

				free(utf8);
				utf8 = NULL;
			}

			LocalFree(msg);
			msg = NULL;
		}else{

			put_log("%s", msg);
		}

		(void)FreeLibrary(handle);
		handle = NULL;
	}
}

bool conv_wide2mb(UINT code_page, char** to, wchar_t* from)
{
	int from_num = (int)wcslen(from);

	int num = WideCharToMultiByte(
		code_page,
		0,
		from,
		from_num,
		NULL,
		0,
		NULL,
		NULL
	);

	if (0 == num){

		return false;
	}

	++num;

	char* p = (char*)calloc(num, sizeof(char));

	if (NULL == p){

		return false;
	}

	num = WideCharToMultiByte(
		code_page,
		0,
		from,
		from_num,
		p,
		num,
		NULL,
		NULL
	);

	if (0 == num){

		free(p);
		p = NULL;

		return false;
	}

	*to = p;

	return true;
}

bool get_ini_values(ini_values* values)
{
	InitializeCriticalSection(&critical_section);

	SecureZeroMemory(values, sizeof(ini_values));

	if (false == get_ini_path()){

		return false;
	}

	typedef struct local_table_{
		wchar_t* key;
		wchar_t* def_str;
		wchar_t* str;
		size_t size;
	}local_table;

	local_table tbl[] = {
		{ L"named_pipe", L"com1", values->named_pipe, sizeof(values->named_pipe) },
		{ L"UART", L"COM1", values->UART, sizeof(values->UART) },
		{ L"test_send_UART", L"COM1", values->test_send_UART, sizeof(values->test_send_UART) },
		{ L"test_recv_UART", L"COM2", values->test_recv_UART, sizeof(values->test_recv_UART) },
		{ NULL, NULL, 0 },
	};

	for (int i = 0; NULL != tbl[i].key; ++i){

		_set_errno(0);

		GetPrivateProfileString(
			L"windbg_logger",
			tbl[i].key,
			tbl[i].def_str,
			tbl[i].str,
			sizeof(tbl[i].str),
			ini_path
		);

		if (ENOENT == errno){

			get_last_error(__func__, __LINE__);

			_set_errno(0);

			return false;
		}
	}

	wchar_t named_pipe[NAMED_PIPE_NAME_LENGTH];

	int code = swprintf_s(
		named_pipe, NAMED_PIPE_NAME_LENGTH, L"\\\\.\\pipe\\%s", values->named_pipe
	);

	if (-1 == code) {

		put_log("swprintf_s() faild %s function.", __func__);

		return false;
	}

	errno_t error_code = wcscpy_s(values->named_pipe, NAMED_PIPE_NAME_LENGTH, named_pipe);

	if (0 != error_code) {

		put_log("wcscpy_s() error_code = (%d) %s function.", error_code, __func__);

		return false;
	}

	UINT test_mode_value = GetPrivateProfileInt(L"windbg_logger", L"test_mode", 0, ini_path);

	if (0 != test_mode_value){

		values->test_mode = true;
	}else{

		values->test_mode = false;
	}

	if (false == get_log_path()){

		return false;
	}

	values->stream = _wfsopen(log_path, L"at", _SH_DENYWR);

	if (NULL == values->stream){

		put_log("_wfsopen() errno = (%d) %s function.", errno, __func__);

		return false;
	}

	return true;
}

static bool get_ini_path(void)
{
	SecureZeroMemory(ini_path, sizeof(ini_path));

	DWORD path_size = GetModuleFileName(NULL, ini_path, sizeof(ini_path));

	if (0 == path_size){

		get_last_error(__func__, __LINE__);

		return false;
	}

	wchar_t drive[_MAX_DRIVE];
	wchar_t dir[_MAX_DIR];
	wchar_t fname[_MAX_FNAME];
	wchar_t ext[_MAX_EXT];

	errno_t error_code = _wsplitpath_s(
		ini_path, drive, _MAX_DRIVE, dir, _MAX_DIR,
		fname, _MAX_FNAME, ext, _MAX_EXT
	);

	if (0 != error_code){

		put_log("_wsplitpath_s() error_code = (%d) %s function.", error_code, __func__);

		return false;
	}

	error_code = wcscat_s(dir, _MAX_DIR, L"..\\setup\\");

	if (0 != error_code){

		put_log("wcscat_s() error_code = (%d) %s function.", error_code, __func__);

		return false;
	}
	
	error_code = wcscpy_s(fname, _MAX_FNAME, L"windbg_logger");

	if (0 != error_code){

		put_log("wcscpy_s() error_code = (%d) %s function.", error_code, __func__);

		return false;
	}

	error_code = wcscpy_s(ext, _MAX_EXT, L"ini");

	if (0 != error_code){

		put_log("wcscpy_s() error_code = (%d) %s function.", error_code, __func__);

		return false;
	}

	SecureZeroMemory(ini_path, sizeof(ini_path));

	error_code = _wmakepath_s(ini_path, sizeof(ini_path) / sizeof(wchar_t), drive, dir, fname, ext);

	if (0 != error_code){

		put_log("_wmakepath_s() error_code = (%d) %s function.", error_code, __func__);

		return false;
	}

	return true;
}

static bool get_log_path(void)
{
	SecureZeroMemory(log_path, sizeof(log_path));

	DWORD path_size = GetModuleFileName(NULL, log_path, sizeof(log_path));

	if (0 == path_size){

		get_last_error(__func__, __LINE__);

		return false;
	}

	wchar_t drive[_MAX_DRIVE];
	wchar_t dir[_MAX_DIR];
	wchar_t fname[_MAX_FNAME];
	wchar_t ext[_MAX_EXT];

	errno_t error_code = _wsplitpath_s(
		log_path, drive, _MAX_DRIVE, dir, _MAX_DIR,
		fname, _MAX_FNAME, ext, _MAX_EXT
	);

	if (0 != error_code){

		put_log("_wsplitpath_s() error_code = (%d) %s function.", error_code, __func__);

		return false;
	}

	error_code = wcscat_s(dir, _MAX_DIR, L"..\\log\\");

	if (0 != error_code) {

		put_log("wcscat_s() error_code = (%d) %s function.", error_code, __func__);

		return false;
	}

	wchar_t suffix[_MAX_FNAME];

	error_code = wcscpy_s(suffix, _MAX_FNAME, fname);

	if (0 != error_code) {

		put_log("wcscpy_s() error_code = (%d) %s function.", error_code, __func__);

		return false;
	}

	SYSTEMTIME time;
	SecureZeroMemory(&time, sizeof(time));

	GetLocalTime(&time);

	int code = swprintf_s(
		fname, _MAX_FNAME, L"%04d-%02d-%02d_%02d%02d%02d_%s",
		time.wYear, time.wMonth, time.wDay,
		time.wHour, time.wMinute, time.wSecond,
		suffix
	);

	if (-1 == code){

		put_log("swprintf_s() failed %s function.", __func__);

		return false;
	}

	error_code = wcscpy_s(ext, _MAX_EXT, L"log");

	if (0 != error_code) {

		put_log("wcscpy_s() error_code = (%d) %s function.", error_code, __func__);

		return false;
	}

	SecureZeroMemory(log_path, sizeof(log_path));

	error_code = _wmakepath_s(log_path, sizeof(log_path) / sizeof(wchar_t), drive, dir, fname, ext);

	if (0 != error_code) {

		put_log("_wmakepath_s() error_code = (%d) %s function.", error_code, __func__);

		return false;
	}

	return true;
}

