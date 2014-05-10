#include "network_core/decafs_client.h"

extern "C" {

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FUSE_USE_VERSION 29
#include <fuse.h>

#define CHECK_PATHLEN if (strlen(path) > 255) return (-ENAMETOOLONG);
#define DECAFS_CLIENT \
  (reinterpret_cast<DecafsClient*>(fuse_get_context()->private_data))

static int custom_getattr(const char *path, struct stat *sd) {
  CHECK_PATHLEN
  // ENOENT - not found

  memset(sd, 0, sizeof(struct stat));

  // TODO(peter): read decafs stat source and then do this
  sd->st_mode = S_IFREG | 0777; // or S_IFDIR
  sd->st_nlink = 1;
  sd->st_uid = 0; // root
  sd->st_gid = 0; // root
  sd->st_size = 0; // GET FROM DECAFS
  sd->st_blksize = 4096; // GET FROM DECAFS
  sd->st_blocks = 0; // st_size / 512?
  sd->st_atim.tv_sec = 0; // GET FROM DECAFS
  sd->st_atim.tv_nsec = 0; // GET FROM DECAFS
  sd->st_mtim.tv_sec = 0; // GET FROM DECAFS
  sd->st_mtim.tv_nsec = 0; // GET FROM DECAFS
  sd->st_ctim.tv_sec = 0; // GET FROM DECAFS
  sd->st_ctim.tv_nsec = 0; // GET FROM DECAFS

  return 0;
}

static int custom_readlink(const char *path, char *buf, size_t bufsize) {
  return -EINVAL; // not supported
}

static int custom_mknod(const char *path, mode_t mode, dev_t dev) {
  return -EPERM; // not supported
}

static int custom_mkdir(const char *path, mode_t mode) {
  CHECK_PATHLEN

  // TODO(peter): do something once directories are a thing
  // EEXIST

  return 0;
}

static int custom_unlink(const char *path) {
  CHECK_PATHLEN

  DECAFS_CLIENT->delete_file(path);
  // EISDIR
  // ENOENT

  return 0;
}

static int custom_rmdir(const char *path) {
  CHECK_PATHLEN

  // TODO(peter): do something once directories are a thing
  // ENOTEMPTY
  // ENOENT

  return 0;
}

static int custom_symlink(const char *path, const char *link_path) {
  return -EPERM; // not supported
}

static int custom_rename(const char *old_path, const char *new_path) {
  return -EPERM; // not supported
}

static int custom_link(const char *path, const char *link_path) {
  return -EPERM; // not supported
}

static int custom_chmod(const char *path, mode_t mode) {
  CHECK_PATHLEN

  if (mode != 0777) // files must always be 0777
    return -EINVAL;

  return 0;
}

static int custom_chown(const char *path, uid_t owner, gid_t group) {
  CHECK_PATHLEN

  if (owner != 0 || owner != -1 ||
      group != 0 || group != -1) // files must always be owned by root
    return -EINVAL;

  return 0;
}

static int custom_truncate(const char *path, off_t length) {
  return -EPERM; // not supported
}

static int custom_open(const char *path, struct fuse_file_info *fi) {
  int fd = DECAFS_CLIENT->open(path, fi->flags);
  if (fd < 0) {
    return -EPERM; // TODO(peter): work with decafs errors
  }

  fi->fh = fd;
  return 0;
}

static int custom_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
  int len = DECAFS_CLIENT->read(fi->fh, buf, size);
  if (len < 0) {
    return -EBADF; // TODO(peter): work with decafs errors
  }

  return len;
}

static int custom_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
  int len = DECAFS_CLIENT->write(fi->fh, buf, size);
  if (len < 0) {
    return -EBADF; // TODO(peter): work with decafs errors
  }

  return len;
}

static int custom_statfs(const char *path, struct statvfs *sd) {
  return -ENOSYS; // not supported
}

static int custom_flush(const char *path, struct fuse_file_info *fi) {
  return 0; // currently all writes are synchronous
}

static int custom_release(const char *path, struct fuse_file_info *fi) {
  // cleanup private_data for file
  return 0;
}

static int custom_fsync(const char *path, int datasync, struct fuse_file_info *fi) {
  // If the datasync parameter is non-zero, then only the user data should be
  // flushed, not the metadata.
  return 0; // currently all writes are synchronous
}

static int custom_setxattr(const char *path, const char *name, const char *value, size_t size, int flags) {
  return -ENOTSUP; // not supported
}

static int custom_getxattr(const char *path, const char *name, char *value, size_t size) {
  return -ENOTSUP; // not supported
}

