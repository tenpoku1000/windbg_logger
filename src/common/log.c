
// Copyright 2016 Shin'ichi Ichikawa. Released under the GNU General Public License version 3.
// See the file COPYING3. If not see <http://www.gnu.org/licenses/>.

#include "log.h"

bool write_log_breakin_packet(PACKET* packet, DWORD bytes);
bool write_log_control_packet(PACKET* packet, DWORD bytes);
// bool write_log_normal_packet_state_change32(PACKET* packet, DWORD bytes);
bool write_log_normal_packet_state_manipulate(PACKET* packet, DWORD bytes);
bool write_log_normal_packet_debug_io(PACKET* packet, DWORD bytes);
bool write_log_normal_packet_state_change64(PACKET* packet, DWORD bytes);
bool write_log_etc_packet_trace_io(PACKET* packet, DWORD bytes);
bool write_log_etc_packet_control_request(PACKET* packet, DWORD bytes);
bool write_log_etc_packet_file_io(PACKET* packet, DWORD bytes);

bool write_windbg_packet_log(LPVOID buffer, DWORD bytes)
{
    if (NULL == buffer){

        put_log("NULL == buffer at %s function.", __func__);

        return false;
    }

    UCHAR* p = (UCHAR*)buffer;

    if ((1 == bytes) && (BREAKIN_PACKET_BYTE == p[0])){

        put_log("BREAKIN_PACKET_BYTE(%x)", p[0]);

        return true;
    }

    PACKET* packet = (PACKET*)buffer;

    if (PACKET_TYPE_MAX <= packet->header.PacketType){

        put_log("PACKET_TYPE_MAX <= packet->header.PacketType at %s function.", __func__);

        return false;
    }

    if (0 == packet->header.ByteCount){

        switch (packet->header.PacketType){
        case PACKET_TYPE_UNUSED:
            return write_windbg_packet_header_log(packet);
        case PACKET_TYPE_KD_POLL_BREAKIN:
            return write_log_breakin_packet(packet, bytes);
        case PACKET_TYPE_KD_ACKNOWLEDGE:
        case PACKET_TYPE_KD_RESEND:
        case PACKET_TYPE_KD_RESET:
            return write_log_control_packet(packet, bytes);
        default:
            put_log("Bad PacketType: %x at %s function(%d).", packet->header.PacketType, __func__, __LINE__);
            return false;
        }

        return true;
    }

    bytes -= (sizeof(KD_PACKET) + 1);

    if (PACKET_TRAILING_BYTE != packet->data[bytes]){

        put_log("Bad PACKET_TRAILING_BYTE: %x at %s function(%d).", packet->data[bytes], __func__, __LINE__);

        return false;
    }

    switch (packet->header.PacketType){
//    case PACKET_TYPE_KD_STATE_CHANGE32:
//        return write_log_normal_packet_state_change32(packet, bytes);
    case PACKET_TYPE_KD_STATE_MANIPULATE:
        return write_log_normal_packet_state_manipulate(packet, bytes);
    case PACKET_TYPE_KD_DEBUG_IO:
        return write_log_normal_packet_debug_io(packet, bytes);
    case PACKET_TYPE_KD_STATE_CHANGE64:
        return write_log_normal_packet_state_change64(packet, bytes);
    case PACKET_TYPE_KD_TRACE_IO:
        return write_log_etc_packet_trace_io(packet, bytes);
    case PACKET_TYPE_KD_CONTROL_REQUEST:
        return write_log_etc_packet_control_request(packet, bytes);
    case PACKET_TYPE_KD_FILE_IO:
        return write_log_etc_packet_file_io(packet, bytes);
    default:
        break;
    }

    put_log("Bad PacketType: %x at %s function(%d).", packet->header.PacketType, __func__, __LINE__);

    return false;
}

