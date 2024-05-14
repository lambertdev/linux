#include <linux/bio.h>
#include <linux/blk-mq.h>
#include <linux/blkdev.h>

struct bio_vec rust_helper_req_bvec(struct request *rq)
{
	return req_bvec(rq);
}
EXPORT_SYMBOL_GPL(rust_helper_req_bvec);

void *rust_helper_blk_mq_rq_to_pdu(struct request *rq)
{
	return blk_mq_rq_to_pdu(rq);
}
EXPORT_SYMBOL_GPL(rust_helper_blk_mq_rq_to_pdu);

struct request *rust_helper_blk_mq_rq_from_pdu(void *pdu)
{
	return blk_mq_rq_from_pdu(pdu);
}
EXPORT_SYMBOL_GPL(rust_helper_blk_mq_rq_from_pdu);

void rust_helper_bio_advance_iter_single(const struct bio *bio,
					 struct bvec_iter *iter,
					 unsigned int bytes)
{
	bio_advance_iter_single(bio, iter, bytes);
}
EXPORT_SYMBOL_GPL(rust_helper_bio_advance_iter_single);

// ----
bool rust_helper_req_ref_inc_not_zero(struct request *req)
{
	return atomic_inc_not_zero(&req->ref);
}
EXPORT_SYMBOL_GPL(rust_helper_req_ref_inc_not_zero);

bool rust_helper_req_ref_put_and_test(struct request *req)
{
	return atomic_dec_and_test(&req->ref);
}
EXPORT_SYMBOL_GPL(rust_helper_req_ref_put_and_test);

void rust_helper_blk_mq_free_request_internal(struct request *req)
{
	__blk_mq_free_request(req);
}
EXPORT_SYMBOL_GPL(rust_helper_blk_mq_free_request_internal);

struct request *rust_helper_blk_mq_tag_to_rq(struct blk_mq_tags *tags,
					     unsigned int tag)
{
	return blk_mq_tag_to_rq(tags, tag);
}
EXPORT_SYMBOL_GPL(rust_helper_blk_mq_tag_to_rq);

unsigned int rust_helper_blk_rq_payload_bytes(struct request *rq)
{
	return blk_rq_payload_bytes(rq);
}
EXPORT_SYMBOL_GPL(rust_helper_blk_rq_payload_bytes);

unsigned short rust_helper_blk_rq_nr_phys_segments(struct request *rq)
{
	return blk_rq_nr_phys_segments(rq);
}
EXPORT_SYMBOL_GPL(rust_helper_blk_rq_nr_phys_segments);
// SPDX-License-Identifier: GPL-2.0

#include <linux/bug.h>

__noreturn void rust_helper_BUG(void)
{
	BUG();
}
EXPORT_SYMBOL_GPL(rust_helper_BUG);
// SPDX-License-Identifier: GPL-2.0

#include <linux/build_bug.h>

/*
 * `bindgen` binds the C `size_t` type as the Rust `usize` type, so we can
 * use it in contexts where Rust expects a `usize` like slice (array) indices.
 * `usize` is defined to be the same as C's `uintptr_t` type (can hold any
 * pointer) but not necessarily the same as `size_t` (can hold the size of any
 * single object). Most modern platforms use the same concrete integer type for
 * both of them, but in case we find ourselves on a platform where
 * that's not true, fail early instead of risking ABI or
 * integer-overflow issues.
 *
 * If your platform fails this assertion, it means that you are in
 * danger of integer-overflow bugs (even if you attempt to add
 * `--no-size_t-is-usize`). It may be easiest to change the kernel ABI on
 * your platform such that `size_t` matches `uintptr_t` (i.e., to increase
 * `size_t`, because `uintptr_t` has to be at least as big as `size_t`).
 */
static_assert(
	sizeof(size_t) == sizeof(uintptr_t) &&
	__alignof__(size_t) == __alignof__(uintptr_t),
	"Rust code expects C `size_t` to match Rust `usize`"
);
// SPDX-License-Identifier: GPL-2.0

#include <linux/export.h>
#include <linux/errname.h>

const char *rust_helper_errname(int err)
{
	return errname(err);
}
EXPORT_SYMBOL_GPL(rust_helper_errname);
// SPDX-License-Identifier: GPL-2.0

#include <linux/cpumask.h>

