#include<stdint.h>
#include<vector>
#include"Vector3.h"

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
	struct IndexSet
	{
		uint32_t xindex;
		uint32_t yindex;
	};

	struct Rect {
		float left;
		float right;
		float bottom;
		float top;
	};

	void ResetMapChipDate();

	void LoadMapChipCsv(const std::string& filepath);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);

	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

	uint32_t GetNumBlockVirtical();
	uint32_t GetNumBlockHorizontal();

private:
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;
};