static int custom_listxattr(const char *path, char *list, size_t size) {
  return -ENOTSUP; // not supported
}

static int custom_removexattr(const char *path, const char *name) {
  return -ENOTSUP; // not supported
}

static int custom_opendir(const char *path, struct fuse_file_info *fi) {
  // TODO(peter): do something once directories are a thing
  return 0;
}

static int custom_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
  // TODO(peter): do something once directories are a thing
  return 0;
}

static int custom_releasedir(const char *path, struct fuse_file_info *fi) {
  // TODO(peter): do something once directories are a thing
  // cleanup private_data for dir
  return 0;
}

static int custom_fsyncdir(const char *path, int datasync, struct fuse_file_info *fi) {
  // If the datasync parameter is non-zero, then only the user data should be
  // flushed, not the metadata.
  return 0; // currently all writes are synchronous
}

static void* custom_init(struct fuse_conn_info *conn) {
  // TODO(peter): make these cmdline parameters
  DecafsClient *dc = new DecafsClient("192.168.1.100", 3333, 2);
  client.openConnection();
  return client;
}

static void custom_destroy(void *private_data) {
  delete reinterpret_cast<DecafsClient*>(private_data);
}

static int custom_access(const char *path, int amode) {
  CHECK_PATHLEN

  // TODO(peter): use stat
  // ENOENT

  return 0;
}

static int custom_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
  return custom_open(path, mode, fi);
}

static int custom_ftruncate(const char *path, off_t length, struct fuse_file_info *fi) {
  return -EPERM; // not supported
}

static int custom_fgetattr(const char *path, struct stat *sd, struct fuse_file_info *fi) {
  return -ENOTSUP; // not supported
}

static int custom_lock(const char *path, struct fuse_file_info *fi, int cmd, struct flock *flock) {
  return -ENOSYS; // not supported
}

static int custom_utimens(const char *path, const struct timespec tv[2]) {
  return -EINVAL; // not supported
}

static int custom_bmap(const char *path, size_t blocksize, uint64_t *idx) {
  return -ENOSYS; // not supported
}

#if 0
static int custom_ioctl(const char *path, int cmd, void *arg, struct fuse_file_info *fi, unsigned int flags, void *data) {
  return 0;
}

static int custom_poll(const char *path, struct fuse_file_info *fi, struct fuse_pollhandle *ph, unsigned *reventsp) {
  return 0;
}

static int custom_write_buf(const char *path, struct fuse_bufvec *buf, off_t off, struct fuse_file_info *fi) {
  return 0;
}

static int custom_read_buf(const char *path, struct fuse_bufvec **bufp, size_t size, off_t off, struct fuse_file_info *fi) {
  return 0;
}
#endif

static int custom_flock(const char *path, struct fuse_file_info *fi, int op) {
  return -ENOSYS; // not supported
}

static int custom_fallocate(const char *path, int mode, off_t offset, off_t len, struct fuse_file_info *fi) {
  return -EOPNOTSUPP; // not supported
}


static struct fuse_operations custom_operations = {
  .getattr = custom_getattr,
  .readlink = custom_readlink,
  .mknod = custom_mknod,
  .mkdir = custom_mkdir,
  .unlink = custom_unlink,
  .rmdir = custom_rmdir,
  .symlink = custom_symlink,
  .rename = custom_rename,
  .link = custom_link,
  .chmod = custom_chmod,
  .chown = custom_chown,
  .truncate = custom_truncate,
  .open = custom_open,
  .read = custom_read,
  .write = custom_write,
  .statfs = custom_statfs,
  .flush = custom_flush,
  .release = custom_release,
  .fsync = custom_fsync,
  .setxattr = custom_setxattr,
  .getxattr = custom_getxattr,
  .listxattr = custom_listxattr,
  .removexattr = custom_removexattr,
  .opendir = custom_opendir,
  .readdir = custom_readdir,
  .releasedir = custom_releasedir,
  .fsyncdir = custom_fsyncdir,
  .init = custom_init,
  .destroy = custom_destroy,
  .access = custom_access,
  .create = custom_create,
  .ftruncate = custom_ftruncate,
  .fgetattr = custom_fgetattr,
  .lock = custom_lock,
  .utimens = custom_utimens,
  .bmap = custom_bmap,
#if 0
  .ioctl = custom_ioctl,
  .poll = custom_poll,
  .write_buf = custom_write_buf,
  .read_buf = custom_read_buf,
#endif
  .flock = custom_flock,
  .fallocate = custom_fallocate
};

int main(int argc, char *argv[]) {
  return fuse_main(argc, argv, &custom_operations, NULL);
}

}