unsigned int rust_helper_num_possible_cpus(void)
{
	return  num_possible_cpus();
}
EXPORT_SYMBOL_GPL(rust_helper_num_possible_cpus);
// SPDX-License-Identifier: GPL-2.0

#include <linux/delay.h>

void rust_helper_mdelay(uint64_t ms)
{
	mdelay(ms);
}
EXPORT_SYMBOL_GPL(rust_helper_mdelay);
// SPDX-License-Identifier: GPL-2.0

#include <linux/device.h>

void *rust_helper_dev_get_drvdata(struct device *dev)
{
	return dev_get_drvdata(dev);
}
EXPORT_SYMBOL_GPL(rust_helper_dev_get_drvdata);

const char *rust_helper_dev_name(const struct device *dev)
{
	return dev_name(dev);
}
EXPORT_SYMBOL_GPL(rust_helper_dev_name);
// SPDX-License-Identifier: GPL-2.0

#include <linux/err.h>
#include <linux/export.h>

__force void *rust_helper_ERR_PTR(long err)
{
	return ERR_PTR(err);
}
EXPORT_SYMBOL_GPL(rust_helper_ERR_PTR);

bool rust_helper_IS_ERR(__force const void *ptr)
{
	return IS_ERR(ptr);
}
EXPORT_SYMBOL_GPL(rust_helper_IS_ERR);

long rust_helper_PTR_ERR(__force const void *ptr)
{
	return PTR_ERR(ptr);
}
EXPORT_SYMBOL_GPL(rust_helper_PTR_ERR);
#include <linux/cacheflush.h>
#include <linux/mm.h>
#include <linux/pagemap.h>

void rust_helper_folio_get(struct folio *folio)
{
	folio_get(folio);
}
EXPORT_SYMBOL_GPL(rust_helper_folio_get);

void rust_helper_folio_put(struct folio *folio)
{
	folio_put(folio);
}
EXPORT_SYMBOL_GPL(rust_helper_folio_put);

struct page *rust_helper_folio_page(struct folio *folio, size_t n)
{
	return folio_page(folio, n);
}

loff_t rust_helper_folio_pos(struct folio *folio)
{
	return folio_pos(folio);
}
EXPORT_SYMBOL_GPL(rust_helper_folio_pos);

size_t rust_helper_folio_size(struct folio *folio)
{
	return folio_size(folio);
}
EXPORT_SYMBOL_GPL(rust_helper_folio_size);

void rust_helper_folio_mark_uptodate(struct folio *folio)
{
	folio_mark_uptodate(folio);
}
EXPORT_SYMBOL_GPL(rust_helper_folio_mark_uptodate);

void rust_helper_folio_set_error(struct folio *folio)
{
	folio_set_error(folio);
}
EXPORT_SYMBOL_GPL(rust_helper_folio_set_error);

#ifndef CONFIG_NUMA
struct folio* rust_helper_folio_alloc(gfp_t gfp, unsigned int order)
{
  return folio_alloc(gfp, order);
}
EXPORT_SYMBOL_GPL(rust_helper_folio_alloc);
#endif

void rust_helper_flush_dcache_folio(struct folio *folio)
{
	flush_dcache_folio(folio);
}
EXPORT_SYMBOL_GPL(rust_helper_flush_dcache_folio);

void *rust_helper_kmap_local_folio(struct folio *folio, size_t offset)
{
	return kmap_local_folio(folio, offset);
}
EXPORT_SYMBOL_GPL(rust_helper_kmap_local_folio);

void *rust_helper_kmap(struct page *page)
{
	return kmap(page);
}
EXPORT_SYMBOL_GPL(rust_helper_kmap);

void rust_helper_kunmap(struct page *page)
{
	return kunmap(page);
}
EXPORT_SYMBOL_GPL(rust_helper_kunmap);
// SPDX-License-Identifier: GPL-2.0

#include <linux/io.h>

u8 rust_helper_readb(const volatile void __iomem *addr)
{
	return readb(addr);
}
EXPORT_SYMBOL_GPL(rust_helper_readb);

u16 rust_helper_readw(const volatile void __iomem *addr)
{
	return readw(addr);
}
EXPORT_SYMBOL_GPL(rust_helper_readw);

u32 rust_helper_readl(const volatile void __iomem *addr)
{
	return readl(addr);
}
EXPORT_SYMBOL_GPL(rust_helper_readl);

