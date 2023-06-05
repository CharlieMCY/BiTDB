global-incdirs-y += include
srcs-y += trustsqlite.c
srcs-y += sqlite3.c
srcs-y += tzvfs_teeuser.c

# To remove a certain compiler flag, add a line like this
#cflags-template_ta.c-y += -Wno-strict-prototypes

# cflags-trustsqlite.c-y += -pg
# cflags-sqlite3.c-y += -pg
# cflags-tzvfs_teeuser.c-y += -pg
