// SPDX-License-Identifier: GPL-2.0

//! Driver for T7XX WWAN Modem

use std::sync::Arc;

use kernel::{
    bindings,
    c_str,
    dma,
    driver,
    error::code::*,
    pci,
    pci::define_pci_id_table,
    prelude::*,
};

struct T7xxPCIDevice;

impl pci::Driver for T7xxPCIDevice {
    type Data = Arc<>;
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

//! TODO: Define module parameters
//!    params: {
//!
//!    }
module! {
    type: T7xxWwan,
    name: "mtk_t7xx",
    author: "Lambert Wang",
    description: "MediaTek PCIe 5G WWAN modem T7xx driver",
    license: "GPL",
}