bool write_windbg_packet_header_log(PACKET* packet)
{
    bool ret_val = true;

    UCHAR c[4] = { 0 };
    memcpy(c, &(packet->header.PacketLeader), sizeof(c));

    switch (packet->header.PacketLeader){
    case BREAKIN_PACKET:
        put_log("PacketLeader: BREAKIN_PACKET(%c%c%c%c)", c[0], c[1], c[2], c[3]);
        break;
    case PACKET_LEADER:
        put_log("PacketLeader: PACKET_LEADER(%c%c%c%c)", c[0], c[1], c[2], c[3]);
        break;
    case CONTROL_PACKET_LEADER:
        put_log("PacketLeader: CONTROL_PACKET_LEADER(%c%c%c%c)", c[0], c[1], c[2], c[3]);
        break;
    default:
        put_log("PacketLeader: unknown packet leader(%c%c%c%c)", c[0], c[1], c[2], c[3]);
        ret_val = false;
        break;
    }

    switch (packet->header.PacketType){
    case PACKET_TYPE_UNUSED:
        put_log("PacketType: PACKET_TYPE_UNUSED(%d)", packet->header.PacketType);
        break;
    case PACKET_TYPE_KD_STATE_CHANGE32:
        put_log("PacketType: PACKET_TYPE_KD_STATE_CHANGE32(%d)", packet->header.PacketType);
        break;
    case PACKET_TYPE_KD_STATE_MANIPULATE:
        put_log("PacketType: PACKET_TYPE_KD_STATE_MANIPULATE(%d)", packet->header.PacketType);
        break;
    case PACKET_TYPE_KD_DEBUG_IO:
        put_log("PacketType: PACKET_TYPE_KD_DEBUG_IO(%d)", packet->header.PacketType);
        break;
    case PACKET_TYPE_KD_ACKNOWLEDGE:
        put_log("PacketType: PACKET_TYPE_KD_ACKNOWLEDGE(%d)", packet->header.PacketType);
        break;
    case PACKET_TYPE_KD_RESEND:
        put_log("PacketType: PACKET_TYPE_KD_RESEND(%d)", packet->header.PacketType);
        break;
    case PACKET_TYPE_KD_RESET:
        put_log("PacketType: PACKET_TYPE_KD_RESET(%d)", packet->header.PacketType);
        break;
    case PACKET_TYPE_KD_STATE_CHANGE64:
        put_log("PacketType: PACKET_TYPE_KD_STATE_CHANGE64(%d)", packet->header.PacketType);
        break;
    case PACKET_TYPE_KD_POLL_BREAKIN:
        put_log("PacketType: PACKET_TYPE_KD_POLL_BREAKIN(%d)", packet->header.PacketType);
        break;
    case PACKET_TYPE_KD_TRACE_IO:
        put_log("PacketType: PACKET_TYPE_KD_TRACE_IO(%d)", packet->header.PacketType);
        break;
    case PACKET_TYPE_KD_CONTROL_REQUEST:
        put_log("PacketType: PACKET_TYPE_KD_CONTROL_REQUEST(%d)", packet->header.PacketType);
        break;
    case PACKET_TYPE_KD_FILE_IO:
        put_log("PacketType: PACKET_TYPE_KD_FILE_IO(%d)", packet->header.PacketType);
        break;
    default:
        put_log("PacketType: unknown packet type(%d)", packet->header.PacketType);
        ret_val = false;
        break;
    }

    put_log("ByteCount(%d)", packet->header.ByteCount);

    if ((packet->header.PacketId & INITIAL_PACKET_ID) && (packet->header.PacketId & SYNC_PACKET_ID)){

        put_log("PacketId: INITIAL_PACKET_ID | SYNC_PACKET_ID(%x)", packet->header.PacketId);
    }else if (packet->header.PacketId & INITIAL_PACKET_ID){

        put_log("PacketId: INITIAL_PACKET_ID(%x)", packet->header.PacketId);
    }else if (packet->header.PacketId & SYNC_PACKET_ID){

        put_log("PacketId: SYNC_PACKET_ID(%x)", packet->header.PacketId);
    }else{

        put_log("PacketId: %x", packet->header.PacketId);
    }

    put_log("Checksum(%d)", packet->header.Checksum);

    return ret_val;
}

bool write_log_data_section(LPVOID buffer, DWORD bytes, DWORD header_section_size)
{
    int size = bytes - header_section_size;

    if (0 == size){

        return true;
    }

    if (0 > size){

        put_log("Bad message size %d at %s function.", size, __func__);

        return false;
    }

    put_log("DataSection:");

    int num = size / 16;

    UCHAR* p = ((UCHAR*)buffer) + header_section_size;

    for (int i = 0; num > i; ++i){

        UCHAR* q = p;

        for (int j = 0; 16 > j; ++j){

            put_log_no_crlf("%02x ", *q);

            ++q;
        }

        q = p;

        for (int j = 0; 16 > j; ++j){

            if (isprint(*q)){

                put_log_no_crlf("%c", *q);
            }else{

                put_log_no_crlf(" ");
            }

            ++q;
        }

        p = q;

        put_log("");
    }

    int mod = size % 16;

    if (mod){

        UCHAR* q = p;

        for (int i = 0; 16 > i; ++i){

            if (mod > i){

                put_log_no_crlf("%02x ", *q);
            }else{

                put_log_no_crlf("   ");
            }

            ++q;
        }

        q = p;

        for (int i = 0; mod > i; ++i){

            if (isprint(*q)){

                put_log_no_crlf("%c", *q);
            }else{

                put_log_no_crlf(" ");
            }

            ++q;
        }

        put_log("");
    }

    return true;
}