#ifdef CONFIG_64BIT
u64 rust_helper_readq(const volatile void __iomem *addr)
{
	return readq(addr);
}
EXPORT_SYMBOL_GPL(rust_helper_readq);
#endif

void rust_helper_writeb(u8 value, volatile void __iomem *addr)
{
	writeb(value, addr);
}
EXPORT_SYMBOL_GPL(rust_helper_writeb);

void rust_helper_writew(u16 value, volatile void __iomem *addr)
{
	writew(value, addr);
}
EXPORT_SYMBOL_GPL(rust_helper_writew);

void rust_helper_writel(u32 value, volatile void __iomem *addr)
{
	writel(value, addr);
}
EXPORT_SYMBOL_GPL(rust_helper_writel);

#ifdef CONFIG_64BIT
void rust_helper_writeq(u64 value, volatile void __iomem *addr)
{
	writeq(value, addr);
}
EXPORT_SYMBOL_GPL(rust_helper_writeq);
#endif

u8 rust_helper_readb_relaxed(const volatile void __iomem *addr)
{
	return readb_relaxed(addr);
}
EXPORT_SYMBOL_GPL(rust_helper_readb_relaxed);

u16 rust_helper_readw_relaxed(const volatile void __iomem *addr)
{
	return readw_relaxed(addr);
}
EXPORT_SYMBOL_GPL(rust_helper_readw_relaxed);

u32 rust_helper_readl_relaxed(const volatile void __iomem *addr)
{
	return readl_relaxed(addr);
}
EXPORT_SYMBOL_GPL(rust_helper_readl_relaxed);

#ifdef CONFIG_64BIT
u64 rust_helper_readq_relaxed(const volatile void __iomem *addr)
{
	return readq_relaxed(addr);
}
EXPORT_SYMBOL_GPL(rust_helper_readq_relaxed);
#endif

void rust_helper_writeb_relaxed(u8 value, volatile void __iomem *addr)
{
	writeb_relaxed(value, addr);
}
EXPORT_SYMBOL_GPL(rust_helper_writeb_relaxed);

void rust_helper_writew_relaxed(u16 value, volatile void __iomem *addr)
{
	writew_relaxed(value, addr);
}
EXPORT_SYMBOL_GPL(rust_helper_writew_relaxed);

void rust_helper_writel_relaxed(u32 value, volatile void __iomem *addr)
{
	writel_relaxed(value, addr);
}
EXPORT_SYMBOL_GPL(rust_helper_writel_relaxed);

#ifdef CONFIG_64BIT
void rust_helper_writeq_relaxed(u64 value, volatile void __iomem *addr)
{
	writeq_relaxed(value, addr);
}
EXPORT_SYMBOL_GPL(rust_helper_writeq_relaxed);
#endif

void rust_helper_memcpy_fromio(void *to, const volatile void __iomem *from, long count)
{
	memcpy_fromio(to, from, count);
}
EXPORT_SYMBOL_GPL(rust_helper_memcpy_fromio);
// SPDX-License-Identifier: GPL-2.0

#include <kunit/test-bug.h>
#include <linux/export.h>

struct kunit *rust_helper_kunit_get_current_test(void)
{
	return kunit_get_current_test();
}
EXPORT_SYMBOL_GPL(rust_helper_kunit_get_current_test);
// SPDX-License-Identifier: GPL-2.0

#include <linux/export.h>
#include <linux/mutex.h>

void rust_helper_mutex_lock(struct mutex *lock)
{
	mutex_lock(lock);
}
EXPORT_SYMBOL_GPL(rust_helper_mutex_lock);
// SPDX-License-Identifier: GPL-2.0

#include <linux/gfp.h>
#include <linux/highmem.h>

struct page *rust_helper_alloc_pages(gfp_t gfp_mask, unsigned int order)
{
	return alloc_pages(gfp_mask, order);
}
EXPORT_SYMBOL_GPL(rust_helper_alloc_pages);

void *rust_helper_kmap_local_page(struct page *page)
{
	return kmap_local_page(page);
}
EXPORT_SYMBOL_GPL(rust_helper_kmap_local_page);

void rust_helper_kunmap_local(const void *addr)
{
	kunmap_local(addr);
}
EXPORT_SYMBOL_GPL(rust_helper_kunmap_local);
// SPDX-License-Identifier: GPL-2.0

