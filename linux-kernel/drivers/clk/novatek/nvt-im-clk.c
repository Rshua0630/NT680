/**
    NVT clock management module
    To handle Linux clock framework API interface
    @file nvt-im-clk.c
    @ingroup
    @note
    Copyright Novatek Microelectronics Corp. 2017. All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/spinlock.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/delay.h>

#include "nvt-im-clk.h"

void __iomem *cg_base = NULL;

#define CG_SETREG(ofs, value)	writel((value), (cg_base+(ofs)))
#define CG_GETREG(ofs)		readl(cg_base+(ofs))

void (*cg_lock) (void) = NULL;
void (*cg_unlock) (void) = NULL;

void nvt_cg_base_remap(void __iomem *remap_base)
{
	cg_base = remap_base;
}

void nvt_cg_lock_register(void (*lock_func) (void))
{
	cg_lock = lock_func;
}

void nvt_cg_unlock_register(void (*unlock_func) (void))
{
	cg_unlock = unlock_func;
}

int nvt_fixed_rate_clk_register(struct nvt_fixed_rate_clk fixed_rate_clks[],
				int array_size)
{
	struct clk *clk;
	int i, ret = 0;

	for (i = 0; i < array_size; i++) {
		clk =
		    clk_register_fixed_rate(NULL, fixed_rate_clks[i].name, NULL,
					    CLK_IS_ROOT,
					    fixed_rate_clks[i].fixed_rate);
		if (IS_ERR(clk)) {
			pr_err
			    ("%s: failed to register fixed rate clock \"%s\"\n",
			     __func__, fixed_rate_clks[i].name);
			ret = -EPERM;
			goto err;
		} else {
			ret = clk_prepare_enable(clk);
			if (ret < 0)
				pr_err
				    ("Fixed clock prepare & enable failed!\n");
		}
	}

err:
	return ret;
}

static int nvt_pll_clk_enable(struct clk_hw *hw)
{
	unsigned int reg_val;
	unsigned long flags;
	int timeout = 100;
	struct nvt_pll_clk *pll = container_of(hw, struct nvt_pll_clk, pll_hw);

	/* race condition protect. enter critical section */
	spin_lock_irqsave(pll->lock, flags);
	cg_lock();
	reg_val = CG_GETREG(pll->gate_reg_offset);
	reg_val |= (0x1 << pll->gate_bit_idx);
	CG_SETREG(pll->gate_reg_offset, reg_val);

	/* Wait PLLx_READY becomes 1 */
	while (timeout-- != 0) {
		if (CG_GETREG(pll->status_reg_offset) &
		    (0x1 << pll->gate_bit_idx))
			break;
	}

	if (timeout == 0)
		pr_err("%s is not ready!\n", pll->name);

	/* race condition protect. leave critical section */
	cg_unlock();
	spin_unlock_irqrestore(pll->lock, flags);

	return 0;
}

static void nvt_pll_clk_disable(struct clk_hw *hw)
{
	unsigned int reg_val;
	unsigned long flags;
	struct nvt_pll_clk *pll = container_of(hw, struct nvt_pll_clk, pll_hw);

	/* race condition protect. enter critical section */
	spin_lock_irqsave(pll->lock, flags);
	cg_lock();
	reg_val = CG_GETREG(pll->gate_reg_offset);
	reg_val &= ~(0x1 << pll->gate_bit_idx);
	CG_SETREG(pll->gate_reg_offset, reg_val);
	/* race condition protect. leave critical section */
	cg_unlock();
	spin_unlock_irqrestore(pll->lock, flags);
}

static int nvt_pll_clk_is_enabled(struct clk_hw *hw)
{
	unsigned long flags;
	bool is_enabled;
	struct nvt_pll_clk *pll = container_of(hw, struct nvt_pll_clk, pll_hw);

	/* race condition protect. enter critical section */
	spin_lock_irqsave(pll->lock, flags);
	cg_lock();
	is_enabled =
	    CG_GETREG(pll->gate_reg_offset) & (0x1 << pll->gate_bit_idx);
	/* race condition protect. leave critical section */
	cg_unlock();
	spin_unlock_irqrestore(pll->lock, flags);

	return is_enabled;
}

static long nvt_pll_clk_round_rate(struct clk_hw *hw, unsigned long rate,
				   unsigned long *parent_rate)
{
	struct nvt_pll_clk *pll;

	pll = container_of(hw, struct nvt_pll_clk, pll_hw);
	return clk_fixed_factor_ops.round_rate(pll->fixed_factor, rate,
					       &pll->parent_rate);
}

static unsigned long nvt_pll_clk_recalc_rate(struct clk_hw *hw,
					     unsigned long parent_rate)
{
	struct nvt_pll_clk *pll;

	pll = container_of(hw, struct nvt_pll_clk, pll_hw);
	return pll->current_rate;
}

static int nvt_pll_clk_set_rate(struct clk_hw *hw, unsigned long rate,
				unsigned long parent_rate)
{
	return -EPERM;
}

static const struct clk_ops nvt_pll_clk_ops = {
	.enable = nvt_pll_clk_enable,
	.disable = nvt_pll_clk_disable,
	.is_enabled = nvt_pll_clk_is_enabled,
	.round_rate = nvt_pll_clk_round_rate,
	.recalc_rate = nvt_pll_clk_recalc_rate,
	.set_rate = nvt_pll_clk_set_rate,
};

