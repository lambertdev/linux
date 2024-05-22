// SPDX-License-Identifier: GPL-2.0

//! Driver for T7XX WWAN Modem


use core::result::Result;

use bindings::pci_resource_start;
use kernel::{
    bindings,
    c_str,
    device::{self, RawDevice},
    dma,
    driver,
    error::code::*,
    io_mem::IoMem,
    pci::{self, define_pci_id_table},
    prelude::*,
    sync::Arc,
};

const T7XX_DMA_MASK_64: u64 = !0;
const T7XX_IREG_BAR: i32 = 1<<0;
const T7XX_EXT_REG_BAR: i32 = 1<<2;

// Size from lspci
const T7XX_IREG_BAR_SIZE: usize = 32768; //32KB
const T7XX_EXT_REG_BAR_SIZE: usize = 8388608; //8MB

// ATR Source
const ATR_SRC_PCI_WIN0: u32 = 0;
const ATR_SRC_PCI_WIN1: u32 = 1;
const ATR_SRC_AXIS_0: u32 = 2;
const ATR_SRC_AXIS_1: u32 = 3;
const ATR_SRC_AXIS_2: u32 = 4;
const ATR_SRC_AXIS_3: u32 = 5;

//ATR Destination
const ATR_DST_PCI_TRX: u32 = 0;
const ATR_DST_PCI_CONFIG: u32 = 1;
const ATR_DST_AXIM_0: u32 = 4;
const ATR_DST_AXIM_1: u32 = 5;
const ATR_DST_AXIM_2: u32 = 6;
const ATR_DST_AXIM_3: u32 = 7;

const T7XX_PCIE_REG_TRSL_ADDR_CHIP: u32 = 0x10000000;
const T7XX_PCIE_REG_SIZE_CHIP: u32 = 0x400000;
const INFRACFG_AO_DEV_CHIP: u32 = 0x10001000;
const T7XX_PCIE_REG_PORT: u32 = 0;
const T7XX_PCIE_REG_TABLE_NUM: u32 = 0;
const T7XX_PCIE_REG_TRSL_PORT: u32 = 4;

const T7XX_PCIE_DEV_DMA_PORT_START: u32 = 2;
const T7XX_PCIE_DEV_DMA_PORT_END: u32 = 4;
const T7XX_PCIE_DEV_DMA_TABLE_NUM: u32 = 0;
const T7XX_PCIE_DEV_DMA_TRSL_ADDR: u32 = 0;
const T7XX_PCIE_DEV_DMA_SRC_ADDR: u32 = 0;
const T7XX_PCIE_DEV_DMA_SIZE: u32 = 0;

const ATR_TABLE_NUM_PER_ATR: u32 = 8;
const ATR_TRANSPARENT_SIZE: u32 = 0x3f;
const ATR_PORT_OFFSET: u32 = 0x100;
const ATR_TABLE_OFFSET: u32 = 0x20;
const ATR_PCIE_WIN0_T0_ATR_PARAM_SRC_ADDR: u32 = 0x600;

struct T7xxAtrConfig {
    src_addr: u64,
    trsl_addr: u64,
    size: u64,
    port: u32,
    table: u32,
    trsl_id: u32,
    transparent: u32,
}

impl T7xxAtrConfig {
    pub fn new(src_addr: u64, trsl_addr: u64, size: u64, port: u32, table: u32, trsl_id: u32, transparent: u32) -> Self {
        Self {
            src_addr,
            trsl_addr,
            size,
            port,
            table,
            trsl_id,
            transparent
        }
    }

    pub fn change_cfg(&mut self, src_addr: u64, trsl_addr: u64, size: u64, port: u32, table: u32, trsl_id: u32, transparent: u32) {
        self.src_addr = src_addr;
        self.trsl_addr = trsl_addr;
        self.size = size,
        self.port = port,
        self.table = table,
        self.trsl_id = trsl_id;
        self.transparent = transparent
    }
}

struct T7xxPCIDevice;

type T7xxData = device::Data<(),(),()>;

impl T7xxPCIDevice {
    fn pm_init(dev: &mut pci::Device) -> Result<u32> {
        Ok(0)
    }

    fn pci_mac_atr_table_disable<const N: usize>(ireg_base: &IoMem<N>, port: u32) -> Result<u32> {
        // CtoRust Attention: How to iterate the enum?
        for i in 0..ATR_TABLE_NUM_PER_ATR {
            let offset:usize = ATR_PORT_OFFSET * port + ATR_TABLE_OFFSET * i;

            // CtoRust Attention: How to return error numbers?
            ireg_base.try_writeq_relaxed(0, offset + ATR_PCIE_WIN0_T0_ATR_PARAM_SRC_ADDR)?;
        }

        Ok(0)
    }

