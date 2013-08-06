#include "List.h"


NEWDEL_IMPL(ListItem)
NEWDEL_IMPL(List)
NEWDEL_IMPL(ListIterator)
NEWDEL_IMPL(ListItemPool)

ListItemPool List::mPool;
