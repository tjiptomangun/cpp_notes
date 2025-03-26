#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <threads.h>
#include <stdatomic.h>
#include "future.h"

int ftr_init_(struct ftr_header *fh, size_t vsize, size_t in_offs, size_t out_offs) {
	fh->is_valid = true;
	fh->is_set = false;
	fh->value_size = vsize;
	fh->in_offs = in_offs;
	fh->out_offs = out_offs;
	if(cnd_init(&fh->cvar)) {
		return ftr_error;
	}
	if(mtx_init(&fh->mtx, mtx_plain)) {
		return ftr_error;
	}
	return ftr_success;
}

struct ftr_header* ftr_new_(size_t wholesize, size_t vsize, size_t in_offs, size_t out_offs) {
	struct ftr_header *fh = calloc(1, wholesize);
	int err = ftr_init_(fh, vsize, in_offs, out_offs);
	if (err) {
		fprintf(stderr, "error initializing future: %s", ftr_errorstr(err));
		return NULL;
	} else {
		return fh;
	}
}
void ftr_destroy_(struct ftr_header *fh) {
	fh->is_valid = false;
	cnd_destroy(&fh->cvar);
	mtx_destroy(&fh->mtx);
}

void ftr_delete_(struct ftr_header *fh) {
	ftr_destroy_(fh);
	free(fh);
}

/**
 * will set none of header value, if is set just return, 
 * otherwise wait until timeout or cnd signaled
 */
int ftr_wait_(struct ftr_header *fh, int32_t timeout_ms) {
	mtx_lock(&fh->mtx);
	if(fh->is_set) {
		mtx_unlock(&fh->mtx);
		return ftr_success;
	}
	if (timeout_ms == 0) {
        mtx_unlock(&fh->mtx);
        return ftr_timedout;
    }

	struct timespec start, end;
	timespec_get(&start, TIME_UTC);

	end.tv_sec = start.tv_sec + (timeout_ms/1000);
	end.tv_nsec = (timeout_ms%1000)*1000000;
	int err = cnd_timedwait(&fh->cvar, &fh->mtx, &end);
	if (err == thrd_timedout) {
		mtx_unlock(&fh->mtx);
		return ftr_timedout;
	} else if (err) {
		mtx_unlock(&fh->mtx);
		return ftr_error;
	} else {
		mtx_unlock(&fh->mtx);
		return ftr_success;
	}
}

/**
 * will try to get result of this future , if not ready until timeout_ms will return
 * if ready will set is_valid to false, 
 */
int ftr_get_(struct ftr_header *fh, int32_t timeout_ms, void *dest, size_t dest_size, size_t check) {
	(void) check;
	int err;
	if (!fh->is_valid) {
		return ftr_invalid;
	} else if(dest_size != fh->value_size) {
		return ftr_destsize;
	} else if ((err = ftr_wait_(fh, timeout_ms)) != ftr_success) {
		return err;
	} else {
		void *src = (void *)((uint8_t *)(fh) + fh->out_offs);
		memcpy(dest, src, fh->value_size);
		fh->is_valid = false;
		return ftr_success;
	}
}

/**
 * ftr_complete_ will set is_set to true and signal cvar that this future is task done. 
 * this is the only function that set is_set to true and assign value out_value
 */
int ftr_complete_(struct ftr_header *fh) {
	if((!fh->is_valid) || fh->is_set) {
		return ftr_invalid;
	} else {
		mtx_lock(&fh->mtx);

		void *src = (void *)((uint8_t *)(fh) + fh->in_offs);//In value
		void *dst = (void *)((uint8_t *)(fh) + fh->out_offs); //Out offs
		memcpy(dst, src, fh->value_size);
		fh->is_set = true;
		cnd_signal(&fh->cvar);
		mtx_unlock(&fh->mtx);
		return ftr_success;
	}
}
const char* ftr_errorstr(int err) {
    switch (err) {
    case ftr_success:   return "ftr_success";
    case ftr_invalid:   return "ftr_invalid: Invalid future object";
    case ftr_timedout:  return "ftr_timedout: Operation timed out";
    case ftr_nomem:     return "ftr_nomem: No memory available";
    case ftr_destsize:  return "ftr_destsize: The destination size is different from the value size";
    default:            return "ftr_errorstr: Unknown error";
    }
}

/*
int main(int argc, char **argv) {
}
*/
