#include "k2EngineLowPreCompile.h"
#include "MapChip2DRender.h"
#include <Level2DRender.h>

namespace nsK2EngineLow {
	void MapChip2DRender::Init(Level2DObjectData* objData)
	{
		//‰Šú‰»B
		m_spriteRender.Init(
			objData->ddsFilePath,
			static_cast<float>(objData->width),
			static_cast<float>(objData->height)
		);
		m_spriteRender.SetPosition(objData->position);
		m_spriteRender.SetScale(objData->scale);
		m_spriteRender.SetPivot(objData->pivot);
	}
}

