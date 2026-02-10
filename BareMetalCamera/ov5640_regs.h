#ifndef OV5640_REGS_H
#define OV5640_REGS_H

// ============ REGISTER ADDRESSES ============

// System Control
#define OV5640_REG_SYSTEM_CTRL0         0x3008

#define OV5640_REG_SC_PLL_CTRL5         0x3039

// Chip ID
#define OV5640_CHIP_ID_HIGH             0x300A
#define OV5640_CHIP_ID_LOW              0x300B

// I/O Pad control
#define OV5640_REG_PAD_CONTROL          0x302C

// Output Enable
#define OV5640_REG_PAD_OUTPUT_ENABLE01  0x3017
#define OV5640_REG_PAD_OUTPUT_ENABLE02  0x3018

// PLL Control Registers
#define OV5640_REG_PLL_CTRL0            0x3034
#define OV5640_REG_PLL_CTRL1            0x3035
#define OV5640_REG_PLL_CTRL2            0x3036
#define OV5640_REG_PLL_CTRL3            0x3037
#define OV5640_REG_PLL_BYPASS           0x3039
#define OV5640_REG_PLL_CLK_SLCT         0x3103

// Format Control
#define OV5640_REG_FORMAT_CTRL          0x4300

// DVP Control
#define OV5640_REG_POLARITY_CTRL        0x4740

// ISP Top Control
#define OV5640_REG_ISP_CONTROL_00       0x5000
#define OV5640_REG_ISP_CONTROL_01       0x5001

// ISP Format
#define OV5640_REG_FORMAT_MUX_CTRL      0x501F

// 8 Color Bar Test
#define OV5640_PRE_ISP_TEST_SETTING     0x503D


#endif // OV5640_REGS_H