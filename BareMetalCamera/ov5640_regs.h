#ifndef OV5640_REGS_H
#define OV5640_REGS_H

// ============ REGISTER ADDRESSES ============

// System Control

#define OV5640_REG_SYSTEM_CTRL0         0x3008

// Chip ID
#define OV5640_CHIP_ID_HIGH             0x300A
#define OV5640_CHIP_ID_LOW              0x300B

// I/O Pad control
#define OV5640_REG_PAD_CONTROL          0x302C

// Output Enable
#define OV5640_REG_PAD_OUTPUT_ENABLE01  0x3017
#define OV5640_REG_PAD_OUTPUT_ENABLE02  0x3018

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