int nvt_pll_clk_register(struct nvt_pll_clk pll_clks[], int array_size,
			 unsigned long pll_div_value, spinlock_t *cg_spinlock)
{
	struct clk *clk;
	struct clk_init_data init;
	struct nvt_pll_clk *p_pll_clk;
	const char *parent_name;
	char *pstr;
	int i, ret = 0;

	pstr = kzalloc(CLK_NAME_STR_SIZE, GFP_KERNEL);
	if (!pstr) {
		ret = -EPERM;
		pr_err("%s: failed to alloc string buf!\n", __func__);
		goto err;
	}

	for (i = 0; i < array_size; i++) {
		snprintf(pstr, CLK_NAME_STR_SIZE, "%s_factor",
			 pll_clks[i].name);

		p_pll_clk = NULL;
		p_pll_clk = kzalloc(sizeof(struct nvt_pll_clk), GFP_KERNEL);
		if (!p_pll_clk) {
			pr_err("%s: failed to alloc pll_clk!\n", __func__);
			kfree(pstr);
			return -EPERM;
		}

		clk =
		    clk_register_fixed_factor(NULL, pstr, "osc_in", 0,
					      pll_clks[i].pll_ratio,
					      pll_div_value);
		p_pll_clk->fixed_factor = __clk_get_hw(clk);
		p_pll_clk->parent_rate = __clk_get_rate(__clk_get_parent(clk));
		p_pll_clk->current_rate = __clk_get_rate(clk);

		memcpy(p_pll_clk->name, pll_clks[i].name, CLK_NAME_STR_SIZE);
		p_pll_clk->pll_ratio = pll_clks[i].pll_ratio;
		p_pll_clk->rate_reg_offset = pll_clks[i].rate_reg_offset;
		p_pll_clk->gate_reg_offset = pll_clks[i].gate_reg_offset;
		p_pll_clk->gate_bit_idx = pll_clks[i].gate_bit_idx;
		p_pll_clk->lock = cg_spinlock;

		init.name = pll_clks[i].name;
		init.ops = &nvt_pll_clk_ops;
		init.flags = CLK_IGNORE_UNUSED;
		init.num_parents = 1;
		parent_name = pstr;
		init.parent_names = &parent_name;
		p_pll_clk->pll_hw.init = &init;

		clk = clk_register(NULL, &p_pll_clk->pll_hw);
		if (IS_ERR(clk)) {
			pr_err("%s: failed to register clk %s!\n", __func__,
			       p_pll_clk->name);
			ret = -EPERM;
			goto err;
		}
		if (clk_register_clkdev(clk, p_pll_clk->name, NULL)) {
			pr_err("%s: failed to register lookup %s!\n", __func__,
			       p_pll_clk->name);
			ret = -EPERM;
			goto err;
		} else {
			ret = clk_prepare_enable(clk);
			if (ret < 0)
				pr_err("%s prepare & enable failed!\n",
				       p_pll_clk->name);
		}
	}

	kfree(pstr);

err:
	return ret;
}

static int nvt_composite_gate_clk_prepare(struct clk_hw *hw)
{
	unsigned int reg_val;
	unsigned long flags;
	struct nvt_composite_gate_clk *gate =
	    container_of(hw, struct nvt_composite_gate_clk, cgate_hw);

	if (gate->do_reset) {
		if ((gate->reset_reg_offset != 0) && (gate->reset_bit_idx != 0)) {
			/* race condition protect. enter critical section */
			spin_lock_irqsave(gate->lock, flags);
			cg_lock();
			reg_val = CG_GETREG(gate->reset_reg_offset);
			reg_val &= ~(0x1 << gate->reset_bit_idx);
			CG_SETREG(gate->reset_reg_offset, reg_val);
			/* race condition protect. leave critical section */
			cg_unlock();
			spin_unlock_irqrestore(gate->lock, flags);
			udelay(10);
		}
	}
	if ((gate->reset_reg_offset != 0) && (gate->reset_bit_idx != 0)) {
		/* race condition protect. enter critical section */
		spin_lock_irqsave(gate->lock, flags);
		cg_lock();
		reg_val = CG_GETREG(gate->reset_reg_offset);
		if ((reg_val & (0x1 << gate->reset_bit_idx)) == 0) {
			reg_val |= 0x1 << gate->reset_bit_idx;
			CG_SETREG(gate->reset_reg_offset, reg_val);
		}
		/* race condition protect. leave critical section */
		cg_unlock();
		spin_unlock_irqrestore(gate->lock, flags);
	}
	return 0;
}

static void nvt_composite_gate_clk_unprepare(struct clk_hw *hw)
{
	/* Do nothing */
}

static int nvt_composite_gate_clk_is_prepared(struct clk_hw *hw)
{
	unsigned int reg_val;
	unsigned long flags;
	struct nvt_composite_gate_clk *gate =
	    container_of(hw, struct nvt_composite_gate_clk, cgate_hw);

	if ((gate->reset_reg_offset != 0) && (gate->reset_bit_idx != 0)) {
		/* race condition protect. enter critical section */
		spin_lock_irqsave(gate->lock, flags);
		cg_lock();
		/* Check if RSTN bit is released */
		reg_val = CG_GETREG(gate->reset_reg_offset);
		/* race condition protect. leave critical section */
		cg_unlock();
		spin_unlock_irqrestore(gate->lock, flags);
		return (reg_val && (0x1 << gate->reset_bit_idx));
	} else
		return 0;
}

static int nvt_composite_gate_clk_enable(struct clk_hw *hw)
{
	unsigned int reg_val;
	unsigned long flags;
	struct nvt_composite_gate_clk *gate =
	    container_of(hw, struct nvt_composite_gate_clk, cgate_hw);

	/* race condition protect. enter critical section */
	spin_lock_irqsave(gate->lock, flags);
	cg_lock();
	reg_val = CG_GETREG(gate->gate_reg_offset);
	reg_val |= (0x1 << gate->gate_bit_idx);
	CG_SETREG(gate->gate_reg_offset, reg_val);
	/* race condition protect. leave critical section */
	cg_unlock();
	spin_unlock_irqrestore(gate->lock, flags);

	return 0;
}

static void nvt_composite_gate_clk_disable(struct clk_hw *hw)
{
	unsigned int reg_val;
	unsigned long flags;
	struct nvt_composite_gate_clk *gate =
	    container_of(hw, struct nvt_composite_gate_clk, cgate_hw);

	/* race condition protect. enter critical section */
	spin_lock_irqsave(gate->lock, flags);
	cg_lock();
	reg_val = CG_GETREG(gate->gate_reg_offset);
	reg_val &= ~(0x1 << gate->gate_bit_idx);
	CG_SETREG(gate->gate_reg_offset, reg_val);
	/* race condition protect. leave critical section */
	cg_unlock();
	spin_unlock_irqrestore(gate->lock, flags);
}

