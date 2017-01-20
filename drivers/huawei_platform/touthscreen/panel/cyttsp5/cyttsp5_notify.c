#include <linux/notifier.h>
#include <linux/export.h>

static BLOCKING_NOTIFIER_HEAD(tp_notifier_list);

/**
 *	tp_register_client - register a client notifier
 *	@nb: notifier block to callback on events
 */
int tp_register_client(struct notifier_block *nb)
{
	return blocking_notifier_chain_register(&tp_notifier_list, nb);
}
EXPORT_SYMBOL_GPL(tp_register_client);

/**
 *	tp_unregister_client - unregister a client notifier
 *	@nb: notifier block to callback on events
 */
int tp_unregister_client(struct notifier_block *nb)
{
	return blocking_notifier_chain_unregister(&tp_notifier_list, nb);
}
EXPORT_SYMBOL_GPL(tp_unregister_client);
/**
 * tp_notifier_call_chain - notify clients of tp_events
 *
 */
int tp_notifier_call_chain(unsigned long val, void *v)
{
	return blocking_notifier_call_chain(&tp_notifier_list, val, v);
}
EXPORT_SYMBOL_GPL(tp_notifier_call_chain);