// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

// THIS FILE HAS BEEN GENERATED, DO NOT EDIT MANUALLY. COMMAND:
// util/make_new_dif.py --mode=regen --only=autogen

#include "sw/device/lib/dif/autogen/dif_dma_autogen.h"

#include "gtest/gtest.h"
#include "sw/device/lib/base/mmio.h"
#include "sw/device/lib/base/mock_mmio.h"
#include "sw/device/lib/dif/dif_test_base.h"

#include "dma_regs.h"  // Generated.

namespace dif_dma_autogen_unittest {
namespace {
using ::mock_mmio::MmioTest;
using ::mock_mmio::MockDevice;
using ::testing::Eq;
using ::testing::Test;

class DmaTest : public Test, public MmioTest {
 protected:
  dif_dma_t dma_ = {.base_addr = dev().region()};
};

class InitTest : public DmaTest {};

TEST_F(InitTest, NullArgs) {
  EXPECT_DIF_BADARG(dif_dma_init(dev().region(), nullptr));
}

TEST_F(InitTest, Success) {
  EXPECT_DIF_OK(dif_dma_init(dev().region(), &dma_));
}

class AlertForceTest : public DmaTest {};

TEST_F(AlertForceTest, NullArgs) {
  EXPECT_DIF_BADARG(dif_dma_alert_force(nullptr, kDifDmaAlertFatalFault));
}

TEST_F(AlertForceTest, BadAlert) {
  EXPECT_DIF_BADARG(
      dif_dma_alert_force(nullptr, static_cast<dif_dma_alert_t>(32)));
}

TEST_F(AlertForceTest, Success) {
  // Force first alert.
  EXPECT_WRITE32(DMA_ALERT_TEST_REG_OFFSET,
                 {{DMA_ALERT_TEST_FATAL_FAULT_BIT, true}});
  EXPECT_DIF_OK(dif_dma_alert_force(&dma_, kDifDmaAlertFatalFault));
}

class IrqGetTypeTest : public DmaTest {};

TEST_F(IrqGetTypeTest, NullArgs) {
  dif_irq_type_t type;

  EXPECT_DIF_BADARG(dif_dma_irq_get_type(nullptr, kDifDmaIrqDmaDone, &type));

  EXPECT_DIF_BADARG(dif_dma_irq_get_type(&dma_, kDifDmaIrqDmaDone, nullptr));

  EXPECT_DIF_BADARG(dif_dma_irq_get_type(nullptr, kDifDmaIrqDmaDone, nullptr));
}

TEST_F(IrqGetTypeTest, BadIrq) {
  dif_irq_type_t type;

  EXPECT_DIF_BADARG(dif_dma_irq_get_type(
      &dma_, static_cast<dif_dma_irq_t>(kDifDmaIrqDmaError + 1), &type));
}

TEST_F(IrqGetTypeTest, Success) {
  dif_irq_type_t type;

  EXPECT_DIF_OK(dif_dma_irq_get_type(&dma_, kDifDmaIrqDmaDone, &type));
  EXPECT_EQ(type, kDifIrqTypeEvent);
}

class IrqGetStateTest : public DmaTest {};

TEST_F(IrqGetStateTest, NullArgs) {
  dif_dma_irq_state_snapshot_t irq_snapshot = 0;

  EXPECT_DIF_BADARG(dif_dma_irq_get_state(nullptr, &irq_snapshot));

  EXPECT_DIF_BADARG(dif_dma_irq_get_state(&dma_, nullptr));

  EXPECT_DIF_BADARG(dif_dma_irq_get_state(nullptr, nullptr));
}

TEST_F(IrqGetStateTest, SuccessAllRaised) {
  dif_dma_irq_state_snapshot_t irq_snapshot = 0;

  EXPECT_READ32(DMA_INTR_STATE_REG_OFFSET,
                std::numeric_limits<uint32_t>::max());
  EXPECT_DIF_OK(dif_dma_irq_get_state(&dma_, &irq_snapshot));
  EXPECT_EQ(irq_snapshot, std::numeric_limits<uint32_t>::max());
}

TEST_F(IrqGetStateTest, SuccessNoneRaised) {
  dif_dma_irq_state_snapshot_t irq_snapshot = 0;

  EXPECT_READ32(DMA_INTR_STATE_REG_OFFSET, 0);
  EXPECT_DIF_OK(dif_dma_irq_get_state(&dma_, &irq_snapshot));
  EXPECT_EQ(irq_snapshot, 0);
}

class IrqIsPendingTest : public DmaTest {};

TEST_F(IrqIsPendingTest, NullArgs) {
  bool is_pending;

  EXPECT_DIF_BADARG(
      dif_dma_irq_is_pending(nullptr, kDifDmaIrqDmaDone, &is_pending));

  EXPECT_DIF_BADARG(dif_dma_irq_is_pending(&dma_, kDifDmaIrqDmaDone, nullptr));

  EXPECT_DIF_BADARG(
      dif_dma_irq_is_pending(nullptr, kDifDmaIrqDmaDone, nullptr));
}

TEST_F(IrqIsPendingTest, BadIrq) {
  bool is_pending;
  // All interrupt CSRs are 32 bit so interrupt 32 will be invalid.
  EXPECT_DIF_BADARG(dif_dma_irq_is_pending(
      &dma_, static_cast<dif_dma_irq_t>(32), &is_pending));
}

TEST_F(IrqIsPendingTest, Success) {
  bool irq_state;

  // Get the first IRQ state.
  irq_state = false;
  EXPECT_READ32(DMA_INTR_STATE_REG_OFFSET,
                {{DMA_INTR_STATE_DMA_DONE_BIT, true}});
  EXPECT_DIF_OK(dif_dma_irq_is_pending(&dma_, kDifDmaIrqDmaDone, &irq_state));
  EXPECT_TRUE(irq_state);

  // Get the last IRQ state.
  irq_state = true;
  EXPECT_READ32(DMA_INTR_STATE_REG_OFFSET,
                {{DMA_INTR_STATE_DMA_ERROR_BIT, false}});
  EXPECT_DIF_OK(dif_dma_irq_is_pending(&dma_, kDifDmaIrqDmaError, &irq_state));
  EXPECT_FALSE(irq_state);
}

class AcknowledgeStateTest : public DmaTest {};

TEST_F(AcknowledgeStateTest, NullArgs) {
  dif_dma_irq_state_snapshot_t irq_snapshot = 0;
  EXPECT_DIF_BADARG(dif_dma_irq_acknowledge_state(nullptr, irq_snapshot));
}

TEST_F(AcknowledgeStateTest, AckSnapshot) {
  constexpr uint32_t num_irqs = 2;
  constexpr uint32_t irq_mask = (uint64_t{1} << num_irqs) - 1;
  dif_dma_irq_state_snapshot_t irq_snapshot = 1;

  // Test a few snapshots.
  for (size_t i = 0; i < num_irqs; ++i) {
    irq_snapshot = ~irq_snapshot & irq_mask;
    irq_snapshot |= (1u << i);
    EXPECT_WRITE32(DMA_INTR_STATE_REG_OFFSET, irq_snapshot);
    EXPECT_DIF_OK(dif_dma_irq_acknowledge_state(&dma_, irq_snapshot));
  }
}

TEST_F(AcknowledgeStateTest, SuccessNoneRaised) {
  dif_dma_irq_state_snapshot_t irq_snapshot = 0;

  EXPECT_READ32(DMA_INTR_STATE_REG_OFFSET, 0);
  EXPECT_DIF_OK(dif_dma_irq_get_state(&dma_, &irq_snapshot));
  EXPECT_EQ(irq_snapshot, 0);
}

class AcknowledgeAllTest : public DmaTest {};

TEST_F(AcknowledgeAllTest, NullArgs) {
  EXPECT_DIF_BADARG(dif_dma_irq_acknowledge_all(nullptr));
}

TEST_F(AcknowledgeAllTest, Success) {
  EXPECT_WRITE32(DMA_INTR_STATE_REG_OFFSET,
                 std::numeric_limits<uint32_t>::max());

  EXPECT_DIF_OK(dif_dma_irq_acknowledge_all(&dma_));
}

class IrqAcknowledgeTest : public DmaTest {};

TEST_F(IrqAcknowledgeTest, NullArgs) {
  EXPECT_DIF_BADARG(dif_dma_irq_acknowledge(nullptr, kDifDmaIrqDmaDone));
}

TEST_F(IrqAcknowledgeTest, BadIrq) {
  EXPECT_DIF_BADARG(
      dif_dma_irq_acknowledge(nullptr, static_cast<dif_dma_irq_t>(32)));
}

TEST_F(IrqAcknowledgeTest, Success) {
  // Clear the first IRQ state.
  EXPECT_WRITE32(DMA_INTR_STATE_REG_OFFSET,
                 {{DMA_INTR_STATE_DMA_DONE_BIT, true}});
  EXPECT_DIF_OK(dif_dma_irq_acknowledge(&dma_, kDifDmaIrqDmaDone));

  // Clear the last IRQ state.
  EXPECT_WRITE32(DMA_INTR_STATE_REG_OFFSET,
                 {{DMA_INTR_STATE_DMA_ERROR_BIT, true}});
  EXPECT_DIF_OK(dif_dma_irq_acknowledge(&dma_, kDifDmaIrqDmaError));
}

class IrqForceTest : public DmaTest {};

TEST_F(IrqForceTest, NullArgs) {
  EXPECT_DIF_BADARG(dif_dma_irq_force(nullptr, kDifDmaIrqDmaDone, true));
}

TEST_F(IrqForceTest, BadIrq) {
  EXPECT_DIF_BADARG(
      dif_dma_irq_force(nullptr, static_cast<dif_dma_irq_t>(32), true));
}

TEST_F(IrqForceTest, Success) {
  // Force first IRQ.
  EXPECT_WRITE32(DMA_INTR_TEST_REG_OFFSET,
                 {{DMA_INTR_TEST_DMA_DONE_BIT, true}});
  EXPECT_DIF_OK(dif_dma_irq_force(&dma_, kDifDmaIrqDmaDone, true));

  // Force last IRQ.
  EXPECT_WRITE32(DMA_INTR_TEST_REG_OFFSET,
                 {{DMA_INTR_TEST_DMA_ERROR_BIT, true}});
  EXPECT_DIF_OK(dif_dma_irq_force(&dma_, kDifDmaIrqDmaError, true));
}

class IrqGetEnabledTest : public DmaTest {};

TEST_F(IrqGetEnabledTest, NullArgs) {
  dif_toggle_t irq_state;

  EXPECT_DIF_BADARG(
      dif_dma_irq_get_enabled(nullptr, kDifDmaIrqDmaDone, &irq_state));

  EXPECT_DIF_BADARG(dif_dma_irq_get_enabled(&dma_, kDifDmaIrqDmaDone, nullptr));

  EXPECT_DIF_BADARG(
      dif_dma_irq_get_enabled(nullptr, kDifDmaIrqDmaDone, nullptr));
}

TEST_F(IrqGetEnabledTest, BadIrq) {
  dif_toggle_t irq_state;

  EXPECT_DIF_BADARG(dif_dma_irq_get_enabled(
      &dma_, static_cast<dif_dma_irq_t>(32), &irq_state));
}

TEST_F(IrqGetEnabledTest, Success) {
  dif_toggle_t irq_state;

  // First IRQ is enabled.
  irq_state = kDifToggleDisabled;
  EXPECT_READ32(DMA_INTR_ENABLE_REG_OFFSET,
                {{DMA_INTR_ENABLE_DMA_DONE_BIT, true}});
  EXPECT_DIF_OK(dif_dma_irq_get_enabled(&dma_, kDifDmaIrqDmaDone, &irq_state));
  EXPECT_EQ(irq_state, kDifToggleEnabled);

  // Last IRQ is disabled.
  irq_state = kDifToggleEnabled;
  EXPECT_READ32(DMA_INTR_ENABLE_REG_OFFSET,
                {{DMA_INTR_ENABLE_DMA_ERROR_BIT, false}});
  EXPECT_DIF_OK(dif_dma_irq_get_enabled(&dma_, kDifDmaIrqDmaError, &irq_state));
  EXPECT_EQ(irq_state, kDifToggleDisabled);
}

class IrqSetEnabledTest : public DmaTest {};

TEST_F(IrqSetEnabledTest, NullArgs) {
  dif_toggle_t irq_state = kDifToggleEnabled;

  EXPECT_DIF_BADARG(
      dif_dma_irq_set_enabled(nullptr, kDifDmaIrqDmaDone, irq_state));
}

TEST_F(IrqSetEnabledTest, BadIrq) {
  dif_toggle_t irq_state = kDifToggleEnabled;

  EXPECT_DIF_BADARG(dif_dma_irq_set_enabled(
      &dma_, static_cast<dif_dma_irq_t>(32), irq_state));
}

TEST_F(IrqSetEnabledTest, Success) {
  dif_toggle_t irq_state;

  // Enable first IRQ.
  irq_state = kDifToggleEnabled;
  EXPECT_MASK32(DMA_INTR_ENABLE_REG_OFFSET,
                {{DMA_INTR_ENABLE_DMA_DONE_BIT, 0x1, true}});
  EXPECT_DIF_OK(dif_dma_irq_set_enabled(&dma_, kDifDmaIrqDmaDone, irq_state));

  // Disable last IRQ.
  irq_state = kDifToggleDisabled;
  EXPECT_MASK32(DMA_INTR_ENABLE_REG_OFFSET,
                {{DMA_INTR_ENABLE_DMA_ERROR_BIT, 0x1, false}});
  EXPECT_DIF_OK(dif_dma_irq_set_enabled(&dma_, kDifDmaIrqDmaError, irq_state));
}

class IrqDisableAllTest : public DmaTest {};

TEST_F(IrqDisableAllTest, NullArgs) {
  dif_dma_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_DIF_BADARG(dif_dma_irq_disable_all(nullptr, &irq_snapshot));

  EXPECT_DIF_BADARG(dif_dma_irq_disable_all(nullptr, nullptr));
}

TEST_F(IrqDisableAllTest, SuccessNoSnapshot) {
  EXPECT_WRITE32(DMA_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_DIF_OK(dif_dma_irq_disable_all(&dma_, nullptr));
}

TEST_F(IrqDisableAllTest, SuccessSnapshotAllDisabled) {
  dif_dma_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_READ32(DMA_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_WRITE32(DMA_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_DIF_OK(dif_dma_irq_disable_all(&dma_, &irq_snapshot));
  EXPECT_EQ(irq_snapshot, 0);
}

TEST_F(IrqDisableAllTest, SuccessSnapshotAllEnabled) {
  dif_dma_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_READ32(DMA_INTR_ENABLE_REG_OFFSET,
                std::numeric_limits<uint32_t>::max());
  EXPECT_WRITE32(DMA_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_DIF_OK(dif_dma_irq_disable_all(&dma_, &irq_snapshot));
  EXPECT_EQ(irq_snapshot, std::numeric_limits<uint32_t>::max());
}

class IrqRestoreAllTest : public DmaTest {};

TEST_F(IrqRestoreAllTest, NullArgs) {
  dif_dma_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_DIF_BADARG(dif_dma_irq_restore_all(nullptr, &irq_snapshot));

  EXPECT_DIF_BADARG(dif_dma_irq_restore_all(&dma_, nullptr));

  EXPECT_DIF_BADARG(dif_dma_irq_restore_all(nullptr, nullptr));
}

TEST_F(IrqRestoreAllTest, SuccessAllEnabled) {
  dif_dma_irq_enable_snapshot_t irq_snapshot =
      std::numeric_limits<uint32_t>::max();

  EXPECT_WRITE32(DMA_INTR_ENABLE_REG_OFFSET,
                 std::numeric_limits<uint32_t>::max());
  EXPECT_DIF_OK(dif_dma_irq_restore_all(&dma_, &irq_snapshot));
}

TEST_F(IrqRestoreAllTest, SuccessAllDisabled) {
  dif_dma_irq_enable_snapshot_t irq_snapshot = 0;

  EXPECT_WRITE32(DMA_INTR_ENABLE_REG_OFFSET, 0);
  EXPECT_DIF_OK(dif_dma_irq_restore_all(&dma_, &irq_snapshot));
}

}  // namespace
}  // namespace dif_dma_autogen_unittest