static int nvt_composite_gate_clk_is_enabled(struct clk_hw *hw)
{
	unsigned long flags;
	bool is_enabled;
	struct nvt_composite_gate_clk *gate =
	    container_of(hw, struct nvt_composite_gate_clk, cgate_hw);

	/* race condition protect. enter critical section */
	spin_lock_irqsave(gate->lock, flags);
	cg_lock();
	is_enabled =
	    CG_GETREG(gate->gate_reg_offset) & (0x1 << gate->gate_bit_idx);
	/* race condition protect. leave critical section */
	cg_unlock();
	spin_unlock_irqrestore(gate->lock, flags);

	return is_enabled;
}

static long nvt_composite_gate_clk_round_rate(struct clk_hw *hw,
					      unsigned long rate,
					      unsigned long *parent_rate)
{
	struct nvt_composite_gate_clk *gate =
	    container_of(hw, struct nvt_composite_gate_clk, cgate_hw);

	if (gate->divider == NULL)
		return rate;
	else
		return clk_divider_ops.round_rate(gate->divider, rate,
						  &gate->parent_rate);
}

static unsigned long nvt_composite_gate_clk_recalc_rate(struct clk_hw *hw,
							unsigned long
							parent_rate)
{
	struct nvt_composite_gate_clk *gate =
	    container_of(hw, struct nvt_composite_gate_clk, cgate_hw);

	return gate->current_rate;
}

static int nvt_composite_gate_clk_set_rate(struct clk_hw *hw,
					   unsigned long rate,
					   unsigned long parent_rate)
{
	int ret;
	struct nvt_composite_gate_clk *gate =
	    container_of(hw, struct nvt_composite_gate_clk, cgate_hw);

	if (gate->divider == NULL)
		return -EPERM;
	/* race condition protect. enter critical section */
	cg_lock();
	ret = clk_divider_ops.set_rate(gate->divider, rate, gate->parent_rate);
	/* race condition protect. leave critical section */
	cg_unlock();
	if (ret == 0)
		gate->current_rate = rate;
	return ret;
}

static int nvt_composite_gate_clk_set_phase(struct clk_hw *hw, int enable)
{
	unsigned int reg_val;
	unsigned long flags;
	struct nvt_composite_gate_clk *gate =
	    container_of(hw, struct nvt_composite_gate_clk, cgate_hw);

	if ((gate->autogating_reg_offset != 0) &&
	    (gate->autogating_bit_idx != 0)) {
		/* race condition protect. enter critical section */
		spin_lock_irqsave(gate->lock, flags);
		cg_lock();
		reg_val = CG_GETREG(gate->autogating_reg_offset);

		if (enable == 0)
			reg_val &= ~(0x1 << gate->autogating_bit_idx);
		else
			reg_val |= (0x1 << gate->autogating_bit_idx);
		CG_SETREG(gate->autogating_reg_offset, reg_val);
		/* race condition protect. leave critical section */
		reg_val = CG_GETREG(gate->autogating_reg_offset);

		cg_unlock();
		spin_unlock_irqrestore(gate->lock, flags);
	}

	return 0;
}

static int nvt_composite_gate_clk_get_phase(struct clk_hw *hw)
{
	unsigned long flags;
	int is_enabled = 0;
	struct nvt_composite_gate_clk *gate =
	    container_of(hw, struct nvt_composite_gate_clk, cgate_hw);

	if ((gate->autogating_reg_offset != 0) &&
	    (gate->autogating_bit_idx != 0)) {
		/* race condition protect. enter critical section */
		spin_lock_irqsave(gate->lock, flags);
		cg_lock();
		is_enabled =
		    CG_GETREG(gate->autogating_reg_offset) &
		    (0x1 << gate->autogating_bit_idx);
		/* race condition protect. leave critical section */
		cg_unlock();
		spin_unlock_irqrestore(gate->lock, flags);
	}

	return is_enabled;
}

static const struct clk_ops nvt_composite_gate_clk_ops = {
	.prepare = nvt_composite_gate_clk_prepare,
	.unprepare = nvt_composite_gate_clk_unprepare,
	.is_prepared = nvt_composite_gate_clk_is_prepared,
	.enable = nvt_composite_gate_clk_enable,
	.disable = nvt_composite_gate_clk_disable,
	.is_enabled = nvt_composite_gate_clk_is_enabled,
	.round_rate = nvt_composite_gate_clk_round_rate,
	.recalc_rate = nvt_composite_gate_clk_recalc_rate,
	.set_rate = nvt_composite_gate_clk_set_rate,
	.set_phase = nvt_composite_gate_clk_set_phase,
	.get_phase = nvt_composite_gate_clk_get_phase,
};

