#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/attr.h>
#include <sys/stat.h>
#include <errno.h>
#include <fts.h>
#include <unistd.h>

void die(char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(-1);
}

struct linkcount_buf_t {
    uint32_t len;
    uint32_t links;
};

uint32_t dirlinkcount(char *path) {
    struct attrlist al;
    bzero(&al, sizeof(al));
    al.bitmapcount = ATTR_BIT_MAP_COUNT;
    al.dirattr = ATTR_DIR_LINKCOUNT;
    
    struct linkcount_buf_t buf;
    if (getattrlist(path, &al, &buf, sizeof(buf), 0))
        die("getattrlist");
    return buf.links;
}

int main(int argc, char *argv[]) {
    char **roots = argv + 1;
    FTS *fts = fts_open(roots, FTS_COMFOLLOW | FTS_PHYSICAL | FTS_XDEV, NULL);
    if (!fts)
        die("fts");
    
    uint64_t blocks = 0;
    FTSENT *ent;
    bool dir;
    uint32_t links;
    struct stat *st;
    while ((ent = fts_read(fts))) {
        dir = false;
        switch (ent->fts_info) {
            case FTS_D:
                dir = true;
            case FTS_F:
            case FTS_SL:
            case FTS_SLNONE:
            case FTS_DEFAULT:
                st = ent->fts_statp;
                links = dir ? dirlinkcount(ent->fts_accpath) : st->st_nlink;
                if (links == 1) {
                    blocks += st->st_blocks;
                    printf("%s\n", ent->fts_path);
                } else if (dir) {
                    if (fts_set(fts, ent, FTS_SKIP))
                        die("fts_set");
                }
                break;
            
            case FTS_DP: break; // don't care
            
            default:
                fprintf(stderr, "fts_info: %d\n", ent->fts_info);
                exit(-2);
        }
    }
    if (errno)
        die("fts_read");
    printf("%llu\n", blocks / 2000); // in MB
    
    return 0;
}
