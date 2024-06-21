#include "DirectXCommon.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

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

	MapChipData mapChipData_;

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	uint32_t GetNumBlockVirtical() const { return kNumBlockVirtical; }
	uint32_t GetNumBlockHorizontal() const { return kNumBlockHorizontal; }

private:
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;
};