int nvt_composite_gate_clk_register(struct nvt_composite_gate_clk
				    composite_gate_clks[], int array_size,
				    spinlock_t *cg_spinlock)
{
	struct clk *clk;
	struct clk_init_data init;
	struct nvt_composite_gate_clk *p_cgate_clk;
	const char *parent_name;
	char *pstr;
	int i, ret = 0;

	pstr = kzalloc(CLK_NAME_STR_SIZE, GFP_KERNEL);
	if (!pstr) {
		ret = -EPERM;
		pr_err("%s: failed to alloc string buf!\n", __func__);
		goto err;
	}

	for (i = 0; i < array_size; i++) {
		snprintf(pstr, CLK_NAME_STR_SIZE, "%s_div",
			 composite_gate_clks[i].name);

		p_cgate_clk = NULL;
		p_cgate_clk =
		    kzalloc(sizeof(struct nvt_composite_gate_clk), GFP_KERNEL);
		if (!p_cgate_clk) {
			pr_err("%s: failed to alloc cgate_clk!\n", __func__);
			kfree(pstr);
			return -EPERM;
		}

		memcpy(p_cgate_clk->name, composite_gate_clks[i].name,
		       CLK_NAME_STR_SIZE);
		p_cgate_clk->div_reg_offset =
		    composite_gate_clks[i].div_reg_offset;
		p_cgate_clk->div_bit_idx = composite_gate_clks[i].div_bit_idx;
		p_cgate_clk->div_bit_width =
		    composite_gate_clks[i].div_bit_width;
		p_cgate_clk->gate_reg_offset =
		    composite_gate_clks[i].gate_reg_offset;
		p_cgate_clk->gate_bit_idx = composite_gate_clks[i].gate_bit_idx;
		p_cgate_clk->do_enable = composite_gate_clks[i].do_enable;
		p_cgate_clk->reset_reg_offset =
		    composite_gate_clks[i].reset_reg_offset;
		p_cgate_clk->reset_bit_idx =
		    composite_gate_clks[i].reset_bit_idx;
		p_cgate_clk->do_reset = composite_gate_clks[i].do_reset;
		p_cgate_clk->autogating_reg_offset =
		    composite_gate_clks[i].autogating_reg_offset;
		p_cgate_clk->autogating_bit_idx =
		    composite_gate_clks[i].autogating_bit_idx;
		p_cgate_clk->do_autogating = composite_gate_clks[i].do_autogating;
		p_cgate_clk->lock = cg_spinlock;

		if (composite_gate_clks[i].div_bit_width == 0) {
			/* No clock divider */
			snprintf(pstr, CLK_NAME_STR_SIZE, "%s",
				 composite_gate_clks[i].parent_name);
			p_cgate_clk->divider = NULL;
		} else {
			clk =
			    clk_register_divider(NULL, pstr,
						 composite_gate_clks
						 [i].parent_name,
						 CLK_GET_RATE_NOCACHE,
						 cg_base +
						 composite_gate_clks
						 [i].div_reg_offset,
						 composite_gate_clks
						 [i].div_bit_idx,
						 composite_gate_clks
						 [i].div_bit_width,
						 composite_gate_clks
						 [i].div_flags, cg_spinlock);
			p_cgate_clk->divider = __clk_get_hw(clk);
			p_cgate_clk->parent_rate =
			    __clk_get_rate(__clk_get_parent(clk));
		}

		if (p_cgate_clk->divider != NULL) {
			/* race condition protect. enter critical section */
			cg_lock();
			clk_divider_ops.set_rate(p_cgate_clk->divider,
						 composite_gate_clks
						 [i].current_rate,
						 p_cgate_clk->parent_rate);
			/* race condition protect. leave critical section */
			cg_unlock();
			p_cgate_clk->current_rate =
			    composite_gate_clks[i].current_rate;
		}

		init.name = composite_gate_clks[i].name;
		init.ops = &nvt_composite_gate_clk_ops;
		init.flags = CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE;
		init.num_parents = 1;
		parent_name = pstr;
		init.parent_names = &parent_name;
		p_cgate_clk->cgate_hw.init = &init;
		clk = clk_register(NULL, &(p_cgate_clk->cgate_hw));
		if (IS_ERR(clk)) {
			pr_err("%s: failed to register clk %s!\n", __func__,
			       p_cgate_clk->name);
			ret = -EPERM;
			goto err;
		}
		if (clk_register_clkdev(clk, p_cgate_clk->name, NULL)) {
			pr_err("%s: failed to register lookup %s!\n", __func__,
			       p_cgate_clk->name);
			ret = -EPERM;
			goto err;
		} else {
			if (p_cgate_clk->divider == NULL) {
				p_cgate_clk->parent_rate =
				    p_cgate_clk->current_rate =
				    __clk_get_rate(__clk_get_parent(clk));
			}
			if (composite_gate_clks[i].do_enable) {
				ret = clk_prepare_enable(clk);
				if (ret < 0)
					pr_err("%s prepare & enable failed!\n",
					       p_cgate_clk->name);
			}
		}
	}

	kfree(pstr);

err:
	return ret;
}

static int nvt_composite_group_pwm_clk_prepare(struct clk_hw *hw)
{
	unsigned int reg_val;
	unsigned long flags;
	struct nvt_composite_group_pwm_clk *gpwm =
	    container_of(hw, struct nvt_composite_group_pwm_clk, cgpwm_hw);

	if ((gpwm->reset_reg_offset != 0) && (gpwm->reset_bit_idx != 0)) {
		/* race condition protect. enter critical section */
		spin_lock_irqsave(gpwm->lock, flags);
		cg_lock();
		reg_val = CG_GETREG(gpwm->reset_reg_offset);
		if ((reg_val & (0x1 << gpwm->reset_bit_idx)) == 0) {
			reg_val |= 0x1 << gpwm->reset_bit_idx;
			CG_SETREG(gpwm->reset_reg_offset, reg_val);
		}
		/* race condition protect. leave critical section */
		cg_unlock();
		spin_unlock_irqrestore(gpwm->lock, flags);
	}

	return 0;
}

static void nvt_composite_group_pwm_clk_unprepare(struct clk_hw *hw)
{
	/* Do nothing */
}

static int nvt_composite_group_pwm_clk_is_prepared(struct clk_hw *hw)
{
	unsigned int reg_val;
	unsigned long flags;
	struct nvt_composite_group_pwm_clk *gpwm =
	    container_of(hw, struct nvt_composite_group_pwm_clk, cgpwm_hw);

	if ((gpwm->reset_reg_offset != 0) && (gpwm->reset_bit_idx != 0)) {
		/* race condition protect. enter critical section */
		spin_lock_irqsave(gpwm->lock, flags);
		cg_lock();
		/* Check if RSTN bit is released */
		reg_val = CG_GETREG(gpwm->reset_reg_offset);
		/* race condition protect. leave critical section */
		cg_unlock();
		spin_unlock_irqrestore(gpwm->lock, flags);
		return (reg_val && (0x1 << gpwm->reset_bit_idx));
	} else
		return 0;
}

static int nvt_composite_group_pwm_clk_enable(struct clk_hw *hw)
{
	unsigned int reg_val;
	unsigned long flags;
	struct nvt_composite_group_pwm_clk *gpwm =
	    container_of(hw, struct nvt_composite_group_pwm_clk, cgpwm_hw);

	/* race condition protect. enter critical section */
	spin_lock_irqsave(gpwm->lock, flags);
	cg_lock();
	reg_val = CG_GETREG(gpwm->gate_reg_offset);
	reg_val |= (0x1 << gpwm->gate_bit_idx);
	CG_SETREG(gpwm->gate_reg_offset, reg_val);
	/* race condition protect. leave critical section */
	cg_unlock();
	spin_unlock_irqrestore(gpwm->lock, flags);

	return 0;
}

