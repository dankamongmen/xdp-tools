/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __UTIL_H
#define __UTIL_H

#include <bpf/libbpf.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif
#define STRERR_BUFSIZE 1024
#define _textify(x)	#x
#define textify(x)	_textify(x)

#ifndef BPF_DIR_MNT
#define BPF_DIR_MNT	"/sys/fs/bpf"
#endif

#ifndef BPF_OBJECT_PATH
#define BPF_OBJECT_PATH "/usr/lib/bpf"
#endif

#define FOR_EACH_MAP_KEY(_err, _map_fd, _map_key, _next_key)            \
  for(_err = bpf_map_get_next_key(_map_fd, NULL, &_next_key);           \
      !_err;                                                            \
      _err = bpf_map_get_next_key(_map_fd, &_map_key, &_next_key),      \
        _map_key = _next_key)

int check_bpf_environ(const char *pin_root_path);
int raise_rlimit(unsigned long limit);

struct bpf_object *open_bpf_file(const char *progname,
                                 struct bpf_object_open_opts *opts);
int load_bpf_object(struct bpf_object *obj, bool raise_rlimit);
int attach_xdp_program(const struct bpf_object *obj, const char *prog_name,
                       int ifindex, bool force, bool skb_mode);

int get_xdp_prog_info(int ifindex, struct bpf_prog_info *info);
int get_bpf_root_dir(char *buf, size_t buf_len, const char *subdir);
int get_pinned_map_fd(const char *bpf_root, const char *map_name,
                      struct bpf_map_info *info);
int unlink_pinned_map(int dir_fd, const char *map_name);

const char *action2str(__u32 action);

int prog_lock_get(const char *progname);
void prog_lock_release(int signal);

#endif
