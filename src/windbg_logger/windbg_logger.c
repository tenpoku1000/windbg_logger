
// Copyright 2016 Shin'ichi Ichikawa. Released under the GNU General Public License version 3.
// See the file COPYING3. If not see <http://www.gnu.org/licenses/>.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "thread.h"
#include "named_pipe.h"
#include "UART.h"
#include "utils.h"
#include "log.h"
#include "windbgkd.h"

typedef struct handles_{
    HANDLE thread;
    HANDLE named_pipe;
    bool test_mode;
    HANDLE UART1;
    HANDLE UART2;
}handles;

static bool init(handles* p);
static bool recv_named_pipe_send_com(handles* p);
static unsigned __stdcall recv_com_send_named_pipe(void* arg);
static bool recv_com(handles* p, char* buffer, DWORD* total_bytes);
static bool end(handles* p);

static BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)
{
    exit(EXIT_SUCCESS);

    return FALSE;
}

int main()
{
    handles arg;
    SecureZeroMemory(&arg, sizeof(arg));

    if (false == init(&arg)){

        goto fail;
    }

    if (false == recv_named_pipe_send_com(&arg)){

        (void)end(&arg);

        goto fail;
    }

    if (false == end(&arg)){

        goto fail;
    }

    exit(EXIT_SUCCESS);

fail:
    close_stream();

    exit(EXIT_FAILURE);
}

static bool init(handles* p)
{
    SetConsoleCtrlHandler(HandlerRoutine, TRUE);

    RemoveMenu(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_BYCOMMAND);

    ini_values values;

    if (false == get_ini_values(&values)){

        return false;
    }

    set_stream(values.stream);

    if (false == create_named_pipe(values.named_pipe, &(p->named_pipe))){

        return false;
    }

    p->test_mode = values.test_mode;

    if (p->test_mode){

        if (false == create_UART(&(p->UART1), values.test_send_UART)){

            return false;
        }

        if (false == create_UART(&(p->UART2), values.test_recv_UART)){

            (void)close_UART(&(p->UART1));

            return false;
        }
    }else{

        if (false == create_UART(&(p->UART1), values.UART)){

            return false;
        }

        p->UART2 = NULL;
    }

    if (false == create_thread(recv_com_send_named_pipe, p, &(p->thread))){

        return false;
    }

    if (false == resume_thread(p->thread)){

        return false;
    }

    return true;
}

static bool recv_named_pipe_send_com(handles* p)
{
    if (false == connect_named_pipe(p->named_pipe)){

        return false;
    }

    static char buffer[PACKET_MAX_SIZE];

    for (;;){

        SecureZeroMemory(buffer, sizeof(buffer));

        DWORD total_bytes = 0;

        if (false == recv_named_pipe(p->named_pipe, buffer, &total_bytes)){

            return false;
        }

        put_log("\n --- Send to target PC. --- \n");

        if (false == write_windbg_packet_log(buffer, total_bytes)){

            return false;
        }

        DWORD bytes = 0;

        if (false == write_UART(p->UART1, buffer, total_bytes, &bytes)){

            return false;
        }

        if (total_bytes != bytes){

            put_log("\n --- write_UART: total_bytes != bytes --- \n");
        }
    }

    return true;
}

static unsigned __stdcall recv_com_send_named_pipe(void* arg)
{
    if (NULL == arg){

        exit(EXIT_FAILURE);
    }

    handles* p = (handles*)arg;

    static char buffer[PACKET_MAX_SIZE];

    for (;;){

        SecureZeroMemory(buffer, sizeof(buffer));

        DWORD total_bytes = 0;

        if (false == recv_com(p, buffer, &total_bytes)){

            exit(EXIT_FAILURE);
        }

        put_log("\n --- Recv from target PC. --- \n");

        if (false == write_windbg_packet_log(buffer, total_bytes)){

            exit(EXIT_FAILURE);
        }

        DWORD bytes = 0;

        if (false == write_named_pipe(p->named_pipe, buffer, total_bytes, &bytes)){

            exit(EXIT_FAILURE);
        }

        if (total_bytes != bytes){

            put_log("\n --- write_named_pipe: total_bytes != bytes --- \n");
        }
    }

    exit(EXIT_SUCCESS);
}

static bool recv_com(handles* p, char* buffer, DWORD* total_bytes)
{
    UCHAR* packet_buffer = (UCHAR*)buffer;

    READ_PACKET_STATE state = READ_PACKET_STATE_FIRST;

    DWORD read_bytes = 1;

    DWORD recv_bytes = 0;

    for (;;){

        DWORD bytes = 0;

        if (p->test_mode){

            if (false == read_UART(p->UART2, packet_buffer, read_bytes, &bytes)){

                exit(EXIT_FAILURE);
            }
        }else{

            if (false == read_UART(p->UART1, packet_buffer, read_bytes, &bytes)){

                exit(EXIT_FAILURE);
            }
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

static bool end(handles* p)
{
    bool ret_val = true;

    close_stream();

    if (false == close_named_pipe(&(p->named_pipe))){

        ret_val = false;
    }

    if (p->test_mode){

        if (false == close_UART(&(p->UART1))){

            ret_val = false;
        }

        if (false == close_UART(&(p->UART2))){

            ret_val = false;
        }
    }else{

        if (false == close_UART(&(p->UART1))){

            ret_val = false;
        }
    }

    if (false == close_thread(&(p->thread))){

        ret_val = false;
    }

    return ret_val;
}