static void nvt_composite_group_pwm_clk_disable(struct clk_hw *hw)
{
	unsigned int reg_val;
	unsigned long flags;
	struct nvt_composite_group_pwm_clk *gpwm =
	    container_of(hw, struct nvt_composite_group_pwm_clk, cgpwm_hw);
	/* race condition protect. enter critical section */
	spin_lock_irqsave(gpwm->lock, flags);
	cg_lock();
	reg_val = CG_GETREG(gpwm->gate_reg_offset);
	reg_val &= ~(0x1 << gpwm->gate_bit_idx);
	CG_SETREG(gpwm->gate_reg_offset, reg_val);
	/* race condition protect. leave critical section */
	cg_unlock();
	spin_unlock_irqrestore(gpwm->lock, flags);
}

static int nvt_composite_group_pwm_clk_is_enabled(struct clk_hw *hw)
{
	unsigned long flags;
	bool is_enabled;
	struct nvt_composite_group_pwm_clk *gpwm =
	    container_of(hw, struct nvt_composite_group_pwm_clk, cgpwm_hw);
	/* race condition protect. enter critical section */
	spin_lock_irqsave(gpwm->lock, flags);
	cg_lock();
	is_enabled =
	    CG_GETREG(gpwm->gate_reg_offset) & (0x1 << gpwm->gate_bit_idx);
	/* race condition protect. leave critical section */
	cg_unlock();
	spin_unlock_irqrestore(gpwm->lock, flags);

	return is_enabled;
}

static long nvt_composite_group_pwm_clk_round_rate(struct clk_hw *hw,
						   unsigned long rate,
						   unsigned long *parent_rate)
{
	struct nvt_composite_group_pwm_clk *gpwm =
	    container_of(hw, struct nvt_composite_group_pwm_clk, cgpwm_hw);

	if (gpwm->divider == NULL)
		return rate;
	else
		return clk_divider_ops.round_rate(gpwm->divider, rate,
						  &gpwm->parent_rate);
}

static unsigned long nvt_composite_group_pwm_clk_recalc_rate(struct clk_hw *hw,
							     unsigned long
							     parent_rate)
{
	struct nvt_composite_group_pwm_clk *gpwm =
	    container_of(hw, struct nvt_composite_group_pwm_clk, cgpwm_hw);
	return gpwm->current_rate;
}

static int nvt_composite_group_pwm_clk_set_rate(struct clk_hw *hw,
						unsigned long rate,
						unsigned long parent_rate)
{
	int ret;
	struct nvt_composite_group_pwm_clk *gpwm =
	    container_of(hw, struct nvt_composite_group_pwm_clk, cgpwm_hw);

	if (gpwm->divider == NULL)
		return -EPERM;
	/* race condition protect. enter critical section */
	cg_lock();
	ret = clk_divider_ops.set_rate(gpwm->divider, rate, gpwm->parent_rate);
	/* race condition protect. leave critical section */
	cg_unlock();
	if (ret == 0)
		gpwm->current_rate = rate;
	return ret;
}

static const struct clk_ops nvt_composite_group_pwm_clk_ops = {
	.prepare = nvt_composite_group_pwm_clk_prepare,
	.unprepare = nvt_composite_group_pwm_clk_unprepare,
	.is_prepared = nvt_composite_group_pwm_clk_is_prepared,
	.enable = nvt_composite_group_pwm_clk_enable,
	.disable = nvt_composite_group_pwm_clk_disable,
	.is_enabled = nvt_composite_group_pwm_clk_is_enabled,
	.round_rate = nvt_composite_group_pwm_clk_round_rate,
	.recalc_rate = nvt_composite_group_pwm_clk_recalc_rate,
	.set_rate = nvt_composite_group_pwm_clk_set_rate,
};

