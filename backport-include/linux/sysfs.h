#ifndef __BACKPORT_LINUX_SYSFS_H
#define __BACKPORT_LINUX_SYSFS_H
#include_next <linux/sysfs.h>
#include <linux/version.h>

#ifndef __ATTR_RW
#define __ATTR_RW(_name) __ATTR(_name, (S_IWUSR | S_IRUGO),		\
			 _name##_show, _name##_store)
#endif

#if LINUX_VERSION_IS_LESS(5,10,0)
#define sysfs_emit sprintf
#endif /* < 5.10 */

#endif /* __BACKPORT_LINUX_SYSFS_H */
