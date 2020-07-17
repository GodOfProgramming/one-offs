// Only has typedefs so safe for xdp dev
#include <inttypes.h>
#include <stddef.h>

#include <linux/bpf.h>
#include <linux/icmp.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/if_vlan.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/kernel.h>
#include <linux/net.h>
#include <linux/stddef.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/version.h>

#include <bpf/bpf_helpers.h>

char __license[] SEC("license") = "GPL";

#define XDP_LABEL(name) xdp_label_##name

#define print(str, ...) bpf_printk("xdp: " str "\n", ##__VA_ARGS__)

#define MAX(a, b)           \
  ({                        \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a > _b ? _a : _b;      \
  })

#define MIN(a, b)           \
  ({                        \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a < _b ? _a : _b;      \
  })

enum ip_type
{
  IP_TYPE_UNKNOWN,
  IP_TYPE_IPV4,
  IP_TYPE_IPV6,
};

enum proto_type
{
  PROTO_TYPE_UNKNOWN,
  PROTO_TYPE_TCP,
  PROTO_TYPE_UDP,
  PROTO_TYPE_ICMP,
};

struct packet_info
{
  void* packet_end;

  struct ethhdr* eth_frame;

  enum ip_type ip_type;
  union
  {
    struct iphdr*   ipv4;
    struct ipv6hdr* ipv6;
  } iphdr;

  enum proto_type proto_type;
  union
  {
    struct tcphdr*  tcp;
    struct udphdr*  udp;
    struct icmphdr* icmp;
  } transporthdr;

  struct
  {
    uint16_t port;
  } dest;

  uint8_t* payload_begin;
};

int packet_info__fill(struct xdp_md* ctx, struct packet_info* info);

SEC("xdp_drop")
int drop(struct xdp_md* ctx)
{
  (void)ctx;
  return XDP_DROP;
}

SEC("xdp_pass")
int pass(struct xdp_md* ctx)
{
  (void)ctx;
  return XDP_PASS;
}

SEC("xdp_filter")
int filter(struct xdp_md* ctx)
{
  struct packet_info info;
  if (!packet_info__fill(ctx, &info)) {
    return XDP_ABORTED;
  }

  if (info.ip_type == IP_TYPE_IPV4 && info.proto_type == PROTO_TYPE_UDP) {
    unsigned char buff[100] = {0};

    int i;
#pragma unroll
    for (i = 0; i < sizeof(buff) - 1; i++) {
      // if ((void*)(info.payload_begin + i) < info.packet_end) {
      //  buff[i] = info.payload_begin[i];
      //} else {
      //  break;
      //}
    }

    print("\npayload: %p\npacket begin: %p\npacket end: %p", info.payload_begin, info.eth_frame, info.packet_end);
    print("udp packet data: %s", buff);
  }

  return XDP_PASS;
}

int packet_info__fill(struct xdp_md* ctx, struct packet_info* info)
{
  info->packet_end = (void*)(long)ctx->data_end;

  // ethernet header, wireless stuff is in <nl80211.h>

  uint16_t iptype;

  info->eth_frame     = (void*)(long)ctx->data;
  void* eth_frame_end = (void*)info->eth_frame + sizeof(*info->eth_frame);
  if (eth_frame_end > info->packet_end) {
    print("bad packet, eth frame length check");
    return 0;
  }

  iptype = __constant_ntohs(info->eth_frame->h_proto);

  // ip protocol

  uint8_t transport_protocol;

  void* ip_header_end = NULL;

  switch (iptype) {
    case ETH_P_IP: {
      info->ip_type = IP_TYPE_IPV4;

      // ip header
      info->iphdr.ipv4 = eth_frame_end;
      ip_header_end    = (void*)info->iphdr.ipv4 + sizeof(*info->iphdr.ipv4);
      if (ip_header_end > info->packet_end) {
        print("bad packet, ip header length check");
        return 0;
      }

      transport_protocol = info->iphdr.ipv4->protocol;
    } break;
    case ETH_P_IPV6: {
      info->ip_type = IP_TYPE_IPV6;

      info->iphdr.ipv6 = eth_frame_end;
      ip_header_end    = (void*)info->iphdr.ipv6 + sizeof(*info->iphdr.ipv6);
      if (ip_header_end > info->packet_end) {
        print("bad packet, ip header length check");
        return 0;
      }

      transport_protocol = info->iphdr.ipv6->nexthdr;
    } break;
    default: {
      info->ip_type    = IP_TYPE_UNKNOWN;
      info->proto_type = PROTO_TYPE_UNKNOWN;
      return 1;
    }
  }

  // transport protocol

  void* transporthdr_end = NULL;

  switch (transport_protocol) {
    case IPPROTO_UDP: {
      info->proto_type = PROTO_TYPE_UDP;

      // protocol header
      info->transporthdr.udp = ip_header_end;
      transporthdr_end       = (void*)info->transporthdr.udp + sizeof(*info->transporthdr.udp);
      if (transporthdr_end > info->packet_end) {
        print("bad packet, proto length check");
        return 0;
      }

      info->dest.port = info->transporthdr.udp->dest;
    } break;
    case IPPROTO_TCP: {
      info->proto_type = PROTO_TYPE_TCP;

      // protocol header
      info->transporthdr.tcp = ip_header_end;
      transporthdr_end       = (void*)info->transporthdr.tcp + sizeof(*info->transporthdr.tcp);
      if (transporthdr_end > info->packet_end) {
        print("bad packet, proto length check");
        return 0;
      }
    } break;
    case IPPROTO_ICMP: {
      info->proto_type = PROTO_TYPE_ICMP;

      // protocol header
      info->transporthdr.icmp = ip_header_end;
      transporthdr_end        = (void*)info->transporthdr.icmp + sizeof(*info->transporthdr.icmp);
      if (transporthdr_end > info->packet_end) {
        print("bad packet, proto length check");
        return 0;
      }
    } break;
    default: {
      info->proto_type = PROTO_TYPE_UNKNOWN;
      return 1;
    }
  }

  // payload
  info->payload_begin = transporthdr_end;

  if ((void*)info->payload_begin > info->packet_end) {
    print("bad packet, payload length check");
    return 0;
  }

  return 1;
}