int nvt_composite_group_pwm_clk_register(struct nvt_composite_group_pwm_clk
					 composite_group_pwm_clks[],
					 int array_size,
					 spinlock_t *cg_spinlock)
{
	struct clk *clk, *div_clk;
	struct clk_init_data init;
	struct nvt_composite_group_pwm_clk *p_cgpwm_clk;
	const char *parent_name;
	char *pstr1, *pstr2;
	int i, j, ret = 0;

	pstr1 = kzalloc(CLK_NAME_STR_SIZE, GFP_KERNEL);
	if (!pstr1) {
		ret = -EPERM;
		pr_err("%s: failed to alloc string buf!\n", __func__);
		goto err;
	}
	pstr2 = kzalloc(CLK_NAME_STR_SIZE, GFP_KERNEL);
	if (!pstr2) {
		ret = -EPERM;
		pr_err("%s: failed to alloc string buf!\n", __func__);
		goto err;
	}

	for (i = 0; i < array_size; i++) {
		if (composite_group_pwm_clks[i].div_bit_width == 0) {
			pr_err("%s: PWM clock should have a divider HW!\n",
			       __func__);
			ret = -EPERM;
			goto err;
		}
		if (composite_group_pwm_clks[i].gate_bit_start_idx >
		    composite_group_pwm_clks[i].gate_bit_end_idx) {
			pr_err
			    ("%s: PWM start idx should be less than end idx!\n",
			     __func__);
			ret = -EPERM;
			goto err;
		}
		snprintf(pstr1, CLK_NAME_STR_SIZE, "pwm_clk%d-%d_div",
			 composite_group_pwm_clks[i].gate_bit_start_idx,
			 composite_group_pwm_clks[i].gate_bit_end_idx);

		div_clk =
		    clk_register_divider(NULL, pstr1,
					 composite_group_pwm_clks
					 [i].parent_name,
					 CLK_GET_RATE_NOCACHE,
					 cg_base +
					 composite_group_pwm_clks
					 [i].div_reg_offset,
					 composite_group_pwm_clks
					 [i].div_bit_idx,
					 composite_group_pwm_clks
					 [i].div_bit_width,
					 composite_group_pwm_clks
					 [i].div_flags, cg_spinlock);
		/* race condition protect. enter critical section */
		cg_lock();
		clk_divider_ops.set_rate(__clk_get_hw(div_clk),
					 composite_group_pwm_clks
					 [i].current_rate,
					 __clk_get_rate(__clk_get_parent
							(div_clk)));
		/* race condition protect. leave critical section */
		cg_unlock();

		for (j = composite_group_pwm_clks[i].gate_bit_start_idx;
		     j <= composite_group_pwm_clks[i].gate_bit_end_idx; j++) {

			p_cgpwm_clk = NULL;
			p_cgpwm_clk =
			    kzalloc(sizeof(struct nvt_composite_group_pwm_clk),
				    GFP_KERNEL);
			if (!p_cgpwm_clk) {
				pr_err("%s: failed to alloc cgpwm_clk!\n",
				       __func__);
				kfree(pstr1);
				ret = -EPERM;
				goto err;
			}

			p_cgpwm_clk->div_reg_offset =
			    composite_group_pwm_clks[i].div_reg_offset;
			p_cgpwm_clk->div_bit_idx =
			    composite_group_pwm_clks[i].div_bit_idx;
			p_cgpwm_clk->div_bit_width =
			    composite_group_pwm_clks[i].div_bit_width;
			p_cgpwm_clk->gate_reg_offset =
			    composite_group_pwm_clks[i].gate_reg_offset;
			p_cgpwm_clk->gate_bit_start_idx =
			    composite_group_pwm_clks[i].gate_bit_start_idx;
			p_cgpwm_clk->gate_bit_end_idx =
			    composite_group_pwm_clks[i].gate_bit_end_idx;
			p_cgpwm_clk->do_enable =
			    composite_group_pwm_clks[i].do_enable;
			p_cgpwm_clk->reset_reg_offset =
			    composite_group_pwm_clks[i].reset_reg_offset;
			p_cgpwm_clk->reset_bit_idx =
			    composite_group_pwm_clks[i].reset_bit_idx;
			p_cgpwm_clk->gate_bit_idx = j;
			p_cgpwm_clk->lock = cg_spinlock;

			p_cgpwm_clk->divider = __clk_get_hw(div_clk);
			p_cgpwm_clk->parent_rate =
			    __clk_get_rate(__clk_get_parent(div_clk));
			p_cgpwm_clk->current_rate =
			    composite_group_pwm_clks[i].current_rate;

			snprintf(pstr2, CLK_NAME_STR_SIZE, "pwm_clk.%d", j);
			init.name = pstr2;
			init.ops = &nvt_composite_group_pwm_clk_ops;
			init.flags = CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE;
			init.num_parents = 1;
			parent_name = pstr1;
			init.parent_names = &parent_name;

			p_cgpwm_clk->cgpwm_hw.init = &init;
			clk = clk_register(NULL, &(p_cgpwm_clk->cgpwm_hw));
			if (IS_ERR(clk)) {
				pr_err("%s: failed to register clk %s!",
				       __func__, pstr2);
				ret = -EPERM;
				goto err;
			}
			if (clk_register_clkdev(clk, pstr2, NULL)) {
				pr_err("%s: failed to register lookup %s!",
				       __func__, pstr2);
				ret = -EPERM;
				goto err;
			} else {
				if (composite_group_pwm_clks[i].do_enable) {
					ret = clk_prepare_enable(clk);
					if (ret < 0)
						pr_err
						    ("%s prepare & enable failed!",
						     pstr2);
				}
			}
		}
	}
	kfree(pstr1);
	kfree(pstr2);
err:
	return ret;
}

static int nvt_composite_mux_clk_prepare(struct clk_hw *hw)
{
	unsigned int reg_val;
	unsigned long flags;
	struct nvt_composite_mux_clk *mux =
	    container_of(hw, struct nvt_composite_mux_clk, cmux_hw);

	if (mux->do_reset) {
		if ((mux->reset_reg_offset != 0) && (mux->reset_bit_idx != 0)) {
			/* race condition protect. enter critical section */
			spin_lock_irqsave(mux->lock, flags);
			cg_lock();
			reg_val = CG_GETREG(mux->reset_reg_offset);
			reg_val &= ~(0x1 << mux->reset_bit_idx);
			CG_SETREG(mux->reset_reg_offset, reg_val);
			/* race condition protect. leave critical section */
			cg_unlock();
			spin_unlock_irqrestore(mux->lock, flags);
			udelay(10);
		}
	}

	if ((mux->reset_reg_offset != 0) && (mux->reset_bit_idx != 0)) {
		/* race condition protect. enter critical section */
		spin_lock_irqsave(mux->lock, flags);
		cg_lock();
		reg_val = CG_GETREG(mux->reset_reg_offset);
		if ((reg_val & (0x1 << mux->reset_bit_idx)) == 0) {
			reg_val |= 0x1 << mux->reset_bit_idx;
			CG_SETREG(mux->reset_reg_offset, reg_val);
			udelay(10);
		}
		/* race condition protect. leave critical section */
		cg_unlock();
		spin_unlock_irqrestore(mux->lock, flags);
	}

	return 0;
}

static void nvt_composite_mux_clk_unprepare(struct clk_hw *hw)
{
	/* Do nothing */
}

static int nvt_composite_mux_clk_is_prepared(struct clk_hw *hw)
{
	unsigned int reg_val;
	unsigned long flags;
	struct nvt_composite_mux_clk *mux =
	    container_of(hw, struct nvt_composite_mux_clk, cmux_hw);

	if ((mux->reset_reg_offset != 0) && (mux->reset_bit_idx != 0)) {
		/* race condition protect. enter critical section */
		spin_lock_irqsave(mux->lock, flags);
		cg_lock();
		/* Check if RSTN bit is released */
		reg_val = CG_GETREG(mux->reset_reg_offset);
		/* race condition protect. leave critical section */
		cg_unlock();
		spin_unlock_irqrestore(mux->lock, flags);
		return (reg_val && (0x1 << mux->reset_bit_idx));
	} else
		return 0;
}

static int nvt_composite_mux_clk_enable(struct clk_hw *hw)
{
	unsigned int reg_val;
	unsigned long flags;
	struct nvt_composite_mux_clk *mux =
	    container_of(hw, struct nvt_composite_mux_clk, cmux_hw);

	/* race condition protect. enter critical section */
	spin_lock_irqsave(mux->lock, flags);
	cg_lock();
	reg_val = CG_GETREG(mux->gate_reg_offset);
	reg_val |= (0x1 << mux->gate_bit_idx);
	CG_SETREG(mux->gate_reg_offset, reg_val);
	/* race condition protect. leave critical section */
	cg_unlock();
	spin_unlock_irqrestore(mux->lock, flags);

	return 0;
}

