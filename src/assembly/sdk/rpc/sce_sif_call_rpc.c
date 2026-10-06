#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/rpc/sce_sif_call_rpc/sceSifCallRpc.s", sceSifCallRpc);
#else
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
struct SifRpcClient
{
  s32 packet;
  s32 request_id;
  s32 semaphore_id;
  u8 pad_C[0x8];
  s32 server_buffer;
  u8 pad_18[0x4];
  s32 end_callback;
  s32 end_argument;
  s32 server;
};
struct SifRpcPacket
{
  u8 pad_0[0x14];
  s32 packet_address;
  s32 request_id;
  s32 client;
  s32 rpc_number;
  s32 send_size;
  s32 receive_address;
  s32 receive_size;
  s32 completion_mode;
  s32 server;
};
extern u8 D_00156800[];
extern s32 CreateSema();
extern s32 DeleteSema();
extern s32 WaitSema();
extern s32 func_0011ACE8();
extern s32 func_0011AD90();
extern s32 sceSifSendCmd();
extern s32 sceSifWriteBackDCache();
s32 sceSifCallRpc(struct SifRpcClient *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg_sp0)
{
  s32 semaphore_parameters[6];
  struct SifRpcPacket *packet;
  s32 semaphore_id;
  s32 skip_cache_writeback;
  s32 request_id;
  s32 allocation_failure_result;
  packet = (struct SifRpcPacket *) func_0011ACE8(D_00156800);
  allocation_failure_result = -1;
  if (packet == 0)
  {
    return allocation_failure_result;
  }
  request_id = packet->request_id;
  skip_cache_writeback = arg2 & 2;
  arg0->end_argument = arg_sp0;
  arg0->packet = (s32) packet;
  arg0->request_id = request_id;
  arg0->end_callback = arg7;
  packet->rpc_number = arg1;
  packet->send_size = arg4;
  packet->receive_address = arg5;
  packet->receive_size = arg6;
  packet->packet_address = (s32) packet;
  packet->server = arg0->server;
  packet->client = (s32) arg0;
  if (!skip_cache_writeback)
  {
    if (arg3 == arg5)
    {
      sceSifWriteBackDCache(arg3, (arg4 >= arg6) ? (arg4) : (arg6));
    }
    else
    {
      if (arg4 > 0)
      {
        sceSifWriteBackDCache(arg3, arg4);
      }
      if (arg6 > 0)
      {
        sceSifWriteBackDCache(arg5, arg6);
      }
    }
  }
  if (arg2 & 1)
  {
    if (arg7 == 0)
    {
      packet->completion_mode = 0;
    }
    else
    {
      packet->completion_mode = 1;
    }
    arg0->semaphore_id = -1;
    if (sceSifSendCmd(0x8000000A, packet, 0x40, arg3, arg0->server_buffer, arg4) != 0)
    {
      return 0;
    }
    else
    {
      func_0011AD90(packet);
      return -2;
    }
  }
  else
  {
    semaphore_parameters[1] = 1;
    semaphore_parameters[2] = 0;
    semaphore_id = CreateSema(semaphore_parameters);
    arg0->semaphore_id = semaphore_id;
    if (semaphore_id < 0)
    {
      func_0011AD90(packet);
      return -3;
    }
    packet->completion_mode = 1;
    if (sceSifSendCmd(0x8000000A, packet, 0x40, arg3, arg0->server_buffer, arg4) == 0)
    {
      DeleteSema(arg0->semaphore_id);
      func_0011AD90(packet);
      return -2;
    }
    WaitSema(arg0->semaphore_id);
    DeleteSema(arg0->semaphore_id);
    return 0;
  }
}

#endif /* NON_MATCHING */
