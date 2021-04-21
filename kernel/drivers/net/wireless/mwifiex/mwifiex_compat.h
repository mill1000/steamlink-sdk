#ifndef __MWIFIEX_COMPAT_H
#define __MWIFIEX_COMPAT_H

#include <linux/etherdevice.h>
#include <net/cfg80211.h>
#include <linux/ieee80211.h>

// etherdevice.h
static inline bool ether_addr_equal_unaligned(const u8 *addr1, const u8 *addr2)
{
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS)
  return ether_addr_equal(addr1, addr2);
#else
  return memcmp(addr1, addr2, ETH_ALEN) == 0;
#endif
}

static inline void ether_addr_copy(u8 *dst, const u8 *src)
{
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS)
  *(u32 *)dst = *(const u32 *)src;
  *(u16 *)(dst + 4) = *(const u16 *)(src + 4);
#else
  u16 *a = (u16 *)dst;
  const u16 *b = (const u16 *)src;

  a[0] = b[0];
  a[1] = b[1];
  a[2] = b[2];
#endif
}

// iee80211.h
#define IEEE80211_VHT_CAP_SOUNDING_DIMENSIONS_SHIFT 16
#define IEEE80211_VHT_CAP_SOUNDING_DIMENSIONS_MASK (7 << IEEE80211_VHT_CAP_SOUNDING_DIMENSIONS_SHIFT)

#define WLAN_EID_AID ((enum ieee80211_eid)197)
#define WLAN_EID_OPMODE_NOTIF ((enum ieee80211_eid)199)

#define WLAN_KEY_LEN_SMS4 ((enum ieee80211_key_len)32)

#define WLAN_EXT_CAPA4_INTERWORKING_ENABLED BIT(7)
#define WLAN_EXT_CAPA8_TDLS_WIDE_BW_ENABLED BIT(5)

#define WLAN_REASON_TDLS_TEARDOWN_UNSPECIFIED ((enum ieee80211_reasoncode)32)

struct ieee80211_wmm_ac_param {
  u8 aci_aifsn; /* AIFSN, ACM, ACI */
  u8 cw; /* ECWmin, ECWmax (CW = 2^ECW - 1) */
  __le16 txop_limit;
} __packed;

struct ieee80211_wmm_param_ie {
  u8 element_id; /* Element ID: 221 (0xdd); */
  u8 len; /* Length: 24 */
  /* required fields for WMM version 1 */
  u8 oui[3]; /* 00:50:f2 */
  u8 oui_type; /* 2 */
  u8 oui_subtype; /* 1 */
  u8 version; /* 1 for WMM version 1.0 */
  u8 qos_info; /* AP/STA specific QoS info */
  u8 reserved; /* 0 */
  /* AC_BE, AC_BK, AC_VI, AC_VO */
  struct ieee80211_wmm_ac_param ac[4];
} __packed;


// cfg80211.h
#define IEEE80211_CHAN_NO_IR ((enum ieee80211_channel_flags)(1 << 7))

#define NL80211_VHT_NSS_MAX 8
#define NL80211_HE_NSS_MAX 8

#define cfg80211_pkt_pattern cfg80211_wowlan_trig_pkt_pattern

// struct station_parameters {
// 	u8 *supported_rates;
// 	struct net_device *vlan;
// 	u32 sta_flags_mask, sta_flags_set;
// 	u32 sta_modify_mask;
// 	int listen_interval;
// 	u16 aid;
// 	u8 supported_rates_len;
// 	u8 plink_action;
// 	u8 plink_state;
// 	struct ieee80211_ht_cap *ht_capa;
// 	struct ieee80211_vht_cap *vht_capa;
// 	u8 uapsd_queues;
// 	u8 max_sp;
// 	u16 capability;
// 	const u8 *ext_capab;
// 	u8 ext_capab_len;
// };

// struct cfg80211_bitrate_mask {
// 	struct {
// 		u32 legacy;
// 		u8 mcs[IEEE80211_HT_MCS_MASK_LEN];
// 		u16 vht_mcs[NL80211_VHT_NSS_MAX];
// 		u16 he_mcs[NL80211_HE_NSS_MAX];
// 	} control[IEEE80211_NUM_BANDS];
// };

// struct cfg80211_wowlan_trig_pkt_pattern {
// 	u8 *mask, *pattern;
// 	int pattern_len;
// 	int pkt_offset;
// };

// struct wiphy_wowlan_support {
// 	u32 flags;
// 	int n_patterns;
// 	int pattern_max_len;
// 	int pattern_min_len;
// 	int max_pkt_offset;
// };


enum nl80211_coalesce_condition
{
  NL80211_COALESCE_CONDITION_MATCH,
  NL80211_COALESCE_CONDITION_NO_MATCH
};

struct cfg80211_coalesce_rules
{
  int delay;
  enum nl80211_coalesce_condition condition;
  struct cfg80211_pkt_pattern *patterns;
  int n_patterns;
};

struct cfg80211_coalesce
{
  struct cfg80211_coalesce_rules *rules;
  int n_rules;
};

struct cfg80211_mgmt_tx_params
{
  struct ieee80211_channel *chan;
  bool offchan;
  unsigned int wait;
  const u8 *buf;
  size_t len;
  bool no_cck;
  bool dont_wait_for_ack;
  int n_csa_offsets;
  const u16 *csa_offsets;
};

struct wiphy_coalesce_support
{
  int n_rules;
  int max_delay;
  int n_patterns;
  int pattern_max_len;
  int pattern_min_len;
  int max_pkt_offset;
};

enum cfg80211_bss_frame_type
{
  CFG80211_BSS_FTYPE_UNKNOWN,
  CFG80211_BSS_FTYPE_BEACON,
  CFG80211_BSS_FTYPE_PRESP,
};

struct station_del_parameters {
  const u8 *mac;
  u8 subtype;
  u16 reason_code;
};

// netdevice.h
// #define NET_NAME_UNKNOWN	0	/* unknown origin (not exposed to userspace) */
// #define NET_NAME_ENUM		1	/* enumerated by kernel */
// #define NET_NAME_PREDICTABLE	2	/* predictably named by the kernel */
// #define NET_NAME_USER		3	/* provided by user-space */
// #define NET_NAME_RENAMED	4	/* renamed by user-space */

typedef u16 (*select_queue_fallback_t)(struct net_device *dev, struct sk_buff *skb);

#endif