static void nvt_composite_mux_clk_disable(struct clk_hw *hw)
{
	unsigned int reg_val;
	unsigned long flags;
	struct nvt_composite_mux_clk *mux =
	    container_of(hw, struct nvt_composite_mux_clk, cmux_hw);

	/* race condition protect. enter critical section */
	spin_lock_irqsave(mux->lock, flags);
	cg_lock();
	reg_val = CG_GETREG(mux->gate_reg_offset);
	reg_val &= ~(0x1 << mux->gate_bit_idx);
	CG_SETREG(mux->gate_reg_offset, reg_val);
	/* race condition protect. leave critical section */
	cg_unlock();
	spin_unlock_irqrestore(mux->lock, flags);
}

static int nvt_composite_mux_clk_is_enabled(struct clk_hw *hw)
{
	unsigned long flags;
	bool is_enabled;
	struct nvt_composite_mux_clk *mux =
	    container_of(hw, struct nvt_composite_mux_clk, cmux_hw);

	/* race condition protect. enter critical section */
	spin_lock_irqsave(mux->lock, flags);
	cg_lock();
	is_enabled =
	    CG_GETREG(mux->gate_reg_offset) & (0x1 << mux->gate_bit_idx);
	/* race condition protect. leave critical section */
	cg_unlock();
	spin_unlock_irqrestore(mux->lock, flags);

	return is_enabled;
}

static long nvt_composite_mux_clk_round_rate(struct clk_hw *hw,
					     unsigned long rate,
					     unsigned long *parent_rate)
{
	struct nvt_composite_mux_clk *mux =
	    container_of(hw, struct nvt_composite_mux_clk, cmux_hw);

	if (mux->divider == NULL)
		return rate;
	else
		return clk_divider_ops.round_rate(mux->divider, rate,
						  &mux->parent_rate);
}

static unsigned long nvt_composite_mux_clk_recalc_rate(struct clk_hw
						       *hw,
						       unsigned long
						       parent_rate)
{
	struct nvt_composite_mux_clk *mux =
	    container_of(hw, struct nvt_composite_mux_clk, cmux_hw);
	return mux->current_rate;
}

static int nvt_composite_mux_clk_set_rate(struct clk_hw *hw,
					  unsigned long rate,
					  unsigned long parent_rate)
{
	int ret;
	struct nvt_composite_mux_clk *mux =
	    container_of(hw, struct nvt_composite_mux_clk, cmux_hw);

	if (mux->divider == NULL)
		return -EPERM;
	/* race condition protect. enter critical section */
	cg_lock();
	ret = clk_divider_ops.set_rate(mux->divider, rate, mux->parent_rate);
	/* race condition protect. leave critical section */
	cg_unlock();
	if (ret == 0)
		mux->current_rate = rate;
	return ret;
}

static int nvt_composite_mux_clk_set_parent(struct clk_hw *hw, u8 index)
{
	return -EPERM;
}

static u8 nvt_composite_mux_clk_get_parent(struct clk_hw *hw)
{
	return -EPERM;
}

static int nvt_composite_mux_clk_set_phase(struct clk_hw *hw, int enable)
{
	unsigned int reg_val;
	unsigned long flags;
	struct nvt_composite_mux_clk *mux =
	    container_of(hw, struct nvt_composite_mux_clk, cmux_hw);

	if ((mux->autogating_reg_offset != 0) && (mux->autogating_bit_idx != 0)) {
		/* race condition protect. enter critical section */
		spin_lock_irqsave(mux->lock, flags);
		cg_lock();
		reg_val = CG_GETREG(mux->autogating_reg_offset);
		if (enable == 0)
			reg_val &= ~(0x1 << mux->autogating_bit_idx);
		else
			reg_val |= (0x1 << mux->autogating_bit_idx);

		CG_SETREG(mux->autogating_reg_offset, reg_val);
		/* race condition protect. leave critical section */
		cg_unlock();
		spin_unlock_irqrestore(mux->lock, flags);
	}

	return 0;
}

static int nvt_composite_mux_clk_get_phase(struct clk_hw *hw)
{
	unsigned long flags;
	int is_enabled = 0;
	struct nvt_composite_mux_clk *mux =
	    container_of(hw, struct nvt_composite_mux_clk, cmux_hw);

	if ((mux->autogating_reg_offset != 0) && (mux->autogating_bit_idx != 0)) {
		/* race condition protect. enter critical section */
		spin_lock_irqsave(mux->lock, flags);
		cg_lock();
		is_enabled =
		    CG_GETREG(mux->autogating_reg_offset) & (0x1 << mux->autogating_bit_idx);
		/* race condition protect. leave critical section */
		cg_unlock();
		spin_unlock_irqrestore(mux->lock, flags);
	}

	return is_enabled;
}

static const struct clk_ops nvt_composite_mux_clk_ops = {
	.prepare = nvt_composite_mux_clk_prepare,
	.unprepare = nvt_composite_mux_clk_unprepare,
	.is_prepared = nvt_composite_mux_clk_is_prepared,
	.enable = nvt_composite_mux_clk_enable,
	.disable = nvt_composite_mux_clk_disable,
	.is_enabled = nvt_composite_mux_clk_is_enabled,
	.round_rate = nvt_composite_mux_clk_round_rate,
	.recalc_rate = nvt_composite_mux_clk_recalc_rate,
	.set_rate = nvt_composite_mux_clk_set_rate,
	.set_parent = nvt_composite_mux_clk_set_parent,
	.get_parent = nvt_composite_mux_clk_get_parent,
	.set_phase = nvt_composite_mux_clk_set_phase,
	.get_phase = nvt_composite_mux_clk_get_phase,
};

