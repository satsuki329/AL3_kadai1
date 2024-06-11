#pragma once
#include "DirectXCommon.h"

enum class MapChipType { 
	kBlank,
	kBlock,
};

struct MapChipDate
{
	std::vector<std::vector<MapChipType>> date;
};

class MapChipField
{
public:
	void ResetMapChipDate();

	void LoadMapChipCsv(const std::string& filepath);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);


private:
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipDate mapChipDate_;
};
