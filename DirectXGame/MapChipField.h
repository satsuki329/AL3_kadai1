#pragma once

#include<stdint.h>
#include<vector>
#include"Vector3.h"

#include "Vector3.h"
#include <stdint.h>
#include <vector>

enum class MapChipType { 
	kBlank,
	kBlock,
};

struct MapChipData
{
	std::vector<std::vector<MapChipType>> data;
};

class MapChipField
{
public:
	void ResetMapChipDate();

	void LoadMapChipCsv(const std::string& filepath);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	uint32_t GetNumBlockVirtical();
	uint32_t GetNumBlockHorizontal();

private:
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;
};
