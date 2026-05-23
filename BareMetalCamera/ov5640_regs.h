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

// Timing Control
#define OV5640_REG_X_START_MSB          0x3800
#define OV5640_REG_X_START_LSB          0x3801
#define OV5640_REG_Y_START_MSB          0x3802
#define OV5640_REG_Y_START_LSB          0x3803
#define OV5640_REG_X_END_MSB            0x3804
#define OV5640_REG_X_END_LSB            0x3805
#define OV5640_REG_Y_END_MSB            0x3806
#define OV5640_REG_Y_END_LSB            0x3807

#define OV5640_REG_WIDTH_MSB            0x3808
#define OV5640_REG_WIDTH_LSB            0x3809
#define OV5640_REG_HEIGHT_MSB           0x380A
#define OV5640_REG_HEIGHT_LSB           0x380B

#define OV5640_REG_HTS_MSB              0x380C
#define OV5640_REG_HTS_LSB              0x380D
#define OV5640_REG_VTS_MSB              0x380E
#define OV5640_REG_VTS_LSB              0x380F

#define OV5640_REG_X_INC                0x3814
#define OV5640_REG_Y_INC                0x3815

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
#define OV5640_REG_8CB_TEST_SETTING     0x503D

// Walking Bit Test
#define OV5640_REG_WB_TEST_SETTING      0X4741

#endif // OV5640_REGS_H