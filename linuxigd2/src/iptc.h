#ifndef _IPTC_H_
#define _IPTC_H_

#include <iptables.h>
#include <libiptc/libiptc.h>

/* Original LinuxIGD used ip_nat_multi_range struct found from linux/netfilter_ipv4/ip_nat.h
 * For some reason that header file doesn't exist anymore and structs defined there were
 * moved to linux headers (.../include/net/netfilter/nf_nat.h). All worked well when that 
 * file was included from headers for kernel 2.6.24-22, but for 2.6.27-11 it didn't work anymore.
 * So basically I copied following three unions and structs from that older kernel-header file. 
 * Someone can do better solution by rewriting some code from iptc.c and here.
 */

#ifndef IP_NAT_RANGE_MAP_IPS
#define IP_NAT_RANGE_MAP_IPS 1
#endif
#ifndef IP_NAT_RANGE_PROTO_SPECIFIED
#define IP_NAT_RANGE_PROTO_SPECIFIED 2
#endif
#ifndef IP_NAT_RANGE_PROTO_RANDOM
#define IP_NAT_RANGE_PROTO_RANDOM 4
#endif

union nf_conntrack_man_proto
{
    /* Add other protocols here. */
    __be16 all;

    struct {
        __be16 port;
    } tcp;
    struct {
        __be16 port;
    } udp;
    struct {
        __be16 id;
    } icmp;
    struct {
        __be16 port;
    } sctp;
    struct {
        __be16 key; /* GRE key is 32bit, PPtP only uses 16bit */
    } gre;
};

/* Single range specification. */
struct nf_nat_range
{
    /* Set to OR of flags above. */
    unsigned int flags;

    /* Inclusive: network order. */
    __be32 min_ip, max_ip;

    /* Inclusive: network order */
    union nf_conntrack_man_proto min, max;
};

/* For backwards compat: don't use in modern code. */
struct nf_nat_multi_range_compat
{
    unsigned int rangesize; /* Must be 1. */

    /* hangs off end. */
    struct nf_nat_range range[1];
};

struct ipt_natinfo
{
    struct ipt_entry_target t;
    struct nf_nat_multi_range_compat mr;
};

struct ipt_entry_match *get_tcp_match(const char *sports, const char *dports, unsigned int *nfcache);
struct ipt_entry_match *get_udp_match(const char *sports, const char *dports, unsigned int *nfcache);
struct ipt_entry_target *get_dnat_target(const char *input, unsigned int *nfcache);

void iptc_add_rule(const char *table,
                   const char *chain,
                   const char *protocol,
                   const char *iiface,
                   const char *oiface,
                   const char *src,
                   const char *dest,
                   const char *srcports,
                   const char *destports,
                   const char *target,
                   const char *dnat_to,
                   const int append);

void iptc_delete_rule(const char *table,
                      const char *chain,
                      const char *protocol,
                      const char *iniface,
                      const char *outiface,
                      const char *src,
                      const char *dest,
                      const char *srcports,
                      const char *destports,
                      const char *target,
                      const char *dnat_to);

#endif // _IPTC_H_