#include <linux/pci.h>

void rust_helper_pci_set_drvdata(struct pci_dev *pdev, void *data)
{
	pci_set_drvdata(pdev, data);
}
EXPORT_SYMBOL_GPL(rust_helper_pci_set_drvdata);

void *rust_helper_pci_get_drvdata(struct pci_dev *pdev)
{
	return pci_get_drvdata(pdev);
}
EXPORT_SYMBOL_GPL(rust_helper_pci_get_drvdata);
// SPDX-License-Identifier: GPL-2.0

#include <linux/rcupdate.h>

void rust_helper_rcu_read_lock(void)
{
	rcu_read_lock();
}
EXPORT_SYMBOL_GPL(rust_helper_rcu_read_lock);

void rust_helper_rcu_read_unlock(void)
{
	rcu_read_unlock();
}
EXPORT_SYMBOL_GPL(rust_helper_rcu_read_unlock);
// SPDX-License-Identifier: GPL-2.0

#include <linux/export.h>
#include <linux/refcount.h>

refcount_t rust_helper_REFCOUNT_INIT(int n)
{
	return (refcount_t)REFCOUNT_INIT(n);
}
EXPORT_SYMBOL_GPL(rust_helper_REFCOUNT_INIT);

void rust_helper_refcount_inc(refcount_t *r)
{
	refcount_inc(r);
}
EXPORT_SYMBOL_GPL(rust_helper_refcount_inc);

bool rust_helper_refcount_dec_and_test(refcount_t *r)
{
	return refcount_dec_and_test(r);
}
EXPORT_SYMBOL_GPL(rust_helper_refcount_dec_and_test);
// SPDX-License-Identifier: GPL-2.0

#include <linux/export.h>
#include <linux/sched/signal.h>

int rust_helper_signal_pending(struct task_struct *t)
{
	return signal_pending(t);
}
EXPORT_SYMBOL_GPL(rust_helper_signal_pending);
// SPDX-License-Identifier: GPL-2.0

#include <linux/export.h>
#include <linux/spinlock.h>

void rust_helper___spin_lock_init(spinlock_t *lock, const char *name,
				  struct lock_class_key *key)
{
#ifdef CONFIG_DEBUG_SPINLOCK
	__raw_spin_lock_init(spinlock_check(lock), name, key, LD_WAIT_CONFIG);
#else
	spin_lock_init(lock);
#endif
}
EXPORT_SYMBOL_GPL(rust_helper___spin_lock_init);

void rust_helper_spin_lock(spinlock_t *lock)
{
	spin_lock(lock);
}
EXPORT_SYMBOL_GPL(rust_helper_spin_lock);

void rust_helper_spin_unlock(spinlock_t *lock)
{
	spin_unlock(lock);
}
EXPORT_SYMBOL_GPL(rust_helper_spin_unlock);
// SPDX-License-Identifier: GPL-2.0

#include <linux/export.h>
#include <linux/sched/task.h>

struct task_struct *rust_helper_get_current(void)
{
	return current;
}
EXPORT_SYMBOL_GPL(rust_helper_get_current);

void rust_helper_get_task_struct(struct task_struct *t)
{
	get_task_struct(t);
}
EXPORT_SYMBOL_GPL(rust_helper_get_task_struct);

void rust_helper_put_task_struct(struct task_struct *t)
{
	put_task_struct(t);
}
EXPORT_SYMBOL_GPL(rust_helper_put_task_struct);
// SPDX-License-Identifier: GPL-2.0

#include <linux/export.h>
#include <linux/wait.h>

void rust_helper_init_wait(struct wait_queue_entry *wq_entry)
{
	init_wait(wq_entry);
}
EXPORT_SYMBOL_GPL(rust_helper_init_wait);
// SPDX-License-Identifier: GPL-2.0

#include <linux/export.h>
#include <linux/workqueue.h>

void rust_helper_init_work_with_key(struct work_struct *work, work_func_t func,
				    bool onstack, const char *name,
				    struct lock_class_key *key)
{
	__init_work(work, onstack);
	work->data = (atomic_long_t)WORK_DATA_INIT();
	lockdep_init_map(&work->lockdep_map, name, key, 0);
	INIT_LIST_HEAD(&work->entry);
	work->func = func;
}
EXPORT_SYMBOL_GPL(rust_helper_init_work_with_key);
