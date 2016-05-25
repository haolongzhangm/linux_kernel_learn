#ifndef _ASM_POWERPC_BOOK3S_64_HASH_4K_H
#define _ASM_POWERPC_BOOK3S_64_HASH_4K_H
/*
 * Entries per page directory level.  The PTE level must use a 64b record
 * for each page table entry.  The PMD and PGD level use a 32b record for
 * each entry by assuming that each entry is page aligned.
 */
#define PTE_INDEX_SIZE  9
#define PMD_INDEX_SIZE  7
#define PUD_INDEX_SIZE  9
#define PGD_INDEX_SIZE  9

#ifndef __ASSEMBLY__
#define PTE_TABLE_SIZE	(sizeof(pte_t) << PTE_INDEX_SIZE)
#define PMD_TABLE_SIZE	(sizeof(pmd_t) << PMD_INDEX_SIZE)
#define PUD_TABLE_SIZE	(sizeof(pud_t) << PUD_INDEX_SIZE)
#define PGD_TABLE_SIZE	(sizeof(pgd_t) << PGD_INDEX_SIZE)
#endif	/* __ASSEMBLY__ */

#define PTRS_PER_PTE	(1 << PTE_INDEX_SIZE)
#define PTRS_PER_PMD	(1 << PMD_INDEX_SIZE)
#define PTRS_PER_PUD	(1 << PUD_INDEX_SIZE)
#define PTRS_PER_PGD	(1 << PGD_INDEX_SIZE)

/* PMD_SHIFT determines what a second-level page table entry can map */
#define PMD_SHIFT	(PAGE_SHIFT + PTE_INDEX_SIZE)
#define PMD_SIZE	(1UL << PMD_SHIFT)
#define PMD_MASK	(~(PMD_SIZE-1))

/* With 4k base page size, hugepage PTEs go at the PMD level */
#define MIN_HUGEPTE_SHIFT	PMD_SHIFT

/* PUD_SHIFT determines what a third-level page table entry can map */
#define PUD_SHIFT	(PMD_SHIFT + PMD_INDEX_SIZE)
#define PUD_SIZE	(1UL << PUD_SHIFT)
#define PUD_MASK	(~(PUD_SIZE-1))

/* PGDIR_SHIFT determines what a fourth-level page table entry can map */
#define PGDIR_SHIFT	(PUD_SHIFT + PUD_INDEX_SIZE)
#define PGDIR_SIZE	(1UL << PGDIR_SHIFT)
#define PGDIR_MASK	(~(PGDIR_SIZE-1))

/* Bits to mask out from a PMD to get to the PTE page */
#define PMD_MASKED_BITS		0
/* Bits to mask out from a PUD to get to the PMD page */
#define PUD_MASKED_BITS		0
/* Bits to mask out from a PGD to get to the PUD page */
#define PGD_MASKED_BITS		0

/* PTE flags to conserve for HPTE identification */
#define _PAGE_HPTEFLAGS (_PAGE_BUSY | _PAGE_HASHPTE | \
			 _PAGE_F_SECOND | _PAGE_F_GIX)

/* shift to put page number into pte */
#define PTE_RPN_SHIFT	(18)

#define _PAGE_4K_PFN		0
#ifndef __ASSEMBLY__
/*
 * 4-level page tables related bits
 */

#define pgd_none(pgd)		(!pgd_val(pgd))
#define pgd_bad(pgd)		(pgd_val(pgd) == 0)
#define pgd_present(pgd)	(pgd_val(pgd) != 0)
#define pgd_page_vaddr(pgd)	(pgd_val(pgd) & ~PGD_MASKED_BITS)

static inline void pgd_clear(pgd_t *pgdp)
{
	*pgdp = __pgd(0);
}

static inline pte_t pgd_pte(pgd_t pgd)
{
	return __pte(pgd_val(pgd));
}

static inline pgd_t pte_pgd(pte_t pte)
{
	return __pgd(pte_val(pte));
}
extern struct page *pgd_page(pgd_t pgd);

#define pud_offset(pgdp, addr)	\
  (((pud_t *) pgd_page_vaddr(*(pgdp))) + \
    (((addr) >> PUD_SHIFT) & (PTRS_PER_PUD - 1)))

#define pud_ERROR(e) \
	pr_err("%s:%d: bad pud %08lx.\n", __FILE__, __LINE__, pud_val(e))

/*
 * On all 4K setups, remap_4k_pfn() equates to remap_pfn_range() */
#define remap_4k_pfn(vma, addr, pfn, prot)	\
	remap_pfn_range((vma), (addr), (pfn), PAGE_SIZE, (prot))

#ifdef CONFIG_HUGETLB_PAGE
/*
 * For 4k page size, we support explicit hugepage via hugepd
 */
static inline int pmd_huge(pmd_t pmd)
{
	return 0;
}

static inline int pud_huge(pud_t pud)
{
	return 0;
}

static inline int pgd_huge(pgd_t pgd)
{
	return 0;
}
#define pgd_huge pgd_huge

static inline int hugepd_ok(hugepd_t hpd)
{
	/*
	 * if it is not a pte and have hugepd shift mask
	 * set, then it is a hugepd directory pointer
	 */
	if (!(hpd.pd & _PAGE_PTE) &&
	    ((hpd.pd & HUGEPD_SHIFT_MASK) != 0))
		return true;
	return false;
}
#define is_hugepd(hpd)		(hugepd_ok(hpd))
#endif

#endif /* !__ASSEMBLY__ */

#endif /* _ASM_POWERPC_BOOK3S_64_HASH_4K_H */