int nvt_composite_mux_clk_register(struct nvt_composite_mux_clk
				   composite_mux_clks[], int array_size,
				   spinlock_t *cg_spinlock)
{
	struct clk *clk;
	struct clk_init_data init;
	struct nvt_composite_mux_clk *p_cmux_clk;
	const char *parent_name;
	char *pstr1, *pstr2;
	int i, ret = 0;

	pstr1 = kzalloc(CLK_NAME_STR_SIZE, GFP_KERNEL);
	if (!pstr1) {
		ret = -EPERM;
		pr_err("%s: failed to alloc string buf\n", __func__);
		goto err;
	}
	pstr2 = kzalloc(CLK_NAME_STR_SIZE, GFP_KERNEL);
	if (!pstr2) {
		ret = -EPERM;
		pr_err("%s: failed to alloc string buf\n", __func__);
		goto err;
	}

	for (i = 0; i < array_size; i++) {
		snprintf(pstr1, CLK_NAME_STR_SIZE, "%s_mux",
			 composite_mux_clks[i].name);
		snprintf(pstr2, CLK_NAME_STR_SIZE, "%s_div",
			 composite_mux_clks[i].name);

		p_cmux_clk = NULL;
		p_cmux_clk =
		    kzalloc(sizeof(struct nvt_composite_mux_clk), GFP_KERNEL);
		if (!p_cmux_clk) {
			pr_err("%s: failed to alloc cmux_clk\n", __func__);
			kfree(pstr1);
			kfree(pstr2);
			return -EPERM;
		}

		memcpy(p_cmux_clk->name, composite_mux_clks[i].name,
		       CLK_NAME_STR_SIZE);
		p_cmux_clk->mux_reg_offset =
		    composite_mux_clks[i].mux_reg_offset;
		p_cmux_clk->mux_bit_idx = composite_mux_clks[i].mux_bit_idx;
		p_cmux_clk->mux_bit_width = composite_mux_clks[i].mux_bit_width;
		p_cmux_clk->div_reg_offset =
		    composite_mux_clks[i].div_reg_offset;
		p_cmux_clk->div_bit_idx = composite_mux_clks[i].div_bit_idx;
		p_cmux_clk->div_bit_width = composite_mux_clks[i].div_bit_width;
		p_cmux_clk->gate_reg_offset =
		    composite_mux_clks[i].gate_reg_offset;
		p_cmux_clk->gate_bit_idx = composite_mux_clks[i].gate_bit_idx;
		p_cmux_clk->do_enable = composite_mux_clks[i].do_enable;
		p_cmux_clk->reset_reg_offset =
		    composite_mux_clks[i].reset_reg_offset;
		p_cmux_clk->reset_bit_idx = composite_mux_clks[i].reset_bit_idx;
		p_cmux_clk->do_reset = composite_mux_clks[i].do_reset;
		p_cmux_clk->autogating_reg_offset =
		    composite_mux_clks[i].autogating_reg_offset;
		p_cmux_clk->autogating_bit_idx = composite_mux_clks[i].autogating_bit_idx;
		p_cmux_clk->do_autogating = composite_mux_clks[i].do_autogating;
		p_cmux_clk->lock = cg_spinlock;

		clk =
		    clk_register_mux(NULL, pstr1,
				     composite_mux_clks[i].parent_names,
				     composite_mux_clks[i].num_parents,
				     composite_mux_clks[i].mux_flags,
				     cg_base +
				     composite_mux_clks[i].mux_reg_offset,
				     composite_mux_clks[i].mux_bit_idx,
				     composite_mux_clks[i].mux_bit_width, 0,
				     cg_spinlock);
		p_cmux_clk->mux = __clk_get_hw(clk);

		/* race condition protect. enter critical section */
		cg_lock();
		clk_set_parent(clk,
			       __clk_lookup(composite_mux_clks[i].parent_names
					    [composite_mux_clks
					     [i].parent_idx]));
		/* race condition protect. leave critical section */
		cg_unlock();

		p_cmux_clk->parent_rate = __clk_get_rate(__clk_get_parent(clk));

		if (composite_mux_clks[i].div_bit_width == 0) {
			/* No clock divider */
			snprintf(pstr2, CLK_NAME_STR_SIZE, "%s", pstr1);
			p_cmux_clk->divider = NULL;
		} else {
			clk =
			    clk_register_divider(NULL, pstr2, pstr1,
						 CLK_GET_RATE_NOCACHE,
						 cg_base +
						 composite_mux_clks
						 [i].div_reg_offset,
						 composite_mux_clks
						 [i].div_bit_idx,
						 composite_mux_clks
						 [i].div_bit_width,
						 composite_mux_clks
						 [i].div_flags, cg_spinlock);
			p_cmux_clk->divider = __clk_get_hw(clk);
		}

		if (p_cmux_clk->divider == NULL)
			p_cmux_clk->current_rate = p_cmux_clk->parent_rate;
		else {
			/* race condition protect. enter critical section */
			cg_lock();
			clk_divider_ops.set_rate(p_cmux_clk->divider,
						 composite_mux_clks
						 [i].current_rate,
						 p_cmux_clk->parent_rate);
			/* race condition protect. leave critical section */
			cg_unlock();
			p_cmux_clk->current_rate =
			    composite_mux_clks[i].current_rate;
		}

		init.name = composite_mux_clks[i].name;
		init.ops = &nvt_composite_mux_clk_ops;
		init.flags = CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE;
		init.num_parents = 1;
		parent_name = pstr2;
		init.parent_names = &parent_name;
		p_cmux_clk->cmux_hw.init = &init;
		clk = clk_register(NULL, &(p_cmux_clk->cmux_hw));
		if (IS_ERR(clk)) {
			pr_err("%s: failed to register clk %s\n",
			       __func__, p_cmux_clk->name);
			ret = -EPERM;
			goto err;
		}
		if (clk_register_clkdev(clk, p_cmux_clk->name, NULL)) {
			pr_err("%s: failed to register lookup %s\n",
			       __func__, p_cmux_clk->name);
			ret = -EPERM;
			goto err;
		} else {
			if (composite_mux_clks[i].do_enable) {
				ret = clk_prepare_enable(clk);
				if (ret < 0)
					pr_err("%s prepare & enable failed!\n",
					       p_cmux_clk->name);
			}
			if (composite_mux_clks[i].do_autogating) {
				ret = clk_set_phase(clk, composite_mux_clks[i].do_autogating);
				if (ret < 0)
					pr_err("%s enable autogating failed!\n",
					       p_cmux_clk->name);
			}
		}
	}

	kfree(pstr1);
	kfree(pstr2);

err:
	return ret;
}