    //ToDo: Change to support different IoMem size
    fn pci_mac_atr_cfg<const N: usize>(ireg_base: &IoMem<N>, atr_cfg: &T7xxAtrConfig) -> Result<u32> {
        let atr_size;

        if(1 == atr_cfg.transparent) {
            atr_size = ATR_TRANSPARENT_SIZE;
        } else {
            if (atr_cfg.src_addr & (atr_cfg.size - 1) != 0) {
                //TODO: Implement dev_err, currently rust only support pr_* series print
                pr_err!("Source address {:#x} is not aligned to size {:#x}", atr_cfg.src_addr, atr_cfg.size);
                return Err(EINVAL);
            }

            if (atr_cfg.trsl_addr & (atr_cfg.size - 1) != 0) {
                //TODO: Implement dev_err, currently rust only support pr_* series print
                pr_err!("Translation address {:#x} is not aligned to size {:#x}", atr_cfg.trsl_addr, atr_cfg.size);
                return Err(EINVAL);
            }

            /// CtoRust Attention: how to support unbinded unexported function in C?
        }
        Ok(0)
    }

    fn pci_mac_atr_init<const N: usize>(dev: &mut pci::Device, ireg_base: &IoMem<N>) -> Result<u32> {

        for i in ATR_SRC_PCI_WIN0..=ATR_SRC_AXIS_3 {
            Self::pci_mac_atr_table_disable(ireg_base, i)?;
        }

        // RC to EP
        let mut atr_cfg = T7xxAtrConfig::new(
            unsafe {pci_resource_start(dev, T7XX_EXT_REG_BAR)},
            T7XX_PCIE_REG_TRSL_ADDR_CHIP,
            T7XX_PCIE_REG_SIZE_CHIP,
            T7XX_PCIE_REG_PORT,
            T7XX_PCIE_REG_TABLE_NUM,
            T7XX_PCIE_REG_TRSL_PORT,
            0
        );
        Self::pci_mac_atr_cfg(ireg_base, &atr_cfg)?;

        for i in T7XX_PCIE_DEV_DMA_PORT_START..=T7XX_PCIE_DEV_DMA_PORT_END {
            atr_cfg.change_cfg(
                T7XX_PCIE_DEV_DMA_SRC_ADDR,
                T7XX_PCIE_DEV_DMA_TRSL_ADDR,
                T7XX_PCIE_DEV_DMA_SIZE,
                i,
                T7XX_PCIE_DEV_DMA_TABLE_NUM,
                ATR_DST_PCI_TRX,
                1
            );
            Self::pci_mac_atr_cfg(ireg_base, &atr_cfg)?;
        }
        // EP to RC
        Ok(0)
    }
}

// ToDo: Should implement data structure for this device
impl pci::Driver for T7xxPCIDevice {
    type Data = Arc<T7xxData>;

    define_pci_id_table! {
        (),
        [(pci::DeviceId::new(0x14c3, 0x4d75), None)]
    }

    fn probe(dev: &mut pci::Device, id: core::prelude::v1::Option<&Self::IdInfo>) -> Result<Self::Data> {
        pr_info!("probe called");

        dev.enable_device_mem()?;
        dev.set_master();

        let bars = T7XX_IREG_BAR | T7XX_EXT_REG_BAR;
        dev.request_selected_regions(bars, c_str!("mtk_t7xx"))?;

        dev.dma_set_mask(T7XX_DMA_MASK_64)?;
        dev.dma_set_coherent_mask(T7XX_DMA_MASK_64)?;

        let ireg = dev.take_resource(0).ok_or(ENXIO)?;
        let ext_reg = dev.take_resource(2).ok_or(ENXIO)?;

        let ireg_base = unsafe {IoMem::<T7XX_IREG_BAR_SIZE>::try_new(ireg)}?;
        let ext_reg_base = unsafe {IoMem::<T7XX_EXT_REG_BAR_SIZE>::try_new(ext_reg)}?;

        Self::pci_mac_atr_init(dev, &ireg_base)?;

        let infra_ao_base = ext_reg_base + INFRACFG_AO_DEV_CHIP - T7XX_PCIE_REG_TRSL_ADDR_CHIP;

        let data: Self::Data = kernel::new_device_data!((),(),(), "t7xx::Data")?.into();

        Ok(data)
    }

    fn remove(_data: &Self::Data) {
        todo!()
    }
}

struct T7xxWwan {
    _registration: Pin<Box<driver::Registration<pci::Adapter<T7xxPCIDevice>>>>,
}

impl kernel::Module for T7xxWwan {
    fn init(name: &'static CStr, module: &'static ThisModule) -> Result<Self> {
        pr_info!("t7xx driver loaded\n");
        let registration = driver::Registration::new_pinned(c_str!("t7xx"), module)?;
        Ok(
            Self{
                _registration: registration,
            }
        )
    }
}

// TODO: Define module parameters
//    params: {
//
//    }
module! {
    type: T7xxWwan,
    name: "mtk_t7xx",
    author: "Lambert Wang",
    description: "MediaTek PCIe 5G WWAN modem T7xx driver",
    license: "GPL",
}
