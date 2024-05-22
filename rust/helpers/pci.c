// SPDX-License-Identifier: GPL-2.0

#include "linux/export.h"
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

u64 rust_helper_pci_resource_start(struct pci_dev *pdev, int bar)
{
	return pci_resource_start(pdev, bar);
}
EXPORT_SYMBOL_GPL(rust_helper_pci_resource_start);
