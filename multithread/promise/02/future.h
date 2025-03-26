#include <stdint.h>
#include <threads.h>
#include <stdatomic.h>
#include <stddef.h>
#ifndef _FTR_FUTURE_

#ifndef define_error
#define define_error(e) e
#endif
enum {
	ftr_success,
	define_error(ftr_timedout),
	define_error(ftr_invalid),
	define_error(ftr_nomem),
	define_error(ftr_destsize),
	define_error(ftr_error),
};

struct ftr_header {
	cnd_t cvar;
	mtx_t mtx;
	atomic_bool is_valid;
	bool is_set;
	size_t value_size;
	size_t in_offs;
	size_t out_offs;

};
#define ftr_of(ValueType) struct {struct ftr_header header; ValueType in_value; ValueType out_value;}
#define ftr_new(FT) ((FT *) ftr_new_(sizeof(FT), sizeof(((FT *)0)->out_value), offsetof(FT, in_value), offsetof(FT, out_value)))
#define ftr_delete(future) (ftr_delete_((struct ftr_header*)(future)))
#define ftr_complete(future, val)((future)->in_value=(val), ftr_complete_((struct ftr_header *)(future)))
#define ftr_get(future, timeout_ms, dest) (ftr_get_((struct ftr_header*)(future), (timeout_ms), (void*)(dest), sizeof(*(dest)), sizeof((*(dest))=future->out_value)))
#define ftr_wait(future, timeout_ms)(ftr_wait_((struct ftr_header*)(future), (timeout_ms)))
int ftr_init_(struct ftr_header* fh, size_t vsize, size_t in_offs, size_t out_offs);
struct ftr_header* ftr_new_(size_t wholesize, size_t vsize, size_t in_offs, size_t outoffs);
void ftr_destroy_(struct  ftr_header *h);
void ftr_delete_(struct  ftr_header *h);
int ftr_wait_(struct  ftr_header *h, int32_t timeout_ms);
int ftr_get_(struct  ftr_header *h, int32_t timeout_ms, void *dest, size_t dest_size, size_t check);
int ftr_complete_(struct ftr_header* fh);
const char *ftr_errorstr(int err);

#endif