
// Copyright 2016 Shin'ichi Ichikawa. Released under the GNU General Public License version 3.
// See the file COPYING3. If not see <http://www.gnu.org/licenses/>.

#include "thread.h"
#include "utils.h"

bool create_thread(thread_func func, void* arg, HANDLE* handle)
{
    unsigned thread_ID = 0;

    *handle = (HANDLE)_beginthreadex(NULL, 0, func, arg, CREATE_SUSPENDED, &thread_ID);

    if (0 == *handle){

        get_last_error(__func__, __LINE__);

        return false;
    }

    return true;
}

bool resume_thread(HANDLE handle)
{
    if (-1 == ResumeThread(handle)){

        get_last_error(__func__, __LINE__);

        return false;
    }

    return true;
}

bool close_thread(HANDLE* handle)
{
    if (FALSE == CloseHandle(*handle)){

        get_last_error(__func__, __LINE__);

        *handle = NULL;

        return false;
    }

    *handle = NULL;

    return true;
}

