#include "MaterialData.h"

NEWDEL_IMPL(MaterialData)
MaterialData::MaterialData(void)
{
	index = -1;
	textureIndex = -1;
	hasAlpha = 0;
}

MaterialData::~MaterialData(void)
{
}
